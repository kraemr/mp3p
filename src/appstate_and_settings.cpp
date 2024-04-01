#include "../include/json.hpp"
#include <fstream>
#include <iostream>
#include "../include/appstate_and_settings.hpp"

//Cutomize stuff is also saved in settings, since it would be kind of redundant to add another struct/namespace or whatever
namespace AppSettingsManager{
    using json = nlohmann::json;
    //loads settings.json
    void resetDefaults(std::string abs_settings_json_path){
        json data;
        data["playlist_directory_path"] = "";
        data["deviceid"] = 0;
        data["device_name"] = "None";
        data["should_recurse_subdirs"] = false;
        data["debug"] = false; 
        
        data["userdata_directory_path"] = "";
        data["playlist_directory_path"] = "";

        std::string jsonStr = data.dump();
        std::ofstream outputFile(abs_settings_json_path);
	    if (outputFile.is_open()) {
            outputFile << jsonStr;
            outputFile.close();
        } 
    }

    int reloadSettings(std::string abs_settings_json_path,AppSettings* pAppSettings){
        std::ifstream f(abs_settings_json_path);
        if(f.is_open() == false){
            resetDefaults(abs_settings_json_path);
            return -2; 
        }
        try{
		    json data = json::parse(f);
            pAppSettings->playlist_directory_path = data["playlist_directory_path"];
            pAppSettings->deviceid = data["deviceid"];
            pAppSettings->device_name = data["device_name"];
            pAppSettings->should_recurse_subdirs = data["should_recurse_subdirs"];
            pAppSettings->debug = data["debug"];
            pAppSettings->caching = data["caching"];
            pAppSettings->userdata_directory_path = data["userdata_directory_path"];

        }
        catch (const json::exception& e){
            std::cerr << "reloadAppSettings failed: " << abs_settings_json_path << std::endl;
            return -2;
        }

        return 0;
    }

    int saveChangedSettings(std::string abs_settings_json_path,AppSettings* pAppSettings){
        json data;
        data["playlist_directory_path"] = pAppSettings->playlist_directory_path;
        data["userdata_directory_path"] = pAppSettings->userdata_directory_path;

        data["deviceid"] = pAppSettings->deviceid;
        data["device_name"] = pAppSettings->device_name;
        data["should_recurse_subdirs"] = pAppSettings->should_recurse_subdirs;
        data["debug"] = pAppSettings->debug;
        data["caching"] = pAppSettings->caching;
        std::string jsonStr = data.dump();
        std::ofstream outputFile(abs_settings_json_path);
	    if (outputFile.is_open()) {
            outputFile << jsonStr;
            outputFile.close();
        } 
        return 0;
    }
}

    
namespace AppStateManager{
    using json = nlohmann::json;

    void resetDefaults(std::string abs_state_json_path){
        json data;
        data["show_customize"] = false;
        data["show_settings"] = false;
        data["show_new_playlist"] = false;
        data["current_playlist_name"] = "";
        data["volume"] = 0.7f;
        std::string jsonStr = data.dump();
        std::ofstream outputFile(abs_state_json_path);
	    if (outputFile.is_open()) {
            outputFile << jsonStr;
            outputFile.close();
        } 
    }

    //loads state.json
    int reloadState(std::string abs_state_json_path,ApplicationState* pAppState){
        std::ifstream f(abs_state_json_path);
        if(f.is_open() == false){
            resetDefaults(abs_state_json_path);
            return -2; 
        }
        try{
		    json data = json::parse(f);
            pAppState->paused = true;
            pAppState->show_customize = data["show_customize"];
            pAppState->show_settings = data["show_settings"];
            pAppState->show_new_playlist = data["show_new_playlist"];
            pAppState->currentPlaylistName = data["current_playlist_name"];
            //pAppState->currentPlaylistPath = data["current_playlist_path"];
            pAppState->volume = data["volume"];
        }
        catch (const json::exception& e){
		    std::cerr << "Error Loading State (AppStateManager) json path: " << abs_state_json_path << std::endl;
		    return -1;
        }
        return 0;
    }

    int saveChangedState(std::string abs_state_json_path,ApplicationState* pAppState){
        json data;
        data["show_customize"] = pAppState->show_customize;
        data["show_settings"] = pAppState->show_settings;
        data["show_new_playlist"] = pAppState->show_new_playlist;
    //    data["current_song_path"] = pAppState->currentSong->filepath;
        data["current_playlist_name"] = pAppState->currentPlaylistName;
        data["volume"] = pAppState->volume;
        std::string jsonStr = data.dump();
        std::ofstream outputFile(abs_state_json_path);
	    if (outputFile.is_open()) {
            outputFile << jsonStr;
            outputFile.close();
        } 
        return 0;
    }
    
}

