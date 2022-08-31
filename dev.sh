LIBS=vendor/OSX
INCLUDES=vendor/OSX/include

g++ __MI_DEVELOPMENT_ENV/main.cpp -I $INCLUDES -framework OpenGL -framework OpenAL $(freetype-config --libs) \
                                  -I vendor/include \
                                  -L $LIBS/glfw $LIBS/glfw/libglfw.3.3.dylib \
                                     $LIBS/glfw/libglfw.3.dylib \
                                     $LIBS/glfw/libglfw.dylib \
                                  -L $LIBS/glew $LIBS/glew/libGLEW.2.2.0.dylib \
                                     $LIBS/glew/libGLEW.2.2.dylib \
                                     $LIBS/glew/libGLEW.dylib \
                                     $LIBS/glew/libGLEW.a \
                                  -L $LIBS/sndfile $LIBS/sndfile/scUBlibsndfile.a \
                                  && ./a.out