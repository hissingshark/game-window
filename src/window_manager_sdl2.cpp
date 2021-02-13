#include "window_manager_sdl2.h"
#include "window_sdl2.h"
// we expect to handle inputs via SDL2
//#include "joystick_manager_linux_gamepad.h"

#include <unistd.h>
//#include <linux/limits.h>
#include <libgen.h>
#include <cstring>
#include <dlfcn.h>
#include <EGL/egl.h>


GameWindowManager::ProcAddrFunc dlsymGetProcAddress(const char* sym) {
//TODO must test SDL_GL_GetProcAddress as a possible replacement
    if (!sym)
        return NULL;

    void *eglFunc;

    // try official EGL method first
    eglFunc = (void*)eglGetProcAddress(sym);
    if (eglFunc)
        return (GameWindowManager::ProcAddrFunc)eglFunc;

    // manual fallback "If the EGL version is not 1.5 or greater, only queries of EGL and client API extension functions will succeed."
    void *libEGL;
    libEGL = dlopen("libEGL.so", RTLD_LAZY | RTLD_GLOBAL);
    if (!libEGL) {
        printf("Error: Unable to open libEGL.so for symbol processing");
        return NULL;
    }

    eglFunc = dlsym(libEGL, sym);
    dlclose(libEGL);

    return (GameWindowManager::ProcAddrFunc)eglFunc;
}

SDL2WindowManager::SDL2WindowManager() {
    /*
    nothing instantiated yet
    just a handle to:
      access the GL libraries via dlsymGetProcAddress()
      the future window
    */
}

GameWindowManager::ProcAddrFunc SDL2WindowManager::getProcAddrFunc() {
    return (GameWindowManager::ProcAddrFunc) dlsymGetProcAddress;
}

std::shared_ptr<GameWindow> SDL2WindowManager::createWindow(const std::string& title, int width, int height,
                                                             GraphicsApi api) {
    return std::shared_ptr<GameWindow>(new SDL2GameWindow(title, width, height, api));
}

// we expect to handle inputs via SDL2
void SDL2WindowManager::addGamepadMappingFile(const std::string &path) {
//    LinuxGamepadJoystickManager::instance.loadMappingsFromFile(path);
}

void SDL2WindowManager::addGamePadMapping(const std::string &content) {
//    LinuxGamepadJoystickManager::instance.loadMappings(content);
}

// Define this window manager as the used one
std::shared_ptr<GameWindowManager> GameWindowManager::createManager() {
    return std::shared_ptr<GameWindowManager>(new SDL2WindowManager());
}
