#include "../include/caching.hpp"
// This code aims to speed up loading of playlists/songs by caching Songs Duration, as the decoding can become quite expensive
// Might even cache a few images
// Here we use a Hashmap to group Playlists
// implement LRU Cache here
// We use the Path of the Song as Key
std::map<const char*,unsigned long long> Cache;


// if unsuccessful ret 0 else duration >= 1 is returned
unsigned long long getCachedDuration(std::string path){
    //if(Cache.contains(path.c_str())){
    if (Cache.find(path.c_str()) != Cache.end()) {
        return Cache[path.c_str()];
    }else{
        return 0;
    }
}

void setCachedDuration(std::string path,unsigned long long duration){
    if(path.size() == 0){
        return;
    }
    Cache[path.c_str()] = duration;
}

void flushCache(){
    Cache.clear();
}
