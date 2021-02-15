#include "window_sdl2.h"
#include "game_window_manager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL2GameWindow::SDL2GameWindow(const std::string& title, int width, int height, GraphicsApi api) :
        GameWindow(title, width, height, api) {

    currentGameWindow = this;
    pointerHidden = true;
    window = NULL;
    glcontext = NULL;
    renderer = NULL;
    mousePointer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) != 0) {
        SDL_Log("Unable to initialize SDL for video and events: %s", SDL_GetError());
//        return 1; // TODO is this how we handle errors? We cannot return from here
    }

    if ((IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_Log("Unable to initialize SDL_image for png loading: %s", SDL_GetError());
//        return 1;
    }

    window = SDL_CreateWindow("Minecraft", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN);
    if (window == NULL) {
        printf("Could not create SDL window: %s\n", SDL_GetError());
//        return 1;
    }

    glcontext = SDL_GL_CreateContext(window);
    if (glcontext == NULL) {
        printf("Could not create SDL glContext: %s\n", SDL_GetError());
//        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Could not create SDL renderer: %s\n", SDL_GetError());
//        return 1;
    }

    mousePointer = IMG_LoadTexture(renderer, "./gfx/mousepointer.png");
    if (mousePointer == NULL) {
        printf("Could not load png for mouse pointer: %s\n", SDL_GetError());
//        return 1;
    }
}

SDL2GameWindow::~SDL2GameWindow() {
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
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                handleKeyboardEvent(&event.key);
printf("Done.\n");
                break;
            default:
                break;
        }
    }
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
    printf("Pointer: ");
    if (disabled)
        printf("DISABLED\n");
    else
        printf("ENABLED\n");
    pointerHidden = disabled;
}

void SDL2GameWindow::setFullscreen(bool fullscreen) {
    // NOOP - always fullscreen
}

void SDL2GameWindow::setClipboardText(std::string const &text) {
    // NOOP - nowhere to cut/paste to/from without a desktop and other applications
}

void SDL2GameWindow::swapBuffers() {
    // TODO is this the best time to composite in our mouse pointer when cursor enabled in menus?
    SDL_Rect dest;
    dest.x = 500;
    dest.y = 500;
    SDL_QueryTexture(mousePointer, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, mousePointer, NULL, &dest);

    SDL_GL_SwapWindow(window);
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
