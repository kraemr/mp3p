#pragma once
#include <cstddef>
#include <thread>
#include <atomic>
#include <chrono>
#include "playlist.hpp"
#include "../include/miniaudio.h"

namespace Mp3Player
{
int initMp3Player(int deviceid);
int initPlaySound(const std::string filepath);
void cleanupMp3Player();
void loadPlaylistsDir(const std::string filepath);
int playSongAtIndex(int i);
void processSignals();
void setVolumePercent(float percent);
float getVolumePercent();
void loadNextSong();
void loadPrevSong();
void pauseMusic();
void startMusic();
void setVolume(float volume);
std::vector<std::string> getDeviceNames();
std::string getPlaylistName();
unsigned long long getDurationOfSong(Song song);
std::string formatMinutesSeconds(unsigned int in_seconds);
void skipToSecond(unsigned long long seconds);
int loadDifferentPlaylist(unsigned int id);
int loadDifferentPlaylist(std::string filepath);
int changeAudioDevice(int id);
int changeAudioDevice(std::string device_name);
void fallbackAudioDevice();
int removeSong(long unsigned int id);
int removePlaylist(int id);
int decodeSongsPlaylist(Playlist* playlist);
void adjustVolumeInSteps(bool vol_down,float step);
bool songIsInPlaylist(Playlist * playlist,std::string song_name);
int refreshAudioDevices();
void shufflePlaylist();
extern Song* currentSong;
extern int signal;
extern std::vector<Playlist> playlistsVec;
extern Playlist* currentPlaylist;
extern float volume;
extern bool paused;
extern unsigned long long current_seconds;
extern int current_device_index;
extern ma_device devices[10];
extern ma_engine engines[10];
extern unsigned int playbackDeviceCount;
extern ma_device_info* pPlaybackDeviceInfos;
extern int currentSongId;
extern std::string current_device_name;
}
