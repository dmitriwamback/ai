#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>

#include "../errors.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define MI_ENGINE_OPENGL

#if defined(_WIN32)
    #if defined(_WIN64)
        #define MI_WINDOWS_IMPLEMENTATION
    #else
        #error "MI::ERROR::PLATFORM: Cannot run x86"
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #define MI_APPLE_IMPLEMENTATION

#elif defined(__linux__)
    #define MI_LINUX_IMPLEMENTATION

#else
    #error "MI::ERROR::PLATFORM: Unknown platform. Cannot support unknown platform."
#endif

#include "../Mi-Font/registry.h"
#include "../Mi-Audio/registry.h"

namespace __MI_DEVELOPMENT_ENVIRONMENT {

    class Shader {
        public:
        int program;
        std::string shaderName;

        static Shader Create(const char* vertexShaderPath, const char* fragmentShaderPath, std::string name) {
            

            Shader shader = Shader();
            shader.shaderName = name;

            int vertexShader = glCreateShader(GL_VERTEX_SHADER);
            int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

            std::ifstream _vs, _fs;
            std::stringstream vss, fss;

            _vs.open(std::string("__MI_DEBUG_debug/").append(vertexShaderPath));
            _fs.open(std::string("__MI_DEBUG_debug/").append(fragmentShaderPath));
            vss << _vs.rdbuf();
            fss << _fs.rdbuf();

            std::string _vss = vss.str();
            std::string _fss = fss.str();

            const char* _vSRC = _vss.c_str();
            const char* _fSRC = _fss.c_str();

            glShaderSource(vertexShader, 1, &_vSRC, NULL);
            glShaderSource(fragmentShader, 1, &_fSRC, NULL);
            glCompileShader(vertexShader);
            glCompileShader(fragmentShader);

            compileShader(vertexShader);
            compileShader(fragmentShader);

            shader.program = glCreateProgram();
            glAttachShader(shader.program, vertexShader);
            glAttachShader(shader.program, fragmentShader);
            glLinkProgram(shader.program);

            return shader;
        }

        static void compileShader(int shader) {
            int success;
            char infoLog[1024];

            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILE_ERROR of type: " << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }

        void use() {
            glUseProgram(program);
        }
    };
}

#include "quad.h"
GLFWwindow* main_window;

namespace __MI_DEVELOPMENT_ENVIRONMENT {


    Mi::IO::Font* font;

    void Begin() {

        if (!glfwInit()) std::cout << "couldn't initialize GLFW\n";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

        glfwWindowHint(GLFW_SAMPLES, 4);
        main_window = glfwCreateWindow(1200, 800, "name", NULL, NULL);
        glfwMakeContextCurrent(main_window);

        glewExperimental = GL_TRUE;
        glewInit();
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);
    }

    void MainLoop() {

        Shader s = Shader::Create("vMain.glsl", "fMain.glsl", "Shader");

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

        font = Mi::IO::Font::Create("__MI_DEBUG_fonts/RobotoFlex-Regular.ttf");
        InitQuad(font);

        float a = 0;

        // AUDIO
        Mi::Audio::AudioPlayer player;
        Mi::Audio::AudioSource source = Mi::Audio::AudioSource::Create("__MI_DEBUG_audio/drm2.mp3", "AUDIO_1");
        player.AddSource(source);
        alSourcePlay(source.GetBuffer());

        while (!glfwWindowShouldClose(main_window)) {

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0, 0.0, 0.0, 0.0);

            int width;
            int height;
            glfwGetWindowSize(main_window, &width, &height);

#if defined(__APPLE__)
            if (width > height) glViewport(0, -abs(width-height), width*2, width*2);
            else glViewport(-abs(width-height), 0, height*2, height*2);
#else   
            if (width > height) glViewport(0, -abs(width-height)/2, width, width);
            else glViewport(-abs(width-height)/2, 0, height, height);
#endif

            // TEXT RENDERING
            RenderQuad(s, "YEAR: " + std::to_string((int)a), glm::vec2(0.f));


            a += 0.1f;

            glfwPollEvents();
            glfwSwapBuffers(main_window);
        }
    }
}