#include <iostream>
#include <filesystem>

// Solution via different platforms
#if defined(_WIN32)
    #include <windows.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
#elif defined(__linux__)
    #include <unistd.h>
#endif

std::filesystem::path getExecutablePath() {
    char pathBuffer[1024];

#if defined(_WIN32)
    GetModuleFileNameA(NULL, pathBuffer, sizeof(pathBuffer));
#elif defined(__APPLE__)
    uint32_t size = sizeof(pathBuffer);
    _NSGetExecutablePath(pathBuffer, &size);
#elif defined(__linux__)
    ssize_t count = readlink("/proc/self/exe", pathBuffer, sizeof(pathBuffer));
    if (count != -1) {
        pathBuffer[count] = '\0';
    }
#endif

    return std::filesystem::path(pathBuffer).parent_path();  // Return directory of the executable
}