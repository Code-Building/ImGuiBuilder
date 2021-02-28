#include "ImGuiBuilder.h"

extern HWND wnd;

ImGuiStyle custom_gui_style;

int activeWindowID = 0;

// function
bool button_ok = false;
int file_menu = 0;
bool color_menu = false, style_menu = false;
std::string file_manege = "File to save";
std::string theme = "theme.col";
std::string style = "Custom.style";

// Proprity formulary
std::string current_item;
int index = 0, family = 0, grandchild = -1;
int type = -1; // none types
std::string name;
ImVec2 FormPos{}, itemsize{}, pos_obj{};
bool moving_obj = false;

static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		// Resize string callback
		auto str = static_cast<std::string*>(data->UserData);
		IM_ASSERT(data->Buf == str->c_str());
		str->resize(data->BufTextLen);
		data->Buf = const_cast<char*>(str->c_str());
	}
	return data->BufSize;
}

bool input_text(const char* label, std::string* str, ImGuiInputTextFlags flags)
{
	flags |= ImGuiInputTextFlags_CallbackResize;
	return ImGui::InputText(label, const_cast<char*>(str->c_str()), str->capacity() + 1, flags, InputTextCallback, static_cast<void*>(str));
}

void ToggleButton(const char* str_id, bool* v)
{
	const auto p = ImGui::GetCursorScreenPos();
	auto* draw_list = ImGui::GetWindowDrawList();

	const auto height = ImGui::GetFrameHeight();
	const auto width = height * 1.55f;
	const auto radius = height * 0.50f;

	if (ImGui::InvisibleButton(str_id, ImVec2(width, height)))
		*v = !*v;
	ImU32 col_bg = 0;
	//custom_gui_style.Colors[22].w, custom_gui_style.Colors[22].x, custom_gui_style.Colors[22].y, custom_gui_style.Colors[22].z
	if (ImGui::IsItemHovered())
		col_bg = *v ? IM_COL32(145 + 20, 211, 68 + 20, 255) : IM_COL32(218 - 20, 218 - 20, 218 - 20, 255);
	else
		col_bg = *v ? IM_COL32(145, 211, 68, 255) : IM_COL32(218, 218, 218, 255);

	draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);

	draw_list->AddCircleFilled(ImVec2(*v ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}

std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

bool gaks(int key1, int key2)
{
	if (GetAsyncKeyState(key1) && (GetAsyncKeyState(key2) & 1))
		return true;
	else
		return false;
}

bool is_number(std::string& s)
{
	auto find = s.find(' ');
	if (find != std::string::npos)
		s.replace(find, 1, "");
	find = s.find('\n');
	if (find != std::string::npos)
		s.replace(find, 1, "");
	find = s.find('\r');
	if (find != std::string::npos)
		s.replace(find, 1, "");

	return std::regex_match(s, std::regex("[+-]?([0-9]*[.])?[0-9]+"));
}

void PushAllColorsDark(ImGuiStyle dark)
{
	auto& style = ImGui::GetStyle();

	for (auto i = 0; i < ImGuiCol_COUNT; i++)
	{
		ImGui::PushStyleColor(static_cast<ImGuiCol_>(i), dark.Colors[i]);
	}
	//Padding
	style.WindowPadding = dark.WindowPadding;
	style.FramePadding = dark.FramePadding;
	style.CellPadding = dark.CellPadding;
	style.ItemSpacing = dark.ItemSpacing;
	style.ItemInnerSpacing = dark.ItemInnerSpacing;
	style.TouchExtraPadding = dark.TouchExtraPadding;
	style.IndentSpacing = dark.IndentSpacing;
	style.ScrollbarSize = dark.ScrollbarSize;
	style.GrabMinSize = dark.GrabMinSize;
	style.WindowBorderSize = dark.WindowBorderSize;
	style.ChildBorderSize = dark.ChildBorderSize;
	style.PopupBorderSize = dark.PopupBorderSize;
	style.FrameBorderSize = dark.FrameBorderSize;
	style.TabBorderSize = dark.TabBorderSize;

	//Rounding
	style.WindowRounding = dark.WindowRounding;
	style.ChildRounding = dark.ChildRounding;
	style.FrameRounding = dark.FrameRounding;
	style.PopupRounding = dark.PopupRounding;
	style.ScrollbarRounding = dark.ScrollbarRounding;
	style.GrabRounding = dark.GrabRounding;
	style.LogSliderDeadzone = dark.LogSliderDeadzone;
	style.TabRounding = dark.TabRounding;

	//Position
	style.WindowTitleAlign = dark.WindowTitleAlign;
	style.WindowMenuButtonPosition = dark.WindowMenuButtonPosition;
	style.ColorButtonPosition = dark.ColorButtonPosition;
	style.ButtonTextAlign = dark.ButtonTextAlign;
	style.SelectableTextAlign = dark.SelectableTextAlign;

	//NANI???
	style.DisplaySafeAreaPadding = dark.DisplaySafeAreaPadding;
}

void PushAllColorsCustom()
{
	auto& style = ImGui::GetStyle();
	//style.Colors = custom_gui_style.Colors;

	for (auto i = 0; i < ImGuiCol_COUNT; i++)
	{
		ImGui::PushStyleColor(static_cast<ImGuiCol_>(i), custom_gui_style.Colors[i]);
	}
	//Padding
	style.WindowPadding = custom_gui_style.WindowPadding;
	style.FramePadding = custom_gui_style.FramePadding;
	style.CellPadding = custom_gui_style.CellPadding;
	style.ItemSpacing = custom_gui_style.ItemSpacing;
	style.ItemInnerSpacing = custom_gui_style.ItemInnerSpacing;
	style.TouchExtraPadding = custom_gui_style.TouchExtraPadding;
	style.IndentSpacing = custom_gui_style.IndentSpacing;
	style.ScrollbarSize = custom_gui_style.ScrollbarSize;
	style.GrabMinSize = custom_gui_style.GrabMinSize;
	style.WindowBorderSize = custom_gui_style.WindowBorderSize;
	style.ChildBorderSize = custom_gui_style.ChildBorderSize;
	style.PopupBorderSize = custom_gui_style.PopupBorderSize;
	style.FrameBorderSize = custom_gui_style.FrameBorderSize;
	style.TabBorderSize = custom_gui_style.TabBorderSize;

	//Rounding
	style.WindowRounding = custom_gui_style.WindowRounding;
	style.ChildRounding = custom_gui_style.ChildRounding;
	style.FrameRounding = custom_gui_style.FrameRounding;
	style.PopupRounding = custom_gui_style.PopupRounding;
	style.ScrollbarRounding = custom_gui_style.ScrollbarRounding;
	style.GrabRounding = custom_gui_style.GrabRounding;
	style.LogSliderDeadzone = custom_gui_style.LogSliderDeadzone;
	style.TabRounding = custom_gui_style.TabRounding;

	//Position
	style.WindowTitleAlign = custom_gui_style.WindowTitleAlign;
	style.WindowMenuButtonPosition = custom_gui_style.WindowMenuButtonPosition;
	style.ColorButtonPosition = custom_gui_style.ColorButtonPosition;
	style.ButtonTextAlign = custom_gui_style.ButtonTextAlign;
	style.SelectableTextAlign = custom_gui_style.SelectableTextAlign;

	//NANI???
	style.DisplaySafeAreaPadding = custom_gui_style.DisplaySafeAreaPadding;
}

void PopAllColorsCustom()
{
	ImGui::PopStyleColor(ImGuiCol_COUNT);
}

void window_flag(ImGuiStyle& custom_gui_style)
{
	//custom_gui_style = ImGui::GetStyle(); //custom_gui_style
	ImGui::Begin("Style Window Editor", &style_menu);
	if (ImGui::Button("LOAD"))
	{
		std::ifstream r_file(style);
		std::string line;

		int fileindex = 0;

		if (r_file.is_open())
		{
			while (!r_file.eof())
			{
				std::getline(r_file, line);

				if (line.find(',') != std::string::npos)
				{
					auto b_line = split(line, ',');

					switch (fileindex)
					{
					case 0:
						custom_gui_style.WindowPadding.x = std::stof(b_line[0]);
						custom_gui_style.WindowPadding.y = std::stof(b_line[1]);
						break;
					case 1:
						custom_gui_style.FramePadding.x = std::stof(b_line[0]);
						custom_gui_style.FramePadding.y = std::stof(b_line[1]);
						break;
					case 2:
						custom_gui_style.CellPadding.x = std::stof(b_line[0]);
						custom_gui_style.CellPadding.y = std::stof(b_line[1]);
						break;
					case 3:
						custom_gui_style.ItemSpacing.x = std::stof(b_line[0]);
						custom_gui_style.ItemSpacing.y = std::stof(b_line[1]);
						break;
					case 4:
						custom_gui_style.ItemInnerSpacing.x = std::stof(b_line[0]);
						custom_gui_style.ItemInnerSpacing.y = std::stof(b_line[1]);
						break;
					case 5:
						custom_gui_style.TouchExtraPadding.x = std::stof(b_line[0]);
						custom_gui_style.TouchExtraPadding.y = std::stof(b_line[1]);
						break;
					case 22:
						custom_gui_style.WindowTitleAlign.x = std::stof(b_line[0]);
						custom_gui_style.WindowTitleAlign.y = std::stof(b_line[1]);
						break;
					case 25:
						custom_gui_style.ButtonTextAlign.x = std::stof(b_line[0]);
						custom_gui_style.ButtonTextAlign.y = std::stof(b_line[1]);
						break;
					case 26:
						custom_gui_style.SelectableTextAlign.x = std::stof(b_line[0]);
						custom_gui_style.SelectableTextAlign.y = std::stof(b_line[1]);
						break;
					case 27:
						custom_gui_style.DisplaySafeAreaPadding.x = std::stof(b_line[0]);
						custom_gui_style.DisplaySafeAreaPadding.y = std::stof(b_line[1]);
						break;
					}
				}

				switch (fileindex)
				{
				case 6:
					custom_gui_style.IndentSpacing = std::stof(line);
					break;
				case 7:
					custom_gui_style.ScrollbarSize = std::stof(line);
					break;
				case 8:
					custom_gui_style.GrabMinSize = std::stof(line);
					break;
				case 9:
					custom_gui_style.WindowBorderSize = std::stof(line);
					break;
				case 10:
					custom_gui_style.ChildBorderSize = std::stof(line);
					break;
				case 11:
					custom_gui_style.PopupBorderSize = std::stof(line);
					break;
				case 12:
					custom_gui_style.FrameBorderSize = std::stof(line);
					break;
				case 13:
					custom_gui_style.TabBorderSize = std::stof(line);
					break;
				case 14:
					custom_gui_style.WindowRounding = std::stof(line);
					break;
				case 15:
					custom_gui_style.ChildRounding = std::stof(line);
					break;
				case 16:
					custom_gui_style.FrameRounding = std::stof(line);
					break;
				case 17:
					custom_gui_style.PopupRounding = std::stof(line);
					break;
				case 18:
					custom_gui_style.ScrollbarRounding = std::stof(line);
					break;
				case 19:
					custom_gui_style.GrabRounding = std::stof(line);
					break;
				case 20:
					custom_gui_style.LogSliderDeadzone = std::stof(line);
					break;
				case 21:
					custom_gui_style.TabRounding = std::stof(line);
					break;
				case 23:
					custom_gui_style.WindowMenuButtonPosition = static_cast<ImGuiDir_>(std::stoi(line));
					break;
				case 24:
					custom_gui_style.ColorButtonPosition = static_cast<ImGuiDir_>(std::stoi(line));
					break;
				}

				fileindex++;
			}
			r_file.close();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("SAVE"))
	{
		std::ofstream f_save;
		f_save.open(style);
		if (f_save.is_open())
		{
			//Padding
			f_save << custom_gui_style.WindowPadding.x << "," << custom_gui_style.WindowPadding.y << "\n";
			f_save << custom_gui_style.FramePadding.x << "," << custom_gui_style.FramePadding.y << "\n";
			f_save << custom_gui_style.CellPadding.x << "," << custom_gui_style.CellPadding.y << "\n";
			f_save << custom_gui_style.ItemSpacing.x << "," << custom_gui_style.ItemSpacing.y << "\n";
			f_save << custom_gui_style.ItemInnerSpacing.x << "," << custom_gui_style.ItemInnerSpacing.y << "\n";
			f_save << custom_gui_style.TouchExtraPadding.x << "," << custom_gui_style.TouchExtraPadding.y << "\n";
			f_save << custom_gui_style.IndentSpacing << "\n";
			f_save << custom_gui_style.ScrollbarSize << "\n";
			f_save << custom_gui_style.GrabMinSize << "\n";
			f_save << custom_gui_style.WindowBorderSize << "\n";
			f_save << custom_gui_style.ChildBorderSize << "\n";
			f_save << custom_gui_style.PopupBorderSize << "\n";
			f_save << custom_gui_style.FrameBorderSize << "\n";
			f_save << custom_gui_style.TabBorderSize << "\n";

			//Rounding
			f_save << custom_gui_style.WindowRounding << "\n";
			f_save << custom_gui_style.ChildRounding << "\n";
			f_save << custom_gui_style.FrameRounding << "\n";
			f_save << custom_gui_style.PopupRounding << "\n";
			f_save << custom_gui_style.ScrollbarRounding << "\n";
			f_save << custom_gui_style.GrabRounding << "\n";
			f_save << custom_gui_style.LogSliderDeadzone << "\n";
			f_save << custom_gui_style.TabRounding << "\n";

			//Position
			f_save << custom_gui_style.WindowTitleAlign.x << "," << custom_gui_style.WindowTitleAlign.y << "\n";
			f_save << custom_gui_style.WindowMenuButtonPosition << "\n";
			f_save << custom_gui_style.ColorButtonPosition << "\n";
			f_save << custom_gui_style.ButtonTextAlign.x << "," << custom_gui_style.ButtonTextAlign.y << "\n";
			f_save << custom_gui_style.SelectableTextAlign.x << "," << custom_gui_style.SelectableTextAlign.y << "\n";

			//NANI???
			f_save << custom_gui_style.DisplaySafeAreaPadding.x << "," << custom_gui_style.DisplaySafeAreaPadding.y << "\n";

			f_save.close();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("EXPORT"))
	{
		ImGui::LogToClipboard();

		ImGui::LogText("namespace ImGui {\n void CustomStyle() {\n");
		ImGui::LogText("ImGuiStyle& style = ImGui::GetStyle();\n");

		// Padding
		ImGui::LogText("style.WindowPadding = ImVec2(%.0f, %.0f);\n", custom_gui_style.WindowPadding.x, custom_gui_style.WindowPadding.y);
		ImGui::LogText("style.FramePadding = ImVec2(%.0f, %.0f);\n", custom_gui_style.FramePadding.x, custom_gui_style.FramePadding.y);
		ImGui::LogText("style.CellPadding = ImVec2(%.0f, %.0f);\n", custom_gui_style.CellPadding.x, custom_gui_style.CellPadding.y);
		ImGui::LogText("style.ItemSpacing = ImVec2(%.0f, %.0f);\n", custom_gui_style.ItemSpacing.x, custom_gui_style.ItemSpacing.y);
		ImGui::LogText("style.ItemInnerSpacing = ImVec2(%.0f, %.0f);\n", custom_gui_style.ItemInnerSpacing.x, custom_gui_style.ItemInnerSpacing.y);
		ImGui::LogText("style.TouchExtraPadding = ImVec2(%.0f, %.0f);\n", custom_gui_style.TouchExtraPadding.x, custom_gui_style.TouchExtraPadding.y);
		ImGui::LogText("style.IndentSpacing = %.0f\n", custom_gui_style.IndentSpacing);
		ImGui::LogText("style.ScrollbarSize = %.0f\n", custom_gui_style.ScrollbarSize);
		ImGui::LogText("style.GrabMinSize = %.0f\n", custom_gui_style.GrabMinSize);
		ImGui::LogText("style.WindowBorderSize = %.0f\n", custom_gui_style.WindowBorderSize);
		ImGui::LogText("style.ChildBorderSize = %.0f\n", custom_gui_style.ChildBorderSize);
		ImGui::LogText("style.PopupBorderSize = %.0f\n", custom_gui_style.PopupBorderSize);
		ImGui::LogText("style.FrameBorderSize = %.0f\n", custom_gui_style.FrameBorderSize);
		ImGui::LogText("style.TabBorderSize = %.0f\n", custom_gui_style.TabBorderSize);

		// Rounding
		ImGui::LogText("style.WindowRounding = %.0f\n", custom_gui_style.WindowRounding);
		ImGui::LogText("style.ChildRounding = %.0f\n", custom_gui_style.ChildRounding);
		ImGui::LogText("style.FrameRounding = %.0f\n", custom_gui_style.FrameRounding);
		ImGui::LogText("style.PopupRounding = %.0f\n", custom_gui_style.PopupRounding);
		ImGui::LogText("style.ScrollbarRounding = %.0f\n", custom_gui_style.ScrollbarRounding);
		ImGui::LogText("style.GrabRounding = %.0f\n", custom_gui_style.GrabRounding);
		ImGui::LogText("style.LogSliderDeadzone = %.0f\n", custom_gui_style.LogSliderDeadzone);
		ImGui::LogText("style.TabRounding = %.0f\n", custom_gui_style.TabRounding);

		// Position
		ImGui::LogText("style.WindowTitleAlign = ImVec2(%.0f, %.0f);\n", custom_gui_style.WindowTitleAlign.x, custom_gui_style.WindowTitleAlign.y);
		ImGui::LogText("style.WindowMenuButtonPosition = %d\n", custom_gui_style.WindowMenuButtonPosition);
		ImGui::LogText("style.ColorButtonPosition = %d\n", custom_gui_style.ColorButtonPosition);
		ImGui::LogText("style.ButtonTextAlign = ImVec2(%.0f, %.0f);\n", custom_gui_style.ButtonTextAlign.x, custom_gui_style.ButtonTextAlign.y);
		ImGui::LogText("style.SelectableTextAlign = ImVec2(%.0f, %.0f);\n", custom_gui_style.SelectableTextAlign.x, custom_gui_style.SelectableTextAlign.y);

		// Nani??
		ImGui::LogText("style.DisplaySafeAreaPadding = ImVec2(%.0f, %.0f);\n", custom_gui_style.DisplaySafeAreaPadding.x, custom_gui_style.DisplaySafeAreaPadding.y);

		ImGui::LogText("}\n}\n");

		ImGui::LogFinish();
	}
	ImGui::InputText("##File_STYLE", const_cast<char*>(style.c_str()), 255, 0);

	ImGui::Text("First");
	if (ImGui::SliderFloat("FrameRounding", &custom_gui_style.FrameRounding, 0.0f, 12.0f, "%.0f"))
		custom_gui_style.GrabRounding = custom_gui_style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
	{
		auto border = (custom_gui_style.WindowBorderSize > 0.0f); if (ImGui::Checkbox("WindowBorder", &border)) { custom_gui_style.WindowBorderSize = border ? 1.0f : 0.0f; } }
	ImGui::SameLine();
	{
		auto border = (custom_gui_style.FrameBorderSize > 0.0f);  if (ImGui::Checkbox("FrameBorder", &border)) { custom_gui_style.FrameBorderSize = border ? 1.0f : 0.0f; } }
	ImGui::SameLine();
	{
		auto border = (custom_gui_style.PopupBorderSize > 0.0f);  if (ImGui::Checkbox("PopupBorder", &border)) { custom_gui_style.PopupBorderSize = border ? 1.0f : 0.0f; } }

	ImGui::Text("Main");
	ImGui::SliderFloat2("WindowPadding", reinterpret_cast<float*>(&custom_gui_style.WindowPadding), 0.0f, 20.0f, "%.0f");
	ImGui::SliderFloat2("FramePadding", reinterpret_cast<float*>(&custom_gui_style.FramePadding), 0.0f, 20.0f, "%.0f");
	ImGui::SliderFloat2("CellPadding", reinterpret_cast<float*>(&custom_gui_style.CellPadding), 0.0f, 20.0f, "%.0f");
	ImGui::SliderFloat2("ItemSpacing", reinterpret_cast<float*>(&custom_gui_style.ItemSpacing), 0.0f, 20.0f, "%.0f");
	ImGui::SliderFloat2("ItemInnerSpacing", reinterpret_cast<float*>(&custom_gui_style.ItemInnerSpacing), 0.0f, 20.0f, "%.0f");
	ImGui::SliderFloat2("TouchExtraPadding", reinterpret_cast<float*>(&custom_gui_style.TouchExtraPadding), 0.0f, 10.0f, "%.0f");
	ImGui::SliderFloat("IndentSpacing", &custom_gui_style.IndentSpacing, 0.0f, 30.0f, "%.0f");
	ImGui::SliderFloat("ScrollbarSize", &custom_gui_style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
	ImGui::SliderFloat("GrabMinSize", &custom_gui_style.GrabMinSize, 1.0f, 20.0f, "%.0f");
	ImGui::Text("Borders");
	ImGui::SliderFloat("WindowBorderSize", &custom_gui_style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
	ImGui::SliderFloat("ChildBorderSize", &custom_gui_style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
	ImGui::SliderFloat("PopupBorderSize", &custom_gui_style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
	ImGui::SliderFloat("FrameBorderSize", &custom_gui_style.FrameBorderSize, 0.0f, 1.0f, "%.0f");
	ImGui::SliderFloat("TabBorderSize", &custom_gui_style.TabBorderSize, 0.0f, 1.0f, "%.0f");
	ImGui::Text("Rounding");
	ImGui::SliderFloat("WindowRounding", &custom_gui_style.WindowRounding, 0.0f, 12.0f, "%.0f");
	ImGui::SliderFloat("ChildRounding", &custom_gui_style.ChildRounding, 0.0f, 12.0f, "%.0f");
	ImGui::SliderFloat("FrameRounding", &custom_gui_style.FrameRounding, 0.0f, 12.0f, "%.0f");
	ImGui::SliderFloat("PopupRounding", &custom_gui_style.PopupRounding, 0.0f, 12.0f, "%.0f");
	ImGui::SliderFloat("ScrollbarRounding", &custom_gui_style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
	ImGui::SliderFloat("GrabRounding", &custom_gui_style.GrabRounding, 0.0f, 12.0f, "%.0f");
	ImGui::SliderFloat("LogSliderDeadzone", &custom_gui_style.LogSliderDeadzone, 0.0f, 12.0f, "%.0f");
	ImGui::SliderFloat("TabRounding", &custom_gui_style.TabRounding, 0.0f, 12.0f, "%.0f");
	ImGui::Text("Alignment");
	ImGui::SliderFloat2("WindowTitleAlign", reinterpret_cast<float*>(&custom_gui_style.WindowTitleAlign), 0.0f, 1.0f, "%.2f");
	auto window_menu_button_position = custom_gui_style.WindowMenuButtonPosition + 1;
	if (ImGui::Combo("WindowMenuButtonPosition", static_cast<int*>(&window_menu_button_position), "None\0Left\0Right\0"))
		custom_gui_style.WindowMenuButtonPosition = window_menu_button_position - 1;
	ImGui::Combo("ColorButtonPosition", static_cast<int*>(&custom_gui_style.ColorButtonPosition), "Left\0Right\0");
	ImGui::SliderFloat2("ButtonTextAlign", reinterpret_cast<float*>(&custom_gui_style.ButtonTextAlign), 0.0f, 1.0f, "%.2f");

	ImGui::SliderFloat2("SelectableTextAlign", reinterpret_cast<float*>(&custom_gui_style.SelectableTextAlign), 0.0f, 1.0f, "%.2f");
	ImGui::Text("Safe Area Padding");
	ImGui::SliderFloat2("DisplaySafeAreaPadding", reinterpret_cast<float*>(&custom_gui_style.DisplaySafeAreaPadding), 0.0f, 30.0f, "%.0f");

	ImGui::End();
}

void color_editor()
{
	auto& style = ImGui::GetStyle();
	static ImGuiStyle ref_saved_style;
	auto* ref = &ref_saved_style;
	ImGui::Begin("Gui Builder color export/import ", &color_menu);

	static auto output_only_modified = false;

	if (ImGui::Button("Export"))
	{
		ImGui::LogToClipboard();

		ImGui::LogText("namespace ImGui {\n void CustomColor() {\n");
		ImGui::LogText("ImVec4* colors = ImGui::GetStyle().Colors;\n");
		for (auto i = 0; i < ImGuiCol_COUNT; i++)
		{
			const auto& col = custom_gui_style.Colors[i];
			const auto* name = ImGui::GetStyleColorName(i);
			if (!output_only_modified || memcmp(&col, &ref->Colors[i], sizeof(ImVec4)) != 0)
				ImGui::LogText("colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);\n",
					name, 23 - static_cast<int>(strlen(name)), "", col.x, col.y, col.z, col.w);
		}
		ImGui::LogText("}\n}\n");
		ImGui::LogFinish();
	}
	ImGui::SameLine();

	if (ImGui::Button("Load"))
	{
		std::string line;
		std::ifstream fileimport(theme);

		if (fileimport.is_open())
		{
			for (auto& i : custom_gui_style.Colors)
			{
				std::getline(fileimport, line);
				auto colors = split(line, ',');
				auto prot = ImVec4(std::stof(colors[0]), std::stof(colors[1]), std::stof(colors[2]), std::stof(colors[3]));
				i = prot;
			}

			fileimport.close();
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Save"))
	{
		remove(theme.c_str());

		std::ofstream filewrite(theme);
		for (const auto col : custom_gui_style.Colors)
			filewrite << col.x << "," << col.y << "," << col.z << "," << col.w << std::endl;

		filewrite.close();
	}

	ImGui::SameLine();
	ImGui::Text("Color theme file:");
	ImGui::SameLine();
	ImGui::InputText("##THEME", const_cast<char*>(theme.c_str()), 100);

	static ImGuiTextFilter filter;
	filter.Draw("Filter colors", ImGui::GetFontSize() * 16);

	static auto alpha_flags = 0;
	if (ImGui::RadioButton("Opaque", alpha_flags == ImGuiColorEditFlags_None)) { alpha_flags = ImGuiColorEditFlags_None; } ImGui::SameLine();
	if (ImGui::RadioButton("Alpha", alpha_flags == ImGuiColorEditFlags_AlphaPreview)) { alpha_flags = ImGuiColorEditFlags_AlphaPreview; } ImGui::SameLine();
	if (ImGui::RadioButton("Both", alpha_flags == ImGuiColorEditFlags_AlphaPreviewHalf)) { alpha_flags = ImGuiColorEditFlags_AlphaPreviewHalf; }// ImGui::SameLine();

	ImGui::BeginChild("##colors", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
	ImGui::PushItemWidth(-160);
	for (auto i = 0; i < ImGuiCol_COUNT; i++)
	{
		const auto* name = ImGui::GetStyleColorName(i);
		//std::cout << "Name: " << name << " Index: " << i << std::endl;
		if (!filter.PassFilter(name))
			continue;
		ImGui::PushID(i);
		ImGui::ColorEdit4("##color", reinterpret_cast<float*>(&custom_gui_style.Colors[i]), ImGuiColorEditFlags_AlphaBar | alpha_flags);
		if (memcmp(&custom_gui_style.Colors[i], &ref->Colors[i], sizeof(ImVec4)) != 0)
		{
			ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Save")) { ref->Colors[i] = custom_gui_style.Colors[i]; }
			ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Revert")) { custom_gui_style.Colors[i] = ref->Colors[i]; }
		}
		ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
		ImGui::TextUnformatted(name);
		ImGui::PopID();
	}
	ImGui::PopItemWidth();
	ImGui::EndChild();

	ImGui::End();
}

ImGuiBuilder::ImGuiBuilder()
{
	ImGui::StyleColorsDark(&dark_);
	for (auto i = 0; i < ImGuiCol_COUNT; i++)
		custom_gui_style.Colors[i] = dark_.Colors[i];

	form_.clear();
	obj_render_me.clear();
}

//LOAD
void ImGuiBuilder::loading_builder(std::string& file)
{
	std::ifstream f_read(file);

	if (f_read.is_open())
	{
		std::string line;
		auto menu = 0;

		while (!f_read.eof())
		{
			form form_load;
			child child_load;
			simple_obj obj_load;

			std::getline(f_read, line);
			if (!line.compare("#forms"))
			{
				menu = 0;
				continue;
			}
			if (!line.compare("#child"))
			{
				menu = 1;
				continue;
			}
			if (!line.compare("#obj"))
			{
				menu = 2;
				continue;
			}
			if (!line.compare(""))
			{
				menu = 5;
				continue;
			}

			// VAR STRING
			auto vstr = split(line, ',');
			switch (menu)
			{
			case 0:

				form_load.id = std::stoi(vstr[0]);
				id_ = form_load.id;
				form_load.name = vstr[1];
				form_load.size.x = std::stof(vstr[2]);
				form_load.size.y = std::stof(vstr[3]);

				form_.push_back(form_load);

				printf("Loading form\n");
				break;

			case 1:
				child_load.id = std::stoi(vstr[0]);
				child_id = child_load.id;
				child_load.father = std::stoi(vstr[1]);
				child_load.name = vstr[2];
				child_load.size.x = std::stof(vstr[3]);
				child_load.size.y = std::stof(vstr[4]);
				child_load.pos.x = std::stof(vstr[5]);
				child_load.pos.y = std::stof(vstr[6]);
				form_[child_load.father].child.push_back(child_load);

				printf("Loading child\n");
				break;

			case 2:
				obj_load.id = std::stoi(vstr[0]);
				obj_id = obj_load.id;
				obj_load.form = std::stoi(vstr[1]);
				obj_load.child = std::stoi(vstr[2]);
				obj_load.name = vstr[3];
				obj_load.my_type = std::stoi(vstr[4]);
				obj_load.size.x = std::stof(vstr[5]);
				obj_load.size.y = std::stof(vstr[6]);
				obj_load.pos.x = std::stof(vstr[7]);
				obj_load.pos.y = std::stof(vstr[8]);

				obj_render_me.push_back(obj_load);

				printf("Loading obj\n");
				break;
			default:

				break;
			}
		}
		f_read.close();
	}
}

void ImGuiBuilder::save_building(std::string& file)
{
	remove(file.c_str());
	std::ofstream f_write(file);

	if (f_write.is_open())
	{
		for (const auto& form : form_)
		{
			f_write << "#forms\n";
			f_write << form.id << "," << form.name << "," << form.size.x << "," << form.size.y << "\n";

			for (const auto& ch : form.child)
			{
				f_write << "#child\n";
				f_write << ch.id << "," << ch.father << "," << ch.name << "," << ch.size.x << "," << ch.size.y << "," << ch.pos.x << "," << ch.pos.y << "\n";
			}

			for (const auto& obj : obj_render_me)
			{
				if (form.id == obj.form)
				{
					f_write << "#obj\n";
					f_write << obj.id << "," << obj.form << "," << obj.child << "," << obj.name << "," << obj.my_type << "," << obj.size.x << "," << obj.size.y << "," << obj.pos.x << "," << obj.pos.y << "\n";
				}
			}
		}

		f_write.close();
	}
}

void ImGuiBuilder::mainform_draw(HWND wnd)
{
	PushAllColorsDark(dark_);
	auto width = 1280;
	RECT rect;
	if (GetWindowRect(wnd, &rect))
	{
		width = rect.right - rect.left;
		//int height = rect.bottom - rect.top;
	}

	window = wnd;

	if (button_ok)
	{
		ImGui::SetNextWindowSize({ 200, 100 });
		ImGui::Begin(file_manege.c_str(), &button_ok);

		ImGui::InputText("##FILE NAME", const_cast<char*>(file.c_str()), 255);

		if (ImGui::Button("OK"))
			button_ok = !button_ok;

		if (!button_ok)
		{
			switch (file_menu)
			{
			case 0:
				save_building(file);
				break;
			case 1:
				loading_builder(file);
				break;
			default:
				break;
			}
		}

		ImGui::End();
	}

	if (color_menu)
		color_editor();

	if (style_menu)
		window_flag(custom_gui_style);

	paste_obj();

	ImGui::SetNextWindowSize({ static_cast<float>(width - 16), 100 });
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::Begin("BUILDER", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Project"))
		{
			if (ImGui::MenuItem("Save"))
			{
				button_ok = !button_ok;
				file_manege = "file to save";
				file_menu = 0;
			}

			if (ImGui::MenuItem("Open"))
			{
				button_ok = !button_ok;
				file_manege = "file to load";
				file_menu = 1;
			}

			if (ImGui::MenuItem("Generate Code"))
			{
				create_builder();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Editor"))
		{
			if (ImGui::MenuItem("Color"))
			{
				color_menu = !color_menu;
			}

			if (ImGui::MenuItem("Style"))
			{
				style_menu = !style_menu;
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::Button("New Form"))
	{
		create_form();
	}
	ImGui::SameLine();
	if (ImGui::Button("New Child"))
	{
		create_child();
	}
	ImGui::SameLine();
	if (ImGui::Button("New Button"))
	{
		create_obj(1);
	}
	ImGui::SameLine();
	if (ImGui::Button("New Label"))
	{
		create_obj(2);
	}
	ImGui::SameLine();
	if (ImGui::Button("New Text"))
	{
		create_obj(3);
	}
	ImGui::SameLine();

	if (ImGui::Button("New Slider Int"))
	{
		create_obj(4);
	}
	ImGui::SameLine();
	if (ImGui::Button("New Slider float"))
	{
		create_obj(5);
	}

	ImGui::SameLine();
	if (ImGui::Button("New CheckBox"))
	{
		create_obj(6);
	}
	ImGui::SameLine();
	if (ImGui::Button("New radio"))
	{
		create_obj(7);
	}
	ImGui::SameLine();
	if (ImGui::Button("New toggle"))
	{
		create_obj(8);
	}

	object_property();
	PopAllColorsCustom();

	PushAllColorsCustom();

	show_form();

	PopAllColorsCustom();

	ImGui::End();
}

void ImGuiBuilder::create_form()
{
	// that give warning...
	id_++;
	form_.push_back({ id_, "form" + std::to_string(id_), { 50,50 } });
}

void ImGuiBuilder::create_child()
{
	// fixed in update...
	// child id is dinamic to form
	child_id = form_[id_].child.size();
	form_[id_].child.push_back({ child_id, "child" + std::to_string(child_id), id_, true, {50,50}, {15,15} });
}

std::string get_name_type(const int type)
{
	switch (type)
	{
	case 1:return "button";
	case 2:return "label";
	case 3:return "edit";
	case 4:return "sliderI";
	case 5:return "sliderF";
	case 6:return "checkbox";;
	case 7:return "radio";
	case 8:return "toggle";
	default:
		break;
	}
	return "";
}

void ImGuiBuilder::create_obj(uint16_t type)
{
	// create any obj
	obj_id++;
	auto name = get_name_type(type);

	name += std::to_string(obj_id);
	const simple_obj new_obj = { obj_id, activeWindowID, -1, name, type, {}, {30,30} };
	//form_[id_].obj_render_me.push_back(new_obj);
	obj_render_me.push_back(new_obj);
}

void ImGuiBuilder::paste_obj() //NOT NEED  OVERLOAD FOR THAT!
{
	if (gaks(VK_LCONTROL, 'V') && wnd == GetForegroundWindow())
	{
		const auto* psz_text = ImGui::GetClipboardText();

		const std::string text(psz_text);
		auto m_copy = split(text, '\n');
		ImVec2 pos = { 30,30 };
		for (const auto& n_text : m_copy)
		{
			auto o = split(n_text, ',');

			if (o.size() != 7)
				return;

			for (auto tx : o)
			{
				if (!is_number(tx))
					return;
			}

			auto name = get_name_type(std::stoi(o[0]));

			if (std::stoi(o[0]) == 10)
			{
				child_id = form_[activeWindowID].child.size();
				form_[activeWindowID].child.push_back({ child_id, "child" + std::to_string(child_id), activeWindowID, std::stoi(o[4]) != 0, {std::stof(o[2]), std::stof(o[3])}, {std::stof(o[4]), std::stof(o[5])} });
				std::cout << "child obj\n";
			}
			else if (!name.empty())
			{
				obj_id++;
				name += std::to_string(obj_id);
				const simple_obj new_obj = { obj_id, activeWindowID, std::stoi(o[1]), name,std::stoi(o[0]) , {std::stof(o[3]), std::stof(o[4])}, {std::stof(o[5]), std::stof(o[6])} };
				obj_render_me.push_back(new_obj);
				std::cout << "paste obj\n";
			}
			pos.x += 15;
			pos.y += 15;
		}
	}
}

void ImGuiBuilder::copy_obj(const int type, const int child, const ImVec2 size, const ImVec2 pos, const bool border, bool selected)
{
	if (gaks(VK_CONTROL, 'C') && wnd == GetForegroundWindow())
	{
		std::string buffer;
		ImGui::LogToClipboard();
		if (!selected)
		{
			if (type == 10) // is child
				buffer = std::to_string(type) + ",0," + std::to_string(size.x) + "," + std::to_string(size.y) + "," + std::to_string(pos.x) + "," + std::to_string(pos.y) + "," + std::to_string(border);
			else
				buffer = std::to_string(type) + "," + std::to_string(child) + ",0," + std::to_string(size.x) + "," + std::to_string(size.y) + "," + std::to_string(pos.x) + "," + std::to_string(pos.y);
		}
		else
		{
			for (auto& copy : obj_render_me)
			{
				if (copy.selected)
				{
					//if (type == 10) // is child
					//	buffer += std::to_string(type) + ",0," + std::to_string(size.x) + "," + std::to_string(size.y) + "," + std::to_string(border) + "\n";
					//else // other obj
					buffer += std::to_string(copy.my_type) + "," + std::to_string(copy.child) + ",0," + std::to_string(copy.size.x) + "," + std::to_string(copy.size.y) +
						", " + std::to_string(copy.pos.x) + ", " + std::to_string(copy.pos.y) + "\n";
				}
			}
		}
		ImGui::LogText(buffer.c_str());
		ImGui::LogFinish();
		std::cout << "copy that " << buffer << std::endl;
	}
}

void ImGuiBuilder::show_form()
{
	for (auto& form : form_)
	{
		ImGui::SetNextWindowSize(form.size);
		//	if any other obj is moving position
		//freezer form because if obj dont have much area for hover like label form move too
		//

		if (form.delete_me)
		{
			delete_form(form.id);
			break;
		}

		ImGui::Begin(form.name.c_str(), nullptr, moving_obj ? ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove : ImGuiWindowFlags_NoCollapse);

		form.pos = ImGui::GetWindowPos(); // get value position form
		form.size = ImGui::GetWindowSize(); // get value size form

		// get propriety of form with user double click mouse
		if (ImGui::IsWindowHovered() && ImGui::IsMouseDoubleClicked(0))
		{
			name = const_cast<char*>(form.name.c_str());
			current_item = form.name + ":" + std::to_string(form.id);
			family = form.id;
			type = 0;
		}

		if (ImGui::IsWindowFocused() || ImGui::IsWindowAppearing() || ImGui::IsWindowHovered())
		{
			activeWindowID = form.id;
		}

		//Render obj save him
		for (auto& obj : obj_render_me)
		{
			if (obj.form == form.id && obj.child < 0) // if really obj for this form and not include in child
			{
				// render obj
				render_obj(obj);
			}
		}
		for (auto& ch_render : form.child)
		{
			ImGui::SetCursorPos(ch_render.pos);

			// if signal of delete object
			if (ch_render.delete_me)
			{
				// delete obj
				form.child.erase(form.child.begin() + ch_render.id);
				child_id = form.child.size() - 1;
				// if obj not is last
				if (ch_render.id != obj_render_me.size())
				{
					// previous object, before rendering the others
					for (auto new_id = ch_render.id - 1; new_id < form.child.size(); ++new_id)
					{
						form.child[new_id].id -= new_id;
					}
				}
				break; //not to render the object
			}

			ImGui::BeginChild(ch_render.name.c_str(), ch_render.size, ch_render.border);

			for (auto& obj : obj_render_me)
			{
				if (obj.form == form.id && obj.child == ch_render.id) // render obj with child
				{
					//Render all obj
					render_obj(obj);
				}
			}

			ImGui::EndChild();

			// check if selected
			ch_render.hover = ImGui::IsItemHovered();

			// thats is shame but... work good....
			if (ch_render.hover && ImGui::IsMouseClicked(0, false))
			{
				current_item = ch_render.name + ':' + std::to_string(form.id);
				family = ch_render.father;
				index = ch_render.id;
				type = 10;
			}
		}
		ImGui::End();
	}
}

void ImGuiBuilder::delete_form(int form_id)
{
	form_.erase(form_.begin() + form_id);
	id_ = form_.size() - 1;
	std::cout << "size id " << id_ << std::endl;

	for (const auto& obj : obj_render_me)
	{
		if (obj.form == form_id)
		{
			obj_render_me.erase(obj_render_me.begin() + obj.id);
			obj_id = obj_render_me.size() - 1;
			for (auto new_id = obj.id - 1; new_id < obj_render_me.size(); ++new_id)
			{
				obj_render_me[new_id].id = new_id;
			}
		}
	}

	for (size_t id = form_id - 1; id < form_.size(); ++id)
	{
		for (auto& obj : obj_render_me)
		{
			if (obj.form == form_[id].id)
				obj.form = id;
		}

		form_[id].id = id;
		std::cout << "Forms id: " << form_[id].id << std::endl;
	}
}

void ImGuiBuilder::render_obj(simple_obj& obj)
{
	// set pos for next obj render
	ImGui::SetCursorPos(obj.pos);

	// if signal of delete object
	if (obj.delete_me)
	{
		// delete obj
		obj_render_me.erase(obj_render_me.begin() + obj.id);
		// reform id objs
		obj_id = obj_render_me.size() - 1;
		// previous object, before rendering the others
		for (auto new_id = obj.id - 1; new_id < obj_render_me.size(); ++new_id)
		{
			obj_render_me[new_id].id = new_id;
			//std::cout << obj_render_me[new_id].id << std::endl;
		}
		return; //not to render the object
	}

	// To type any obj for render set in case
	// to render all obj for 1 time in loop

	// buffer for inputs
	std::string buffer = "text here";
	int valueI = 0;
	float valueF = 0;
	static bool true_bool = false;

	if (obj.selected)
	{
		ImGui::PushStyleColor(0, { 0,100,255,80 });
		ImGui::PushStyleColor(21, { 255,0,0,80 });
		ImGui::PushStyleColor(7, { 255,0,0,80 });
	}

	// render obj
	switch (obj.my_type)
	{
	case 1:
		ImGui::Button(obj.name.c_str(), obj.size);
		break;
	case 2:
		ImGui::Text(obj.name.c_str());
		break;
	case 3:
		ImGui::InputText(obj.name.c_str(), const_cast<char*>(buffer.c_str()), 254);
		break;
	case 4:
		ImGui::SliderInt(obj.name.c_str(), &valueI, 0, 100);

		break;
	case 5:
		ImGui::SliderFloat(obj.name.c_str(), &valueF, 0, 100);
		break;
	case 6:
		ImGui::Checkbox(obj.name.c_str(), &true_bool);

		break;
	case 7:
		ImGui::RadioButton(obj.name.c_str(), true_bool);
		break;
	case 8:
		ToggleButton(obj.name.c_str(), &true_bool);
		break;
	default:
		break;
	}
	if (obj.selected)
	{
		ImGui::PopStyleColor(3);
	}

	// get size and hover of object
	obj.size_obj = ImGui::GetItemRectSize();
	obj.hover = ImGui::IsItemHovered();
	// Set family and type child etc for propri and execution modification on type
	if (obj.hover && ImGui::IsMouseClicked(0, false))
	{
		if (GetKeyState(VK_CONTROL) & 0x8000)
			obj.selected = !obj.selected;
		else
		{
			for (auto& o_obj : obj_render_me)
				o_obj.selected = false;
		}

		current_item = obj.name + ':' + std::to_string(obj.id);
		family = obj.form;
		grandchild = obj.child;
		index = obj.id;
		type = obj.my_type;
	}
}

//Move obj with cursor
POINT old_pos = { 0, 0 };
int count_pos = 0;
struct move_obj
{
	int index{};
	ImVec2 pos{};
};
std::vector<move_obj> mto;
std::vector<ImVec2> old_pos_obj;
std::vector<move_obj> Move_item(std::vector<move_obj>& mto, const HWND window, bool& continue_edt)
{
	if (old_pos.x == 0 && count_pos == 2)
	{
		old_pos_obj.clear();
		GetCursorPos(&old_pos);
		ScreenToClient(GetForegroundWindow(), &old_pos);
		for (auto& i : mto)
			old_pos_obj.push_back(i.pos);
	}
	else
		++count_pos;

	moving_obj = true;
	if (GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON) && window == GetForegroundWindow() && continue_edt)
	{
		if (count_pos >= 10) // click and secure for 5 frames 30x5 ms
		{
			for (size_t i = 0; i < mto.size(); ++i)
			{
				/// <summary>
				///  work fine but have same bugs not yet fixed pls if you see that try fix
				/// </summary>
				/// <param name="mto"></param>
				/// <param name="window"></param>
				/// <param name="continue_edt"></param>
				/// <returns></returns>

				if (mto.size() != old_pos_obj.size())
				{
					count_pos = -10;
					moving_obj = false;

					return mto;
				}

				POINT pos;
				GetCursorPos(&pos);
				ScreenToClient(GetForegroundWindow(), &pos);
				if (old_pos_obj[i].x == 0 || old_pos_obj[i].y == 0)
					return mto;

				const auto x_pos = old_pos_obj[i].x + pos.x - old_pos.x;
				const auto y_pos = old_pos_obj[i].y + pos.y - old_pos.y;
				mto[i].pos.x = x_pos;
				mto[i].pos.y = y_pos;
				continue_edt = true;
				//std::cout << "moving multiply obj\n";
			}
		}
	}
	else
	{
		old_pos_obj.clear();
		moving_obj = false;
		continue_edt = false;
		count_pos = 0;
		old_pos = { 0, 0 };
	}
	std::cout << "[GROUP MOVING]X " << mto[0].pos.x << " Y  " << mto[0].pos.y << std::endl;
	return mto;
}

ImVec2 Move_item(ImVec2 obj_pos, HWND window, bool& continue_edt)
{
	if (old_pos.x == 0 && count_pos == 0)
	{
		GetCursorPos(&old_pos);
		ScreenToClient(GetForegroundWindow(), &old_pos);
		pos_obj = obj_pos;
	}
	else
		++count_pos;

	moving_obj = true;

	if ((GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON)) && window == GetForegroundWindow() && continue_edt)
	{
		if (count_pos >= 5) // click and secure for 5 frames 30x5 ms
		{
			POINT pos;
			GetCursorPos(&pos);
			ScreenToClient(GetForegroundWindow(), &pos);
			if (pos_obj.y == 0 || pos_obj.x == 0)
			{
				count_pos = -10;// try later geting cursor and original obj pos
				moving_obj = false;
			}

			//pos.x -= FormPos.x + itemsize.x / 2; // set cursor center of obj
			//pos.y -= FormPos.y + itemsize.y / 2;
			//
			//
			//obj_pos.x = static_cast<float>(pos.x);
			//obj_pos.y = static_cast<float>(pos.y);
			//
			//pos_obj + pos_cursor = push obj to diference of obj and cursor
			//pos_obj - init_pos_cursor = push obj to obj to diference of obj and cursor  (negative form)
			//
			//So
			//	if pos cursor - init cursor pos is zero and pos obj init is 10 cursor permen in the obj position
			//
			//	but if calculos is negative beetwen error
			//
			//	phps if i pos_obj + cursor - old_cursor
			//
			//
			//	no need form pos
			//	no need size of obj simple and direct!

			pos.x -= FormPos.x + itemsize.x / 2; // set cursor center of obj
			pos.y -= FormPos.y + itemsize.y / 2;
			obj_pos.x = static_cast<float>(pos.x);
			obj_pos.y = static_cast<float>(pos.y);
			//const auto x_pos = pos_obj.x + pos.x -old_pos.x;
			//const auto y_pos = pos_obj.y + pos.y -old_pos.y;
			//obj_pos.x = pos.x;
			//obj_pos.y = pos.y;
			continue_edt = true;
		}
	}
	else
	{
		pos_obj = { 0,0 };
		moving_obj = false;
		continue_edt = false;
		count_pos = 0;
		old_pos = { 0, 0 };
	}
	std::cout << "[NORMAL MOVING]X " << obj_pos.x << " Y  " << obj_pos.y << std::endl;
	return obj_pos;
}
ImVec2 Move_item(ImVec2 Obj_pos, HWND window) // overload
{
	auto hover_emulation = true;
	return  Move_item(Obj_pos, window, hover_emulation);
}

void ImGuiBuilder::object_property()
{
	ImGui::SetNextWindowPos({ 0, 100 });
	ImGui::SetNextWindowSize({ 300, 700 - 100 });
	ImGui::Begin("property", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus);

	if (ImGui::BeginCombo("##itens", current_item.c_str()))
	{
		// list all obj render in array child and form

		for (auto& n : form_)
		{
			if (n.delete_me)
			{
				break;
			}

			// it is simply possible to simplify this please do this
			auto item = n.name + ":" + std::to_string(n.id);
			const auto is_selected = (current_item == item);

			if (ImGui::Selectable(item.c_str(), is_selected))
			{
				name = const_cast<char*>(n.name.c_str());
				current_item = item;
				type = 0;
				family = n.id;
			}

			for (auto& c : n.child)
			{
				item = c.name + ":" + std::to_string(n.id);
				if (ImGui::Selectable(item.c_str(), is_selected))
				{
					family = n.id;
					index = c.id;
					type = 10;
					current_item = item;
				}
			}

			item = "";

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		for (auto& o : obj_render_me)
		{
			if (o.delete_me)
			{
				break;
			}

			auto item = o.name + ":" + std::to_string(o.id);
			const auto is_selected = (current_item == item);

			item = o.name + ":" + std::to_string(o.id);
			if (ImGui::Selectable(item.c_str(), is_selected))
			{
				family = o.form;
				grandchild = o.child;
				index = o.id;
				type = o.my_type;
				current_item = item;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	// vars for simplification functions less line length
	child chl;
	simple_obj obj;
	form fm;

	switch (type)
	{
	case -1: // none

		break;

	case 0: // form
		fm = form_[family];
		ImGui::InputInt("ID", &fm.id, 0);

		input_text("Name form", &name, 0);
		//ImGui::InputText("name form", name, 255);
		if (ImGui::Button("Apply name"))
		{
			if(name != "")
				fm.name = name;
		}
		ImGui::InputFloat("SizeX", &fm.size.x, 1);
		ImGui::InputFloat("SizeY", &fm.size.y, 1);
		ImGui::InputFloat("PosX", &fm.pos.x, 1);
		ImGui::InputFloat("PosY", &fm.pos.y, 1);

		if (ImGui::Button("DELETE") || GetAsyncKeyState(VK_DELETE) & 1)
		{
			fm.delete_me = true;
			current_item = "";
			type = -1;
		}

		form_[family] = fm;
		break;

	case 10: // child
		chl = form_[family].child[index];
		FormPos = form_[family].pos;
		ImGui::InputInt("ID", &chl.id, 0);
		ImGui::InputInt("Form Father", &chl.father, 1);

		ImGui::InputFloat("SizeX", &chl.size.x, 1);
		ImGui::InputFloat("SizeY", &chl.size.y, 1);
		ImGui::InputFloat("PosX", &chl.pos.x, 1);
		ImGui::InputFloat("PosY", &chl.pos.y, 1);
		ImGui::Checkbox("Border", &chl.border);
		itemsize = chl.size;
		if (chl.hover)
			chl.change_pos = true;

		chl.pos = Move_item(chl.pos, window, chl.change_pos);

		copy_obj(10, 0, chl.size, chl.pos, chl.border, obj.selected);

		if (ImGui::Button("DELETE") || GetAsyncKeyState(VK_DELETE) & 1)
		{
			chl.delete_me = true;
			current_item = "";
			type = -1;
		}

		form_[family].child[index] = chl;
		break;
	default: // another obj

		if (grandchild > -1)
		{
			obj = obj_render_me[index];
			FormPos = form_[family].child[grandchild].pos;
			FormPos.x += form_[family].pos.x;
			FormPos.y += form_[family].pos.y;
		}
		else
		{
			if (obj_render_me.size() > static_cast<unsigned>(index))
			{
				obj = obj_render_me[index];
				FormPos = form_[family].pos;
			}
		}

		itemsize = obj.size_obj;
		ImGui::InputInt("ID", &obj.id, 0);
		ImGui::InputInt("Form Father", &obj.form, 1);
		ImGui::InputInt("Child Father", &obj.child, 1);
		input_text("Name", &obj.name, 0);
		//ImGui::InputText("Name", name, 255);
		ImGui::InputFloat("PosX", &obj.pos.x, 1, 1);
		ImGui::InputFloat("PosY", &obj.pos.y, 1, 1);
		ImGui::InputFloat("SizeX", &obj.size.x, 1, 1);
		ImGui::InputFloat("SizeY", &obj.size.y, 1, 1);

		//obj.name = name;
		// check if hover because need for change position
		if (obj.hover)
			obj.change_pos = true;

		if (obj.selected)
		{
			mto.clear();
			for (auto& r_obj : obj_render_me)
			{
				if (r_obj.selected == true)
				{
					mto.push_back({ r_obj.id, r_obj.pos });
				}
			}

			Move_item(mto, window, obj.change_pos);

			for (auto& teste : mto)
			{
				if (obj.id == teste.index)
					obj.pos = teste.pos;
				else
					obj_render_me[teste.index].pos = teste.pos;
			}
		}
		else
			obj.pos = Move_item(obj.pos, window, obj.change_pos);

		copy_obj(obj.my_type, obj.child, obj.size, obj.pos, false, obj.selected);

		// dont delete here!
		if (ImGui::Button("DELETE") || GetAsyncKeyState(VK_DELETE) & 1)
		{
			obj.delete_me = true;
			current_item = "";
			type = -1;
		}

		obj_render_me[index] = obj;

		break;
	}

	ImGui::End();
}

void ImGuiBuilder::create_builder()
{
	static int fctn = 0;
	file_builder = "";

	file_builder.append("void ToggleButton(const char* str_id, bool* v)\n");
	file_builder.append("{ \nImVec2 p = ImGui::GetCursorScreenPos(); \nImDrawList* draw_list = ImGui::GetWindowDrawList();\nfloat height = ImGui::GetFrameHeight();\n");
	file_builder.append("float width = height * 1.55f;\nfloat radius = height * 0.50f;\n if (ImGui::InvisibleButton(str_id, ImVec2(width, height)))\n ");
	file_builder.append("  *v = !*v;\nImU32 col_bg;\nif (ImGui::IsItemHovered())\n\t  col_bg = *v ? IM_COL32(145 + 20, 211, 68 + 20, 255) : IM_COL32(218 - 20, 218 - 20, 218 - 20, 255);\n");
	file_builder.append(" else\n\t   col_bg = *v ? IM_COL32(145, 211, 68, 255) : IM_COL32(218, 218, 218, 255);\n  draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);\n   draw_list->AddCircleFilled(ImVec2(*v ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));\n}\n\n\n");

	for (const auto& form : form_)
	{
		file_builder.append("void gui_builder" + std::to_string(fctn) + "()\n{\n");
		file_builder.append("ImGui::SetNextWindowSize({" + std::to_string(static_cast<int>(form.size.x)) + ".f," + std::to_string(static_cast<int>(form.size.y)) + ".f});\n");
		file_builder.append("\nImGui::Begin( \"" + form.name + "\");\n");

		for (const auto& obj : obj_render_me)
		{
			for (const auto& chl : form.child)
			{
				//obj with child
				if (obj.child == chl.id)
				{
					file_builder.append("ImGui::SetCursorPos({" + std::to_string(static_cast<int>(chl.pos.x)) + ".f," + std::to_string(static_cast<int>(chl.pos.y)) + ".f});\n");
					file_builder.append("ImGui::BeginChild(\"" + chl.name + "\",{" + std::to_string(static_cast<int>(chl.size.x)) + ".f," + std::to_string(static_cast<int>(chl.size.y)) + ".f},true );");
					if (obj.child == chl.id && obj.form == form.id && chl.father == form.id)
					{
						file_builder.append("ImGui::SetCursorPos({" + std::to_string(static_cast<int>(obj.pos.x)) + ".f," + std::to_string(static_cast<int>(obj.pos.y)) + ".f});\n");
						//file_builder.append("ImGui::PushItemWidth(" + std::to_string(obj.size.x) + ".f);\n");
						switch (obj.my_type)
						{
						case 1:
							file_builder.append("if(ImGui::Button(\"" + obj.name + "\"," + std::to_string(static_cast<int>(obj.size.x)) + ".f," + std::to_string(static_cast<int>(obj.size.y)) + ".f}))\n{\n\n}\n");
							break;
						case 2:
							file_builder.append("ImGui::Text(\"" + obj.name + "\");\n");
							break;
						case 3:
							file_builder.append("ImGui::InputText(\"" + obj.name + "\", buffer, 255);\n");
							break;
						case 4:
							file_builder.append("ImGui::SliderInt(\"" + obj.name + "\", &valueI,0,100);\n");
							break;
						case 5:
							file_builder.append("ImGui::SliderFloat(\"" + obj.name + "\", &valueF,0,100);\n");
							break;
						case 6:
							file_builder.append("ImGui::Checkbox(\"" + obj.name + "\", &the_bool);\n");
							break;
						case 7:
							file_builder.append("ImGui::RadioButton(\"" + obj.name + "\", the_bool);\n");
							break;
						case 8:
							file_builder.append("ToggleButton(\"" + obj.name + "\", the_bool);\n");
							break;
						default:
							break;
						}
					}
					file_builder.append("\nImGui::EndChild();");
				}
			}

			//obj none child
			if (obj.child == -1 && obj.form == form.id)
			{
				file_builder.append("ImGui::SetCursorPos({" + std::to_string(static_cast<int>(obj.pos.x)) + ".f," + std::to_string(static_cast<int>(obj.pos.y)) + ".f});\n");
				//file_builder.append("ImGui::PushItemWidth(" + std::to_string(obj.size.x) + ".f);\n");
				switch (obj.my_type)
				{
				case 1:
					file_builder.append("if(ImGui::Button(\"" + obj.name + "\"," + std::to_string(static_cast<int>(obj.size.x)) + ".f," + std::to_string(static_cast<int>(obj.size.y)) + ".f}))\n{\n\n}\n");
					break;
				case 2:
					file_builder.append("ImGui::Text(\"" + obj.name + "\");\n");
					break;
				case 3:
					file_builder.append("ImGui::InputText(\"" + obj.name + "\", buffer, 255);\n");
					break;
				case 4:
					file_builder.append("ImGui::SliderInt(\"" + obj.name + "\", &valueI,0,100);\n");
					break;
				case 5:
					file_builder.append("ImGui::SliderFloat(\"" + obj.name + "\", &valueF,0,100);\n");
					break;
				case 6:
					file_builder.append("ImGui::Checkbox(\"" + obj.name + "\", &the_bool);\n");
					break;
				case 7:
					file_builder.append("ImGui::RadioButton(\"" + obj.name + "\", the_bool);\n");
					break;
				case 8:
					file_builder.append("ToggleButton(\"" + obj.name + "\", the_bool);\n");
					break;
				default:
					break;
				}
			}
		}

		file_builder.append("\n\nImGui::End();\n}\n\n\n");
		fctn += 1;
	}

	std::ofstream file_to_save;
	std::string name_file = "ImGuiBuilder_";
	name_file.append("0.cpp");
	file_to_save.open(name_file.c_str());
	for (auto i : file_builder)
		file_to_save << i;

	file_to_save.close();
	MessageBoxA(NULL, "Code been generated!", "ImGui Builder", MB_OK | MB_ICONINFORMATION);
}
