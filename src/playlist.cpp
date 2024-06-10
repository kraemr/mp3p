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

//TODO replace the JSON-Error string with PlaylistResult for better error handling
#include <cstddef>
#include <fstream>
#include <iostream>
#include "../include/json.hpp"
#include <vector>
#include "../include/playlist.hpp"
#include "../include/stdafx.hpp"
#include <algorithm>
#include <random>
#include <sys/stat.h>
using json = nlohmann::json;
namespace fs = std::filesystem;
struct PlaylistResult{
	Playlist playlist;
	int error;	
};


//this handles paths that contain $, which indicate an env variable
std::string getPath(std::string path){
        bool hasEnvVar=false;
		if(path.size() == 0){
			return path;
		}
        std::vector<std::string>  pathparts;
        std::string temp;
        std::string envpart;
        for(long unsigned int i = 0; i < path.size();i++ ){
                if(path.at(i) == '$'){ // env var was found
                        hasEnvVar=true;
                        continue;
                }
                if(hasEnvVar && path.at(i) == '/'){
                        hasEnvVar=false; // we got all the chars for our env var
                        const char * envVal = std::getenv(envpart.c_str());
                        pathparts.push_back(envVal);
                        temp = "";
                }else if(hasEnvVar==false && path.at(i) == '/'){
                        pathparts.push_back(temp);
                        temp = "";
                }
                else if(hasEnvVar){
                        envpart += path.at(i);
                }
                else{
                        temp += path.at(i);
                }
        }
        pathparts.push_back(temp); // this is fine because even if last char is / it will just push an empty string
        std::string retstring="";
        for(long unsigned int i = 0; i < pathparts.size()-1;i++){
                retstring += pathparts[i] + "/";
        }
        if(pathparts.size() > 1){
                retstring += pathparts[pathparts.size()-1];
        }
        return retstring;
}

Playlist create_playlist_from_dir(){
	Playlist playlist;
	return playlist;
}

Playlist read_playlist_json(std::string filepath){
	try{
	    std::vector<Song> playlist_songs;
        Playlist playlist;
	    std::ifstream f(filepath);
		json data = json::parse(f);
		for (auto& el : data["songs"].items()){
			struct Song song;
			song.songname = el.value()["songname"];
			song.filepath = el.value()["path"];
			playlist_songs.push_back(song);
		}
		playlist.name = data["name"];
		playlist.path = filepath;
	    playlist.songs = playlist_songs;
		playlist.shuffled = false;
		if(data["image_path"].is_null() && data["image_path"] == ""){
			playlist.image_path = "";
		}
		else if(data.contains("image_path") && !data["image_path"].is_null()){
			playlist.image_path = getPath(data["image_path"]);
		//	std::cout << "playlist " << playlist.name <<  playlist.image_path << std::endl;
		}
		else{
			//data["image_path"] = "";
		}

		if(data.contains("description")){
			playlist.description = data["description"];
		}
		return playlist;
	}
	catch (const json::exception& e)
    	{
		std::cerr << "The playlist in which the error occurred: " << filepath << std::endl << std::endl;
 		std::cerr << e.what() << std::endl; 
		Playlist playlist;
		playlist.name = "__ERROR_JSON";
		playlist.description = "";
		playlist.image_path = "";
		playlist.songs = std::vector<Song>(0);
		return playlist;
    	}
}

// This creates a json_file with the same name as
void save_changed_playlist(Playlist* playlist){ 
	if(playlist == nullptr || playlist == NULL ){
		return;
	}
	try{
	json data;
	data["name"] = playlist->name;
	data["path"] = playlist->path;
	#ifdef CPPMP3_DEBUG
	std::cout << "orig Path" << p.path << std::endl;
	#endif
	json songsJsonArr;
	for (const Song& song : playlist->songs){
		json songJson;
		songJson["songname"] = song.songname;
		songJson["path"] = song.filepath;
		songsJsonArr.push_back(songJson);
	}
    data["songs"] = songsJsonArr;

	data["image_path"] = playlist->image_path;
	
	data["description"] = playlist->description;
    
	std::string jsonStr = data.dump();
    std::ofstream outputFile(playlist->path);
	if (outputFile.is_open()) {
        outputFile << jsonStr;
        outputFile.close();
    } 
	}
	catch (const json::exception& e){
		std::cerr << "Error Saving Playlist " << playlist->name << std::endl;
		return;
	}
}

// Use this if you load playlists from different locations
void merge_playlists(std::vector<Playlist>& playlist1,std::vector<Playlist> playlist2){
	return;
}

void print_songs(Playlist& playlist){
	/*std::cout << STDAFX_YELLOW << playlist->name << ": " << std::endl;
	for (struct Song song : playlist->songs){
		std::cout << "songname: " << song.songname << "filepath: " << song.filepath << std::endl;
	}
	std::cout << STDAFX_RESET_COLOR <<std::endl;*/
}



std::vector<Playlist> read_playlists_dir(std::string path){
	std::vector<Playlist> playlists;
	try{
		struct stat sb;
		const std::string split = ".json";
		for(const auto& entry : fs::directory_iterator(path)){
			std::filesystem::path filename = entry.path();
			std::string filename_str = filename.string();
			const char * path = filename_str.c_str();
			if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR)){
				Playlist playlist = read_playlist_json(filename_str);
				if(playlist.name == "__ERROR_JSON"){
					continue; // found and read playlist, but it isnt valid
				}
				playlist.path = filename_str;
				playlists.push_back(playlist);
				#ifdef CPPMP3_DEBUG
					std::cout << STDAFX_YELLOW << path << STDAFX_RESET_COLOR  << std::endl;
            	    for(auto element : playlist.songs){
						std::cout << STDAFX_GREEN << element.songname << STDAFX_RESET_COLOR << std::endl;
						std::cout << element.filepath << std::endl;
					}
				#endif
			}
		}
		return playlists;
	}
	catch(std::filesystem::__cxx11::filesystem_error err){
		return playlists;
	}
}

// This shuffles the songs in-mem
void shuffle_playlist(Playlist* playlist){
	if(playlist == nullptr || playlist == NULL){
		return;
	}
	std::random_device rd;
	std::default_random_engine rng(rd());
	playlist->shuffled = true;
	std::shuffle(playlist->songs.begin(), playlist->songs.end(), rng);
}


