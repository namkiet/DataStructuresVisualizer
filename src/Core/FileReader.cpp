#include "Core/FileReader.hpp"
#include <iostream>
#include <string>
#include <locale>
#include <cstdlib> // For mbstowcs
#include "Core/tinyfiledialogs.h"

std::wstring OpenFileDialog() {
    const char* filePath = tinyfd_openFileDialog(
        "Open File", "", 0, nullptr, nullptr, 0
    );

    if (filePath) {
        std::string utf8Path(filePath);
        size_t size = std::mbstowcs(nullptr, utf8Path.c_str(), 0) + 1;
        std::wstring wstr(size, L'\0');
        std::mbstowcs(&wstr[0], utf8Path.c_str(), size);
        return wstr;
    }
    return L"";
}
