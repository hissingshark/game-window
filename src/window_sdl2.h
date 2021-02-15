#pragma once

#include <game_window.h>
#include <SDL2/SDL.h>

class SDL2GameWindow : public GameWindow {

private:
    static SDL2GameWindow *currentGameWindow;
    bool focused = true;
    SDL_Window *window = NULL;
    SDL_GLContext glcontext = NULL;
//    int windowedWidth = -1, windowedHeight = -1;

    static void handleKeyboardEvent(SDL_KeyboardEvent *event);
    static KeyCode getKeyMinecraft(int keyCode);

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
