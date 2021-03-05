#pragma once

#include <game_window.h>
#include <EGL/egl.h>
#include <SDL2/SDL.h>


struct bgra_pixel {
    // channels in framebuffer order
    char b;
    char g;
    char r;
    char a;
};


class SDL2GameWindow : public GameWindow {

private:

    struct framebuffer_state {
        int fd;
//        char *mmap;
        bgra_pixel *mmap;
        int mmap_size;
        int w;
        int h;
        bgra_pixel *backbuff;
    } fb;

    struct egl_state {
        EGLDisplay display;
        EGLContext context;
        EGLSurface surface;
    } egl;

    struct sdl_state {
        SDL_Window *window;
    } sdl;

    struct cursor_state {
        bgra_pixel **img;
        int size;
        bool hidden;
        int x;
        int y;
    } cursor;

    SDL2GameWindow *currentGameWindow;

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
