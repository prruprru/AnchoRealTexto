#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <string>
#include <algorithm>

#pragma comment(lib, "gdi32.lib")

extern "C" __declspec(dllexport) int WINAPI BM_AnchoTexto(
    LPCSTR texto,
    LPCSTR fuente,
    int    tamano,
    LPCSTR estilo,
    BOOL   cursiva)
{
    if (!texto || !*texto) return 0;
    if (!fuente || !*fuente) fuente = "Tahoma";

    std::string sEstilo = estilo ? estilo : "N";
    if (cursiva)
    {
        if (sEstilo.find('I') == std::string::npos)
            sEstilo += 'I';
    }

    bool negrita  = (sEstilo.find('B') != std::string::npos);
    bool italica  = (sEstilo.find('I') != std::string::npos);
    bool subray   = (sEstilo.find('U') != std::string::npos);
    bool tachado  = (sEstilo.find('-') != std::string::npos);

    HDC hdc = GetDC(nullptr);
    if (!hdc) return 0;

    int logPixSy = GetDeviceCaps(hdc, LOGPIXELSY);
    int lfHeight = -MulDiv(tamano > 0 ? tamano : 10, logPixSy, 72);

    HFONT hFont = CreateFontA(
        lfHeight, 0, 0, 0,
        negrita ? FW_BOLD : FW_NORMAL,
        italica ? TRUE : FALSE,
        subray  ? TRUE : FALSE,
        tachado ? TRUE : FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        fuente);

    HFONT hOld = (HFONT)SelectObject(hdc, hFont);

    SIZE size = {0, 0};
    GetTextExtentPoint32A(hdc, texto, static_cast<int>(strlen(texto)), &size);

    SelectObject(hdc, hOld);
    DeleteObject(hFont);
    ReleaseDC(nullptr, hdc);

    return size.cx;
}
