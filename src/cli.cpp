#include <iostream>
#include "../include/mp3player.hpp"
#include <chrono>
#include <thread>

// cli music player
int main(int argc,const char* argv[]){
    using namespace std::chrono_literals;
    Mp3Player::initMp3Player(0);
    std::string input_str="";
    while(true){
        std::this_thread::sleep_for(20ms);
        // get input here
    }
}