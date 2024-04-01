#ifndef GUI
#define GUI
#include "../include/imgui/imgui.h"
#include "../include/imgui/imconfig.h"
#include "../include/imgui/imgui_impl_glfw.h"
#include "../include/imgui/imgui_impl_opengl3.h"
#include <vector>
#include <string>
#include "../include/mp3player.hpp"
#include "../include/appstate_and_settings.hpp"
#include <iostream>
#include <bits/stdc++.h>
#include "../include/glm/glm/glm.hpp"
#include "../include/glew.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif


namespace Mp3Gui {
extern GLFWwindow * window;
extern ImGuiIO* io;
extern int windowWidth;
extern int windowHeight;
struct stb_IMAGE{
    GLuint texture;
    int w;
    int h;
};

extern stb_IMAGE playButton_img;
extern stb_IMAGE pauseButton_img;
extern stb_IMAGE prevButton_img;
extern stb_IMAGE exitButton_img;
extern stb_IMAGE skipButton_img;
extern stb_IMAGE settingsButton_img;
extern stb_IMAGE cutomizeButton_img;
extern stb_IMAGE currentPlaylist_image;
extern void renderSettingsWindow(ApplicationState& app_state,AppSettings& app_settings);
extern void renderCustomizeWindow(ApplicationState& app_state,AppSettings& app_settings);
extern void renderPlayer(ApplicationState& app_state,AppSettings& app_settings);
extern void renderDebug(ApplicationState& app_state,AppSettings& app_settings);
extern void renderPlaylists(ApplicationState& app_state,AppSettings& app_settings);
extern void renderSongSelect(ApplicationState& app_state,AppSettings& app_settings);
extern void renderSidebar(ApplicationState& app_state,AppSettings& app_settings);
extern void renderNewPlaylist(ApplicationState& app_state,AppSettings& app_settings);
extern void renderEditPlaylist(ApplicationState& app_state,AppSettings& app_settings);
extern void cleanup();
extern int initGui();
extern void updatePositionsAndSizes();

}
#endif
