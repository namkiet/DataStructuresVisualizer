#pragma once

#include <windows.h>
#include <iostream>
#include <string>

typedef BOOL (WINAPI *GetOpenFileNamePtr)(OPENFILENAMEW*);

extern std::wstring OpenFileDialog();