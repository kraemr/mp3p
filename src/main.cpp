//extern void runApi(); // 
#include <string>
#include <thread>
#include <iostream>
#include "../include/api.hpp"
extern void run(std::string str,std::string str1); // gui.cpp

// need this void func for std::thread
void run_api(){
  MP3PlayerHTTPAPI::runApi("0.0.0.0", 16080);
}

int main(int argc,char ** argv){
    bool start_server=true;
    bool start_app=true;
    std::thread server_thread;
    if(start_server){ 
        server_thread = std::thread(run_api);
    }
    if(start_app){
        // User has to set env variables for where it should be saved 
        // Print error message with instructions if user did not set it
        const char * userdata_dir =  getenv("CPPMP3_USERDATA");
        const char * playlist_dir =  getenv("CPPMP3_PLAYLISTS");
        std::cout << playlist_dir << std::endl;
        if(userdata_dir == NULL){
          userdata_dir = "";
          std::cerr << "ERROR you need to set CPPMP3_USERDATA in your environment or the registry in windows" << std::endl;
          return 1;
        }
        if(playlist_dir == NULL){
          playlist_dir = "";
          std::cerr << "ERROR you need to set CPPMP3_PLAYLISTS in your environment or the registry in windows" << std::endl;
          return 1;
        }
        run(userdata_dir,playlist_dir);//will block execution
    }
    if(start_server){
        server_thread.join();
    }
}
