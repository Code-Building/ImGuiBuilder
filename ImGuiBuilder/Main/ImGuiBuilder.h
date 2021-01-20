#pragma once
#include "shared.h"

void ToggleButton(const char* str_id, bool* v);
void color_editor();

struct indentification_btn
{
    uint32_t Form_id = 0;
    uint32_t btn_id = 0;
    ImVec2 Pos_item = { 0,0 };
    ImVec2 size_item = { 0,0 };
    bool edition_pos = false;
    std::string name_item = "bnt id:";
    ImVec2 size_obj_ac = { 0, 0 };
    bool hover = false;
	
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
    ImVec2 size_obj_ac = { 0, 0 };
    bool hover = false;
};

struct indentification_form
{
    uint32_t form_id = 0;
    ImVec2 size = { 200,100 };
    bool edition = false;
    std::string name_form = "form";
    ImVec2 pos = { 0, 0 };
    bool edt_pos = false;
    bool hover = false;
};

struct indentification_basic
{
    uint32_t form_id = 0;
    uint32_t id = 0;
    ImVec2 Pos_item = {};
    std::string name = "basic item";
    ImVec2 size_obj_ac = { 0, 0 };
    bool edt_pos = false;
    bool hover = false;
};

struct child_bar
{
    indentification_basic a;
    bool border = false;
    ImVec2 size = {};
};

struct identification_slider
{
    uint32_t Form_id = 0;
    uint32_t slider_id = 0;
    ImVec2 Pos_item = {};
    float wight = 0;
    std::string name = "sliderID";
    ImVec2 size_obj_ac = { 0, 0 };
    bool edt_pos = false;
    bool hover = false;
};

class ImGuiBuilder
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
    uint32_t lbl_ = 0;
    std::vector<indentification_form>       forms;
    std::vector<indentification_btn>        buttons;
    std::vector<indentification_text>       texts;
    std::vector<indentification_basic>      toggle;
    std::vector<indentification_basic>      checkbox;
    std::vector<identification_slider>      SliderF;
    std::vector<identification_slider>      SliderI;
    std::vector<indentification_basic>      Radio;
    std::vector<child_bar>                  childs;
    std::vector<indentification_basic>      label;
    HWND window{};
    std::string file_builder;
    ImGuiStyle dark_;
    std::string file = "project.builder";

public:
    ImGuiBuilder();

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
    void create_label();

    void show_form(HWND window);
    void save_building(std::string& file);
    void loading_builder(std::string& file);
    void show_propriedades_btn(indentification_btn& item_button);
    void show_propriedades_txt(indentification_text& text);
    void show_propriedades_form(indentification_form& form);
    void show_propriedades_slider(identification_slider& slider);
    void show_child_propriedade(child_bar& child);
    void show_propriets_geral();
    void show_propriedades_basic(indentification_basic& obj_basic);

    void create_builder();
};
