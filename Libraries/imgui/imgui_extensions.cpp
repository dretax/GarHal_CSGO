#include "imgui_extensions.h"

static auto VectorGetter = [](void* vec, int idx, const char** out_text)
{
    auto& vector = *static_cast<std::vector<std::string>*>(vec);
    if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
    *out_text = vector.at(idx).c_str();
    return true;
};

// It is up for you as a reader to figure out how to implement custom hotkeys
bool ImGui::IsCustomKeyPressed(UINT hotkey, bool isHeldDownKeyBind = false)
{
    const auto& io = GetIO();
    if (isHeldDownKeyBind)
    {
        return io.KeysDown[hotkey];
    }
    return io.KeysPressed[hotkey];
}

bool ImGui::ListBox(const char* label, int* current_item, const std::vector<std::string>& values)
{
    if (values.empty())
        return false;
    return ImGui::ListBoxNew(label, current_item, VectorGetter, (void*) &values, values.size());     
}


// Merely a helper function if you want a window of certain proportions to be displayed in center of your screen
void ImGui::SetMenuLocation(ImVec2 _resolution)
{
    assert(_resolution.x > 0.0f && _resolution.y > 0.0f);

    ImVec2 menu_size(_resolution.x * 0.2f, _resolution.y * 0.4f);
    ImVec2 menu_position((_resolution - menu_size) * 0.5f);

    ImGui::SetNextWindowSize(menu_size);
    //ImGui::SetNextWindowPos(menu_position);
}

bool ImGui::TabNew(const char* str_id, const bool isActive, const ImVec2& size_arg, ImTextureID icon, ImGuiButtonFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const bool has_icon = (icon != nullptr);

    ImDrawList* draw = ImGui::GetWindowDrawList();    
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(str_id);

    //Calculate item width
    const ImVec2 frame_size(size_arg);
    const ImVec2 image_size(frame_size.x, frame_size.y);
    const ImVec2 label_size = ImGui::CalcTextSize(str_id, NULL, true);

    const ImRect frame_bb(ImVec2(window->DC.CursorPos.x, window->DC.CursorPos.y), ImVec2(window->DC.CursorPos.x + frame_size.x, window->DC.CursorPos.y + frame_size.y));
    const ImRect image_bb(ImFloor(ImVec2(frame_bb.Min.x + (frame_size.x - image_size.x) * 0.5f, frame_bb.Min.y + (frame_size.y - image_size.y) * 0.5f)), ImFloor(ImVec2(frame_bb.Min.x + (frame_size.x - image_size.x) * 0.5f + image_size.x, frame_bb.Min.y + (frame_size.y - image_size.y) * 0.5f + image_size.y)));


    //Add item
    const ImRect total_bb(frame_bb.Min, frame_bb.Max);
    ItemSize(total_bb);
    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(frame_bb, id, &hovered, &held, flags);

    if (pressed)
        MarkItemEdited(id);

    //Render part
    const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    RenderNavHighlight(frame_bb, id);

    if (isActive)
        draw->AddRectFilled(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_BorderShadow), style.FrameRounding);

    draw->AddRect(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_Border), style.FrameRounding);
    if (has_icon)
    {
        draw->AddImage(icon, image_bb.Min, image_bb.Max);
    }

    if (label_size.x > 0.0f && !has_icon)
        RenderTextClipped(frame_bb.Min, frame_bb.Max, str_id, NULL, &label_size, ImVec2(0.5f, 0.25f));

    return pressed;
}


// Copy/pasted from imgui_widgets.cpp
// Those MIN/MAX values are not define because we need to point to them
static const signed char    IM_S8_MIN = -128;
static const signed char    IM_S8_MAX = 127;
static const unsigned char  IM_U8_MIN = 0;
static const unsigned char  IM_U8_MAX = 0xFF;
static const signed short   IM_S16_MIN = -32768;
static const signed short   IM_S16_MAX = 32767;
static const unsigned short IM_U16_MIN = 0;
static const unsigned short IM_U16_MAX = 0xFFFF;
static const ImS32          IM_S32_MIN = INT_MIN;    // (-2147483647 - 1), (0x80000000);
static const ImS32          IM_S32_MAX = INT_MAX;    // (2147483647), (0x7FFFFFFF)
static const ImU32          IM_U32_MIN = 0;
static const ImU32          IM_U32_MAX = UINT_MAX;   // (0xFFFFFFFF)
#ifdef LLONG_MIN
static const ImS64          IM_S64_MIN = LLONG_MIN;  // (-9223372036854775807ll - 1ll);
static const ImS64          IM_S64_MAX = LLONG_MAX;  // (9223372036854775807ll);
#else
static const ImS64          IM_S64_MIN = -9223372036854775807LL - 1;
static const ImS64          IM_S64_MAX = 9223372036854775807LL;
#endif
static const ImU64          IM_U64_MIN = 0;
#ifdef ULLONG_MAX
static const ImU64          IM_U64_MAX = ULLONG_MAX; // (0xFFFFFFFFFFFFFFFFull);
#else
static const ImU64          IM_U64_MAX = (2ULL * 9223372036854775807LL + 1);
#endif


template<typename TYPE, typename SIGNEDTYPE, typename FLOATTYPE>
bool ImGui::SliderBehaviorTNew(const ImRect& bb, ImGuiID id, ImGuiDataType data_type, TYPE* v, const TYPE v_min, const TYPE v_max, const char* format, ImGuiSliderFlags flags, ImRect* out_grab_bb)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    const ImGuiAxis axis = (flags & ImGuiSliderFlags_Vertical) ? ImGuiAxis_Y : ImGuiAxis_X;
    const bool is_logarithmic = (flags & ImGuiSliderFlags_Logarithmic) != 0;
    const bool is_floating_point = (data_type == ImGuiDataType_Float) || (data_type == ImGuiDataType_Double);
    const SIGNEDTYPE v_range = (v_min < v_max ? v_max - v_min : v_min - v_max);

    // Calculate bounds
    //const float grab_padding = 2.0f; // FIXME: Should be part of style.
    const float grab_padding = 0.0f;
    float grab_sz = bb.GetHeight();
    const float slider_sz = (bb.Max[axis] - bb.Min[axis]) - grab_padding * 2.0f;
    //if (!is_floating_point && v_range >= 0)                                     // v_range < 0 may happen on integer overflows
        //grab_sz = ImMax((float)(slider_sz / (v_range + 1)), grab_sz); // For integer sliders: if possible have the grab size represent 1 unit

    grab_sz = ImMin(grab_sz, slider_sz);

    const float slider_usable_pos_min = bb.Min[axis];
    const float slider_usable_pos_max = bb.Max[axis];
    const float slider_usable_sz = slider_usable_pos_max - slider_usable_pos_min;

    float logarithmic_zero_epsilon = 0.0f; // Only valid when is_logarithmic is true
    float zero_deadzone_halfsize = 0.0f; // Only valid when is_logarithmic is true
    if (is_logarithmic)
    {
        // When using logarithmic sliders, we need to clamp to avoid hitting zero, but our choice of clamp value greatly affects slider precision. We attempt to use the specified precision to estimate a good lower bound.
        const int decimal_precision = is_floating_point ? ImParseFormatPrecision(format, 3) : 1;
        logarithmic_zero_epsilon = ImPow(0.1f, (float)decimal_precision);
        zero_deadzone_halfsize = (style.LogSliderDeadzone * 0.5f) / ImMax(slider_usable_sz, 1.0f);
    }

    // Process interacting with the slider
    bool value_changed = false;
    if (g.ActiveId == id)
    {
        bool set_new_value = false;
        float clicked_t = 0.0f;
        if (g.ActiveIdSource == ImGuiInputSource_Mouse)
        {
            if (!g.IO.MouseDown[0])
            {
                ClearActiveID();
            }
            else
            {
                const float mouse_abs_pos = g.IO.MousePos[axis];
                clicked_t = (slider_usable_sz > 0.0f) ? ImClamp((mouse_abs_pos - slider_usable_pos_min) / slider_usable_sz, 0.0f, 1.0f) : 0.0f;
                if (axis == ImGuiAxis_Y)
                    clicked_t = 1.0f - clicked_t;
                set_new_value = true;
            }
        }
        else if (g.ActiveIdSource == ImGuiInputSource_Nav)
        {
            if (g.ActiveIdIsJustActivated)
            {
                g.SliderCurrentAccum = 0.0f; // Reset any stored nav delta upon activation
                g.SliderCurrentAccumDirty = false;
            }

            const ImVec2 input_delta2 = GetNavInputAmount2d(ImGuiNavDirSourceFlags_Keyboard | ImGuiNavDirSourceFlags_PadDPad, ImGuiInputReadMode_RepeatFast, 0.0f, 0.0f);
            float input_delta = (axis == ImGuiAxis_X) ? input_delta2.x : -input_delta2.y;
            if (input_delta != 0.0f)
            {
                const int decimal_precision = is_floating_point ? ImParseFormatPrecision(format, 3) : 0;
                if (decimal_precision > 0)
                {
                    input_delta /= 100.0f;    // Gamepad/keyboard tweak speeds in % of slider bounds
                    if (IsNavInputDown(ImGuiNavInput_TweakSlow))
                        input_delta /= 10.0f;
                }
                else
                {
                    if ((v_range >= -100.0f && v_range <= 100.0f) || IsNavInputDown(ImGuiNavInput_TweakSlow))
                        input_delta = ((input_delta < 0.0f) ? -1.0f : +1.0f) / (float)v_range; // Gamepad/keyboard tweak speeds in integer steps
                    else
                        input_delta /= 100.0f;
                }
                if (IsNavInputDown(ImGuiNavInput_TweakFast))
                    input_delta *= 10.0f;

                g.SliderCurrentAccum += input_delta;
                g.SliderCurrentAccumDirty = true;
            }

            float delta = g.SliderCurrentAccum;
            if (g.NavActivatePressedId == id && !g.ActiveIdIsJustActivated)
            {
                ClearActiveID();
            }
            else if (g.SliderCurrentAccumDirty)
            {
                clicked_t = ScaleRatioFromValueT<TYPE, SIGNEDTYPE, FLOATTYPE>(data_type, *v, v_min, v_max, is_logarithmic, logarithmic_zero_epsilon, zero_deadzone_halfsize);

                if ((clicked_t >= 1.0f && delta > 0.0f) || (clicked_t <= 0.0f && delta < 0.0f)) // This is to avoid applying the saturation when already past the limits
                {
                    set_new_value = false;
                    g.SliderCurrentAccum = 0.0f; // If pushing up against the limits, don't continue to accumulate
                }
                else
                {
                    set_new_value = true;
                    float old_clicked_t = clicked_t;
                    clicked_t = ImSaturate(clicked_t + delta);

                    // Calculate what our "new" clicked_t will be, and thus how far we actually moved the slider, and subtract this from the accumulator
                    TYPE v_new = ScaleValueFromRatioT<TYPE, SIGNEDTYPE, FLOATTYPE>(data_type, clicked_t, v_min, v_max, is_logarithmic, logarithmic_zero_epsilon, zero_deadzone_halfsize);
                    if (!(flags & ImGuiSliderFlags_NoRoundToFormat))
                        v_new = RoundScalarWithFormatT<TYPE, SIGNEDTYPE>(format, data_type, v_new);
                    float new_clicked_t = ScaleRatioFromValueT<TYPE, SIGNEDTYPE, FLOATTYPE>(data_type, v_new, v_min, v_max, is_logarithmic, logarithmic_zero_epsilon, zero_deadzone_halfsize);

                    if (delta > 0)
                        g.SliderCurrentAccum -= ImMin(new_clicked_t - old_clicked_t, delta);
                    else
                        g.SliderCurrentAccum -= ImMax(new_clicked_t - old_clicked_t, delta);
                }

                g.SliderCurrentAccumDirty = false;
            }
        }

        if (set_new_value)
        {
            TYPE v_new = ScaleValueFromRatioT<TYPE, SIGNEDTYPE, FLOATTYPE>(data_type, clicked_t, v_min, v_max, is_logarithmic, logarithmic_zero_epsilon, zero_deadzone_halfsize);

            // Round to user desired precision based on format string
            if (!(flags & ImGuiSliderFlags_NoRoundToFormat))
                v_new = RoundScalarWithFormatT<TYPE, SIGNEDTYPE>(format, data_type, v_new);

            // Apply result
            if (*v != v_new)
            {
                *v = v_new;
                value_changed = true;
            }
        }
    }

    if (slider_sz < 1.0f)
    {
        *out_grab_bb = ImRect(bb.Min, bb.Min);
    }
    else
    {
        // Output grab position so it can be displayed by the caller
        float grab_t = ScaleRatioFromValueT<TYPE, SIGNEDTYPE, FLOATTYPE>(data_type, *v, v_min, v_max, is_logarithmic, logarithmic_zero_epsilon, zero_deadzone_halfsize);
        if (axis == ImGuiAxis_Y)
            grab_t = 1.0f - grab_t;
        const float grab_pos = ImLerp(slider_usable_pos_min, slider_usable_pos_max - grab_sz, grab_t);
        if (axis == ImGuiAxis_X)
            *out_grab_bb = ImRect(grab_pos, bb.GetCenter().y - grab_sz * 0.5f, grab_pos + grab_sz, bb.GetCenter().y + grab_sz * 0.5f);
        else
            *out_grab_bb = ImRect(bb.Min.x + grab_padding, grab_pos - grab_sz * 0.5f, bb.Max.x - grab_padding, grab_pos + grab_sz * 0.5f);
    }

    return value_changed;
}

bool ImGui::SliderBehaviorNew(const ImRect& bb, ImGuiID id, ImGuiDataType data_type, void* p_v, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags, ImRect* out_grab_bb)
{
    // Read imgui.cpp "API BREAKING CHANGES" section for 1.78 if you hit this assert.
    IM_ASSERT((flags == 1 || (flags & ImGuiSliderFlags_InvalidMask_) == 0) && "Invalid ImGuiSliderFlags flag!  Has the 'float power' argument been mistakenly cast to flags? Call function with ImGuiSliderFlags_Logarithmic flags instead.");

    // Those are the things we can do easily outside the SliderBehaviorT<> template, saves code generation.
    ImGuiContext& g = *GImGui;
    if ((g.LastItemData.InFlags & ImGuiItemFlags_ReadOnly) || (flags & ImGuiSliderFlags_ReadOnly))
        return false;

    switch (data_type)
    {
    case ImGuiDataType_S8: { ImS32 v32 = (ImS32) * (ImS8*)p_v;  bool r = SliderBehaviorTNew<ImS32, ImS32, float>(bb, id, ImGuiDataType_S32, &v32, *(const ImS8*)p_min, *(const ImS8*)p_max, format, flags, out_grab_bb); if (r) *(ImS8*)p_v = (ImS8)v32;  return r; }
    case ImGuiDataType_U8: { ImU32 v32 = (ImU32) * (ImU8*)p_v;  bool r = SliderBehaviorTNew<ImU32, ImS32, float>(bb, id, ImGuiDataType_U32, &v32, *(const ImU8*)p_min, *(const ImU8*)p_max, format, flags, out_grab_bb); if (r) *(ImU8*)p_v = (ImU8)v32;  return r; }
    case ImGuiDataType_S16: { ImS32 v32 = (ImS32) * (ImS16*)p_v; bool r = SliderBehaviorTNew<ImS32, ImS32, float>(bb, id, ImGuiDataType_S32, &v32, *(const ImS16*)p_min, *(const ImS16*)p_max, format, flags, out_grab_bb); if (r) *(ImS16*)p_v = (ImS16)v32; return r; }
    case ImGuiDataType_U16: { ImU32 v32 = (ImU32) * (ImU16*)p_v; bool r = SliderBehaviorTNew<ImU32, ImS32, float>(bb, id, ImGuiDataType_U32, &v32, *(const ImU16*)p_min, *(const ImU16*)p_max, format, flags, out_grab_bb); if (r) *(ImU16*)p_v = (ImU16)v32; return r; }
    case ImGuiDataType_S32:
        IM_ASSERT(*(const ImS32*)p_min >= IM_S32_MIN / 2 && *(const ImS32*)p_max <= IM_S32_MAX / 2);
        return SliderBehaviorTNew<ImS32, ImS32, float >(bb, id, data_type, (ImS32*)p_v, *(const ImS32*)p_min, *(const ImS32*)p_max, format, flags, out_grab_bb);
    case ImGuiDataType_U32:
        IM_ASSERT(*(const ImU32*)p_max <= IM_U32_MAX / 2);
        return SliderBehaviorTNew<ImU32, ImS32, float >(bb, id, data_type, (ImU32*)p_v, *(const ImU32*)p_min, *(const ImU32*)p_max, format, flags, out_grab_bb);
    case ImGuiDataType_S64:
        IM_ASSERT(*(const ImS64*)p_min >= IM_S64_MIN / 2 && *(const ImS64*)p_max <= IM_S64_MAX / 2);
        return SliderBehaviorTNew<ImS64, ImS64, double>(bb, id, data_type, (ImS64*)p_v, *(const ImS64*)p_min, *(const ImS64*)p_max, format, flags, out_grab_bb);
    case ImGuiDataType_U64:
        IM_ASSERT(*(const ImU64*)p_max <= IM_U64_MAX / 2);
        return SliderBehaviorTNew<ImU64, ImS64, double>(bb, id, data_type, (ImU64*)p_v, *(const ImU64*)p_min, *(const ImU64*)p_max, format, flags, out_grab_bb);
    case ImGuiDataType_Float:
        IM_ASSERT(*(const float*)p_min >= -FLT_MAX / 2.0f && *(const float*)p_max <= FLT_MAX / 2.0f);
        return SliderBehaviorTNew<float, float, float >(bb, id, data_type, (float*)p_v, *(const float*)p_min, *(const float*)p_max, format, flags, out_grab_bb);
    case ImGuiDataType_Double:
        IM_ASSERT(*(const double*)p_min >= -DBL_MAX / 2.0f && *(const double*)p_max <= DBL_MAX / 2.0f);
        return SliderBehaviorTNew<double, double, double>(bb, id, data_type, (double*)p_v, *(const double*)p_min, *(const double*)p_max, format, flags, out_grab_bb);
    case ImGuiDataType_COUNT: break;
    }
    IM_ASSERT(0);
    return false;
}

bool ImGui::SliderScalarNew(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, bool isActive, const char* format, ImGuiSliderFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    ImDrawList* draw = GetWindowDrawList();
    
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const ImRect frame_bb(ImVec2(window->DC.CursorPos.x, window->DC.CursorPos.y), ImVec2(window->DC.CursorPos.x + GetContentRegionAvail().x, window->DC.CursorPos.y + label_size.y)); //text coordinates
    const ImRect spacing_bb(ImVec2(frame_bb.Min.x, frame_bb.Max.y), ImVec2(frame_bb.Max.x, frame_bb.Max.y + frame_bb.GetHeight() * 0.5f));
    const ImRect slider_bb(ImVec2(spacing_bb.Min.x, spacing_bb.Max.y), ImVec2(spacing_bb.Max.x, spacing_bb.Max.y + frame_bb.GetHeight())); //slider active zone coordinates (right under frame_bb)
    const ImRect slider_frame_bb(ImVec2(slider_bb.Min.x + 1, slider_bb.GetCenter().y - (frame_bb.GetHeight() * 1 / 6) * 0.5f), ImVec2(slider_bb.Max.x - 1, slider_bb.GetCenter().y + (frame_bb.GetHeight() * 1 / 6) * 0.5f)); //drawable frame
    //const ImRect value_bb(ImVec2(slider_bb.Min.x, slider_bb.Max.y), ImVec2(slider_bb.Max.x, slider_bb.Max.y + slider_bb.GetHeight() * 0.5f));
    const ImRect value_bb(ImVec2(slider_bb.Min.x, slider_bb.Max.y), ImVec2(slider_bb.Max.x, slider_bb.Max.y + (slider_bb.GetHeight() * 0.5f)));
    const float slider_rounding = style.FrameRounding;

    //Add item to window
    const ImRect total_bb(frame_bb.Min, value_bb.Max);

    const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
    ItemSize(total_bb, frame_bb.GetHeight() + slider_bb.GetHeight());
    if (!ItemAdd(total_bb, id))
        return false;

    // Default format string when passing NULL (maybe will need in future)
    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;


    RenderNavHighlight(frame_bb, id);

    // Slider behavior
    ImRect grab_bb;
    const bool value_changed = SliderBehaviorNew(slider_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    const ImVec2 grab_center(grab_bb.GetCenter());
    const float grab_radius = grab_bb.GetHeight();

    const ImU32 frame_col = GetColorU32(isActive ? ImGuiCol_FrameBgActive : g.ActiveId == id ? ImGuiCol_FrameBgActive : g.HoveredId == id ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    const ImU32 frame_colDisabled = GetColorU32(ImGuiCol_FrameBg);
    const ImU32 text_col = GetColorU32(isActive ? ImGuiCol_Text : ImGuiCol_TextDisabled);

    const bool hovered = ImGui::ItemHoverable(slider_bb, id);
    bool temp_input_is_active = temp_input_allowed && ImGui::TempInputIsActive(id);
    if (!temp_input_is_active && isActive)
    {
        // Tabbing or CTRL-clicking on Slider turns it into an input box
        const bool input_requested_by_tabbing = temp_input_allowed && (g.LastItemData.StatusFlags & ImGuiItemStatusFlags_FocusedByTabbing) != 0;
        const bool clicked = (hovered && g.IO.MouseClicked[0]);
        const bool make_active = (input_requested_by_tabbing || clicked || g.NavActivateId == id);
        if (make_active && temp_input_allowed)
            if (input_requested_by_tabbing || (clicked && g.IO.KeyCtrl))
                temp_input_is_active = true;

        if (make_active && !temp_input_is_active)
        {
            ImGui::SetActiveID(id, window);
            ImGui::SetFocusID(id, window);
            ImGui::FocusWindow(window);
            g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
        }
    }

    // Rendering

    RenderFrame(slider_frame_bb.Min, ImVec2(slider_frame_bb.Max.x, slider_frame_bb.Max.y), frame_colDisabled, true, slider_rounding);   //Draw background frame       
    RenderFrame(ImVec2(slider_frame_bb.Min.x, slider_frame_bb.Min.y), ImVec2(grab_center.x, slider_frame_bb.Max.y), frame_col, true, slider_rounding); //Draw fill frame (behind slider
    draw->AddCircleFilled(grab_center, grab_bb.GetHeight() * 0.5f, frame_col, 64); //Draw grab

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    char value_buf[64];
    const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

    ImGui::PushStyleColor(ImGuiCol_Text, text_col);
    ImVec2 ovSize = CalcTextSize(value_buf, NULL, true);
    if ((label_size.x + ovSize.x + 5) > frame_bb.GetWidth()) //number is max distance between 
    {
        RenderText(ImVec2((frame_bb.Min.x + (frame_bb.Max.x - frame_bb.Min.x) * 0.5f) - CalcTextSize(label).x * 0.5f, frame_bb.Min.y), label);
        RenderText(ImVec2(frame_bb.Min.x + (frame_bb.Max.x - frame_bb.Min.x) * 0.5f - ovSize.x * 0.5f, frame_bb.Max.y), value_buf, value_buf_end);
    }
    else
    {
        RenderText(ImVec2(frame_bb.Min.x, frame_bb.Min.y), label);
        RenderText(ImVec2(frame_bb.Max.x - ovSize.x, frame_bb.Min.y), value_buf, value_buf_end);
    }
    ImGui::PopStyleColor();
        

    return value_changed;
}

bool ImGui::SliderFloatNew(const char* label, float* v, float v_min, float v_max, bool isActive, const char* format, ImGuiSliderFlags flags)
{
    return ImGui::SliderScalarNew(label, ImGuiDataType_Float, v, &v_min, &v_max, isActive, format, flags);
}

bool ImGui::SliderIntNew(const char* label, int* v, int v_min, int v_max, bool isActive, const char* format, ImGuiSliderFlags flags)
{    
    return ImGui::SliderScalarNew(label, ImGuiDataType_S32, v, &v_min, &v_max, isActive, format, flags);
}


bool ImGui::CheckboxNew(const char* label, bool* v, ImGuiButtonFlags flags)
{
    ImGuiContext& g = *GImGui;

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    ImDrawList* draw = window->DrawList;

    //Calculate item width  
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);    
    const ImRect frame_bb(ImVec2(window->DC.CursorPos.x, window->DC.CursorPos.y), ImVec2(window->DC.CursorPos.x + label_size.x, window->DC.CursorPos.y + label_size.y * 1.5f));
        
    const int checkbox_height = frame_bb.GetHeight() * 0.8f;
    const ImVec2 checkbox_size(checkbox_height, checkbox_height);    
    float indent = label_size.y * 0.5f;
    const ImRect label_bb(frame_bb.Min, ImVec2(frame_bb.Min.x + label_size.x + indent, frame_bb.Max.y));
    const ImRect button_bb(ImVec2(label_bb.Max.x, frame_bb.Min.y + ((frame_bb.GetHeight() - checkbox_size.y) * 0.5f)), ImVec2(label_bb.Max.x + checkbox_size.x, frame_bb.Max.y - ((frame_bb.GetHeight() - checkbox_size.y) * 0.5f)));

    //Add item
    const ImRect total_bb(ImVec2(frame_bb.Min.x, frame_bb.Min.y), ImVec2(frame_bb.Max.x + +button_bb.GetWidth(), frame_bb.Max.y));
    ItemSize(total_bb);
    if (!ItemAdd(total_bb, id))    
        return false;
   
    bool hovered, held;
    bool pressed = ButtonBehavior(button_bb, id, &hovered, &held, flags);

    if (pressed)
    {
        MarkItemEdited(id);
        *v = !*v;
    }
    RenderNavHighlight(frame_bb, id);

    // Colors    
    const ImU32 colFrame = GetColorU32(ImGuiCol_FrameBg);        
    const ImU32 color = *v ?  GetColorU32(ImGuiCol_ButtonActive) : hovered ? GetColorU32(ImGuiCol_ButtonHovered) : GetColorU32(ImGuiCol_Button);
              
    // Render
    RenderTextClipped(label_bb.Min, label_bb.Max, label, NULL, NULL, ImVec2(0, 0.5f));    
    draw->AddRectFilled(button_bb.Min, button_bb.Max, color, style.FrameRounding);
    draw->AddRectFilled(button_bb.Min, button_bb.Max, color, style.FrameRounding);
    draw->AddRect(button_bb.Min, button_bb.Max, GetColorU32(ImGuiCol_Border), style.FrameRounding);    

    return pressed;
}

bool ImGui::ListBoxNew(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int height_in_items)
{
    ImGuiContext& g = *GImGui;

    // Calculate size from "height_in_items"
    if (height_in_items < 0)
        height_in_items = ImMin(items_count, 7);
    float height_in_items_f = height_in_items + 0.25f;
    //ImVec2 size(0.0f, ImFloor(GetTextLineHeightWithSpacing() * height_in_items_f + g.Style.FramePadding.y * 2.0f));
    ImVec2 size(GetContentRegionAvail().x * 0.5f, ImFloor(GetTextLineHeightWithSpacing() * height_in_items_f + g.Style.FramePadding.y * 2.0f));

    if (!BeginListBoxNew(label, size))
        return false;

    // Assume all items have even height (= 1 line of text). If you need items of different height,
    // you can create a custom version of ListBox() in your code without using the clipper.
    bool value_changed = false;
    ImGuiListClipper clipper;
    clipper.Begin(items_count, GetTextLineHeightWithSpacing()); // We know exactly our line height here so we pass it as a minor optimization, but generally you don't need to.
    while (clipper.Step())
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        {
            const char* item_text;
            if (!items_getter(data, i, &item_text))
                item_text = "*Unknown item*";

            PushID(i);
            const bool item_selected = (i == *current_item);
            if (Selectable(item_text, item_selected))
            {
                *current_item = i;
                value_changed = true;
            }
            if (item_selected)
                SetItemDefaultFocus();
            PopID();
        }
    EndListBox();

    if (0)
        MarkItemEdited(g.LastItemData.ID);

    return 0;
}

bool ImGui::BeginListBoxNew(const char* label, const ImVec2& size_arg)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    // Size default to hold ~7.25 items.
    // Fractional number of items helps seeing that we can scroll down/up without looking at scrollbar.
    ImVec2 size = ImFloor(CalcItemSize(size_arg, CalcItemWidth(), GetTextLineHeightWithSpacing() * 7.25f + style.FramePadding.y * 2.0f));
    ImVec2 frame_size = ImVec2(size.x, size.y + label_size.y);
    ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + frame_size); // Total frame size
    ImRect label_bb(frame_bb.Min, frame_bb.Min + label_size + style.ItemSpacing);
    ImRect child_bb(ImVec2(frame_bb.Min.x, label_bb.Max.y), frame_bb.Max);    
    g.NextItemData.ClearFlags();
    
    // FIXME-OPT: We could omit the BeginGroup() if label_size.x but would need to omit the EndGroup() as well.
    BeginGroup();
    if (label_size.x > 0.0f)
    {        
        ImVec2 label_pos = label_bb.Min;
        RenderText(label_pos, label, NULL, true);
        window->DC.CursorPos = ImVec2(window->DC.CursorPos.x, window->DC.CursorPos.y + label_bb.GetHeight());
    }

    if (!IsRectVisible(frame_bb.Min, frame_bb.Max))
    {
        ItemSize(frame_bb.GetSize(), style.FramePadding.y);
        ItemAdd(frame_bb, NULL);
        return false;
    }

    BeginChildFrame(id, child_bb.GetSize());
    return true;
}


void ImGui::InitStyle()
{

    ImGuiStyle& style = ImGui::GetStyle();
    style.Alpha = 1.f;
    
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.DisplayWindowPadding = ImVec2(22, 22);
    style.DisplaySafeAreaPadding = ImVec2(4, 4);
    style.AntiAliasedLines = true;
    style.CurveTessellationTol = 1.25f;

    // Windows
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.WindowPadding = ImVec2(10.f, 10.0f);
    style.WindowMinSize = ImVec2(32, 32);

    style.WindowBorderSize = 1.f;
    style.ChildBorderSize = 1.f;
    style.PopupBorderSize = 1.f;
    
    style.WindowRounding = 0.0f; //4.0 for slight curve
    style.ChildRounding = 0.0f;
    style.PopupRounding = 6.f;

    // Objects
    style.FramePadding = ImVec2(4, 3);
    style.FrameRounding = 0.0f; //2.0    
    style.ItemSpacing = ImVec2(8, 4);
    style.ItemInnerSpacing = ImVec2(4, 2);    
    style.TouchExtraPadding = ImVec2(0, 0);
    style.IndentSpacing = 21.0f;
    style.ColumnsMinSpacing = 3.0f;

    // Scrollbar
    style.ScrollbarSize = 6.0f;
    style.ScrollbarRounding = 12.0f; //16.0

    // Slider
    style.GrabMinSize = 6.0f;    
    style.GrabRounding = 16.0f; //16.0



}

void ImGui::InitColors()
{
    // The best way to setup colors in Dear ImGui is to call Style Editor
    // Include imgui_demo.cpp into your project and in your render loop call:
    /*
                ImGui::Begin("Dear ImGui Style Editor");
                ImGui::ShowStyleEditor();
                ImGui::End();
    */
    // Be sure imgui_demo.cpp has the same version with your project's imgui
    // Current Garhal's ImGui version: 1.84 WIP
    // To copy colors open style editor, go to colors tab, uncheck "Only modified Colors" and Export to clipboard    
    
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.91f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.71f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.71f);
    colors[ImGuiCol_Border] = ImVec4(0.78f, 0.78f, 0.78f, 0.12f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.42f, 1.00f, 0.18f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.38f, 0.38f, 0.39f, 0.64f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.99f, 1.00f, 0.40f, 0.32f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.10f, 0.71f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.95f, 0.00f, 0.34f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.59f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


}