#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>

void compile_and_attach(GLuint program, GLenum ty, const char* path) {
  GLuint shader = glCreateShader(ty);
  char* content;
  {
    FILE* file = fopen(path, "r");
    long len;
    fseek (file , 0 , SEEK_END);
    len = ftell (file);
    rewind (file);

    content = (char*) malloc (sizeof(char)*(len + 1));
    content[len] = 0;
    if (content == NULL) { fputs ("Memory error",stderr); exit (2); }

    size_t result = fread (content,1,len,file);
    if (result != len) {fputs ("Reading error",stderr); exit (3);}
    fclose(file);
  }

  // Attempt to compile the shader
  glShaderSource(shader, 1, &content, NULL);
  glCompileShader(shader);

  // Get the compile status
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

  if (status != GL_TRUE ){
    fputs("error compiling shader", stderr);
    exit(4);
  }
  glAttachShader(program, shader);
}

void link_shader(GLuint program) {
  glLinkProgram(program);

  // Get the link status
  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);

  if (status != GL_TRUE) {
    fputs("failed to link shader", stderr);
    exit(5);
  };
}

int main() {
  SDL_Window *mainwindow; /* Our window handle */
  SDL_GLContext maincontext; /* Our opengl context handle */

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fputs("init failed", stderr);
    exit(6);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  /* Create our window centered at 512x512 resolution */
  mainwindow = SDL_CreateWindow("shadertest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if (!mainwindow) {
    fputs("Unable to create window", stderr);
    exit(7);
  }

  /* Create our opengl context and attach it to our window */
  maincontext = SDL_GL_CreateContext(mainwindow);

  if (!maincontext) {
    fputs("Unable to create context", stderr);
    exit(8);
  }

  /* This makes our buffer swap syncronized with the monitor's vertical refresh */
  SDL_GL_SetSwapInterval(1);

  GLuint texture_shader = glCreateProgram();
  compile_and_attach(texture_shader, GL_VERTEX_SHADER, "shaders/points.vert");
  compile_and_attach(texture_shader, GL_GEOMETRY_SHADER, "shaders/points.geom");
  compile_and_attach(texture_shader, GL_FRAGMENT_SHADER, "shaders/points.frag");
  link_shader(texture_shader);

  GLuint color_shader = glCreateProgram();
  compile_and_attach(color_shader, GL_VERTEX_SHADER, "shaders/line.vert");
  compile_and_attach(color_shader, GL_FRAGMENT_SHADER, "shaders/line.frag");
  link_shader(color_shader);

  GLuint empty_array;
  glGenVertexArrays(1, &empty_array);

  glBindVertexArray(empty_array);

  if(glGetError() != GL_NO_ERROR) {
    fputs("OpenGL error in loading", stderr);
    exit(6);
  }

  for(;;) {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT) {
        exit(0);
      }
    }

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(texture_shader);
    glDrawArrays(GL_POINTS, 0, 1);

    // If you remove this, the triangle renders green.
    glUseProgram(color_shader);
    // If you remove this, the triangle renders green.
    glDrawArrays(GL_LINES, 0, 2);

    SDL_GL_SwapWindow(mainwindow);

    glFlush();
    glFinish();
  }
}
