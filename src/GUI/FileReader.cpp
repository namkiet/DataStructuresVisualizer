#include "GUI/FileReader.hpp"

std::wstring OpenFileDialog() {
    // Save the current working directory
    wchar_t originalDir[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, originalDir); 

    // Load the comdlg32.dll library
    HMODULE hComdlg32 = LoadLibraryW(L"comdlg32.dll");
    if (!hComdlg32) {
        std::cerr << "Failed to load comdlg32.dll\n";
        return L"";
    }

    // Get function pointer to GetOpenFileNameW
    GetOpenFileNamePtr pGetOpenFileName = (GetOpenFileNamePtr)GetProcAddress(hComdlg32, "GetOpenFileNameW");
    if (!pGetOpenFileName) {
        std::cerr << "Failed to get GetOpenFileNameW\n";
        FreeLibrary(hComdlg32);
        return L"";
    }

    // Initialize the OPENFILENAMEW structure
    OPENFILENAMEW ofn;
    wchar_t szFile[MAX_PATH] = {0};
    ZeroMemory(&ofn, sizeof(ofn));
    
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = L"All Files\0*.*\0";  // File filter
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    
    // Show the open file dialog
    if (pGetOpenFileName(&ofn)) {
        std::wstring selectedFile = std::wstring(szFile);

        // Restore the original working directory
        SetCurrentDirectoryW(originalDir);

        FreeLibrary(hComdlg32);
        return selectedFile;
    }

    // Restore the original working directory if the dialog is canceled
    SetCurrentDirectoryW(originalDir);
    FreeLibrary(hComdlg32);
    return L"";
}
