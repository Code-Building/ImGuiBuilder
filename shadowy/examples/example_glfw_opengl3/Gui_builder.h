#pragma once
#include "my_head.h"

void ToggleButton(const char* str_id, bool* v);
void color_editor();
struct Indentification_btn
{
    uint32_t Form_id;
    uint32_t btn_id;
    ImVec2 Pos_item;
    ImVec2 size_item;
    bool edition_pos = false;
    std::string name_item;
};

struct Indentification_text
{
    uint32_t Form_id;
    uint32_t text_id;
    ImVec2 Pos_item;
    float wight;
    bool edition = false;
    std::string name_text;
    std::string same_buffer = "";
};

struct Indentification_from
{
    uint32_t Form_id;
    ImVec2 size = {200,100};
    bool edtion = false;
    std::string name_form;
};

struct Indentification_basic
{
    uint32_t form_id;
    uint32_t id;
    ImVec2 Pos_item;
    std::string name;
};

struct child_bar
{
    Indentification_basic a;
    bool borda;
    ImVec2 size;
};


struct Indentification_slider
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

    uint32_t id;
    uint32_t btn_id;
    uint32_t txt_id;
    uint32_t tg_id = 0;
    uint32_t chk_id = 0;
    uint32_t sf = 0;
    uint32_t si = 0;
    uint32_t rd = 0;
    uint32_t chld = 0;
    std::vector<Indentification_from>       forms;
    std::vector<Indentification_btn>        buttons;
    std::vector<Indentification_text>       texts;
    std::vector<Indentification_basic>      toggle;
    std::vector<Indentification_basic>      checkbox;
    std::vector<Indentification_slider>     SliderF;
    std::vector<Indentification_slider>     SliderI;
    std::vector<Indentification_basic>      Radio;
    std::vector< child_bar>                 childs;
    HWND window;
    std::string file_builder;
    
public:
    Gui_builder() { id = 0; btn_id = 0;  txt_id = 0; };

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

    void show_propriedades_btn(Indentification_btn& item_button);
    void show_propriedades_txt(Indentification_text& text);
    void show_propriedades_form(Indentification_from& form);
    void show_propriedades_chk_tlg(Indentification_basic& checkbox_toggle);
    void show_propriedades_slider(Indentification_slider& slider);
    void show_child_propriedade(child_bar& child);
    void show_propriets_geral();

    void create_builder();
};

