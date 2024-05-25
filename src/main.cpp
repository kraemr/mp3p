//extern void runApi(); // 
#include <string>
#include <thread>
#include <iostream>
extern void run(std::string str,std::string str1); // gui.cpp

int main(int argc,char ** argv){
    bool start_server=false;
    bool start_app=true;
  //  std::thread server_thread;
    if(start_server){ 
  //      server_thread = std::thread(runApi);
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
        run(userdata_dir,playlist_dir);  
    }
    
    if(start_server){
    //    server_thread.join();
    }
}
