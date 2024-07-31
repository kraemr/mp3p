#include <GLFW/glfw3.h>
namespace Mp3PlayerImage {
    static float image_ratio = 1;
    struct stb_IMAGE{
        GLuint texture;
        int w;
        int h;
    };
    bool LoadTextureFromFile(const char* filename,struct stb_IMAGE* image);
    bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
}