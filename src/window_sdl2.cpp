#include "window_sdl2.h"
#include "game_window_manager.h"

#include <SDL2/SDL.h>
//#include "joystick_manager_sdl2.h"

// from glfw
//#include <codecvt>
//#include <iomanip>
//#include <thread>
//#include <math.h>

// from EGLUT
//#include <cstring>
//#include <sstream>

SDL2GameWindow::SDL2GameWindow(const std::string& title, int width, int height, GraphicsApi api) :
        GameWindow(title, width, height, api), windowedWidth(width), windowedHeight(height) {

    // TODO should SDL find the native resolution to set?
    if (SDL_Init(SDL_INIT_VIDEO) != 0) { // TODO could init other subsystems now
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
//        return 1; // TODO is this how we handle errors? We cannot return from here
    }

    window = SDL_CreateWindow("Minecraft", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN);
    if (window == NULL) {
        printf("Could not create SDL window: %s\n", SDL_GetError());
//        return 1;
    }
// TODO context and surface creation - do these need to be shared externally?  I don't think so as the context is globally set
    glcontext = SDL_GL_CreateContext(window);

/*
    sdl2DefaultWindowHints();
    if (api == GraphicsApi::OPENGL_ES2) {
        sdl2WindowHint(SDL2_CLIENT_API, SDL2_OPENGL_ES_API);
        sdl2WindowHint(SDL2_CONTEXT_CREATION_API, SDL2_EGL_CONTEXT_API);
        sdl2WindowHint(SDL2_CONTEXT_VERSION_MAJOR, 2);
        sdl2WindowHint(SDL2_CONTEXT_VERSION_MINOR, 0);
    } else if (api == GraphicsApi::OPENGL) {
        sdl2WindowHint(SDL2_CONTEXT_VERSION_MAJOR, 3);
        sdl2WindowHint(SDL2_CONTEXT_VERSION_MINOR, 2);
        sdl2WindowHint(SDL2_OPENGL_FORWARD_COMPAT, GL_TRUE);
        sdl2WindowHint(SDL2_OPENGL_PROFILE, SDL2_OPENGL_CORE_PROFILE);
    }
    window = sdl2CreateWindow(width, height, title.c_str(), nullptr, nullptr);
    sdl2SetWindowUserPointer(window, this);
    sdl2SetFramebufferSizeCallback(window, _sdl2WindowSizeCallback);
    sdl2SetCursorPosCallback(window, _sdl2CursorPosCallback);
    sdl2SetMouseButtonCallback(window, _sdl2MouseButtonCallback);
    sdl2SetScrollCallback(window, _sdl2ScrollCallback);
    sdl2SetWindowCloseCallback(window, _sdl2WindowCloseCallback);
    sdl2SetKeyCallback(window, _sdl2KeyCallback);
    sdl2SetCharCallback(window, _sdl2CharCallback);
    sdl2SetWindowFocusCallback(window, _sdl2WindowFocusCallback);
    sdl2SetWindowContentScaleCallback(window, _sdl2WindowContentScaleCallback);
    sdl2MakeContextCurrent(window);

    setRelativeScale();
*/
}

SDL2GameWindow::~SDL2GameWindow() {
/*
    SDL2JoystickManager::removeWindow(this);
    sdl2DestroyWindow(window);
*/
}

void SDL2GameWindow::setIcon(std::string const& iconPath) {
    // NOOP - borderless window
}

void SDL2GameWindow::getWindowSize(int& width, int& height) const {
    SDL_GetWindowSize(window, &width, &height);
}

void SDL2GameWindow::show() {
/* TODO is it not always shown? Might be a NOOP
    SDL2JoystickManager::addWindow(this);
    sdl2ShowWindow(window);
*/
}

void SDL2GameWindow::close() {
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDL2GameWindow::pollEvents() {
}

void SDL2GameWindow::setCursorDisabled(bool disabled) {
    // TODO
}

void SDL2GameWindow::setFullscreen(bool fullscreen) {
    // NOOP - always fullscreen
}

void SDL2GameWindow::setClipboardText(std::string const &text) {
    // TODO ?
}

void SDL2GameWindow::swapBuffers() {
    // TODO is this the best time to composite in our mouse pointer when cursor enabled in menus?
    SDL_GL_SwapWindow(window);
}

void SDL2GameWindow::setSwapInterval(int interval) {
    SDL_GL_SetSwapInterval(interval);
}

/*
void SDL2GameWindow::_sdl2WindowSizeCallback(SDL2window* window, int w, int h) {
    SDL2GameWindow* user = (SDL2GameWindow*) sdl2GetWindowUserPointer(window);
    user->onWindowSizeChanged(w, h);
}

void SDL2GameWindow::_sdl2CursorPosCallback(SDL2window* window, double x, double y) {
    SDL2GameWindow* user = (SDL2GameWindow*) sdl2GetWindowUserPointer(window);

    if (sdl2GetInputMode(window, SDL2_CURSOR) == SDL2_CURSOR_DISABLED) {
        double dx = (x - user->lastMouseX) * user->getRelativeScale();
        double dy = (y - user->lastMouseY) * user->getRelativeScale();

        user->onMouseRelativePosition(dx, dy);
        user->lastMouseX = x;
        user->lastMouseY = y;
    } else {
        x *= user->getRelativeScale();
        y *= user->getRelativeScale();

        user->onMousePosition(x, y);
    }
}

void SDL2GameWindow::_sdl2MouseButtonCallback(SDL2window* window, int button, int action, int mods) {
    SDL2GameWindow* user = (SDL2GameWindow*) sdl2GetWindowUserPointer(window);
    double x, y;
    sdl2GetCursorPos(window, &x, &y);

    x *= user->getRelativeScale();
    y *= user->getRelativeScale();

    user->onMouseButton(x, y, button + (button > SDL2_MOUSE_BUTTON_3 ? 5 : 1), action == SDL2_PRESS ? MouseButtonAction::PRESS : MouseButtonAction::RELEASE);
}

void SDL2GameWindow::_sdl2ScrollCallback(SDL2window* window, double x, double y) {
    SDL2GameWindow* user = (SDL2GameWindow*) sdl2GetWindowUserPointer(window);
    double cx, cy;
    sdl2GetCursorPos(window, &cx, &cy);
    user->onMouseScroll(cx, cy, x, y);
}

*/
// TODO one universally required function, but codes will need to be checked for SDL2 compatability
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
//        case SDLK_RIGHT_SUPER:
//            return KeyCode::RIGHT_SUPER;
        case SDLK_LALT:
            return KeyCode::LEFT_ALT;
        case SDLK_RALT:
            return KeyCode::RIGHT_ALT;
    }
    if (keyCode < 256)
        return (KeyCode) keyCode;
    return KeyCode::UNKNOWN;
}

/*
void SDL2GameWindow::_sdl2KeyCallback(SDL2window* window, int key, int scancode, int action, int mods) {
    SDL2GameWindow* user = (SDL2GameWindow*) sdl2GetWindowUserPointer(window);
#ifdef __APPLE__
    if (action == SDL2_PRESS && mods & SDL2_MOD_SUPER && key == SDL2_KEY_V) {
#else
    if (action == SDL2_PRESS && mods & SDL2_MOD_CONTROL && key == SDL2_KEY_V) {
#endif
        user->onPaste(sdl2GetClipboardString(window));
    }
    if (action == SDL2_PRESS || action == SDL2_REPEAT) {
        if (key == SDL2_KEY_BACKSPACE)
            user->onKeyboardText("\x08");
        if (key == SDL2_KEY_ENTER)
            user->onKeyboardText("\n");
    }
    KeyAction enumAction = (action == SDL2_PRESS ? KeyAction::PRESS :
                            (action == SDL2_REPEAT ? KeyAction::REPEAT : KeyAction::RELEASE));
    auto minecraftKey = getKeyMinecraft(key);
    if (key != SDL2_KEY_UNKNOWN && minecraftKey != KeyCode::UNKNOWN) {
        user->onKeyboard(minecraftKey, enumAction);
    } else {
        if (!user->warnedButtons) {
            user->warnedButtons = true;
            GameWindowManager::getManager()->getErrorHandler()->onError("SDL2 Unknown Key", "Please check your Keyboard Layout. Falling back to scancode for unknown Keys.");
        }
        user->onKeyboard((KeyCode) scancode, enumAction);
    }

}

void SDL2GameWindow::_sdl2CharCallback(SDL2window* window, unsigned int ch) {
    SDL2GameWindow* user = (SDL2GameWindow*) sdl2GetWindowUserPointer(window);
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt;
    user->onKeyboardText(cvt.to_bytes(ch));
}

void SDL2GameWindow::_sdl2WindowCloseCallback(SDL2window* window) {
    SDL2GameWindow* user = (SDL2GameWindow*) sdl2GetWindowUserPointer(window);
    sdl2SetWindowShouldClose(window, SDL2_FALSE);
    user->onClose();
}

void SDL2GameWindow::_sdl2WindowFocusCallback(SDL2window* window, int focused) {
    SDL2GameWindow* user = (SDL2GameWindow*) sdl2GetWindowUserPointer(window);
    SDL2JoystickManager::onWindowFocused(user, focused == SDL2_TRUE);
    user->focused = (focused == SDL2_TRUE);
}

void SDL2GameWindow::_sdl2WindowContentScaleCallback(SDL2window* window, float scalex, float scaley) {
    SDL2GameWindow* user = (SDL2GameWindow*) sdl2GetWindowUserPointer(window);
    user->setRelativeScale();
}
*/
