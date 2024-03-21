#include "../include/cpp-httplib/httplib.h"
#include "../include/mp3player.hpp"

#include <iostream>
#include <thread>

std::thread server_thread;
void handle_stop(const httplib::Request& req, httplib::Response& res) {
    res.set_content("Stopping ...", "text/plain");
    Mp3Player::pauseMusic();

    // stop playing
}

void handle_start(const httplib::Request& req, httplib::Response& res) {
    res.set_content("Starting ...", "text/plain");
    Mp3Player::startMusic();
}

void handle_skip(const httplib::Request& req, httplib::Response& res) {
    res.set_content("Skipping...", "text/plain");
    Mp3Player::loadNextSong();
}

void handle_prev(const httplib::Request& req, httplib::Response& res) {
    res.set_content("Previous...", "text/plain");
    Mp3Player::loadPrevSong();
}

void runApi(){
    httplib::Server svr;
    svr.Get("/stop", handle_stop);
    svr.Get("/start", handle_start);
    svr.Get("/skip", handle_skip);
    svr.Get("/prev", handle_prev);
    std::cout << "Server started at http://localhost:16080" << std::endl;
    svr.listen("localhost", 16080); // Start the server
}

void stopApi(){
}

