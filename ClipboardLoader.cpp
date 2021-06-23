// ClipboardLoader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>

int main()
{
    std::cout << "Hello World!\n";

    // Open the clipboard (a prerequisite for subsequent operations below)
    if (!::OpenClipboard(nullptr)) {
        return -1;
    }

    // Empty the clipboard and take ownership of it
    if (!::EmptyClipboard()) {
        return -1;
    }

    // Get a unique number that represents the "HTML Format"
    unsigned int htmlFormat = ::RegisterClipboardFormat(L"HTML Format");
    if (!htmlFormat) {
        return -1;
    }
    
    // Some test HTML content to put on the clipboard
    const char* html = "testing!!!";

    
    // Allocate kernel memory big enough for the test content and a null terminator
    size_t byteCount = strlen(html) + 1;
    HGLOBAL global = ::GlobalAlloc(GMEM_MOVEABLE, byteCount);
    if (!global) {
        return -1;
    }

    // Lock memory and copy bytes, then unlock again
    char* bytes = static_cast<char*>(::GlobalLock(global));
    if (!bytes) {
        return -1;
    }

    memcpy(bytes, html, byteCount);
    ::GlobalUnlock(global);

    // Transfer ownership of global memory to clipboard
    if (!::SetClipboardData(htmlFormat, global)) {
        return -1;
    }

    if (!::CloseClipboard()) {
        return -1;
    }

    return 0;
}