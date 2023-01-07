#include "CustomTheme.h"


void CustomTheme::setup()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    
    style->WindowMinSize = ImVec2(160, 65);
    style->FramePadding = ImVec2(4, 2);
    style->ItemSpacing = ImVec2(6, 2);
    style->ItemInnerSpacing = ImVec2(6, 4);
    style->Alpha = 1.0f;
    style->WindowRounding = 0.0f;
    style->FrameRounding = 0.0f;
    style->IndentSpacing = 6.0f;
    style->ItemInnerSpacing = ImVec2(2, 4);
    style->ColumnsMinSpacing = 50.0f;
    style->GrabMinSize = 14.0f;
    style->GrabRounding = 0.0f;
    style->ScrollbarSize = 12.0f;
    style->ScrollbarRounding = 0.0f;
    
    style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.8f, 0.8f, 0.8f, 0.58f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.2f, 0.2f,0.2f,0.10f);
    style->Colors[ImGuiCol_ChildBg] = ImVec4(0.2f,0.2f,0.2f, 0.58f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.3f, 0.3f, 0.78f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2f, 0.2f, 0.2f, 0.75f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.3f, 0.3f, 0.3f, 0.47f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.7f, 0.7f, 0.7f, 0.21f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.7f, 0.7f, 0.7f, 0.78f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.7f, 0.7f, 0.7f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.9f, 0.9f, 0.9f, 0.80f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.5f, 0.5f, 0.5f, 0.14f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.3f, 0.3f, 0.3f, 0.14f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4f, 0.4f, 0.4f, 0.86f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.2f, 0.2f, 0.2f, 0.76f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.2f, 0.2f, 0.2f, 0.86f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
    style->Colors[ImGuiCol_Separator] = ImVec4(0.4f, 0.4f, 0.4f, 0.32f);
    style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.4f, 0.4f, 0.4f, 0.78f);
    style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.5f, 0.5f, 0.5f, 0.04f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.5f, 0.5f, 0.5f, 0.78f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.8f, 0.8f, 0.8f, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.8f, 0.8f, 0.8f,1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8f, 0.8f, 0.8f, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.8f, 0.8f, 0.8f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.43f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.92f);
    style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.73f);
}
