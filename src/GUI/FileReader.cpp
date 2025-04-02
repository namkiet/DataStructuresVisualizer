#include "GUI/FileReader.hpp"
#include <iostream>
#include <string>
#include <codecvt>
#include <locale>
#include "GUI/tinyfiledialogs.h"

std::wstring OpenFileDialog() {
    // Use tinyfiledialogs for all platforms
    const char* filePath = tinyfd_openFileDialog(
        "Open File",           // Title
        "",                    // Default path
        0,                     // Number of filter patterns
        nullptr,               // Filter patterns (nullptr for all files)
        nullptr,               // Single filter description
        0                      // Allow multiple selects (0 = no)
    );

    if (filePath) {
        // Convert UTF-8 char* to wstring
        std::string utf8Path(filePath);
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(utf8Path);
    }
    return L"";
}