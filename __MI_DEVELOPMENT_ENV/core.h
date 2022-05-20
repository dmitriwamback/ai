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

#define MI_ENGINE_OPENGL

#include "../Mi-Font/registry.h"

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

            _vs.open(std::string("debug/").append(vertexShaderPath));
            _fs.open(std::string("debug/").append(fragmentShaderPath));
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

            shader.program = glCreateProgram();
            glAttachShader(shader.program, vertexShader);
            glAttachShader(shader.program, fragmentShader);
            glLinkProgram(shader.program);

            return shader;
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

        font = Mi::IO::Font::Create("fonts/Cinzel-Regular.ttf");
        InitQuad(font->characters['a']);

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

            RenderQuad(s);

            glfwPollEvents();
            glfwSwapBuffers(main_window);
        }
    }
}