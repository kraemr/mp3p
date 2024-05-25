#include "../include/caching.hpp"
// This code aims to speed up loading of playlists/songs by caching Songs Duration, as the decoding can become quite expensive
// Might even cache a few images
// Here we use a Hashmap to group Playlists
// implement LRU Cache here
// We use the Path of the Song as Key
std::map<std::string,unsigned long long> Cache;



std::map<std::string,unsigned long long> getCache(){
    return Cache;
}


// if unsuccessful ret 0 else duration >= 1 is returned
unsigned long long getCachedDuration(std::string path){
    if (Cache.find(path) == Cache.end()) {
        return 0;
    }else{
        return Cache[path];
    }
}

void setCachedDuration(std::string path,unsigned long long duration){
    if(path.size() == 0){
        return;
    }
    //std::cout << "setCachedDuration: " << path << "  " << duration; 
    Cache[path] = duration;
}

void flushCache(){
    Cache.clear();
}
