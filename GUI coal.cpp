#include <windows.h>
#include <string>

using namespace std;

#define ID_INPUT   1
#define ID_BUTTON  2
#define ID_COUNT   3
#define ID_ENC     4
#define ID_DEC     5

// Same lookup tables as Assembly
string enc_lo = "qwertyuiopasdfghjklzxcvbnm";
string dec_lo = "kxvmcnophqrszyijadlegwbuft";
string enc_up = "QWERTYUIOPASDFGHJKLZXCVBNM";
string dec_up = "KXVMCNOPHQRSZYIJADLEGWBUFT";

// =========================
// Map single character
// =========================
char map_char(char c, bool encrypt)
{
    if (c >= 'a' && c <= 'z')
    {
        int idx = c - 'a';
        return encrypt ? enc_lo[idx] : dec_lo[idx];
    }
    else if (c >= 'A' && c <= 'Z')
    {
        int idx = c - 'A';
        return encrypt ? enc_up[idx] : dec_up[idx];
    }
    return c;
}

// =========================
// Encrypt / Decrypt string
// =========================
string crypt(string text, bool encrypt)
{
    for (int i = 0; i < text.length(); i++)
        text[i] = map_char(text[i], encrypt);
    return text;
}

// =========================
// Window Procedure
// =========================
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    static HWND hInput, hCount, hEnc, hDec;

    switch (msg)
    {
    case WM_CREATE:
        CreateWindow("STATIC", "Enter Text:",
            WS_VISIBLE | WS_CHILD,
            20, 20, 100, 20,
            hwnd, NULL, NULL, NULL);

        hInput = CreateWindow("EDIT", "",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            20, 45, 300, 25,
            hwnd, (HMENU)ID_INPUT, NULL, NULL);

        CreateWindow("BUTTON", "Process",
            WS_VISIBLE | WS_CHILD,
            20, 80, 100, 30,
            hwnd, (HMENU)ID_BUTTON, NULL, NULL);

        CreateWindow("STATIC", "Character Count:",
            WS_VISIBLE | WS_CHILD,
            20, 120, 150, 20,
            hwnd, NULL, NULL, NULL);

        hCount = CreateWindow("EDIT", "",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY,
            180, 120, 140, 20,
            hwnd, (HMENU)ID_COUNT, NULL, NULL);

        CreateWindow("STATIC", "Encrypted:",
            WS_VISIBLE | WS_CHILD,
            20, 150, 100, 20,
            hwnd, NULL, NULL, NULL);

        hEnc = CreateWindow("EDIT", "",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY,
            20, 175, 300, 25,
            hwnd, (HMENU)ID_ENC, NULL, NULL);

        CreateWindow("STATIC", "Decrypted:",
            WS_VISIBLE | WS_CHILD,
            20, 210, 100, 20,
            hwnd, NULL, NULL, NULL);

        hDec = CreateWindow("EDIT", "",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY,
            20, 235, 300, 25,
            hwnd, (HMENU)ID_DEC, NULL, NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wp) == ID_BUTTON)
        {
            char buffer[256];
            GetWindowText(hInput, buffer, 256);

            string input = buffer;
            int count = input.length();

            string encrypted = crypt(input, true);
            string decrypted = crypt(encrypted, false);

            char countStr[20];
            wsprintf(countStr, "%d", count);

            SetWindowText(hCount, countStr);
            SetWindowText(hEnc, encrypted.c_str());
            SetWindowText(hDec, decrypted.c_str());
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}

// =========================
// WinMain
// =========================
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmd)
{
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = "AsmEncryptGUI";

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
        "AsmEncryptGUI",
        "Assembly Encryption System (C++ GUI)",
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
        300, 200, 380, 330,
        NULL, NULL, hInst, NULL);

    ShowWindow(hwnd, nCmd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

