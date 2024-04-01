#pragma once
#include "../include/json.hpp"
#include "../include/playlist.hpp"
#include <fstream>
struct AppSettings{
    bool should_recurse_subdirs;
    std::string playlist_directory_path;
    int deviceid;
    std::string device_name;
    bool allow_scraping; //Allow scraping web for Metadata
    bool load_playlist_image; // Turning it off speeds up playlist loading
    bool caching; //Enabled by default, Significantly speeds up load-times for playlists, by caching durations and images up to a certain point
    unsigned int MaxcacheMemory; // This tells the program at what point something needs to be removed from the cache
    bool debug;
    std::string userdata_directory_path;
};

struct ApplicationState{
    bool show_settings;
    bool show_customize;
    bool show_new_playlist;
    bool edit_playlist;
    bool paused;
    long long unsigned int timer;
    Song* currentSong;// this just takes the pointer from Mp3player
    Playlist* currentPlaylist; // this just takes the pointer from Mp3player
    std::string currentPlaylistPath;
    int* currentPlaylistid;
    std::string currentSongPath;
    std::string currentPlaylistName;
    float volume;
};

//Cutomize stuff is also saved in settings, since it would be kind of redundant to add another struct/namespace or whatever
namespace AppSettingsManager{
    int reloadSettings(std::string abs_settings_json_path,AppSettings* pAppSettings);
    int saveChangedSettings(std::string abs_settings_json_path,AppSettings* pAppSettings);
}

namespace AppStateManager{
    int reloadState(std::string abs_state_json_path,ApplicationState* pAppState);
    int saveChangedState(std::string abs_state_json_path,ApplicationState* pAppState);
    void resetDefaults(std::string abs_state_json_path);
}

