#pragma once
#include "my_head.h"

void ToggleButton(const char* str_id, bool* v);
void color_editor();
struct indentification_btn
{
	uint32_t Form_id;
	uint32_t btn_id;
	ImVec2 Pos_item;
	ImVec2 size_item;
	bool edition_pos = false;
	std::string name_item;
};

struct indentification_text
{
	uint32_t Form_id = 0;
	uint32_t text_id = 0;
	ImVec2 Pos_item = {};
	float wight = 0;
	bool edition = false;
	std::string name_text = "txt";
	std::string same_buffer = "";
};

struct indentification_form
{
	uint32_t Form_id;
	ImVec2 size = { 200,100 };
	bool edtion = false;
	std::string name_form;
};

struct indentification_basic
{
	uint32_t form_id;
	uint32_t id;
	ImVec2 Pos_item;
	std::string name;
};

struct child_bar
{
	indentification_basic a;
	bool border = false;
	ImVec2 size;
};

struct identification_slider
{
	uint32_t Form_id;
	uint32_t slider_id;
	ImVec2 Pos_item;
	float wight;
	std::string name;
};

class Gui_builder
{
private:

	uint32_t id_;
	uint32_t btn_id_;
	uint32_t txt_id_;
	uint32_t tg_id_ = 0;
	uint32_t chk_id_ = 0;
	uint32_t sf_ = 0;
	uint32_t si_ = 0;
	uint32_t rd_ = 0;
	uint32_t chld_ = 0;
	std::vector<indentification_form>       forms;
	std::vector<indentification_btn>        buttons;
	std::vector<indentification_text>       texts;
	std::vector<indentification_basic>      toggle;
	std::vector<indentification_basic>      checkbox;
	std::vector<identification_slider>      SliderF;
	std::vector<identification_slider>      SliderI;
	std::vector<indentification_basic>      Radio;
	std::vector<child_bar>                  childs;
	HWND window{};
	std::string file_builder;
	ImGuiStyle dark_;
	std::string file = "project.builder";

public:
	Gui_builder();

	//void init_DarkColor();
	//void init_CustomColor();

	//void drawimgui(GLFWwindow* window, HWND hWnd); wtf!

	void mainform_draw(HWND wnd);

	void create_from();
	void create_button();
	void create_text();
	void create_toggle();
	void create_chk();
	void create_sliderI();
	void create_sliderF();
	void create_radio();
	void create_child();

	void show_form(HWND window);
	void save_building(std::string& file);
	void loading_builder(std::string& file);
	void show_propriedades_btn(indentification_btn& item_button);
	void show_propriedades_txt(indentification_text& text);
	void show_propriedades_form(indentification_form& form);
	void show_propriedades_chk_tlg(indentification_basic& checkbox_toggle);
	void show_propriedades_slider(identification_slider& slider);
	void show_child_propriedade(child_bar& child);
	void show_propriets_geral();

	void create_builder();
};
