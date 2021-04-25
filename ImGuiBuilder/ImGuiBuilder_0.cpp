void ToggleButton(const char* str_id, bool* v)
{ 
ImVec2 p = ImGui::GetCursorScreenPos(); 
ImDrawList* draw_list = ImGui::GetWindowDrawList();
float height = ImGui::GetFrameHeight();
float width = height * 1.55f;
float radius = height * 0.50f;
 if (ImGui::InvisibleButton(str_id, ImVec2(width, height)))
   *v = !*v;
ImU32 col_bg;
if (ImGui::IsItemHovered())
	  col_bg = *v ? IM_COL32(145 + 20, 211, 68 + 20, 255) : IM_COL32(218 - 20, 218 - 20, 218 - 20, 255);
 else
	   col_bg = *v ? IM_COL32(145, 211, 68, 255) : IM_COL32(218, 218, 218, 255);
  draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
   draw_list->AddCircleFilled(ImVec2(*v ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}


void gui_builder0()
{
ImGui::SetNextWindowSize({281.f,329.f});

ImGui::Begin( "form0");
ImGui::SetCursorPos({30.f,30.f});
ImGui::Text("label0");
ImGui::SetCursorPos({109.f,175.f});
if(ImGui::Button("button1", {0.f,0.f}))
{

}


ImGui::End();
}


