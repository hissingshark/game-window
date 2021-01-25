#define EGLUT_NO_X11_INCLUDE
#include "window_manager_fb.h"
#include "window_fb.h"
// we expect to handle inputs via SDL2
#include "joystick_manager_linux_gamepad.h"
#include <eglut.h>
#include <eglut_fb.h>

#include <unistd.h>
//#include <linux/limits.h>
#include <libgen.h>
#include <cstring>
#include <dlfcn.h>
#include <EGL/egl.h>


GameWindowManager::ProcAddrFunc dlsymGetProcAddress(const char* sym) {
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

FBWindowManager::FBWindowManager() {
    eglutInit(0, nullptr); // the args aren't really required and are troublesome to pass with this system
}

GameWindowManager::ProcAddrFunc FBWindowManager::getProcAddrFunc() {
    return (GameWindowManager::ProcAddrFunc) dlsymGetProcAddress;
}

std::shared_ptr<GameWindow> FBWindowManager::createWindow(const std::string& title, int width, int height,
                                                             GraphicsApi api) {
    return std::shared_ptr<GameWindow>(new FBWindow(title, width, height, api));
}

// we expect to handle inputs via SDL2
void FBWindowManager::addGamepadMappingFile(const std::string &path) {
    LinuxGamepadJoystickManager::instance.loadMappingsFromFile(path);
}

void FBWindowManager::addGamePadMapping(const std::string &content) {
    LinuxGamepadJoystickManager::instance.loadMappings(content);
}

// Define this window manager as the used one
std::shared_ptr<GameWindowManager> GameWindowManager::createManager() {
    return std::shared_ptr<GameWindowManager>(new FBWindowManager());
}
