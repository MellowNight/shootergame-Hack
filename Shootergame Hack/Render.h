#pragma once
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>

namespace Render
{
    void Start();
    void End();

    void Text(ImVec2 pos, ImColor color, const char* text_begin, const char* text_end, float wrap_width,
        const ImVec4* cpu_fine_clip_rect);
    void EasyText(ImVec2 pos, ImColor color, const char* text, int widthText = 800);
    void EasyTextLarge(ImVec2 pos, ImColor color, const char* text);
    void RectFilled(int x0, int y0, int x1, int y1, ImColor color, float rounding, int rounding_corners_flags);
    void Line(ImVec2 a, ImVec2 b, ImColor color, float thickness);
    void DrawBox(ImColor color, int x, int y, int w, int h);
    void Progress(int x, int y, int w, int h, int phealth);
    void Circle(ImVec2 point, ImColor color, float radius, int num_segments, float thickness);
}


void    HelpMarker(const char* desc);

void	initWindow(int processID);
