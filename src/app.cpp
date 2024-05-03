// This includes things that do not belong to gui logic or setup
#include "../include/app.hpp"
#include <thread>

int second;
int last_second;
AppSettings app_settings;
ApplicationState app_state;

int setupMp3Player(){
    Mp3Player::initMp3Player(0);
    Mp3Player::loadPlaylistsDir(app_settings.playlist_directory_path);
    Mp3Player::setVolume(app_state.volume);
    for(long unsigned int i = 0; i < Mp3Player::playlistsVec.size();i++){
        if(Mp3Player::playlistsVec[i].name == app_state.currentPlaylistName){
            Mp3Player::currentPlaylist = &Mp3Player::playlistsVec[i];
            Mp3Player::loadDifferentPlaylist(i);
            Mp3Player::decodeSongsPlaylist(Mp3Player::currentPlaylist);
            break; // break out of loop
        }
        }
        Mp3Player::decodeSongsPlaylist(Mp3Player::currentPlaylist);
    return 0;
}

void setEnemyMouseStyle(){
ImGuiStyle& style = ImGui::GetStyle();
style.Alpha = 1.0;
style.WindowRounding = 3;
style.GrabRounding = 1;
style.GrabMinSize = 20;
style.FrameRounding = 3;
style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
//style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);

style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
style.Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);

style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);

style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.21f, 0.73f);
style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);

style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.27f);
style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);

style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);

style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);

//style.Colors[ImGuiCol_ComboBg] = ImVec4(0.16f, 0.24f, 0.22f, 0.60f);
style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);

style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
style.Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.65f, 0.65f, 0.46f);

style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.01f, 1.00f, 1.00f, 0.43f);
style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.62f);

style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 1.00f, 1.00f, 0.33f);
style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.42f);

style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
//style.Colors[ImGuiCol_Column] = ImVec4(0.00f, 0.50f, 0.50f, 0.33f);
//style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.00f, 0.50f, 0.50f, 0.47f);
//style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.00f, 0.70f, 0.70f, 1.00f);
style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
//style.Colors[ImGuiCol_CloseButton] = ImVec4(0.00f, 0.78f, 0.78f, 0.35f);
//style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.00f, 0.78f, 0.78f, 0.47f);
//style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.00f, 0.78f, 0.78f, 1.00f);
style.Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);
//style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.00f, 0.13f, 0.13f, 0.90f);
//style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.04f, 0.10f, 0.09f, 0.51f);
}

ApplicationState* getAppStatePointer(){
    return (ApplicationState*)(&app_state);
}

int loadTheme(std::string path){
    //Load Json file here with Paths to the images
    //Later also enable editing of Imgui Theme through the json
    /*
    LoadTextureFromFile("",&playButton_img);
    LoadTextureFromFile("",&pauseButton_img);
    LoadTextureFromFile("",&prevButton_img);
    LoadTextureFromFile("",&exitButton_img);
    LoadTextureFromFile("",&skipButton_img);
    LoadTextureFromFile("",&settingsButton_img);
    LoadTextureFromFile("",&cutomizeButton_img);
    */
    ImGui::StyleColorsClassic();
    setEnemyMouseStyle();
    return 0;
}

inline bool checkMouseMoved(){
    bool moved = ( Mp3Gui::io->MousePosPrev.x == Mp3Gui::io->MousePos.x && Mp3Gui::io->MousePosPrev.y == Mp3Gui::io->MousePos.y );
    bool clicked = (Mp3Gui::io->MouseClicked[0] | Mp3Gui::io->MouseClicked[1] | Mp3Gui::io->MouseClicked[2] | Mp3Gui::io->MouseClicked[3] | Mp3Gui::io->MouseClicked[4] );
    return moved || clicked;
}

int run(std::string userdata_dir,std::string playlists_dir){
    app_settings.userdata_directory_path = userdata_dir;
    app_settings.playlist_directory_path = playlists_dir;

    int ret = AppSettingsManager::reloadSettings(userdata_dir+"settings.json",&app_settings);
    if(ret == -2){
        std::cerr << "reloadSettings Failed ?" << std::endl;
        ret = AppSettingsManager::reloadSettings(userdata_dir+"settings.json",&app_settings);
    }
    
    ret = AppStateManager::reloadState(userdata_dir+"state.json",&app_state);
    app_settings.playlist_directory_path = playlists_dir;
    if(ret == -2){ // Then state.json didnt exist, after the call it was created so reload again
        ret = AppStateManager::reloadState(userdata_dir+"state.json",&app_state);
    }
    ret = (int)setupMp3Player();
    Mp3Gui::initGui();
    Mp3Player::setVolume(app_state.volume);
    ret = Mp3Player::changeAudioDevice(app_settings.device_name);
    std::cout << "change Audio: "<<ret << std::endl;    
    loadTheme("theme.json");
    if(ret != 0){
        std::cout << "couldnt use audio device: " << app_settings.device_name<<std::endl; 
        Mp3Player::fallbackAudioDevice();
    }
    ImVec4 clear_color = ImVec4(0, 0, 0, 0.1f);
    loadTheme("theme.json");
    while (!glfwWindowShouldClose(Mp3Gui::window))
    {
        // if no input is given this reduces the cpu usage in release down to 0.6 - 1.3 with input coming in and music ~4%
        // cpu usage correlates HEAVILY with fps
        // 60 fps causes 8% cpu usage without music playing -> TODO let the user pick his poison with settings ... 
        // I dont really like the behaviour of glfwWaitEventsTimeout since it WAITS for timeout till it renders ANYTHING
        // AND glfwWaitEvents only renders when any kind of user input is coming in
        // A workaround would be to set ms_to_sleep:
        // std::this_thread::sleep_for(std::chrono::milliseconds(ms_to_sleep)); 
        /*

        if user hasnt moved mouse or OS Window lost focus, then increase ms_to_sleep
        This could be done gradually or hardcoded.
        hardcoded is probably better.
        A sensible default probably would be 15 fps
        Make that available as a setting to the user, 
        if the user wanted to he could set fps to 1 or even not render anything AT ALL if he so chooses
        */
        glfwPollEvents();
        // Lets keep it at that for now TODO: Custom FPS for inactive and active 
        if(!glfwGetWindowAttrib(Mp3Gui::window, GLFW_FOCUSED) || !checkMouseMoved()  ){
            std::this_thread::sleep_for(std::chrono::milliseconds(66));//15 fps   
        }else{
            std::this_thread::sleep_for(std::chrono::milliseconds(33));//30 fps   
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        Mp3Gui::updatePositionsAndSizes();
        Mp3Gui::renderPlaylists(app_state,app_settings);
        Mp3Gui::renderSettingsWindow(app_state,app_settings);
        Mp3Gui::renderCustomizeWindow(app_state,app_settings);
        Mp3Gui::renderSongSelect(app_state,app_settings);
        Mp3Gui::renderPlayer(app_state,app_settings);
        Mp3Gui::renderNewPlaylist(app_state,app_settings);
        Mp3Gui::renderEditPlaylist(app_state,app_settings);

        if(app_settings.debug){
            Mp3Gui::renderDebug(app_state,app_settings);
        }
        app_state.currentPlaylist = Mp3Player::currentPlaylist;
        app_state.currentSong = Mp3Player::currentSong;
        if(Mp3Player::currentPlaylist != nullptr){
            app_state.currentPlaylistName = app_state.currentPlaylist->name;
        }
        app_state.volume = Mp3Player::volume;
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(Mp3Gui::window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(Mp3Gui::window);
    }
    Mp3Gui::cleanup();
    AppStateManager::saveChangedState(userdata_dir + "state.json",&app_state);
    AppSettingsManager::saveChangedSettings(userdata_dir + "settings.json", &app_settings);
    return 0;
}