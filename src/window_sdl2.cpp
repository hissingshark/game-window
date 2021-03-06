#include "window_sdl2.h"
#include "game_window_manager.h"

#include <EGL/egl.h>
//#include <EGL/eglext.h>
#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>
#include <SDL2/SDL.h>

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

    initFrameBuffer();
    initEGL();
    initSDL();
    initCursor();
}

SDL2GameWindow::~SDL2GameWindow() {
    munmap (fb.mmap, fb.mmap_size);
    ::close(fb.fd);
    delete cursor.img;

    eglDestroySurface(egl.display, egl.surface);
    eglDestroyContext(egl.display, egl.context);
    eglMakeCurrent(egl.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    SDL_DestroyWindow(sdl.window);
    SDL_Quit();
}

void SDL2GameWindow::initFrameBuffer() {
    // map framebuffer for drawing operations and to get dimensions
    fb.fd = open("/dev/fb0", O_RDWR);
    if (fb.fd < 0) {
      printf("Failed to open fb0!\n");
      return;
    }

    struct fb_var_screeninfo vinfo;

    ioctl (fb.fd, FBIOGET_VSCREENINFO, &vinfo);

    fb.w = vinfo.xres;
    fb.h = vinfo.yres;
    int fb_bpp = vinfo.bits_per_pixel;
    int fb_bytes = fb_bpp / 8;

    fb.mmap_size = fb.w * fb.h * fb_bytes * 2;

    fb.mmap = (bgra_pixel*)mmap (0, fb.mmap_size,
            PROT_READ | PROT_WRITE, MAP_SHARED, fb.fd, (off_t)0);
// this was for writing to both buffers, but we should be determining which is the back buffer?
//    int page_offset = (fb.h * fb.w * 4);
    fb.backbuff = fb.mmap; // for now - may be front, may be back
}

void SDL2GameWindow::initEGL() {
    // init EGL diplay, window, surface and context
    EGLConfig config;
    EGLint num_config;
    EGLint const config_attribute_list[] = {
        EGL_RED_SIZE, 1,
        EGL_GREEN_SIZE, 1,
        EGL_BLUE_SIZE, 1,
        EGL_ALPHA_SIZE, 1,
        EGL_DEPTH_SIZE, 1,
        EGL_STENCIL_SIZE, 8,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    EGLint const context_attribute_list[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    egl.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (eglGetError() != EGL_SUCCESS)
        printf("EGL error when getting display!\n");

    if (! eglInitialize(egl.display, NULL, NULL))
        printf("Failed to initialize EGL Display!\n");

    eglChooseConfig(egl.display, config_attribute_list, &config, 1, &num_config);
    if (eglGetError() != EGL_SUCCESS)
        printf("EGL error when choosing config!\n");

    if (! eglBindAPI(EGL_OPENGL_ES_API))
        printf("Failed to bind EGL API!\n");

    egl.context = eglCreateContext(egl.display, config, EGL_NO_CONTEXT, context_attribute_list);
    if (! egl.context)
        printf("Failed to create EGL Context!\n");

    egl.surface = eglCreateWindowSurface(egl.display, config, NULL, NULL);
    if (egl.surface == EGL_NO_SURFACE)
        printf("Failed to create EGL Window Surface!\n");

    if (! eglMakeCurrent(egl.display, egl.surface, egl.surface, egl.context))
        printf("Failed to make EGL Context current!\n");

// include GL testing to find the backbuffer address
}

void SDL2GameWindow::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) != 0) {
        SDL_Log("Unable to initialize SDL for input: %s", SDL_GetError());
//        return 1; // TODO is this how we handle errors? We cannot return from here
    }

//    sdl.window = SDL_CreateWindow("Minecraft", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, fb.w, fb.h, SDL_WINDOW_HIDDEN|SDL_WINDOW_FOREIGN);
    sdl.window = SDL_CreateWindowFrom(NULL);
    if (sdl.window == NULL) {
        printf("Could not create SDL window: %s\n", SDL_GetError());
//        return 1;
    }
}

void SDL2GameWindow::initCursor() {
    const char mcw = 16; // raw mouse cursor image width
    // rgba mouse cursor image data
    const char mci[16][16][4] = {
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

    // calculate scale factor to make mouse cursor image 1/8th of screen height
    const char mcs = fb.h / 8 / mcw;
    cursor.size = mcw * mcs; // store width for drawing operations later

    // allocate storage for scaled cursor image
    cursor.img =  new bgra_pixel*[mcw*mcs]; // allocate rows
    // allocate columns
    for(int row=0; row<(mcw*mcs); row++) {
        cursor.img[row] = new bgra_pixel[mcw * mcs];
    }

    // scale image into storage, remapping from rgba to bgra in the process
    for(int u=0; u<mcw; u++) {
        for(int v=0; v<mcw; v++) {
            bgra_pixel pix = { .b = mci[u][v][2], .g = mci[u][v][1], .r = mci[u][v][0], .a = mci[u][v][3] }; // remap raw to pixel
            // draw pixel as scaled square
            for(int x=u*mcs; x<(u*mcs)+mcs; x++) {
                for(int y=v*mcs; y<(v*mcs)+mcs; y++) {
                    cursor.img[x][y] = pix;
                }
            }
        }
    }

    setCursorDisabled(false);
}

void SDL2GameWindow::drawCursor() {
    // clip and draw pre-scaled cursor image to back surface of framebuffer at mouse position
    int mx = cursor.x, my = cursor.y;

    int clipw = fb.w - mx;
    if(clipw > cursor.size)
        clipw = cursor.size;

    int cliph = fb.h - my;
    if(cliph > cursor.size)
        cliph = cursor.size;

    for(int u=0; u<clipw; u++) {
        for(int v=0; v<cliph; v++) {
            if(cursor.img[u][v].a == 255)
                fb.backbuff[(my+v)*fb.w + (mx+u)] = cursor.img[u][v];
        }
    }
}

void SDL2GameWindow::setIcon(std::string const& iconPath) {
    // NOOP - borderless window
}

void SDL2GameWindow::getWindowSize(int& width, int& height) const {
//    SDL_GetWindowSize(window, &width, &height);
    width = fb.w;
    height = fb.h;
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
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                handleMouseClickEvent(&event.button);
                break;
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
        // provide own abs coords as SDL won't do this for a "hidden window"
        int tx = cursor.x + motionevent->xrel;
        cursor.x = tx < 0 ? 0 : (tx >= fb.w ? fb.w-1 : tx);
        int ty = cursor.y + motionevent->yrel;
        cursor.y = ty < 0 ? 0 : (ty >= fb.h ? fb.h-1 : ty);
        currentGameWindow->onMousePosition(cursor.x, cursor.y);
    }
}

void SDL2GameWindow::handleMouseClickEvent(SDL_MouseButtonEvent *clickevent) {
    currentGameWindow->onMouseButton(cursor.x, cursor.y, clickevent->button, (clickevent->state==SDL_PRESSED ? MouseButtonAction::PRESS : MouseButtonAction::RELEASE));
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
;
// really don't think this does anything anymore
//        SDL_SetRelativeMouseMode(SDL_TRUE);
    else {
        // TODO test without - these might not do anything if you draw your own pointer as the system reports rel and abs anyway?
//        SDL_SetRelativeMouseMode(SDL_FALSE);

        // warp mouse to center for first display
        cursor.x = fb.w / 2;
        cursor.y = fb.h / 2;
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
    if (! cursor.hidden) {
        glFinish();
        drawCursor();
    }

    eglSwapBuffers(egl.display, egl.surface);
}

void SDL2GameWindow::setSwapInterval(int interval) {
    eglSwapInterval(egl.display, interval);
}

// TODO fix QWERTY and numpad mapping.
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
    if (keyCode < 256)
        return (KeyCode) keyCode;
    return KeyCode::UNKNOWN;
*/
}

