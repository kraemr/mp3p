#pragma once
#include <string>
#include <vector>
struct Song {
        std::string songname;
        std::string filepath;
		unsigned long long duration;
};

struct Playlist {
	std::string name; // name of playlist (is used to identify --> must be unique)
	std::string path; // This actually has the absolute path to the actual file
	std::vector<Song> songs;
	std::string description;
	// IF this is true and you are saving the playlist read it ahain from disk and apply changes to that and save instead of saving the shuffled version
	bool shuffled; 
	// IF this is true and the last song is played, then go back to the beginning
	bool shouldLoop; 
	std::string image_path;
};

Playlist read_playlist_json(std::string filepath);
void shuffle_playlist(Playlist* playlist);
std::vector<Playlist> read_playlists_dir(std::string path);
void save_changed_playlist(Playlist* playlist);
std::string getPath(std::string path);