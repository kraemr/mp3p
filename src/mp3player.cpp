/*
MIT License

Copyright (c) 2023 Robin Krämer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include "../include/mp3player.hpp"
#include "../include/stdafx.hpp"
#include "../include/caching.hpp"
namespace Mp3Player
{
/* Settings */
//####################
bool amplification_allowed = false; // can not go above 100% relative volume if false
bool fade_out = false; //every song fades out with a little delay
bool fade_in = false; // same thing except its at the beginning
f32 fade_out_delay = 0.0f;
f32 fade_in_delay = 0.0f;
f32 volume = 1.0f; // starting relative volume
u64 current_seconds=0;
volatile bool playOnLoad=true;
float volume_step = 0.05f; // This says how many percent the volume should go down for each call of 

//#####################

/*Constants*/
//#####################
#define SAMPLE_FORMAT   ma_format_f32
#define CHANNEL_COUNT   2
#define SAMPLE_RATE     48000
#define MAX_DEVICES     10
//#####################

/*Miniaudio*/
//#################################
ma_result result;


ma_sound sound;
ma_decoder decoder;
ma_engine engines[MAX_DEVICES];
ma_device devices[MAX_DEVICES];


ma_device_config deviceConfig;
ma_engine_config engineConfig;
ma_decoder_config decoderConfig;

ma_context context;
ma_device_info* pPlaybackDeviceInfos;
u32 playbackDeviceCount;
u32 iAvailableDevice;
ma_resource_manager_config resourceManagerConfig;
ma_resource_manager resourceManager;

//####################################
volatile bool isInitialized=false;
volatile bool playlistLoaded=false;
volatile bool exit=false;
i32 currentSongId=0;
i32 signal=0;
i32 current_device_index=0;
std::string current_device_name="None";
Song* currentSong;
bool songLoaded=false;
std::vector<Playlist> playlistsVec;
Playlist* currentPlaylist=nullptr;
bool paused=true;
u64 framesRead=0;



void resetSeconds(){
    current_seconds = 0;
}


bool songIsInPlaylist(Playlist * playlist,std::string song_name){
    if(playlist != nullptr){
        return false;
    }
    if(playlist->songs.size() == 0){
        return false;
    }
    for(u32 i = 0; i < playlist->songs.size();i++){
        if(playlist->songs[i].songname == song_name){
            return true;
        }
    }
    return false;
}

//0 is error
u64 getDurationOfSong(Song song){
    u64 duration=0;  // duration in frames
    std::string str = getPath(song.filepath);
    result = ma_decoder_init_file(str.c_str(), NULL, &decoder);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to init Decoder: " << ma_result_description(result) << std::endl;
        return duration; //
    }
    result = ma_decoder_get_length_in_pcm_frames(&decoder,&duration);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to get length in pcm frames: "<< song.songname << " at " << song.filepath << " might be missing" << std::endl;
        return duration; //
    }
    u64 res = (duration / decoder.outputSampleRate);
    ma_decoder_uninit(&decoder);
    //std::cout << "getDurationOfSong: " << song.songname << "  Duration:  " << song.duration << std::endl;
    return res;
}

std::string getPlaylistName(){
    if(currentPlaylist == nullptr){
        return "none";
    }
    if(currentPlaylist->name.size() == 0){
        return "none";
    }
    return currentPlaylist->name;
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    if(!songLoaded || paused){
        return;
    }
    if(ma_sound_at_end(&sound)){ 
        currentSongId++;        // Autoplay next Song
        i32 res = 0;
        if(currentPlaylist != nullptr && currentSongId < currentPlaylist->songs.size()){
            res = (i32)playSongAtIndex(currentSongId);
            if(res < 0){
                std::cerr << "data_callback couldnt play next song automatically" << std::endl;
            }
        }
        else if(currentPlaylist != nullptr && currentSongId >= currentPlaylist->songs.size()){
            currentSongId = 0;
            res = (i32)playSongAtIndex(currentSongId);
            if(res < 0){
                std::cerr << "data_callback couldnt play next song automatically" << std::endl;
            }
        }
    }
    (void)pInput;
    ma_engine_read_pcm_frames((ma_engine*)pDevice->pUserData, pOutput, frameCount,&framesRead);
    
    /*
    calculate the magnitude of sound here, for audio visualization.
    */


    current_seconds = (u64)(ma_engine_get_time_in_milliseconds(&engines[current_device_index]) / 1000);
}

/*
use ma_context_get_devices on a temp context and pPlaybackDeviceInfos
Then determine which devices are missing from the list and add them

typedef enum
{
    ma_device_state_uninitialized = 0,
    ma_device_state_stopped       = 1,   The device's default state after initialization. 
    ma_device_state_started       = 2,   The device is started and is requesting and/or delivering audio data. 
    ma_device_state_starting      = 3,   Transitioning from a stopped state to started. 
    ma_device_state_stopping      = 4    Transitioning from a started state to stopped. 
} ma_device_state;
MA_ATOMIC_SAFE_TYPE_DECL(i32, 4, device_state)
*/

void shufflePlaylist(){
    if(currentPlaylist != nullptr){
        shuffle_playlist(currentPlaylist);
    }
}


i32 refreshAudioDevices(){
    ma_device_info* tmp_playback_dev_info;
    ma_context tmp_context;
    u32 tmp_playback_dev_count;
    result = ma_context_get_devices(&context, &pPlaybackDeviceInfos, &playbackDeviceCount, NULL, NULL);


    if (result != MA_SUCCESS) {
        std::cerr << "Failed to enumerate playback devices." << std::endl;
        ma_context_uninit(&context);
        return -1;
    }
  

    for(int i = 0; i< playbackDeviceCount;i++){
        deviceConfig = ma_device_config_init(ma_device_type_playback);
        deviceConfig.playback.pDeviceID = &pPlaybackDeviceInfos[i].id;
        deviceConfig.playback.format   = SAMPLE_FORMAT;
        deviceConfig.playback.channels = 2;
        deviceConfig.sampleRate        = SAMPLE_RATE;
        deviceConfig.dataCallback      = data_callback;
        deviceConfig.pUserData         = &engines[0];
        
        //dev started
        if(devices[i].state.value == 2){
            
        }

        //dev uninitialized
        if(devices[i].state.value == 0){
            context = tmp_context;
            std::cout << "found uninitialized device: " ;
            result = ma_device_init(&context, &deviceConfig,&devices[i]);
            if (result != MA_SUCCESS) {
                std::cerr << "Failed to initialize Device" << std::endl;
                return -1;
            }
        }
    }
    
    return 0;
}

i32 changeAudioDevice(int id){
    ma_engine_uninit(&engines[0]);
    engineConfig = ma_engine_config_init();
    engineConfig.pDevice          = &devices[id];
    engineConfig.pResourceManager = &resourceManager;
    engineConfig.noAutoStart      = MA_TRUE;    /* Don't start the engine by default - we'll do that manually below. */
    result = (ma_result)ma_engine_init(&engineConfig, &engines[0]);
    if(result != MA_SUCCESS){
        return -1;
    }
    result = ma_engine_start(&engines[0]);
    if(result != MA_SUCCESS){
        return -2;
    }
    Mp3Player::current_device_name = pPlaybackDeviceInfos[id].name;
    return 0;
}


bool cmpDeviceStr(char * deviceNameBuffer,char * device_name,int nbytes){
    // buffer overflow mitigation, playbackdeviceinfos[i].name is 256 bytes
    if(nbytes > 256){
        return false;
    }
    std::cout<< "comparing: " << deviceNameBuffer << " "<< device_name << std::endl;
    for(int i= 0; i < nbytes; i++){
        if(deviceNameBuffer[i] != device_name[i]){
            return false;
        }
    }
    return true;
}

i32 changeAudioDevice (std::string device_name){
    // -1 means invalid
    int id = -1;    
    for(i32 i = 0; i < playbackDeviceCount;i++ ){
        bool device_found = cmpDeviceStr(pPlaybackDeviceInfos[i].name,(char*)device_name.c_str(),device_name.size());

        if(device_found){
            id = i;
            break; 
        }

    }
    return changeAudioDevice(id);
}




void fallbackAudioDevice(){
    changeAudioDevice(0);
}

std::vector<std::string> getDeviceNames(){
    std::vector<std::string> str;
    for(int i = 0; i < playbackDeviceCount;i++){
        str.push_back(pPlaybackDeviceInfos[i].name);
    }
    return str;
}



i32 initMp3Player(int deviceid){
    resourceManagerConfig = ma_resource_manager_config_init();
    resourceManagerConfig.decodedFormat     = SAMPLE_FORMAT;    /* ma_format_f32 should almost always be used as that's what the engine (and most everything else) uses for mixing. */
    resourceManagerConfig.decodedChannels   = 2;                /* Setting the channel count to 0 will cause sounds to use their native channel count. */
    resourceManagerConfig.decodedSampleRate = SAMPLE_RATE;            /* Using a consistent sample rate is useful for avoiding expensive resampling in the audio thread. This will result in resampling being performed by the loading thread(s). */

    result = ma_resource_manager_init(&resourceManagerConfig, &resourceManager);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize resource manager.");
        return -1;
    }
    

    result = ma_context_init(NULL, 0, NULL, &context);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize context.");
        return -1;
    }

    result = ma_context_get_devices(&context, &pPlaybackDeviceInfos, &playbackDeviceCount, NULL, NULL);
    if (result != MA_SUCCESS) {
        printf("Failed to enumerate playback devices.");
        ma_context_uninit(&context);
        return -1;
    }
    
    for(int i = 0; i< playbackDeviceCount;i++){
        deviceConfig = ma_device_config_init(ma_device_type_playback);
        deviceConfig.playback.pDeviceID = &pPlaybackDeviceInfos[i].id;
        deviceConfig.playback.format   = SAMPLE_FORMAT;
        deviceConfig.playback.channels = 2;
        deviceConfig.sampleRate        = SAMPLE_RATE;
        deviceConfig.dataCallback      = data_callback;
        deviceConfig.pUserData         = &engines[0];
        result = ma_device_init(&context, &deviceConfig,&devices[i]);
        if (result != MA_SUCCESS) {
            printf("Failed to initialize Device");
            return -1;
        }
    }
    ma_engine_uninit(&engines[0]);
    engineConfig = ma_engine_config_init();
    engineConfig.pDevice          = &devices[0];
    engineConfig.pResourceManager = &resourceManager;
    engineConfig.noAutoStart      = MA_TRUE;    /* Don't start the engine by default - we'll do that manually below. */
    result = (ma_result)ma_engine_init(&engineConfig, &engines[0]);
    result = ma_engine_start(&engines[0]);
    //changeAudioDevice(1);
    isInitialized = true;
    return 0;
}

// You can pass 100% for full volume and 0% for muting
// if amplification_allowed is on then you can go above 100% Volume though this is disabled by default
void setVolumePercent(f32 percent){
    if(percent < 0.0f){
        return; // why would you even try ? ...
    }
    if(amplification_allowed){
        volume = percent/100.0f;
        result = ma_engine_set_volume(&engines[current_device_index],volume);
    }
    else{
        percent = percent > 100.0f ? 100.0f : percent;
        volume = percent/100.0f;
        result = ma_engine_set_volume(&engines[current_device_index],volume);
    }
}

void setVolume(f32 _volume){
    volume = _volume;
    result = ma_engine_set_volume(&engines[current_device_index],volume);
    if(result != MA_SUCCESS){
        std::cerr << "couldnt set Volume: " << result << std::endl;
    }else{
        std::cout << "set Volume to: " << _volume << std::endl;
    }
}

// 0.05 would be 5 % for a step up or down
void adjustVolumeInSteps(bool vol_down,float step){
    if(vol_down){
        volume = volume - step;
    }else{
        volume = volume + step;
    }
    if(volume > 1.0f){
        volume = 1.0f;
    }
    else if(volume < 0.0f){
        volume = 0.0f;
    }
    result = ma_engine_set_volume(&engines[current_device_index],volume);
    if(result != MA_SUCCESS){
        std::cerr << "couldnt set Volume: " << result << std::endl;
    }
}

u64 getCurrentFrame(){
    return ma_sound_get_time_in_pcm_frames(&sound);
}

u64 getLengthInFrames(){
    u64 length=0;
    result = ma_sound_get_length_in_pcm_frames(&sound, &length);
    return length;
}

// this seeks to the next mpeg frame in seconds interval and also returns it
u64 seekSecondsFrame(u64 second){
 //   ma_engine_get_time_in_pcm_frames(&engine) + (ma_engine_get_sample_rate(&engine) * 2
  //  ma_sound_seek_to_pcm_frame(&sound, ma_engine_get_time_in_pcm_frames(&engine) );
    return 0;
}

f32 getVolumePercent(){
    return volume * 100.0f;
}


i32 initPlaySound(const std::string filepath){
    ma_engine_set_time_in_milliseconds(&engines[current_device_index], 0); // WE NEED TO RESET IT EVERYTIME A SONG IS LOADED
    (void)ma_sound_uninit(&sound); // uninit seems to be safe to use when uninitialized :)
    result = (ma_result)ma_sound_init_from_file(&engines[current_device_index], filepath.c_str(), MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM, NULL, NULL, &sound);  
    songLoaded = true;
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to initialize sound";
        return -1;
    }
    if(playOnLoad == true){
        result = (ma_result)ma_sound_start(&sound);
    }
    return 0;
}

//Deallocate everything
void cleanupMp3Player(){
    if(isInitialized){
        ma_engine_uninit(&engines[current_device_index]);
        isInitialized = false;
    }
}

i32 playSongAtIndex(i32 i){
    if(currentPlaylist == nullptr){
        printf("Error No playlist loaded, but tried to play a song from it!!\n");
        return -1;
    }
    if(i >= currentPlaylist->songs.size()){
        printf("Failed to playSongAtIndex %d\n",i);
        return -1;
    }
    currentSong = &currentPlaylist->songs[i];
    currentSongId = i;
   // i32 res = initPlaySound(currentPlaylist->songs[i].filepath);
    std::string str = getPath(currentPlaylist->songs[i].filepath);
    i32 res = initPlaySound(str);
    
    if(res == -1){
        printf("Failed to playSongAtIndex %d\n",i);
        return -1;
    }
    return 0;
}

void loadPrevSong(){
    currentSongId--;
    i32 res;
    if(currentPlaylist != nullptr && currentSongId >= 0){
        res = (i32)playSongAtIndex(currentSongId);
    }
    else if(currentPlaylist != nullptr && currentSongId < 0){
        currentSongId = currentPlaylist->songs.size() - 1;
        res = (i32)playSongAtIndex(currentSongId);
    }
}

void loadNextSong(){
    currentSongId++;        // Autoplay next Song
    i32 res=0;
    if(currentPlaylist != nullptr && currentSongId < currentPlaylist->songs.size()){
        res = (i32)playSongAtIndex(currentSongId);
        if(res < 0){
            std::cout << "couldnt load next Song" << std::endl;
        }
    }
    else if(currentPlaylist != nullptr && currentSongId >= currentPlaylist->songs.size()){
        currentSongId = 0;
        res = (i32)playSongAtIndex(currentSongId);
        if(res < 0){
            std::cout << "couldnt load next Song"<< std::endl;
        }
    }
}

void pauseMusic(){
    if(currentSong != nullptr){
        ma_engine_stop(&engines[current_device_index]);
        paused = true;
    }
}

void startMusic(){
    if(currentSong != nullptr){
        result = ma_engine_start(&engines[current_device_index]);
        paused = false;
        currentSong = &currentPlaylist->songs[currentSongId];
    }
}

// searches Song By its name attribute
// returns the first match
Song findSongByName(){
    Song song;
    return song;
}
// Later enable loading playlists from multiple directories/filepaths
void loadPlaylistsDir(const std::string filepath){
    playlistsVec = read_playlists_dir(filepath);
    if(playlistsVec.size() == 0){
        return;
    }
    currentPlaylist = &playlistsVec[0]; 
    // Later read from a hidden file the last one, that was played
}

//format Seconds to this schema: 1:30 when its 90 secs for example
std::string formatMinutesSeconds(u32 in_seconds){
    u32 minutes = in_seconds / 60;
    u32 seconds = in_seconds % 60;
    std::stringstream formattedStream;
    formattedStream << minutes << ":";
    if(seconds < 10){
        formattedStream << "0"; 
    }
    formattedStream << seconds;
    return     formattedStream.str();
}

i32 loadDifferentPlaylist(std::string filepath){
    Playlist playlist = read_playlist_json(filepath);
    currentPlaylist = &playlist;
    
    return 0;
}

i32 decodeSongsPlaylist(Playlist* playlist){
    //std::cout << (playlist == 0) << std::endl;
    if(playlist == NULL){
        return -1;
    }
    if(playlist == nullptr ){
        std::cerr << "decodeSongsPlaylist: " <<  playlist->name << " Playlist was null"<<std::endl;
        return -1;
    }
    else if(playlist->songs.size() == 0){
        std::cerr << "decodeSongsPlaylist: " <<  playlist->name << " Playlist was empty"<<std::endl;
        return -1;
    }
    for(int i = 0; i < (int)playlist->songs.size();i++){
        unsigned long long dur = getCachedDuration(playlist->songs[i].filepath);
      
        if(dur > 0){
            playlist->songs[i].duration = getCachedDuration(playlist->songs[i].filepath);
           // std::cout << "song: " << playlist->songs[i].songname << " was in cache" << std::endl;
        }else{
            playlist->songs[i].duration = getDurationOfSong(playlist->songs[i]);
            setCachedDuration(playlist->songs[i].filepath,playlist->songs[i].duration);
         //   std::cout << "song: " << playlist->songs[i].filepath << " was not in cache" << std::endl;
        }

    }
    return 0;
}

i32 loadDifferentPlaylist(u32 id){
    currentPlaylist = &playlistsVec[id];
    if(currentPlaylist == nullptr){
        std::cerr << "No Playlist At id: " << id << std::endl;
        return -1;
    }
    if(currentPlaylist->songs.size() > 0){
        currentSong = &currentPlaylist->songs[0];
    }
    else{
        std::cerr << "No Songs in Playlist" << currentPlaylist->name << std::endl;
        return -1;
    }
    playSongAtIndex(0);
    pauseMusic();
    decodeSongsPlaylist(currentPlaylist);
    return 0;
}

i32 removeSong(long unsigned int id){
    if(currentSong != nullptr && currentPlaylist != nullptr){
        if(currentSong == &currentPlaylist->songs[id]){
            currentPlaylist->songs.erase((currentPlaylist->songs.begin()+id));
            if(currentPlaylist->songs.size() <= id){
                pauseMusic();
                resetSeconds();
                currentSong = nullptr;
                return 0;
            }
            playSongAtIndex(id);
        }else{
            currentPlaylist->songs.erase((currentPlaylist->songs.begin()+id));            
        }
    }else{
        return -1;
    }
    return 0;
}

i32 removePlaylist(i32 id){
    if(currentPlaylist != nullptr && currentPlaylist == &playlistsVec[id]){ 
        pauseMusic();
        currentSong = nullptr;
        currentPlaylist = nullptr;
    }
    std::remove(playlistsVec[id].path.c_str());
    playlistsVec.erase(playlistsVec.begin() + id);
    return 0;
}


}
