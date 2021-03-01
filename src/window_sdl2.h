#pragma once

#include <game_window.h>
#include <EGL/egl.h>
#include <SDL2/SDL.h>



class SDL2GameWindow : public GameWindow {

private:

struct framebuffer_state {
    int fd;
    char *data;
    int width;
    int height;
    char *bb;
    int data_size;
} fb;

struct egl_state {
    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
} egl;

struct cursor_state {
    char *image;
    int size;
    bool hidden;
    int x;
    int y;
} cursor;

    SDL2GameWindow *currentGameWindow;
//    window_state ws;

    void initFrameBuffer();
    void initEGL();
    void initSDL();
    void initCursor();
    void drawCursor();
    void handleMouseMotionEvent(SDL_MouseMotionEvent *motionevent);
    void handleMouseClickEvent(SDL_MouseButtonEvent *clickevent);
    void handleKeyboardEvent(SDL_KeyboardEvent *event);
    KeyCode getKeyMinecraft(int keyCode);

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
