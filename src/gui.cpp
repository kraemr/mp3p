#include "../include/app.hpp"
#include <cstddef>
#include <iterator>
#include <string>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#endif
namespace Mp3Gui{
    GLFWwindow * window;
    ImGuiIO* io;

    int windowWidth=1280;
    int windowHeight=720;

    stb_IMAGE playButton_img;
    stb_IMAGE pauseButton_img;
    stb_IMAGE prevButton_img;
    stb_IMAGE exitButton_img;
    stb_IMAGE skipButton_img;
    stb_IMAGE settingsButton_img;
    stb_IMAGE cutomizeButton_img;
    stb_IMAGE currentPlaylist_image;
    
    ImFont* title_font;
    ImFont* text_font;

    ImVec2 playlistPos = {(float)0,(float)0};;
    ImVec2 playlistSize = {(float)windowWidth/6,(float)windowHeight};;
    
    ImVec2 playerPos = {(float)windowWidth,(float)windowHeight};
    ImVec2 playerSize = {(float)windowWidth - playlistSize.x,(float)windowHeight/12};

    ImVec2 SongsPos = {playlistSize.x,0};;
    ImVec2 SongsSize = {(float)windowWidth-playlistSize.x,(float)windowHeight};;
    
    ImVec2 PlaylistImageSize = {(float)windowWidth/8,(float)windowWidth/8};;
    //This is the playlist getting created by new Playlist window
    Playlist created_playlist;
    char name[256];
    char description[4096];

void updatePositionsAndSizes(){
    playlistPos = {(float)0,(float)0};;
    playlistSize = {(float)io->DisplaySize.x/6,io->DisplaySize.y};;
    
    playerSize = {(float)io->DisplaySize.x - playlistSize.x,((float)io->DisplaySize.y) / (float)11.5};
    playerPos = {(float)playlistPos.x + playlistSize.x,(float)io->DisplaySize.y-playerSize.y};
    
    SongsPos = {playlistSize.x,0};;
    SongsSize = {(float)io->DisplaySize.x-playlistSize.x,(float)io->DisplaySize.y-playerSize.y};

    PlaylistImageSize = {(float)io->DisplaySize.x /8,(float)io->DisplaySize.x/8};
}
    
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    io->DisplaySize = ImVec2(width, height);
}

bool LoadTextureFromFile(const char* filename,struct stb_IMAGE* image)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;
    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);
    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same
    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);
    image->texture = image_texture;
    image->w = image_width;
    image->h = image_height;
    return true;
}

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;
    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);
    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same
    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}



void saveAllChangesToPlaylists(){
    for(Playlist playlist : Mp3Player::playlistsVec){
        save_changed_playlist(&playlist);
    }
}

void cleanup(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    saveAllChangesToPlaylists();
}


void printStringVector(std::vector<std::string> vec){
    for(auto str : vec){
        std::cout << str << std::endl;
    }
}


std::vector<std::string> stringSplit(std::string str,char split_on){
    std::vector<std::string> vec;
    std::string curr_str;
    for(char ch : str){
        if(ch == split_on){
    vec.push_back(curr_str);
    curr_str = "";

        }else curr_str += ch;
    }
    vec.push_back(curr_str);
    return vec;
}


static void glfw_exit_window_callback(GLFWwindow* window ){
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void HandleDropEvent(int count, const char** paths) {
    Song song;
    ApplicationState* papp_state = getAppStatePointer();

    if(papp_state->show_new_playlist ){
        if(count == 1){
            const char* image_path = paths[0];
            created_playlist.image_path = image_path;
        }
    }
    else if(papp_state->edit_playlist){
        if(count == 1){
            Mp3Player::currentPlaylist->image_path = paths[0];
            if(Mp3Player::currentPlaylist != nullptr){
                LoadTextureFromFile(Mp3Player::currentPlaylist->image_path.c_str(),&currentPlaylist_image);
            }
        }
    }
    else if(papp_state->show_new_playlist == false && papp_state->edit_playlist == false){
        if(Mp3Player::currentPlaylist != nullptr){ // File Has To Be mp3
            for (int i = 0; i < count; ++i) {
                const char* filePath = paths[i];
                song.filepath = filePath;
                std::vector<std::string> strs  =  stringSplit(song.filepath,'/');
                assert(strs.size()>0);
                std::vector<std::string> strs2 =  stringSplit(strs[strs.size()-1],'.');
                assert(strs2.size()>0);
                // This seems to work properly
                if( !(( std::strcmp(strs2[strs2.size()-1].c_str(),"mp3" ) != 0 ) ^ ( std::strcmp(strs2[strs2.size()-1].c_str(),"wav" ) != 0)) ){
                }
                song.songname = "";            
                for(int j = 0; j < strs2.size()-1;j++){
                    song.songname += strs2[j]; // reassemble the string
                }
                if(Mp3Player::songIsInPlaylist(Mp3Player::currentPlaylist, song.songname) == true){
                //Then do not add it to the playlist
                    continue;
                }
                song.duration = Mp3Player::getDurationOfSong(song);
                Mp3Player::currentPlaylist->songs.push_back(song);
            }
                Mp3Player::currentSong = &Mp3Player::currentPlaylist->songs[Mp3Player::currentSongId];
                save_changed_playlist(Mp3Player::currentPlaylist);
        }
    }
   
}

void GLFWDropCallback(GLFWwindow* window, int count, const char** paths) {
    HandleDropEvent(count, paths);
}

static void glfw_error_callback(int error, const char* description){
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void renderSettingsWindow(ApplicationState& app_state,AppSettings& app_settings){
    if(app_state.show_settings){
        ImGui::Begin("Settings",&app_state.show_settings);
        ImGui::SetWindowSize({io->DisplaySize.x - (io->DisplaySize.x/4)
        ,(float)(io->DisplaySize.y-(io->DisplaySize.y/12))},0);
        ImGui::Checkbox("Debug",&app_settings.debug);
        ImGui::Checkbox("Cache Song Duration in Memory",&app_settings.caching);
        
        std::vector<std::string> devices = Mp3Player::getDeviceNames();
        for(long unsigned int i = 0; i < devices.size();i++){
            if(ImGui::Button(devices[i].c_str())){
                int res = Mp3Player::changeAudioDevice(devices[i]);
                if(res >= 0){
                    app_settings.device_name = Mp3Player::current_device_name;
                }
                else{
                    app_settings.device_name = "DEVICE NOT FOUND";
                }
            }
        }
        ImGui::End();
    }
}

void renderCustomizeWindow(ApplicationState& app_state,AppSettings& app_settings){
        if(app_state.show_customize){
	    ImGui::Begin("Customization",&app_state.show_customize);
	    ImGui::SetWindowSize({(float)windowWidth-float(windowWidth/4),(float)(windowHeight-(windowHeight/12))},0);
	    ImGui::End();
        }
}

void renderPlayer(ApplicationState& app_state,AppSettings& app_settings){
    
    ImGui::SetNextWindowSize(playerSize,0);
    ImGui::SetNextWindowPos(playerPos,0);
    ImGui::Begin("Player",nullptr,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNav );
    std::string fmt_current_time = Mp3Player::formatMinutesSeconds(Mp3Player::current_seconds);
    ImGui::Text("%s",fmt_current_time.c_str());
    ImGui::SameLine();
    if(ImGui::Button("|<")){
        Mp3Player::loadPrevSong();
    }
    ImGui::SameLine();
    if(ImGui::Button(">")){
        if(Mp3Player::paused){
            Mp3Player::startMusic();
        }else{
            Mp3Player::pauseMusic();
        }
    }
    ImGui::SameLine();
    if(ImGui::Button(">|")){
        Mp3Player::loadNextSong();
    }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::SliderFloat("Volume", &Mp3Player::volume, 0.0f, 1.0f);
    if(app_state.volume != Mp3Player::volume){
        (void)Mp3Player::setVolume(Mp3Player::volume);
    } 
    ImGui::SameLine();
    if(Mp3Player::currentSong != nullptr && !Mp3Player::currentSong->songname.empty()){
        ImGui::Text("%s",Mp3Player::currentSong->songname.c_str()); 
    }else{
        ImGui::Text("No Song Playing ...");
    }
    
    ImGui::SameLine();
    if(ImGui::Button("settings")){
        int res = Mp3Player::refreshAudioDevices();
        app_state.show_settings = true;
    }
    ImGui::SameLine();
    if(ImGui::Button("Exit")){
        ImGui::End();
        cleanup();
        Mp3Player::cleanupMp3Player();
        saveAllChangesToPlaylists();
        int res;
        res=AppStateManager::saveChangedState(app_settings.userdata_directory_path + "state.json",&app_state);
        if(res < 0){
            std::cout << "error couldnt save State (saveChangedState)";
        }
        res=AppSettingsManager::saveChangedSettings(app_settings.userdata_directory_path + "settings.json", &app_settings);
        if(res < 0){
            std::cout << "error couldnt save Settings (saveChangedSettings)";
        }
        exit(0);
        return;
    }
    ImGui::End();
}

void renderDebug(ApplicationState& app_state,AppSettings& app_settings){
    
    ImGui::Begin("Debug");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
    ImGui::Text("Playlist != nullptr: %d",Mp3Player::currentPlaylist != nullptr);
    ImGui::Text("currentSong != nullptr: %d",Mp3Player::currentSong != nullptr);
    ImGui::Text("song pointer %llu",(long long unsigned int) Mp3Player::currentSong);
    std::vector<std::string> deviceNames = Mp3Player::getDeviceNames();
    ImGui::Text("%d %s",Mp3Player::current_device_index,deviceNames[Mp3Player::current_device_index].c_str());
    ma_device* dev = ma_engine_get_device(&Mp3Player::engines[Mp3Player::current_device_index]);
    ImGui::Text("current Device pointer: %llu",(long long unsigned int) dev);
    ImGui::Text("Playlist Path: %s",app_settings.playlist_directory_path.c_str());
    ImGui::Text("Userdata Path: %s",app_settings.userdata_directory_path.c_str());
    ImGui::Text("Devices:");

    for(unsigned int i = 0; i < Mp3Player::playbackDeviceCount;i++ ){
        ImGui::Text("%s %llu",Mp3Player::pPlaybackDeviceInfos[i].name,(long long unsigned int) &Mp3Player::devices[i]);
    }

    ImGui::End(); 
}

void renderPlaylists(ApplicationState& app_state,AppSettings& app_settings){
    ImGui::SetNextWindowSize(playlistSize,0);
    ImGui::SetNextWindowPos(playlistPos);
    ImGui::Begin("Playlists");

    if(ImGui::Button("[+] New Playlist")){
        app_state.show_new_playlist = true;
    }
    for(unsigned int i = 0; i <  Mp3Player::playlistsVec.size(); i++){
        std::string DbuttonStr="-##DButton";
        DbuttonStr += Mp3Player::playlistsVec[i].name + std::to_string(i);     
        std::string PbuttonStr ="##PButton";    
        std::string pstr = Mp3Player::playlistsVec[i].name  + "##PButton" + std::to_string(i);
        if( ImGui::Button(pstr.c_str() ) && app_state.edit_playlist == false){
            Mp3Player::loadDifferentPlaylist(i);
            LoadTextureFromFile(Mp3Player::currentPlaylist->image_path.c_str(),&currentPlaylist_image);//Eh this should work
            Mp3Player::decodeSongsPlaylist(Mp3Player::currentPlaylist);

        }
        ImGui::SameLine();
        if(ImGui::Button(DbuttonStr.c_str())&& app_state.edit_playlist == false){
           Mp3Player::removePlaylist(i); 
           AppStateManager::saveChangedState(app_settings.userdata_directory_path + "state.json",&app_state);
        }
    }
    ImGui::End();
}


void ClickableTableRow(const char* label, bool* selected,short* mouse_click) {
    if (ImGui::Selectable(label, *selected)) {
        *selected = !(*selected); // Toggle selection
    }
    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            (*mouse_click) = 1;
        }
        else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
        {
            (*mouse_click) = 2;
        }
    }
}


void renderEditPlaylist(ApplicationState& app_state,AppSettings& app_settings){
    if(app_state.edit_playlist){
        ImGui::SetNextWindowSize({SongsSize.x /2,SongsSize.y/2},0);
        ImGui::Begin( "edit Playlist",&app_state.edit_playlist);
        ImGui::InputText("Playlist Name:",name,256);
        ImGui::InputText("Playlist Description:",description,4096);
        ImGui::Text("Drag and Drop image here to set it as the playlist image");
        ImGui::Text("Playlist Image: ");
        ImGui::Image((void*)(intptr_t)(currentPlaylist_image.texture), PlaylistImageSize);    
        if(ImGui::Button("Save")){
            if(Mp3Player::currentPlaylist != nullptr){
                Mp3Player::currentPlaylist->name = name;
                Mp3Player::currentPlaylist->description = description;
            }
            saveAllChangesToPlaylists();
        }
        ImGui::End();
    }
}


// Simplify song select by removing the PlAY Button
void renderSongSelect(ApplicationState& app_state,AppSettings& app_settings){
    int res=0;
    ImGui::SetNextWindowSize(SongsSize,0);
    ImGui::SetNextWindowPos(SongsPos);
    // if resized then SetWindowSize and Posiion
    ImGui::Begin("Song Select");
    ImGui::Image((void*)(intptr_t)(currentPlaylist_image.texture), PlaylistImageSize);    
    std::string current_playlist_str=Mp3Player::getPlaylistName();
    ImGui::SameLine();

    if(Mp3Player::currentPlaylist != nullptr){
        ImGui::Text("Current Playlist: %s \nDescription: %s",current_playlist_str.c_str(),Mp3Player::currentPlaylist->description.c_str());
    }    
    
    if(ImGui::Button("Edit")){
        app_state.edit_playlist = true;
    }    
    ImGui::Columns(2, "##songselectColumns", false);
    ImGui::Text("Song / Artist");
    ImGui::NextColumn();
    ImGui::Text("Duration");
    ImGui::NextColumn();
    bool row_clicked=false;
    short mouse_click = 0; // no click = 0, 1 is left , 2 is right

    if(Mp3Player::currentPlaylist != nullptr){
        for(long unsigned int i = 0; i < Mp3Player::currentPlaylist->songs.size();i++){
            std::string PbuttonStr="##PButton";//## Gets ignored but this enables us to have an internal id
            PbuttonStr +=std::to_string(i);
            mouse_click = 0;
            ClickableTableRow(PbuttonStr.c_str(),&row_clicked,&mouse_click);            
            ImGui::SameLine();
            if(mouse_click == 0){
                //no op
            }            
            else if( mouse_click == 1){
               
                res=Mp3Player::playSongAtIndex(i);
                if(res < 0){
                    std::cout << "couldnt play Song at" << i << std::endl;
                }
                Mp3Player::startMusic();
            
            }
            else {
                std::cout << "remove: " << i << std::endl;
                Mp3Player::removeSong(i);
            }
            
            if(Mp3Player::currentPlaylist->songs[i].duration != 0){
                ImGui::Text("%s",Mp3Player::currentPlaylist->songs[i].songname.c_str());
            }else{
                ImGui::Text("%s","Error");
            }
            int col = ImGui::GetColumnOffset(-1);
            ImGui::SetColumnOffset(col,0);
            ImGui::SetColumnWidth(col,(float)(SongsSize.x-(SongsSize.x/7)));
            
            //Duration
            std::string formatted_minutes_seconds_str = Mp3Player::formatMinutesSeconds(Mp3Player::currentPlaylist->songs[i].duration);
            ImGui::NextColumn();
            ImGui::Text("%s",formatted_minutes_seconds_str.c_str());
            ImGui::SameLine();
            /*
            ImGui::NextColumn();
            if(ImGui::Button(DbuttonStr.c_str())){
                res = Mp3Player::removeSong(i);
            }*/
            ImGui::NextColumn();
        }
        
    }
    ImGui::End();
}

char playlistTempBuf[256];
// possible buffer overflow IF ImGui::InputText has a logic flaw letting users write more than 256 bytes
void renderNewPlaylist(ApplicationState& app_state,AppSettings& app_settings){
    if(app_state.show_new_playlist){
        ImGui::SetNextWindowBgAlpha(1.00f); // Transparent background
        ImGui::SetNextWindowSize({io->DisplaySize.x/4,io->DisplaySize.y/4},0);
        ImGui::Begin("New Playlist",&app_state.show_new_playlist);
        ImGui::InputText("Playlist Name:",playlistTempBuf,256);
        ImGui::InputText("Playlist Description:",description,4096);
        ImGui::Text("Drag and Drop image here to set it as the playlist image");
        ImGui::Text("Playlist Image: ");
        if( ImGui::Button("Create")){
            created_playlist.name = playlistTempBuf;
            created_playlist.path = app_settings.playlist_directory_path + playlistTempBuf + ".json";
            created_playlist.shouldLoop = false;
            created_playlist.description = description;
            Mp3Player::playlistsVec.push_back(created_playlist);
            save_changed_playlist(&created_playlist);
            Mp3Player::loadDifferentPlaylist(Mp3Player::playlistsVec.size()-1);
            app_state.show_new_playlist = false;
        }
        ImGui::End();
    }
}



void zero_buf(){
    for(int i = 0; i < 256;i++){
        playlistTempBuf[i] = 0;
    }
}

int initGui(){
    zero_buf();
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // 3.0+ only
#endif
window = glfwCreateWindow(windowWidth, windowHeight, "Mp3Player", nullptr, nullptr);
if (window == nullptr)
    return 1;
glfwMakeContextCurrent(window);
//glfwSwapInterval(1); // Enable vsync
// Setup Dear ImGui context
IMGUI_CHECKVERSION();
ImGui::CreateContext();
io = &ImGui::GetIO(); 
(void)io;
io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
   // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);
    glewInit();

    if(Mp3Player::currentPlaylist != nullptr){
        LoadTextureFromFile(Mp3Player::currentPlaylist->image_path.c_str(),&currentPlaylist_image);
    }
    glfwSetDropCallback(window,GLFWDropCallback);
    glfwSetWindowCloseCallback(window, glfw_exit_window_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return 0;
}
}