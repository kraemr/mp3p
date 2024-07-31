#include "../include/api.hpp"
#include "../include/json.hpp"
namespace MP3PlayerHTTPAPI {
    bool started;    
    using nlohmann::json;
    void handle_stop(const httplib::Request& req, httplib::Response& res) {
        res.set_content("{\"success\":true,\"info\":\"Stopping ...\"}", "application/json");
        Mp3Player::pauseMusic();
    }

    void handle_start(const httplib::Request& req, httplib::Response& res) {
        res.set_content("{\"success\":true,\"info\":\"Starting ...\"}", "application/json");
        Mp3Player::startMusic();
    }

    void handle_skip(const httplib::Request& req, httplib::Response& res) {
        res.set_content("{\"success\":true,\"info\":\"Loading Next Song ...\"}", "application/json");
        Mp3Player::loadNextSong();
    }

    void handle_prev(const httplib::Request& req, httplib::Response& res) {
        res.set_content("{\"success\":true,\"info\":\"Loading Next Song ...\"}", "application/json");
        Mp3Player::loadPrevSong();
    }

    void handle_get_playlist(const httplib::Request& req, httplib::Response& res){
        json data;
        std::string jsonStr = data.dump();
        res.set_content(jsonStr,"application/json");
    }

    void handle_get_current_song(const httplib::Request& req, httplib::Response& res){
        json data;
        data["name"] = Mp3Player::currentSong->songname;
        data["duration"] = Mp3Player::currentSong->duration;
        data["filepath"] = Mp3Player::currentSong->filepath;
        std::string jsonStr = data.dump();
        res.set_content(jsonStr,"application/json");
    }

    bool runApi(const char * host,unsigned int port){
        if(started == true){
            return false;
        }
        started = false;
        httplib::Server svr;
        svr.Get("/stop", handle_stop);
        svr.Get("/start", handle_start);
        svr.Get("/skip", handle_skip);
        svr.Get("/prev", handle_prev);
        svr.Get("/current_song",handle_get_current_song);
        svr.listen(host, port); // Start the server
        std::cout << "Server started at " << host << " port: " << port << std::endl;
        started = true;
        return true;
    }

    void stopApi(){    
    }
}



