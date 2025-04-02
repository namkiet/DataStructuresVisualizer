#include "GUI/FileReader.hpp"

#ifdef _WIN32  // Windows
#include <windows.h>
#include <iostream>

std::wstring OpenFileDialog() {
    wchar_t originalDir[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, originalDir);

    OPENFILENAMEW ofn;
    wchar_t szFile[MAX_PATH] = {0};
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = L"All Files\0*.*\0";
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        std::wstring selectedFile(szFile);
        SetCurrentDirectoryW(originalDir);
        return selectedFile;
    }

    SetCurrentDirectoryW(originalDir);
    return L"";
}

#elif __APPLE__  // macOS
#include <iostream>
#import <AppKit/AppKit.h>

std::wstring OpenFileDialog() {
    @autoreleasepool {
        NSOpenPanel* openPanel = [NSOpenPanel openPanel];
        [openPanel setCanChooseFiles:YES];
        [openPanel setCanChooseDirectories:NO];
        [openPanel setAllowsMultipleSelection:NO];

        if ([openPanel runModal] == NSModalResponseOK) {
            NSURL* selectedFileURL = [[openPanel URLs] firstObject];
            if (selectedFileURL) {
                std::string path = std::string([[selectedFileURL path] UTF8String]);
                return std::wstring(path.begin(), path.end());
            }
        }
    }
    return L"";
}

#else  // Unsupported OS
#include <iostream>
std::wstring OpenFileDialog() {
    std::cerr << "OpenFileDialog is not supported on this platform." << std::endl;
    return L"";
}
#endif
