#pragma once
#include "../header.h"
#include "structs.hpp"
namespace ImGui
{
	void ToggleButton( const char* str_id, bool* v );
	void PushAllColorsDark( ImGuiStyle dark );
	void PushAllColorsCustom( ImGuiStyle custom_gui_style );
	void PopAllColorsCustom( );
	bool InputTextEx( const char* label, std::string* str, ImGuiInputTextFlags flags );
	void DrawObjBorder( ImVec2 obj_pos, ImVec2 obj_size, float distance_thickness = 5.f, ImU32 col = IM_COL32( 255, 255, 0, 255 ) );
	void DrawObjBorder( basic_obj obj, float distance_thickness = 5.f, ImU32 col = IM_COL32( 255, 255, 0, 255 ) );
}