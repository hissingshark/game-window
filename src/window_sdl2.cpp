#include "window_sdl2.h"
#include "game_window_manager.h"

#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengles2.h>

#include <linux/fb.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

SDL2GameWindow::SDL2GameWindow(const std::string& title, int width, int height, GraphicsApi api) :
        GameWindow(title, width, height, api) {

    currentGameWindow = this;
    pointerHidden = false;
    window = NULL;
    glcontext = NULL;
    mousePointer = NULL;
    fb0 = NULL;

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) != 0) {
        SDL_Log("Unable to initialize SDL for video and events: %s", SDL_GetError());
//        return 1; // TODO is this how we handle errors? We cannot return from here
    }

/*
    if ((IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_Log("Unable to initialize SDL_image for png loading: %s", SDL_GetError());
//        return 1;
    }
*/
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    window = SDL_CreateWindow("Minecraft", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN);
    if (window == NULL) {
        printf("Could not create SDL window: %s\n", SDL_GetError());
//        return 1;
    }

    fb0 = SDL_GetWindowSurface(window);
    if (fb0 == NULL) {
        printf("DEBUG: Could not create SDL window surface (framebuffer): %s\n", SDL_GetError());
//        return 1;
    }

    // let's just show some classic code for reference
    mousePointer = SDL_LoadBMP("pointer.bmp"); // loads image

    glcontext = SDL_GL_CreateContext(window);
    if (glcontext == NULL) {
        printf("DEBUG: Could not create SDL glContext: %s\n", SDL_GetError());
//        return 1;
    }
/*
    mousePointer = IMG_LoadTexture(renderer, "./gfx/mousepointer.png");
    if (mousePointer == NULL) {
        printf("Could not load png for mouse pointer: %s\n", SDL_GetError());
//        return 1;
    }
*/
}

SDL2GameWindow::~SDL2GameWindow() {
    SDL_FreeSurface(mousePointer);
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDL2GameWindow::setIcon(std::string const& iconPath) {
    // NOOP - borderless window
}

void SDL2GameWindow::getWindowSize(int& width, int& height) const {
    SDL_GetWindowSize(window, &width, &height);
}

void SDL2GameWindow::show() {
    // NOOP - borderless window
}

void SDL2GameWindow::close() {
    // NOOP - borderless window
}

void SDL2GameWindow::pollEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_MOUSEMOTION:
                handleMouseMotionEvent(&event.motion);
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                handleMouseClickEvent(&event.button);
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                handleKeyboardEvent(&event.key);
                break;
            case SDL_QUIT:
                SDL_Quit();
                break;
            default:
                break;
        }
    }
}

void SDL2GameWindow::handleMouseMotionEvent(SDL_MouseMotionEvent *motionevent) {
    if (pointerHidden) {
        currentGameWindow->onMouseRelativePosition(motionevent->xrel, motionevent->yrel);
    }
    else {
        currentGameWindow->onMousePosition(motionevent->x, motionevent->y);
    }
}

void SDL2GameWindow::handleMouseClickEvent(SDL_MouseButtonEvent *clickevent) {
}

void SDL2GameWindow::handleKeyboardEvent(SDL_KeyboardEvent *keyevent) {
    KeyCode key = getKeyMinecraft(keyevent->keysym.scancode);

    KeyAction action;
    if (keyevent->repeat) {
        action = KeyAction::REPEAT;
    }
    else {
        switch (keyevent->state) {
            case SDL_PRESSED:
                action = KeyAction::PRESS;
                break;
            case SDL_RELEASED:
                action = KeyAction::RELEASE;
                break;
            default:
              return;
        }
    }

    currentGameWindow->onKeyboard(key, action);
}

void SDL2GameWindow::setCursorDisabled(bool disabled) {
    if (disabled)
        SDL_SetRelativeMouseMode(SDL_TRUE);
    else {
        // these might not do anything if you draw your own pointer as the system reports rel and abs anyway?
        SDL_SetRelativeMouseMode(SDL_FALSE);

        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        SDL_WarpMouseInWindow(window, w/2, h/2);
    }

    pointerHidden = disabled;
}

void SDL2GameWindow::setFullscreen(bool fullscreen) {
    // NOOP - always fullscreen
}

void SDL2GameWindow::setClipboardText(std::string const &text) {
    // NOOP - nowhere to cut/paste to/from without a desktop and other applications
}

void SDL2GameWindow::swapBuffers() {
// code to convert mousecoordinates to frustrum for vertex placement
//targetPosition.x = targetLeftMost + (sourcePosition.x / sourceWidth) * targetWidth

    if (pointerHidden) {
        SDL_GL_SwapWindow(window);
        return;
    }

    int fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd < 0) {
      printf("Failed to open fb0\n");
      return;
    }

    struct fb_var_screeninfo vinfo;

    ioctl (fbfd, FBIOGET_VSCREENINFO, &vinfo);

    int fb_width = vinfo.xres;
    int fb_height = vinfo.yres;
    int fb_bpp = vinfo.bits_per_pixel;
    int fb_bytes = fb_bpp / 8;

    int fb_data_size = fb_width * fb_height * fb_bytes *2;
    int page_offset = (fb_height * fb_width * 4);

    char *fbdata = (char*)mmap (0, fb_data_size,
            PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, (off_t)0);

    int mx, my;
    SDL_GetMouseState(&mx, &my);

  for(int u=0; u<mpw; u++) {
    for(int v=0; v<mpw; v++) {
      if (!mpi[u][v][3])
        continue;
      for(int x=mx+(u*mps); x<mx+(u*mps)+mps; x++) {
        for(int y=my+(v*mps); y<my+(v*mps)+mps; y++) {
          int offset = (y * fb_width + x) * 4;
          fbdata [offset + 0] = mpi[u][v][2];
          fbdata [offset + 1] = mpi[u][v][1];
          fbdata [offset + 2] = mpi[u][v][0];
          fbdata [offset + 3] = 255; // alpha at max opacity to avoid a black pixel (rather than a transparent one)
          fbdata [page_offset + offset + 0] = mpi[u][v][2];
          fbdata [page_offset + offset + 1] = mpi[u][v][1];
          fbdata [page_offset + offset + 2] = mpi[u][v][0];
          fbdata [page_offset + offset + 3] = 255; // alpha at max opacity to avoid a black pixel (rather than a transparent one)
        }
      }
    }
  }

    glFinish();
    SDL_GL_SwapWindow(window);

  for(int u=0; u<mpw; u++) {
    for(int v=0; v<mpw; v++) {
      if (!mpi[u][v][3])
        continue;
      for(int x=mx+(u*mps); x<mx+(u*mps)+mps; x++) {
        for(int y=my+(v*mps); y<my+(v*mps)+mps; y++) {
          int offset = (y * fb_width + x) * 4;
          fbdata [offset + 0] = mpi[u][v][2];
          fbdata [offset + 1] = mpi[u][v][1];
          fbdata [offset + 2] = mpi[u][v][0];
          fbdata [offset + 3] = 255; // alpha at max opacity to avoid a black pixel (rather than a transparent one)
          fbdata [page_offset + offset + 0] = mpi[u][v][2];
          fbdata [page_offset + offset + 1] = mpi[u][v][1];
          fbdata [page_offset + offset + 2] = mpi[u][v][0];
          fbdata [page_offset + offset + 3] = 255; // alpha at max opacity to avoid a black pixel (rather than a transparent one)
        }
      }
    }
  }

    munmap (fbdata, fb_data_size);
    ::close(fbfd);
}

void SDL2GameWindow::setSwapInterval(int interval) {
    SDL_GL_SetSwapInterval(interval);
}

// TODO fix QWERTY and numpad mapping.  ? use scancode instead of keysym?
KeyCode SDL2GameWindow::getKeyMinecraft(int keyCode) {
    if (keyCode >= SDL_SCANCODE_F1 && keyCode <= SDL_SCANCODE_F12)
        return (KeyCode) (keyCode - SDL_SCANCODE_F1 + (int) KeyCode::FN1);
    switch (keyCode) {
        case SDL_SCANCODE_BACKSPACE:
            return KeyCode::BACKSPACE;
        case SDL_SCANCODE_TAB:
            return KeyCode::TAB;
        case SDL_SCANCODE_RETURN:
            return KeyCode::ENTER;
        case SDL_SCANCODE_LSHIFT:
            return KeyCode::LEFT_SHIFT;
        case SDL_SCANCODE_RSHIFT:
            return KeyCode::RIGHT_SHIFT;
        case SDL_SCANCODE_LCTRL:
            return KeyCode::LEFT_CTRL;
        case SDL_SCANCODE_RCTRL:
            return KeyCode::RIGHT_CTRL;
        case SDL_SCANCODE_PAUSE:
            return KeyCode::PAUSE;
        case SDL_SCANCODE_CAPSLOCK:
            return KeyCode::CAPS_LOCK;
        case SDL_SCANCODE_ESCAPE:
            return KeyCode::ESCAPE;
        case SDL_SCANCODE_PAGEUP:
            return KeyCode::PAGE_UP;
        case SDL_SCANCODE_PAGEDOWN:
            return KeyCode::PAGE_DOWN;
        case SDL_SCANCODE_END:
            return KeyCode::END;
        case SDL_SCANCODE_HOME:
            return KeyCode::HOME;
        case SDL_SCANCODE_LEFT:
            return KeyCode::LEFT;
        case SDL_SCANCODE_UP:
            return KeyCode::UP;
        case SDL_SCANCODE_RIGHT:
            return KeyCode::RIGHT;
        case SDL_SCANCODE_DOWN:
            return KeyCode::DOWN;
        case SDL_SCANCODE_INSERT:
            return KeyCode::INSERT;
        case SDL_SCANCODE_DELETE:
            return KeyCode::DELETE;
        case SDL_SCANCODE_NUMLOCKCLEAR:
            return KeyCode::NUM_LOCK;
        case SDL_SCANCODE_SCROLLLOCK:
            return KeyCode::SCROLL_LOCK;
        case SDL_SCANCODE_SEMICOLON:
            return KeyCode::SEMICOLON;
        case SDL_SCANCODE_EQUALS:
            return KeyCode::EQUAL;
        case SDL_SCANCODE_COMMA:
            return KeyCode::COMMA;
        case SDL_SCANCODE_MINUS:
            return KeyCode::MINUS;
        case SDL_SCANCODE_PERIOD:
            return KeyCode::PERIOD;
        case SDL_SCANCODE_SLASH:
            return KeyCode::SLASH;
        case SDL_SCANCODE_GRAVE:
            return KeyCode::GRAVE;
        case SDL_SCANCODE_LEFTBRACKET:
            return KeyCode::LEFT_BRACKET;
        case SDL_SCANCODE_BACKSLASH:
            return KeyCode::BACKSLASH;
        case SDL_SCANCODE_RIGHTBRACKET:
            return KeyCode::RIGHT_BRACKET;
        case SDL_SCANCODE_APOSTROPHE:
            return KeyCode::APOSTROPHE;
        case SDL_SCANCODE_APPLICATION:
            return KeyCode::LEFT_SUPER;
        case SDL_SCANCODE_LALT:
            return KeyCode::LEFT_ALT;
        case SDL_SCANCODE_RALT:
            return KeyCode::RIGHT_ALT;
    }
    if (keyCode < 256)
        return (KeyCode) keyCode;
    return KeyCode::UNKNOWN;
}

