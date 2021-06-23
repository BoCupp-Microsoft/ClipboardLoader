// ClipboardLoader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#include <windows.h>

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cout << "Usage: clipload.exe \"Format Name\" filename.clip\n";
    }
    else {
        std::cout << "arg0: " << argv[0] << "\n";
        std::cout << "arg1: " << argv[1] << "\n";
        std::cout << "arg2: " << argv[2] << "\n";
    }

    std::ifstream clipFile(argv[2], std::ios::in | std::ios::binary);
    if (!clipFile.is_open()) {
        return -1;
    }
    
    // position the pointer to the end 
    clipFile.seekg(0, std::ios::end);
    size_t byteCount = clipFile.tellg();
    
    // Allocate kernel memory big enough for the file's content and a null terminator
    HGLOBAL global = ::GlobalAlloc(GMEM_MOVEABLE, byteCount + 1);
    if (!global) {
        return -1;
    }

    // Lock memory and copy bytes, then unlock again
    char* bytes = static_cast<char*>(::GlobalLock(global));
    if (!bytes) {
        return -1;
    }

    clipFile.seekg(0, std::ios::beg);
    clipFile.read(bytes, byteCount);

    bytes[byteCount] = '\0';

    // Unlock the memory now that we've finished writing to it.
    ::GlobalUnlock(global);

    // Open the clipboard (a prerequisite for subsequent operations below)
    if (!::OpenClipboard(nullptr)) {
        return -1;
    }

    // Empty the clipboard and take ownership of it
    if (!::EmptyClipboard()) {
        return -1;
    }

    // Get a unique number that represents the "HTML Format"
    unsigned int htmlFormat = ::RegisterClipboardFormatA(argv[1]);
    if (!htmlFormat) {
        return -1;
    }

    // Transfer ownership of global memory to clipboard
    if (!::SetClipboardData(htmlFormat, global)) {
        return -1;
    }

    if (!::CloseClipboard()) {
        return -1;
    }

    return 0;
}