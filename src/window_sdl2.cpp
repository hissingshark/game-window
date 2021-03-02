#include "window_sdl2.h"
#include "game_window_manager.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>

#define GL_GLEXT_PROTOTYPES 1
//#include <SDL2/SDL_opengles2.h>

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
    cursor.hidden = false;

    initFrameBuffer();
    initEGL();
    initSDL();
//    initCursor();
}

SDL2GameWindow::~SDL2GameWindow() {
    munmap (fb.data, fb.data_size);
    ::close(fb.fd);

    eglDestroySurface(egl.display, egl.surface);
    eglDestroyContext(egl.display, egl.context);
    eglMakeCurrent(egl.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    SDL_Quit();
}

void SDL2GameWindow::initFrameBuffer() {
    fb.fd = open("/dev/fb0", O_RDWR);
    if (fb.fd < 0) {
      printf("Failed to open fb0\n");
      return;
    }

    struct fb_var_screeninfo vinfo;

    ioctl (fb.fd, FBIOGET_VSCREENINFO, &vinfo);

    fb.width = vinfo.xres;
    fb.height = vinfo.yres;
    int fb_bpp = vinfo.bits_per_pixel;
    int fb_bytes = fb_bpp / 8;

    fb.data_size = fb.width * fb.height * fb_bytes * 2;
    int page_offset = (fb.height * fb.width * 4);

    fb.data = (char*)mmap (0, fb.data_size,
            PROT_READ | PROT_WRITE, MAP_SHARED, fb.fd, (off_t)0);
}

void SDL2GameWindow::initEGL() {
int x, y;
getWindowSize(x, y);
printf("Window is %d x %d\n", x, y);

// init EGL diplay, window , surface and context
        EGLConfig config;
        EGLint num_config;
        EGLint const attribute_list[] = {
            EGL_RED_SIZE, 1,
            EGL_GREEN_SIZE, 1,
            EGL_BLUE_SIZE, 1,
            EGL_ALPHA_SIZE, 1,
            EGL_DEPTH_SIZE, 1,
            EGL_STENCIL_SIZE, 8,
            EGL_NONE
        };

        egl.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        printf("EGL display = %d", egl.display);

        if (! eglInitialize(egl.display, NULL, NULL))
            printf("Failed to initialize EGL Display!");

        eglChooseConfig(egl.display, attribute_list, &config, 1, &num_config);

        if (! eglBindAPI(EGL_OPENGL_ES_API))
            printf("Failed to bind EGL API!");

        egl.context = eglCreateContext(egl.display, config, EGL_NO_CONTEXT, NULL);
        if (! egl.context)
            printf("Failed to create EGL Context!");

        egl.surface = eglCreateWindowSurface(egl.display, config, 0, NULL);
        if (! egl.surface)
            printf("Failed to create EGL Window Surface!");

        if (! eglMakeCurrent(egl.display, egl.surface, egl.surface, egl.context))
            printf("Failed to make EGL Context current!");


// include GL testing to find the backbuffer address
}

void SDL2GameWindow::initSDL() {
    if (SDL_Init(SDL_INIT_EVENTS) != 0) {
        SDL_Log("Unable to initialize SDL for input: %s", SDL_GetError());
//        return 1; // TODO is this how we handle errors? We cannot return from here
    }
}

void SDL2GameWindow::initCursor() {
// scale to 1/8th of the screen  height e.g h / 8 / 16

const char mpw = 16, mps = 8;
const char mpi[16][16][4] = {
{{6,61,51,255}, {6,61,51,255}, {6,61,51,255}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}},
{{2,32,26,255}, {165,253,240,255}, {165,253,240,255}, {6,61,51,255}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}},
{{2,32,26,255}, {165,253,240,255}, {38,200,174,255}, {165,253,240,255}, {6,61,51,255}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}},
{{0,0,0,0}, {2,32,26,255}, {165,253,240,255}, {38,200,174,255}, {165,253,240,255}, {6,61,51,255}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}},
{{0,0,0,0}, {0,0,0,0}, {2,32,26,255}, {47,236,204,255}, {38,200,174,255}, {165,253,240,255}, {6,61,51,255}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}},
{{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {2,32,26,255}, {47,236,204,255}, {38,200,174,255}, {165,253,240,255}, {6,61,51,255}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}},
{{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {2,32,26,255}, {47,236,204,255}, {38,200,174,255}, {47,236,204,255}, {6,61,51,255}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {6,61,51,255}, {6,61,51,255}, {0,0,0,0}, {0,0,0,0}},
{{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {2,32,26,255}, {47,236,204,255}, {38,200,174,255}, {47,236,204,255}, {6,61,51,255}, {0,0,0,0}, {6,61,51,255}, {13,99,84,255}, {6,61,51,255}, {0,0,0,0}, {0,0,0,0}},
{{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {2,32,26,255}, {47,236,204,255}, {24,139,120,255}, {47,236,204,255}, {2,32,26,255}, {24,139,120,255}, {6,61,51,255}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}},
{{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {2,32,26,255}, {47,236,204,255}, {13,99,84,255}, {24,139,120,255}, {24,139,120,255}, {6,61,51,255}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}},
{{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {2,32,26,255}, {6,61,51,255}, {13,99,84,255}, {6,61,51,255}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}},
{{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {2,32,26,255}, {6,61,51,255}, {6,61,51,255}, {2,32,26,255}, {104,77,24,255}, {71,51,13,255}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}},
{{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {2,32,26,255}, {6,61,51,255}, {2,32,26,255}, {2,32,26,255}, {0,0,0,0}, {35,24,3,255}, {138,103,34,255}, {71,51,13,255}, {0,0,0,0}, {0,0,0,0}},
{{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {2,32,26,255}, {2,32,26,255}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {35,24,3,255}, {104,77,24,255}, {6,61,51,255}, {6,61,51,255}},
{{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {2,32,26,255}, {13,99,84,255}, {6,61,51,255}},
{{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {2,32,26,255}, {2,32,26,255}, {2,32,26,255}}};

    int mx, my;
    SDL_GetMouseState(&mx, &my);

  for(int u=0; u<mpw; u++) {
    for(int v=0; v<mpw; v++) {
      if (!mpi[u][v][3])
        continue;
      for(int x=mx+(u*mps); x<mx+(u*mps)+mps; x++) {
        for(int y=my+(v*mps); y<my+(v*mps)+mps; y++) {
          int offset = (y * fb.width + x) * 4;
          fb.data [offset + 0] = mpi[u][v][2];
          fb.data [offset + 1] = mpi[u][v][1];
          fb.data [offset + 2] = mpi[u][v][0];
          fb.data [offset + 3] = 255; // alpha at max opacity to avoid a black pixel (rather than a transparent one)
        }
      }
    }
  }

}

void SDL2GameWindow::drawCursor() {
    // clip and draw pre-scaled image
}

void SDL2GameWindow::setIcon(std::string const& iconPath) {
    // NOOP - borderless window
}

void SDL2GameWindow::getWindowSize(int& width, int& height) const {
//    SDL_GetWindowSize(window, &width, &height);
    width = fb.width;
    height = fb.height;
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
    if (cursor.hidden) {
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
//        SDL_SetRelativeMouseMode(SDL_TRUE);
;
    else {
        // TODO test without - these might not do anything if you draw your own pointer as the system reports rel and abs anyway?
//        SDL_SetRelativeMouseMode(SDL_FALSE);
;
        // warp mouse to center for first display
        cursor.x = fb.width / 2;
        cursor.y = fb.height / 2;
    }

    cursor.hidden = disabled;
}

void SDL2GameWindow::setFullscreen(bool fullscreen) {
    // NOOP - always fullscreen
}

void SDL2GameWindow::setClipboardText(std::string const &text) {
    // NOOP - nowhere to cut/paste to/from without a desktop and other applications
}

void SDL2GameWindow::swapBuffers() {
printf("Swapping buffers!\n");
    if (! cursor.hidden) {
        glFinish();
        drawCursor();
    }

    eglSwapBuffers(egl.display, egl.surface);
}

void SDL2GameWindow::setSwapInterval(int interval) {
//    SDL_GL_SetSwapInterval(interval);
    eglSwapInterval(egl.display, interval);
}

// TODO fix QWERTY and numpad mapping.  ? use scancode instead of keysym?
KeyCode SDL2GameWindow::getKeyMinecraft(int keyCode) {
    if (keyCode >= SDLK_F1 && keyCode <= SDLK_F12)
        return (KeyCode) (keyCode - SDLK_F1 + (int) KeyCode::FN1);
    switch (keyCode) {
        case SDLK_BACKSPACE:
            return KeyCode::BACKSPACE;
        case SDLK_TAB:
            return KeyCode::TAB;
        case SDLK_RETURN:
            return KeyCode::ENTER;
        case SDLK_LSHIFT:
            return KeyCode::LEFT_SHIFT;
        case SDLK_RSHIFT:
            return KeyCode::RIGHT_SHIFT;
        case SDLK_LCTRL:
            return KeyCode::LEFT_CTRL;
        case SDLK_RCTRL:
            return KeyCode::RIGHT_CTRL;
        case SDLK_PAUSE:
            return KeyCode::PAUSE;
        case SDLK_CAPSLOCK:
            return KeyCode::CAPS_LOCK;
        case SDLK_ESCAPE:
            return KeyCode::ESCAPE;
        case SDLK_PAGEUP:
            return KeyCode::PAGE_UP;
        case SDLK_PAGEDOWN:
            return KeyCode::PAGE_DOWN;
        case SDLK_END:
            return KeyCode::END;
        case SDLK_HOME:
            return KeyCode::HOME;
        case SDLK_LEFT:
            return KeyCode::LEFT;
        case SDLK_UP:
            return KeyCode::UP;
        case SDLK_RIGHT:
            return KeyCode::RIGHT;
        case SDLK_DOWN:
            return KeyCode::DOWN;
        case SDLK_INSERT:
            return KeyCode::INSERT;
        case SDLK_DELETE:
            return KeyCode::DELETE;
        case SDLK_NUMLOCKCLEAR:
            return KeyCode::NUM_LOCK;
        case SDLK_SCROLLLOCK:
            return KeyCode::SCROLL_LOCK;
        case SDLK_SEMICOLON:
            return KeyCode::SEMICOLON;
        case SDLK_EQUALS:
            return KeyCode::EQUAL;
        case SDLK_COMMA:
            return KeyCode::COMMA;
        case SDLK_MINUS:
            return KeyCode::MINUS;
        case SDLK_PERIOD:
            return KeyCode::PERIOD;
        case SDLK_SLASH:
            return KeyCode::SLASH;
        case SDLK_BACKQUOTE:
            return KeyCode::GRAVE;
        case SDLK_LEFTBRACKET:
            return KeyCode::LEFT_BRACKET;
        case SDLK_BACKSLASH:
            return KeyCode::BACKSLASH;
        case SDLK_RIGHTBRACKET:
            return KeyCode::RIGHT_BRACKET;
        case SDLK_QUOTE:
            return KeyCode::APOSTROPHE;
        case SDLK_APPLICATION:
            return KeyCode::LEFT_SUPER;
        case SDLK_LALT:
            return KeyCode::LEFT_ALT;
        case SDLK_RALT:
            return KeyCode::RIGHT_ALT;
    }
/*
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
*/
    if (keyCode < 256)
        return (KeyCode) keyCode;
    return KeyCode::UNKNOWN;
}

