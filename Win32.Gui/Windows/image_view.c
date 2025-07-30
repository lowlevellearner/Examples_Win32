#include <windows.h>

#define IDB_MY_BITMAP 101

HBITMAP hBitmap;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        hBitmap = (HBITMAP)LoadImage(NULL, "sample.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (!hBitmap) {
            MessageBox(hwnd, "Failed to load image!", "Error", MB_OK | MB_ICONERROR);
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        if (hBitmap) {
            HDC hMemDC = CreateCompatibleDC(hdc);
            SelectObject(hMemDC, hBitmap);

            // Draw at top-left
            BitBlt(hdc, 0, 0, 300, 200, hMemDC, 0, 0, SRCCOPY);

            DeleteDC(hMemDC);
        }
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        DeleteObject(hBitmap);
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "ImageWindow";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Image Viewer (Win32)", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
