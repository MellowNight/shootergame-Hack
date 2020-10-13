#include "Render.h"
#include "globals.h"


void Render::Start()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(Globals->windowWidth, Globals->windowHeight), ImGuiCond_Always);
    ImGui::Begin(" ",
        (bool*)true,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);

    //ImVec4* colors = ImGui::GetStyle().Colors;
    //colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
}

void Render::End()
{
    ImGui::End();
}

void Render::Text(ImVec2 pos, ImColor color, const char* text_begin, const char* text_end, float wrap_width,
    const ImVec4* cpu_fine_clip_rect)
{
    ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), pos, color, text_begin, text_end,
        wrap_width, cpu_fine_clip_rect);
}

void Render::RectFilled(int x0, int y0, int x1, int y1, ImColor color, float rounding, int rounding_corners_flags)
{
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
}

void Render::Line(ImVec2 a, ImVec2 b, ImColor color, float thickness)
{
    ImGui::GetWindowDrawList()->AddLine(a, b, color, thickness);
}

void Render::EasyText(ImVec2 pos, ImColor color, const char* text, int widthText)
{
    Render::Text(pos, color, text, text + strlen(text), widthText, 0);
}

void Render::EasyTextLarge(ImVec2 pos, ImColor color, const char* text)
{
    ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize() + 20, pos, color, text, text + strlen(text),
        800, 0);
}

void Render::DrawBox(ImColor color, int x, int y, int w, int h)
{
    float thicc = 2.0f;

    // ------
    // |    |
    Line(ImVec2(x, y), ImVec2(x + w, y), color, thicc);

    // |
    // |
    // |
    Line(ImVec2(x, y), ImVec2(x, y + h), color, thicc);

    //      |
    //      |
    //      |
    Line(ImVec2(x + w, y), ImVec2(x + w, y + h), color, thicc);

    // |    |
    // ------
    Line(ImVec2(x, y + h), ImVec2(x + w, y + h), color, thicc);
}

void Render::Circle(ImVec2 point, ImColor color, float radius, int num_segments, float thickness)
{
    ImGui::GetWindowDrawList()->AddCircle(point, radius, color, num_segments, thickness);
}

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))
void Render::Progress(int x, int y, int w, int h, int phealth)
{
    int healthValue = max(0, min(phealth, 100));
    float HealthPerc = healthValue / 100.f;

    ImColor barColor = ImColor(0, 255, 0);

    RectFilled(x, y, x + w, y + (int)(((float)h / 100.0f) * (float)phealth), barColor, 0.0f, 0);

}

HWND    FindTopWindow(DWORD pid)
{
    std::pair<HWND, DWORD> params = { 0, pid };

    // Enumerate the windows using a lambda to process each window
    BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL
        {
            auto pParams = (std::pair<HWND, DWORD>*)(lParam);

            DWORD processId;
            if (GetWindowThreadProcessId(hwnd, &processId) && processId == pParams->second)
            {
                // Stop enumerating
                SetLastError(-1);
                pParams->first = hwnd;
                return FALSE;
            }

            // Continue enumerating
            return TRUE;
        }, (LPARAM)&params);

    if (!bResult && GetLastError() == -1 && params.first)
    {
        return params.first;
    }

    return 0;
}

void	initWindow(int processID)
{
    HWND	hwnd = FindTopWindow(processID);

    RECT rect;

    if (GetWindowRect(hwnd, &rect))
    {
        Globals->windowWidth = rect.right - rect.left;

        Globals->windowHeight = rect.bottom - rect.top;

        Globals->positionX = rect.left;
        Globals->positionY = rect.top;
    }
}



void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}
