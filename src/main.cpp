#include <iostream>
#include <string>
#include <filesystem>

// OS Specific headers
#ifdef _WIN32
    #include <windows.h>
#elif __linux__
    #include <unistd.h>
    #include <limits.h>
#endif


#include "mother_window.h"

std::filesystem::path get_executable_dir() {
    char buffer[1024];
    std::string path = "";

    #ifdef _WIN32
        if (GetModuleFileNameA(NULL, buffer, sizeof(buffer))) {
            path = buffer;
        }
    #elif __linux__
        ssize_t count = readlink("/proc/self/exe", buffer, sizeof(buffer));
        if (count != -1) {
            path = std::string(buffer, count);
        }
    #endif

    if (path.empty()) return std::filesystem::current_path(); // Fallback

    return std::filesystem::path(path).parent_path();
}

int main()
{
    auto ui = MotherWindow::create();
    ui->run();

    const std::filesystem::path DB_PATH = get_executable_dir().parent_path() / "data" / "dictionary.db";

    return 0;
}
