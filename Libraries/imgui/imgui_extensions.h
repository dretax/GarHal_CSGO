#pragma once

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include <Windows.h>
#include "imgui.h"
#include "imgui_internal.h"
#include <string>
#include <vector>

namespace ImGui
{
    bool IsCustomKeyPressed(UINT hotkey, bool isHeldDownKeyBind);
    bool ListBox(const char* label, int* current_item, const std::vector<std::string>& values);


    // Helper functions
    void SetMenuLocation(ImVec2 _resolution);
    bool TabNew(const char* str_id, const bool isActive, const ImVec2& size_arg, ImTextureID icon = nullptr, ImGuiButtonFlags flags = NULL);

    // Another slider logic was neccesarry due to fixed values like grab padding, grab size, centring in original code. Made grab match it's boundaries.
    // This is not a duplicate
    template<typename TYPE, typename SIGNEDTYPE, typename FLOATTYPE>
    bool SliderBehaviorTNew(const ImRect& bb, ImGuiID id, ImGuiDataType data_type, TYPE* v, const TYPE v_min, const TYPE v_max, const char* format, ImGuiSliderFlags flags, ImRect* out_grab_bb);
    bool SliderBehaviorNew(const ImRect& bb, ImGuiID id, ImGuiDataType data_type, void* p_v, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags, ImRect* out_grab_bb);
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // Old sliders
    bool SliderScalarNew(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, bool isActive = true, const char* format = NULL, ImGuiSliderFlags flags = NULL);
    bool SliderFloatNew(const char* label, float* v, float v_min, float v_max, bool isActive = true, const char* format = "%.3f", ImGuiSliderFlags flags = NULL);
    bool SliderIntNew(const char* label, int* v, int v_min, int v_max, bool isActive = true, const char* format = "%d", ImGuiSliderFlags flags = NULL);

    bool CheckboxNew(const char* label, bool* v, ImGuiButtonFlags flags = NULL);

    bool ListBoxNew(const char* label, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);
    bool BeginListBoxNew(const char* label, const ImVec2& size = ImVec2(0, 0));


    // Dummy functions for initing imgui styling
    void InitStyle();
    void InitColors();
}
