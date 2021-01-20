#include "ImGuiBuilder.h"

ImGuiStyle custom_gui_style;

bool button_ok = false;
int file_menu = 0;
bool color = false;
std::string file_manege = "File to save";
std::string theme = "theme.col";

// TYPE INDENTIFICATIONS
std::string current_item;
int identf = 0, index = 0;
indentification_btn btn;
indentification_form frm;
indentification_text txt;
indentification_basic chk;
indentification_basic tlg;
identification_slider slider_float;
identification_slider slider_integer;
indentification_basic radio;
indentification_basic lbl;
child_bar child;

ImVec2 FormPos, itemsize;
bool moving_obj = false;

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

std::string style = "Custom.style";
void window_flag(ImGuiStyle& custom_gui_style)
{
	//custom_gui_style = ImGui::GetStyle(); //custom_gui_style
	ImGui::Begin("Style Window Editor", &color);
	if (ImGui::Button("LOAD"))
	{
		std::ifstream r_file(style);
		std::string line;

		auto fileindex = 0;

		if (r_file.is_open())
		{
			while (!r_file.eof())
			{
				std::getline(r_file, line);

				if (line.find(",") != std::string::npos)
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
	window_flag(custom_gui_style);
	ImGui::Begin("Gui Builder color export/import ", &color);

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

	//ImGui::SetNextItemWidth(120);
	//ImGui::Combo("##output_type", &output_dest, "To Clipboard\0");

	// ImGui::SameLine();

	// ImGui::Checkbox("Only Modified Colors", &output_only_modified);

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
	id_ = 0; btn_id_ = 0;  txt_id_ = 0;

	for (auto i = 0; i < ImGuiCol_COUNT; i++)
		custom_gui_style.Colors[i] = dark_.Colors[i];
}

void ImGuiBuilder::loading_builder(std::string& file)
{
	std::ifstream f_read(file);

	if (f_read.is_open())
	{
		std::string line;
		auto menu = 0;
		indentification_form form_load;
		indentification_btn btn_load;
		indentification_text text_load;
		indentification_basic toggle_load, checkbox_load, radio_load;
		identification_slider sliderI_load, sliderF_load;

		while (!f_read.eof())
		{
			std::getline(f_read, line);

			/// <summary>
			/// MENU
			/// </summary>
			/// <param name="file"></param>

			if (!line.compare("#forms"))
			{
				menu = 0; continue;
			}	 if (!line.compare("#buttons"))
			{
				menu = 1; continue;
			}
			if (!line.compare("#txt"))
			{
				menu = 2; continue;
			}if (!line.compare("#sliderI"))
			{
				menu = 3; continue;
			}
			if (!line.compare("#sliderF"))
			{
				menu = 4; continue;
			}	 if (!line.compare("#toggle"))
			{
				menu = 5; continue;
			}
			if (!line.compare("#checkbox"))
			{
				menu = 6; continue;
			} if (!line.compare("#radio"))
			{
				menu = 7; continue;
			}if (!line.compare("#label"))
			{
				menu = 8; continue;
			}if (!line.compare("#childs"))
			{
				menu = 10; continue;
			}
			if (line.empty())
				continue;

			auto obj = split(line, ',');
			ImVec2 size;
			ImVec2 pos;

			switch (menu)
			{
			case 0:
				std::cout << "#forms\n";
				form_load.form_id = std::atoi(obj[0].c_str());
				id_ = form_load.form_id;
				form_load.name_form = obj[1];
				size = { static_cast<float>(std::atof(obj[2].c_str())), static_cast<float>(std::atof(obj[3].c_str())) };

				form_load.size = size;
				forms.push_back(form_load);
				//form
				break;
			case 1:
				std::cout << "##buttons\n";
				btn_load.Form_id = std::atoi(obj[0].c_str());
				btn_load.btn_id = std::atoi(obj[1].c_str());
				btn_id_ = btn_load.btn_id;

				size = { static_cast<float>(std::atof(obj[2].c_str())), static_cast<float>(std::atof(obj[3].c_str())) };

				btn_load.size_item = size;
				pos = { static_cast<float>(std::atof(obj[4].c_str())), static_cast<float>(std::atof(obj[5].c_str())) };
				btn_load.Pos_item = pos;
				btn_load.name_item = obj[6];
				buttons.push_back(btn_load);
				//btn
				break;

			case 2:
				std::cout << "##txt\n";

				text_load.Form_id = std::atoi(obj[0].c_str());
				text_load.text_id = std::atoi(obj[1].c_str());
				txt_id_ = text_load.text_id;
				text_load.wight = static_cast<float>(std::atof(obj[2].c_str()));
				pos = { static_cast<float>(std::atof(obj[3].c_str())), static_cast<float>(std::atof(obj[4].c_str())) };
				text_load.Pos_item = pos;
				text_load.name_text = obj[5];
				text_load.same_buffer = obj[6];
				texts.push_back(text_load);
				//txts
				break;
			case 3:
				std::cout << "##sliderI\n";
				sliderI_load.Form_id = std::atoi(obj[0].c_str());
				sliderI_load.slider_id = std::atoi(obj[1].c_str());
				si_ = sliderI_load.slider_id;
				sliderI_load.wight = static_cast<float>(std::atof(obj[2].c_str()));
				pos = { static_cast<float>(std::atof(obj[3].c_str())), static_cast<float>(std::atof(obj[4].c_str())) };
				sliderI_load.Pos_item = pos;
				sliderI_load.name = obj[5];
				SliderI.push_back(sliderI_load);
				//slider integer
				break;
			case 4:
				std::cout << "##sliderF\n";

				sliderF_load.Form_id = std::atoi(obj[0].c_str());
				sliderF_load.slider_id = std::atoi(obj[1].c_str());
				sf_ = sliderF_load.slider_id;
				sliderF_load.wight = static_cast<float>(std::atof(obj[2].c_str()));
				pos = { static_cast<float>(std::atof(obj[3].c_str())), static_cast<float>(std::atof(obj[4].c_str())) };
				sliderF_load.Pos_item = pos;
				sliderF_load.name = obj[5];
				SliderI.push_back(sliderF_load);
				//slider float
				break;
			case 5:
				std::cout << "##toggle\n";

				toggle_load.form_id = std::atoi(obj[0].c_str());
				toggle_load.id = std::atoi(obj[1].c_str());;
				tg_id_ = toggle_load.id;
				pos = { static_cast<float>(std::atof(obj[2].c_str())), static_cast<float>(std::atof(obj[3].c_str())) };
				toggle_load.Pos_item = pos;
				toggle_load.name = obj[4];
				toggle.push_back(toggle_load);

				//toggle
				break;
			case 6:
				std::cout << "##checkbox\n";
				checkbox_load.form_id = std::atoi(obj[0].c_str());
				checkbox_load.id = std::atoi(obj[1].c_str());;
				chk_id_ = checkbox_load.id;
				pos = { static_cast<float>(std::atof(obj[2].c_str())), static_cast<float>(std::atof(obj[3].c_str())) };
				checkbox_load.Pos_item = pos;
				checkbox_load.name = obj[4];
				checkbox.push_back(checkbox_load);
				//checkbox
				break;
			case 7:
				std::cout << "##radio\n";
				radio_load.form_id = std::atoi(obj[0].c_str());
				radio_load.id = std::atoi(obj[1].c_str());;
				rd_ = radio_load.id;
				pos = { static_cast<float>(std::atof(obj[2].c_str())), static_cast<float>(std::atof(obj[3].c_str())) };
				radio_load.Pos_item = pos;
				radio_load.name = obj[4];
				Radio.push_back(radio_load);
				//radio
				break;
			case 8:
				std::cout << "##label\n";
				lbl.form_id = std::atoi(obj[0].c_str());
				lbl.id = std::atoi(obj[1].c_str());;
				lbl_ = lbl.id;
				pos = { static_cast<float>(std::atof(obj[2].c_str())), static_cast<float>(std::atof(obj[3].c_str())) };
				lbl.Pos_item = pos;
				lbl.name = obj[4];
				label.push_back(lbl);
				//label
				break;
			case 10:
				std::cout << "##child\n";
				child.a.form_id = std::atoi(obj[0].c_str());
				child.a.id = std::atoi(obj[1].c_str());;
				chld_ = child.a.id;
				child.border = std::atoi(obj[2].c_str());
				pos = { static_cast<float>(std::atof(obj[3].c_str())), static_cast<float>(std::atof(obj[4].c_str())) };
				child.a.Pos_item = pos;
				size = { static_cast<float>(std::atof(obj[5].c_str())), static_cast<float>(std::atof(obj[6].c_str())) };
				child.size = size;
				child.a.name = obj[7];
				childs.push_back(child);
				//child
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
		f_write << "#forms\n";
		for (const auto& form : forms)
		{
			f_write << form.form_id << "," << form.name_form << "," << form.size.x << "," << form.size.y << "\n";
		}
		if (!buttons.empty())
			f_write << "#buttons\n";
		for (const auto& obj : buttons)
		{
			f_write << obj.Form_id << "," << obj.btn_id << "," << obj.size_item.x << "," << obj.size_item.y;
			f_write << "," << obj.Pos_item.x << "," << obj.Pos_item.y << "," << obj.name_item << "\n";
		}
		if (!texts.empty())
			f_write << "#txt\n";
		for (const auto& obj : texts)
		{
			f_write << obj.Form_id << "," << obj.text_id << "," << obj.wight;
			f_write << "," << obj.Pos_item.x << "," << obj.Pos_item.y << "," << obj.name_text << "," << obj.same_buffer << "\n";
		}
		if (!SliderI.empty())
			f_write << "#sliderI\n";
		for (const auto& obj : SliderI)
		{
			f_write << obj.Form_id << "," << obj.slider_id << "," << obj.wight;
			f_write << "," << obj.Pos_item.x << "," << obj.Pos_item.y << "," << obj.name << "\n";
		}
		if (!SliderF.empty())
			f_write << "#sliderF\n";
		for (const auto& obj : SliderF)
		{
			f_write << obj.Form_id << "," << obj.slider_id << "," << obj.wight;
			f_write << "," << obj.Pos_item.x << "," << obj.Pos_item.y << "," << obj.name << "\n";
		}
		if (!toggle.empty())
			f_write << "#toggle\n";
		for (const auto& obj : toggle)
		{
			f_write << obj.form_id << "," << obj.id;
			f_write << "," << obj.Pos_item.x << "," << obj.Pos_item.y << "," << obj.name << "\n";
		}
		if (!checkbox.empty())
			f_write << "#checkbox\n";
		for (const auto& obj : checkbox)
		{
			f_write << obj.form_id << "," << obj.id;
			f_write << "," << obj.Pos_item.x << "," << obj.Pos_item.y << "," << obj.name << "\n";
		}
		if (!Radio.empty())
			f_write << "#radio\n";
		for (const auto& obj : Radio)
		{
			f_write << obj.form_id << "," << obj.id;
			f_write << "," << obj.Pos_item.x << "," << obj.Pos_item.y << "," << obj.name << "\n";
		}
		if (!label.empty())
			f_write << "#label\n";
		for (const auto& obj : label)
		{
			f_write << obj.form_id << "," << obj.id;
			f_write << "," << obj.Pos_item.x << "," << obj.Pos_item.y << "," << obj.name << "\n";
		}
		if (!childs.empty())
			f_write << "#childs\n";
		for (const auto& obj : childs)
		{
			f_write << obj.a.form_id << "," << obj.a.id << "," << obj.border;
			f_write << "," << obj.a.Pos_item.x << "," << obj.a.Pos_item.y;
			f_write << "," << obj.size.x << "," << obj.size.y << "," << obj.a.name << "\n";
		}

		f_write.close();
	}
}

void ImGuiBuilder::mainform_draw(HWND wnd)
{
	//PushAllColorsDark(); no make  sanse
	//ImGui::StyleColorsDark();

	PushAllColorsDark(dark_);
	auto width = 1280;
	RECT rect;
	if (GetWindowRect(wnd, &rect))
	{
		width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
	}

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

	ImGui::SetNextWindowSize({ static_cast<float>(width - 16), 100 });
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::Begin("BUILDER", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);

	//std::cout << width << std::endl;

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
		ImGui::EndMenuBar();
	}

	if (ImGui::Button("New Form"))
		create_from();
	ImGui::SameLine();
	if (ImGui::Button("New Button"))
		create_button();
	ImGui::SameLine();
	if (ImGui::Button("New Text"))
		create_text();
	ImGui::SameLine();
	if (ImGui::Button("New CheckBox"))
		create_chk();
	ImGui::SameLine();
	if (ImGui::Button("New Toggle Button"))
		create_toggle();
	ImGui::SameLine();
	if (ImGui::Button("New Slider Int"))
		create_sliderI();
	ImGui::SameLine();
	if (ImGui::Button("New Slider float"))
		create_sliderF();
	ImGui::SameLine();
	if (ImGui::Button("New RadioButton"))
		create_radio();
	ImGui::SameLine();
	if (ImGui::Button("New Label"))
		create_label();
	ImGui::SameLine();
	if (ImGui::Button("New Child"))
		create_child();
	ImGui::SameLine();

	/* if (ImGui::Button("Generate Code"))
	{
		create_builder();
	} */

	ImGui::SameLine();

	ImGui::Checkbox("Editor", &color);
	if (color)
		color_editor();

	show_propriets_geral();
	PopAllColorsCustom();
	//PopAllColorsDark(); no make sense

	PushAllColorsCustom();

	show_form(wnd);

	PopAllColorsCustom();

	ImGui::End();
}

void ImGuiBuilder::create_from()
{
	++id_;
	std::string form_name = "form id:";
	form_name.append(std::to_string(id_));
	forms.push_back({ id_, { 200, 100 }, false, form_name });
	current_item = form_name + ":" + std::to_string(id_);
	frm = forms[id_ - 1];
	index = id_ - 1;
	identf = 1; // forms
}

void ImGuiBuilder::create_button()
{
	btn_id_++;
	std::string name = "button";
	name.append(std::to_string(btn_id_));
	buttons.push_back({ id_, btn_id_, { 10, 10 }, { 0, 0 }, false, name });
	current_item = name + ":" + std::to_string(btn_id_);
	btn = buttons[btn_id_ - 1];
	index = btn_id_ - 1;
	identf = 2;
}

void ImGuiBuilder::create_text()
{
	txt_id_++;
	std::string name = "txt_edit";
	name.append(std::to_string(txt_id_));
	texts.push_back({ id_, txt_id_, { 10, 20 }, 0, false, name, "Text here" });
	current_item = name + ":" + std::to_string(txt_id_);
	txt = texts[txt_id_ - 1];
	index = txt_id_ - 1;
	identf = 3;
}

void ImGuiBuilder::create_chk()
{
	chk_id_++;
	const auto name = "checkbox" + std::to_string(chk_id_);
	checkbox.push_back({ id_, chk_id_, { 10, 20 }, name });
	current_item = name + ":" + std::to_string(chk_id_);
	chk = checkbox[chk_id_ - 1];
	index = chk_id_ - 1;
	identf = 4;
}

void ImGuiBuilder::create_toggle()
{
	tg_id_++;
	const auto name = "toggle" + std::to_string(tg_id_);
	toggle.push_back({ id_, tg_id_, { 10, 20 }, name });
	current_item = name + ":" + std::to_string(tg_id_);
	tlg = toggle[tg_id_ - 1];
	index = tg_id_ - 1;
	identf = 5;
}

void ImGuiBuilder::create_sliderI()
{
	si_++;
	const auto name = "SliderInt" + std::to_string(si_);
	SliderI.push_back({ id_, si_, { 10, 20 }, 0, name });
	current_item = name + ":" + std::to_string(si_);
	slider_integer = SliderI[si_ - 1];
	index = si_ - 1;
	identf = 6;
}

void ImGuiBuilder::create_sliderF()
{
	sf_++;
	const auto name = "SliderFloat" + std::to_string(sf_);
	SliderF.push_back({ id_, sf_, { 10, 20 }, 0, name });
	current_item = name + ":" + std::to_string(sf_);
	slider_float = SliderF[sf_ - 1];
	index = sf_ - 1;
	identf = 7;
}

void ImGuiBuilder::create_radio()
{
	rd_++;
	const auto name = "Radio" + std::to_string(rd_);
	Radio.push_back({ id_, rd_, { 10, 20 }, name });
	current_item = name + ":" + std::to_string(rd_);
	index = rd_ - 1;
	radio = Radio[index];
	identf = 8;
}

void ImGuiBuilder::create_label()
{
	lbl_++;
	const auto name = "label:" + std::to_string(lbl_);
	label.push_back({ id_, lbl_, { 10, 20 }, "You text here" });
	current_item = "You text here:" + std::to_string(lbl_);
	index = lbl_ - 1;
	lbl = label[index];
	identf = 9;
}

void ImGuiBuilder::create_child()
{
	chld_++;
	const auto name = "Child:" + std::to_string(chld_);

	childs.push_back({ { id_, chld_, { 10, 20 }, name }, true, { 30, 30 } });
	current_item = name + ":" + std::to_string(chld_);
	index = chld_ - 1;
	child = childs[index];
	identf = 20;
}

void ImGuiBuilder::show_form(HWND window)
{
	ImGuiBuilder::window = window;

	for (size_t id_form = 0; id_form < forms.size(); id_form++)
	{
		if (forms[id_form].edition)
		{
			continue;
		}

		ImGui::SetNextWindowSize(forms[id_form].size);
		if (!moving_obj)
			ImGui::Begin(forms[id_form].name_form.c_str(), nullptr, 0);
		else
			ImGui::Begin(forms[id_form].name_form.c_str(), nullptr, ImGuiWindowFlags_NoMove);

		forms[id_form].size = ImGui::GetWindowSize();
		forms[id_form].pos = ImGui::GetWindowPos();

		if( ImGui::IsWindowHovered() && ImGui::IsMouseDoubleClicked(0))
		{
			current_item = forms[id_form].name_form + ":" + std::to_string(forms[id_form].form_id);
			//frm = forms[ n ];
			index = id_form;
			identf = 1; // forms
		}

		
		//Child
		for (auto& id_child : childs)
		{
			if (id_child.a.form_id == forms[id_form].form_id)
			{
				ImGui::SetCursorPos(id_child.a.Pos_item);
				ImGui::BeginChild(id_child.a.name.c_str(), id_child.size, id_child.border, ImGuiWindowFlags_NoBringToFrontOnFocus);

				ImGui::EndChild();
				id_child.hover = ImGui::IsItemHovered();
				//itemsize = ImGui::GetItemRectSize(); // not need
			}
		}

		//Buttons
		for (auto& button : buttons)
		{
			if (button.Form_id == forms[id_form].form_id)
			{
				ImGui::SetCursorPos(button.Pos_item);
				if(ImGui::Button(button.name_item.c_str(), button.size_item))
				{
					//Insecure
					current_item = button.name_item + ":" + std::to_string(button.btn_id);
					//frm = forms[ n ];
					index = button.btn_id-1;
					identf = 2; // forms
				}
				button.size_obj_ac = ImGui::GetItemRectSize();
				button.hover = ImGui::IsItemHovered();
				
			}
		}

		//Texts
		for (auto& text : texts)
		{
			if (text.Form_id == forms[id_form].form_id)
			{
				ImGui::SetCursorPos(text.Pos_item);
				ImGui::PushItemWidth(text.wight);
				ImGui::InputText(text.name_text.c_str(), const_cast<char*>(text.same_buffer.c_str()), 25);
				text.size_obj_ac = ImGui::GetItemRectSize();
				text.hover = ImGui::IsItemHovered();
				//printf("ON RENDER %f %f \n", text.size_obj_ac.x, text.size_obj_ac.y);
			}
		}

		//CheckBox
		for (auto& id_chk : checkbox)
		{
			if (id_chk.form_id == forms[id_form].form_id)
			{
				ImGui::SetCursorPos(id_chk.Pos_item);
				static bool vb;
				ImGui::Checkbox(id_chk.name.c_str(), &vb);
				id_chk.size_obj_ac = ImGui::GetItemRectSize();
				id_chk.hover = ImGui::IsItemHovered();
			}
		}

		//Toggle
		for (auto& id_tg : toggle)
		{
			if (id_tg.form_id == forms[id_form].form_id)
			{
				ImGui::SetCursorPos(id_tg.Pos_item);
				static bool vb;
				ToggleButton(id_tg.name.c_str(), &vb);
				id_tg.size_obj_ac = ImGui::GetItemRectSize();
				id_tg.hover = ImGui::IsItemHovered();
			}
		}
		//Radio box
		for (size_t i = 0; i < Radio.size(); ++i)
		{
			if (Radio[i].form_id == forms[id_form].form_id)
			{
				ImGui::SetCursorPos(Radio[i].Pos_item);
				static int vi;
				ImGui::RadioButton(Radio[i].name.c_str(), &vi, i);
				Radio[i].size_obj_ac = ImGui::GetItemRectSize();
				Radio[i].hover = ImGui::IsItemHovered();
			}
		}
		//Slider Integer
		for (auto& id_si : SliderI)
		{
			if (id_si.Form_id == forms[id_form].form_id)
			{
				ImGui::SetCursorPos(id_si.Pos_item);
				static int vl;
				ImGui::PushItemWidth(id_si.wight);
				ImGui::SliderInt(id_si.name.c_str(), &vl, 0, 100);
				id_si.size_obj_ac = ImGui::GetItemRectSize();
				id_si.hover = ImGui::IsItemHovered();
			}
		}

		//Slider Float
		for (auto& id_si : SliderF)
		{
			if (id_si.Form_id == forms[id_form].form_id)
			{
				ImGui::SetCursorPos(id_si.Pos_item);
				static float vl;
				ImGui::PushItemWidth(id_si.wight);
				ImGui::SliderFloat(id_si.name.c_str(), &vl, 0, 100);
				id_si.size_obj_ac = ImGui::GetItemRectSize();
				id_si.hover = ImGui::IsItemHovered();
			}
		}
		//label
		for (auto& lbl : label)
		{
			if (lbl.form_id == forms[id_form].form_id)
			{
				ImGui::SetCursorPos(lbl.Pos_item);
				ImGui::Text(lbl.name.c_str());
				lbl.size_obj_ac = ImGui::GetItemRectSize();
				lbl.hover = ImGui::IsItemHovered();
			}
		}

		ImGui::End();
	}
}

//Move obj with cursor
POINT old_pos = { 0, 0 };
int count_pos = 0;
ImVec2 Move_item(ImVec2 Obj_pos, HWND window, bool& continue_edt)
{
	if (old_pos.x == 0 && count_pos == 0)
		GetCursorPos(&old_pos);

	//else if (count_pos == 1)
	//    SetCursorPos(Obj_pos.x, Obj_pos.y);

	POINT new_pos;

	moving_obj = true;
	if (GetAsyncKeyState(VK_LBUTTON) && window == GetForegroundWindow() && continue_edt) //
	{
		if (count_pos >= 5) // click and secure for 5 frames 30x5 ms
		{
			//GetCursorPos(&new_pos);
			//Obj_pos.x = new_pos.x;
			//Obj_pos.y = new_pos.y;
			POINT pos;
			GetCursorPos(&pos);
			ScreenToClient(GetForegroundWindow(), &pos);
			pos.x -= FormPos.x + itemsize.x / 2;
			pos.y -= FormPos.y + itemsize.y / 2;
			// printf_s( "%.f %.f\n", itemsize.x, itemsize.y );
			Obj_pos.x = static_cast<float>(pos.x);
			Obj_pos.y = static_cast<float>(pos.y);
			continue_edt = true;
		}
		++count_pos;
	}
	else
	{
		moving_obj = false;
		continue_edt = false;
		count_pos = 0;
		//SetCursorPos(old_pos.x, old_pos.y);
		old_pos = { 0, 0 };
	}
	return Obj_pos;
}
ImVec2 Move_item(ImVec2 Obj_pos, HWND window)
{
	auto hover_emulation = true;
	return  Move_item(Obj_pos, window, hover_emulation);
}

void ImGuiBuilder::show_propriets_geral()
{
	PushAllColorsDark(dark_);
	ImGui::SetNextWindowPos({ 0, 100 });
	ImGui::SetNextWindowSize({ 300, 700 - 100 });
	ImGui::Begin("PROPRIEDADES", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus);

	if (ImGui::BeginCombo("##itens", current_item.c_str()))
	{
		for (size_t n = 0; n < forms.size(); n++)
		{
			if (forms[n].edition)
			{
				forms[n].edition = false;
				forms.erase(forms.begin() + n);

				break;
			}

			auto item = forms[n].name_form + ":" + std::to_string(forms[n].form_id);
			const auto is_selected = (current_item == item);
			if (ImGui::Selectable(item.c_str(), is_selected))
			{
				current_item = forms[n].name_form + ":" + std::to_string(forms[n].form_id);
				//frm = forms[ n ];
				index = n;
				identf = 1; // forms
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		for (size_t n = 0; n < buttons.size(); n++)
		{
			auto item = buttons[n].name_item + ":" + std::to_string(buttons[n].btn_id);

			const auto is_selected = (current_item == item);
			if (ImGui::Selectable(item.c_str(), is_selected))
			{
				current_item = buttons[n].name_item + ":" + std::to_string(buttons[n].btn_id);
				//btn = buttons[ n ];
				index = n;
				identf = 2;// buttons
			}

			if (is_selected)
			{
				//btn = buttons[n];
				ImGui::SetItemDefaultFocus();
			}
		}
		for (size_t n = 0; n < texts.size(); n++)
		{
			auto item = texts[n].name_text + ":" + std::to_string(texts[n].text_id);
			const auto is_selected = (current_item == item);
			if (ImGui::Selectable(item.c_str(), is_selected))
			{
				current_item = texts[n].name_text + ":" + std::to_string(texts[n].text_id);
				index = n;
				identf = 3;
				// obsolete
				//txt = texts[n];
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		for (size_t n = 0; n < checkbox.size(); n++)
		{
			auto item = checkbox[n].name + ":" + std::to_string(checkbox[n].id);
			const auto is_selected = (current_item == item);
			if (ImGui::Selectable(item.c_str(), is_selected))
			{
				current_item = checkbox[n].name + ":" + std::to_string(checkbox[n].id);
				//chk = checkbox[ n ];
				index = n;
				identf = 4;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		for (size_t n = 0; n < toggle.size(); n++)
		{
			auto item = toggle[n].name + ":" + std::to_string(toggle[n].id);
			const auto is_selected = (current_item == item);
			if (ImGui::Selectable(item.c_str(), is_selected))
			{
				current_item = toggle[n].name + ":" + std::to_string(toggle[n].id);
				//tlg = toggle[ n ];
				index = n;
				identf = 5;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		for (size_t n = 0; n < SliderI.size(); n++)
		{
			auto item = SliderI[n].name + ":" + std::to_string(SliderI[n].slider_id);
			const auto is_selected = (current_item == item);
			if (ImGui::Selectable(item.c_str(), is_selected))
			{
				current_item = SliderI[n].name + ":" + std::to_string(SliderI[n].slider_id);
				//slider_integer = SliderI[ n ];
				index = n;
				identf = 6;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		for (size_t n = 0; n < SliderF.size(); n++)
		{
			auto item = SliderF[n].name + ":" + std::to_string(SliderF[n].slider_id);
			const auto is_selected = (current_item == item);
			if (ImGui::Selectable(item.c_str(), is_selected))
			{
				current_item = SliderF[n].name + ":" + std::to_string(SliderF[n].slider_id);
				//slider_float = SliderF[ n ];
				index = n;
				identf = 7;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		for (size_t n = 0; n < Radio.size(); n++)
		{
			auto item = Radio[n].name + ":" + std::to_string(Radio[n].id);
			const auto is_selected = (current_item == item);
			if (ImGui::Selectable(item.c_str(), is_selected))
			{
				current_item = Radio[n].name + ":" + std::to_string(Radio[n].id);
				//radio = Radio[ n ];
				index = n;
				identf = 8;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		for (size_t n = 0; n < childs.size(); n++)
		{
			auto item = childs[n].a.name + ":" + std::to_string(childs[n].a.id);
			const auto is_selected = (current_item == item);
			if (ImGui::Selectable(item.c_str(), is_selected))
			{
				current_item = childs[n].a.name + ":" + std::to_string(childs[n].a.id);
				//child = childs[ n ];
				index = n;
				identf = 20;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		for (size_t n = 0; n < label.size(); n++)
		{
			auto item = label[n].name + ":" + std::to_string(label[n].id);
			const auto is_selected = (current_item == item);
			if (ImGui::Selectable(item.c_str(), is_selected))
			{
				current_item = label[n].name + ":" + std::to_string(label[n].id);
				//lbl = label[ n ];
				index = n;
				identf = 9;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	ImGui::End();

	switch (identf)
	{
	case 1:
		show_propriedades_form(forms[index]);
		// forms[index] = frm;
		break;

	case 2:
		itemsize = buttons[index].size_obj_ac;
		//hover_obj = btn.hover;
		//FormPos = forms[btn.Form_id-1].pos;
		show_propriedades_btn(buttons[index]); // otimize frame per responce
		//buttons[ index ] = btn;
		break;

	case 3:

		itemsize = texts[index].size_obj_ac;
		show_propriedades_txt(texts[index]);
		//texts[ index ] = txt;
		break;
	case 4:
		itemsize = checkbox[index].size_obj_ac;

		show_propriedades_basic(checkbox[index]);
		//checkbox[ index ] = chk;
		break;
	case 5:
		itemsize = toggle[index].size_obj_ac;

		show_propriedades_basic(toggle[index]);
		//toggle[ index ] = tlg;
		break;

	case 6:
		itemsize = SliderI[index].size_obj_ac;
		show_propriedades_slider(SliderI[index]);
		// SliderI[index] = slider_integer;
		break;
	case 7:
		itemsize = SliderF[index].size_obj_ac;

		show_propriedades_slider(SliderF[index]);
		//SliderF[ index ] = slider_float;
		break;
	case 8:
		itemsize = Radio[index].size_obj_ac;

		show_propriedades_basic(Radio[index]);
		//Radio[ index ] = radio;
		break;
	case 9:
		itemsize = label[index].size_obj_ac;

		show_propriedades_basic(label[index]);
		//label[ index ] = lbl;
		break;
	case 20:
		itemsize = childs[index].size;
		show_child_propriedade(childs[index]);
		//childs[ index ] = child;
		break;

	default:
		break;
	}
	PopAllColorsCustom();
}

void ImGuiBuilder::show_propriedades_basic(indentification_basic& obj_basic)
{
	ImGui::SetNextWindowPos({ 0, 100 });
	ImGui::SetNextWindowSize({ 200, 700 - 100 });
	ImGui::Begin("PROPRIEDADES", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus);
	float v[2] = { obj_basic.Pos_item.x, obj_basic.Pos_item.y };
	ImGui::SliderFloat("posX", &v[0], 0, 1000);
	ImGui::SliderFloat("posY", &v[1], 0, 1000);
	auto* name = const_cast<char*>(obj_basic.name.c_str());
	ImGui::InputText("Name", name, 100);

	for (const auto& fmp : forms)
	{
		if (btn.Form_id == fmp.form_id)
			FormPos = fmp.pos;
	}

	int father = obj_basic.form_id;
	ImGui::InputInt("ID Form PAI", &father, 1, 10);
	obj_basic.form_id = father;

	for (const auto& fmp : forms)
	{
		if (obj_basic.form_id == fmp.form_id)
			FormPos = fmp.pos;
	}
	obj_basic.Pos_item.x = v[0];
	obj_basic.Pos_item.y = v[1];

	if (obj_basic.hover)
		obj_basic.edt_pos = true;

	obj_basic.Pos_item = Move_item(obj_basic.Pos_item, window, obj_basic.edt_pos);
	obj_basic.name = name;
	ImGui::End();
}

void ImGuiBuilder::show_propriedades_slider(identification_slider& slider)
{
	ImGui::SetNextWindowPos({ 0, 100 });
	ImGui::SetNextWindowSize({ 200, 700 - 100 });
	ImGui::Begin("PROPRIEDADES", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus);
	float v[2] = { slider.Pos_item.x, slider.Pos_item.y };
	ImGui::SliderFloat("posX", &v[0], 0, 1000);
	ImGui::SliderFloat("posY", &v[1], 0, 1000);
	auto* name = const_cast<char*>(slider.name.c_str());
	ImGui::InputText("Name", name, 100);
	auto width = slider.wight;

	ImGui::InputFloat("width", &width, 1, 1);
	int father = slider.Form_id;
	ImGui::InputInt("ID Form PAI", &father, 1, 10);
	slider.Form_id = father;

	for (const auto& fmp : forms)
	{
		if (slider.Form_id == fmp.form_id)
			FormPos = fmp.pos;
	}

	slider.Pos_item.x = v[0];
	slider.Pos_item.y = v[1];
	if (slider.hover)
		slider.edt_pos = true;

	slider.Pos_item = Move_item(slider.Pos_item, window, slider.edt_pos);
	slider.wight = width;

	slider.name = name;
	ImGui::End();
}

void ImGuiBuilder::show_propriedades_btn(indentification_btn& item_button)
{
	ImGui::SetNextWindowPos({ 0, 100 });
	ImGui::SetNextWindowSize({ 200, 700 - 100 });
	ImGui::Begin("PROPRIEDADES", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus);
	float v[2] = { item_button.Pos_item.x, item_button.Pos_item.y };
	ImGui::SliderFloat("posX", &v[0], 0, 1000);
	ImGui::SliderFloat("posY", &v[1], 0, 1000);
	auto* name = const_cast<char*>(item_button.name_item.c_str());
	ImGui::InputText("Name", name, 100);
	auto width = item_button.size_item.x;
	auto height = item_button.size_item.y;
	ImGui::InputFloat("width", &width, 1, 1);
	ImGui::InputFloat("height", &height, 1, 1);
	int father = item_button.Form_id;
	ImGui::InputInt("ID Form PAI", &father, 1, 10);

	const ImVec2 size(width, height);

	item_button.Form_id = father;
	for (const auto& fmp : forms)
	{
		if (item_button.Form_id == fmp.form_id)
			FormPos = fmp.pos;
	}

	item_button.Pos_item.x = v[0];
	item_button.Pos_item.y = v[1];

	if (item_button.hover)
		item_button.edition_pos = true;

	item_button.Pos_item = Move_item(item_button.Pos_item, window, item_button.edition_pos);

	item_button.size_item = size;
	item_button.name_item = name;

	ImGui::End();
}

void ImGuiBuilder::show_propriedades_txt(indentification_text& text)
{
	ImGui::SetNextWindowPos({ 0, 100 });
	ImGui::SetNextWindowSize({ 200, 700 - 100 });
	ImGui::Begin("PROPRIEDADES", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus);
	float v[2] = { text.Pos_item.x, text.Pos_item.y };
	ImGui::SliderFloat("posX", &v[0], 0, 1000);
	ImGui::SliderFloat("posY", &v[1], 0, 1000);
	auto* name = const_cast<char*>(text.name_text.c_str());
	ImGui::InputText("Name", name, 100);

	auto width = text.wight;

	ImGui::InputFloat("width", &width, 1, 1);
	int father = text.Form_id;
	ImGui::InputInt("ID Form PAI", &father, 1, 10);

	//ImGui::InputInt("Value", text.)

	text.Form_id = father;
	for (const auto& fmp : forms)
	{
		if (text.Form_id == fmp.form_id)
			FormPos = fmp.pos;
	}

	text.Pos_item.x = v[0];
	text.Pos_item.y = v[1];

	if (text.hover)
		text.edition = true;

	text.Pos_item = Move_item(text.Pos_item, window, text.edition);
	text.wight = width;
	text.name_text = name;

	ImGui::End();
}

void ImGuiBuilder::show_propriedades_form(indentification_form& form)
{
	ImGui::SetNextWindowPos({ 0, 100 });
	ImGui::SetNextWindowSize({ 200, 700 - 100 });
	ImGui::Begin("PROPRIEDADES", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus);

	static char name[100] = { ' ' };
	if (name[0] == ' ')
		strcpy(name, form.name_form.c_str());

	int ids = form.form_id;
	ImGui::InputInt("ID", &ids, 1, 1);
	ImGui::InputText("Name", name, 100);
	if (ImGui::Button("Aplicar"))
	{
		form.name_form = name;
		strcpy(name, " ");
	}

	ImGui::InputFloat("width", &form.size.x, 1, 1);
	ImGui::InputFloat("height", &form.size.y, 1, 1);

	// ImVec2 size(width, height);

	form.form_id = ids;
	// form.size = size;

	if (ImGui::Button("APAGAR"))
	{
		form.edition = true;
		identf = 0;
		current_item = "";
	}

	ImGui::End();
}

void ImGuiBuilder::show_child_propriedade(child_bar& child)
{
	ImGui::SetNextWindowPos({ 0, 100 });
	ImGui::SetNextWindowSize({ 200, 700 - 100 });
	ImGui::Begin("PROPRIEDADES", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus);
	float v[2] = { child.a.Pos_item.x, child.a.Pos_item.y };
	ImGui::SliderFloat("posX", &v[0], 0, 1000);
	ImGui::SliderFloat("posY", &v[1], 0, 1000);
	auto* name = const_cast<char*>(child.a.name.c_str());
	ImGui::InputText("Name", name, 100);
	ImGui::Checkbox("Borda", &child.border);
	auto width = child.size.x;
	auto height = child.size.y;
	ImGui::InputFloat("width", &width, 1, 1);
	ImGui::InputFloat("height", &height, 1, 1);

	child.size = { width, height };
	int father = child.a.form_id;
	ImGui::InputInt("ID Form PAI", &father, 1, 10);
	child.a.form_id = father;
	for (const auto& fmp : forms)
	{
		if (child.a.form_id == fmp.form_id)
			FormPos = fmp.pos;
	}

	child.a.Pos_item.x = v[0];
	child.a.Pos_item.y = v[1];

	if (child.hover)
		child.edt_pos = true;
	child.a.Pos_item = Move_item(child.a.Pos_item, window, child.edt_pos);

	child.a.name = name;
	ImGui::End();
}

void ImGuiBuilder::create_builder()
{
	file_builder = "";

	if (!toggle.empty())
	{
		file_builder.append("void ToggleButton(const char* str_id, bool* v)\n");
		file_builder.append("{ \nImVec2 p = ImGui::GetCursorScreenPos(); \nImDrawList* draw_list = ImGui::GetWindowDrawList();\nfloat height = ImGui::GetFrameHeight();\n");
		file_builder.append("float width = height * 1.55f;\nfloat radius = height * 0.50f;\n if (ImGui::InvisibleButton(str_id, ImVec2(width, height)))\n ");
		file_builder.append("  *v = !*v;\nImU32 col_bg;\nif (ImGui::IsItemHovered())\n\t  col_bg = *v ? IM_COL32(145 + 20, 211, 68 + 20, 255) : IM_COL32(218 - 20, 218 - 20, 218 - 20, 255);\n");
		file_builder.append(" else\n\t   col_bg = *v ? IM_COL32(145, 211, 68, 255) : IM_COL32(218, 218, 218, 255);\n  draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);\n   draw_list->AddCircleFilled(ImVec2(*v ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));\n}");
	}

	for (size_t i = 0; i < forms.size(); ++i)
	{
		file_builder.append("\nvoid ImGuiBuilder" + std::to_string(i) + "()\n{\n");
		file_builder.append("ImGui::SetNextWindowSize({" + std::to_string(static_cast<int>(forms[i].size.x)) + ".f," + std::to_string(static_cast<int>(forms[i].size.y)) + ".f});\n");
		file_builder.append("ImGui::Begin(\"" + forms[i].name_form + "\");");
		file_builder.append("\n");

		for (auto& button : buttons)
		{
			if (button.Form_id == forms[i].form_id)
			{
				file_builder.append("ImGui::SetCursorPos({" + std::to_string(static_cast<int>(button.Pos_item.x)) + ".f," + std::to_string(static_cast<int>(
					button.Pos_item.y)) + ".f});\n");
				file_builder.append("ImGui::Button(\"" + button.name_item + "\", {" + std::to_string(static_cast<int>(
					button.size_item.x)) + ".f," + std::to_string(static_cast<int>(button.size_item.y)) + ".f});\n\n");
			}
		}
		for (auto& text : texts)
		{
			if (text.Form_id == forms[i].form_id)
			{
				file_builder.append("ImGui::SetCursorPos({" + std::to_string(static_cast<int>(text.Pos_item.x)) + ".f," + std::to_string(static_cast<int>(text.Pos_item.y)) + ".f});\n");
				file_builder.append("ImGui::PushItemWidth(" + std::to_string(static_cast<int>(text.wight)) + ".f);\n");
				file_builder.append("ImGui::InputText(\"" + text.name_text + "\",\"" + text.same_buffer + "\",25);\n\n");
			}
		}
		for (auto& chk : checkbox)
		{
			if (chk.form_id == forms[i].form_id)
			{
				file_builder.append("ImGui::SetCursorPos({" + std::to_string(static_cast<int>(chk.Pos_item.x)) + ".f," + std::to_string(static_cast<int>(chk.Pos_item.y)) + ".f});\n");
				file_builder.append("static bool var_bool;\n");
				file_builder.append("ImGui::Checkbox(" + chk.name + ", var_bool);\n\n");
			}
		}

		for (auto& tgl : toggle)
		{
			if (tgl.form_id == forms[i].form_id)
			{
				file_builder.append("ImGui::SetCursorPos({" + std::to_string(static_cast<int>(tgl.Pos_item.x)) + ".f," + std::to_string(static_cast<int>(tgl.Pos_item.y)) + ".f});\n");
				file_builder.append("static bool var_tgl;\n");
				file_builder.append("ToggleButton(" + tgl.name + ", var_tgl);\n\n");
			}
		}

		for (auto& radio : Radio)
		{
			if (radio.form_id == forms[i].form_id)
			{
				file_builder.append("ImGui::SetCursorPos({" + std::to_string(static_cast<int>(radio.Pos_item.x)) + ".f," + std::to_string(static_cast<int>(radio.Pos_item.y)) + ".f});\n");
				file_builder.append("static bool var_radio;\n");
				file_builder.append(" ImGui::RadioButton(\"" + radio.name + "\", var_radio);\n\n");
			}
		}

		for (auto& si : SliderI)
		{
			if (si.Form_id == forms[i].form_id)
			{
				file_builder.append("ImGui::SetCursorPos({" + std::to_string(static_cast<int>(si.Pos_item.x)) + ".f," + std::to_string(static_cast<int>(si.Pos_item.y)) + ".f});\n");
				file_builder.append("ImGui::PushItemWidth(" + std::to_string(si.wight) + ".f);\n");
				file_builder.append("Static int var_int;\n");
				file_builder.append(" ImGui::SliderInt(\"" + si.name + "\", &var_int, 0,100);\n\n");
			}
		}
		for (auto& sf : SliderF)
		{
			if (sf.Form_id == forms[i].form_id)
			{
				file_builder.append("ImGui::SetCursorPos({" + std::to_string(static_cast<int>(sf.Pos_item.x)) + ".f," + std::to_string(static_cast<int>(sf.Pos_item.y)) + ".f});\n");
				file_builder.append("ImGui::PushItemWidth(" + std::to_string(static_cast<int>(sf.wight)) + ".f);\n");
				file_builder.append("Static float var_float;\n");
				file_builder.append("ImGui::SliderFloat(\"" + sf.name + "\", &var_float, 0,100);\n\n");
			}
		}

		for (auto& lbl : label)
		{
			if (lbl.form_id == forms[i].form_id)
			{
				file_builder.append("ImGui::SetCursorPos({" + std::to_string(static_cast<int>(lbl.Pos_item.x)) + ".f," + std::to_string(static_cast<int>(lbl.Pos_item.y)) + ".f});\n");
				file_builder.append("ImGui::Text(\"" + lbl.name + "\");\n\n");
			}
		}

		for (auto& childer : childs)
		{
			if (childer.a.form_id == forms[i].form_id)
			{
				file_builder.append("ImGui::SetCursorPos({" + std::to_string(static_cast<int>(childer.a.Pos_item.x)) + ".f," + std::to_string(static_cast<int>(childer.a.Pos_item.y)) + ".f});\n");
				file_builder.append("ImGui::BeginChild(\"" + childer.a.name + "\",{"
					+ std::to_string(static_cast<int>(childer.size.x)) + ".f," + std::to_string(static_cast<int>(childer.size.y)) + ".f}, " + std::to_string(childer.border) + ");\n\n\n");
				file_builder.append("//Move code enter to your group child\n\n\n");
				file_builder.append("ImGui::EndChild();\n");
			}
		}

		file_builder.append("\n");
		file_builder.append("ImGui::End();");
		file_builder.append("\n}");
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