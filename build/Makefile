#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install libglfw-dev
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
#

#CXX = g++
CXX = clang++-16
#CXX = x86_64-w64-mingw32-g++
EXE = mp3.out
IMGUI_DIR = ../include/imgui
INCLUDE_DIR = ../include
SRC_DIR = ../src
SOURCES =$(SRC_DIR)/main.cpp $(SRC_DIR)/mp3player.cpp $(SRC_DIR)/playlist.cpp $(SRC_DIR)/miniaudio.cpp $(SRC_DIR)/appstate_and_settings.cpp $(SRC_DIR)/caching.cpp $(SRC_DIR)/app.cpp $(SRC_DIR)/gui.cpp 
#$(SRC_DIR)/api.cpp 

SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL -lGLEW
CXXFLAGS = -std=c++20 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends
CXXFLAGS +=  -Wall -Wformat -O2 -fno-char8_t
#-DSTATIC_GLFW3
LIBS =
STATIC_LBIS = libglfw3_win32.a
GLFW_INCLUDE = $(INCLUDE_DIR)/glfw/include
##---------------------------------------------------------------------
## OPENGL ES
##---------------------------------------------------------------------

## This assumes a GL ES library available in the system, e.g. libGLESv2.so
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2
# LINUX_GL_LIBS = -lGLESv2

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	#LIBS += -lglfw3
	LIBS += -lglfw

	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(CXX), x86_64-w64-mingw32-g++)
	ECHO_MESSAGE = "MinGW"

	CXXFLAGS += -I$(GLFW_INCLUDE)
	LIBS += $(INCLUDE_DIR)/$(STATIC_LBIS) -lgdi32 -lopengl32 -limm32
	CFLAGS = $(CXXFLAGS)
endif



##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:$(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
