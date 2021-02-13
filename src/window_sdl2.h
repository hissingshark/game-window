#pragma once

#include <game_window.h>
#include <SDL2/SDL.h>

class SDL2GameWindow : public GameWindow {

private:
    SDL_Window *window = NULL;
    SDL_GLContext glcontext = NULL;
    int windowedWidth = -1, windowedHeight = -1;
/* how many actually needed?
    double lastMouseX = 0.0, lastMouseY = 0.0;
    int windowedX = -1, windowedY = -1;
    int relativeScale;
    bool focused = true;
    bool warnedButtons = false;
*/
//    friend class SDL2JoystickManager;

    static KeyCode getKeyMinecraft(int keyCode);
/*
    static void _sdl2WindowSizeCallback(SDL2window* window, int w, int h);
    static void _sdl2CursorPosCallback(SDL2window* window, double x, double y);
    static void _sdl2MouseButtonCallback(SDL2window* window, int button, int action, int mods);
    static void _sdl2ScrollCallback(SDL2window* window, double x, double y);
    static void _sdl2KeyCallback(SDL2window* window, int key, int scancode, int action, int mods);
    static void _sdl2CharCallback(SDL2window* window, unsigned int ch);
    static void _sdl2WindowCloseCallback(SDL2window* window);
    static void _sdl2WindowFocusCallback(SDL2window* window, int focused);
    static void _sdl2WindowContentScaleCallback(SDL2window* window, float scalex, float scaley);
*/
public:

    SDL2GameWindow(const std::string& title, int width, int height, GraphicsApi api);

    ~SDL2GameWindow() override;

    void setIcon(std::string const& iconPath) override;

    void show() override;

    void close() override;

    void pollEvents() override;

    void setCursorDisabled(bool disabled) override;

    void setFullscreen(bool fullscreen) override;

    void getWindowSize(int& width, int& height) const override;

    void setClipboardText(std::string const& text) override;

    void swapBuffers() override;

    void setSwapInterval(int interval) override;

};
