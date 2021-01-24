#pragma once
#include "shared.h"

void ToggleButton(const char* str_id, bool* v);
void color_editor();

struct simple_obj
{
	int id = 0;
	int form = 0;
	int child = -1;
	std::string name = "obj";
	uint16_t my_type = 0;
	ImVec2 size{};
	ImVec2 pos{ 10,10 };
	ImVec2 size_obj{};
	bool change_pos = false;
	bool hover = false;
	bool delete_me = false;

	bool selected = false;
};

struct child
{
	int id = 0;
	std::string name = "form";
	int father = 0;
	bool border = true;
	ImVec2 size{};
	ImVec2 pos{};

	// AGAIN REPEAT FIX THAT!
	bool change_pos = false;
	bool hover = false;
	bool delete_me = false;
	//std::vector<simple_obj> obj_render_me; // is very unstable that
};

struct form
{
	int id = 0;
	std::string name = "form";
	ImVec2 size{};
	ImVec2 pos{};
	uint16_t type_me;

	// AGAIN REPEAT FIX THAT!
	bool change_pos = false;
	bool hover = false;
	bool delete_me = false;
	std::vector<child> child;
};

class ImGuiBuilder
{
private:

	HWND window{};
	int id_ = -1, obj_id = -1, child_id = -1;

	std::vector<form> form_;
	std::vector<simple_obj> obj_render_me;
	std::string file_builder;
	ImGuiStyle dark_;
	std::string file = "project.builder";

public:
	ImGuiBuilder();

	void mainform_draw(HWND wnd);

	void show_form();
	void render_obj(simple_obj& obj);

	void create_form();

	void create_child();
	void create_child(int formPai, float sizeX, float sizeY, bool border);

	void create_obj(uint16_t type);
	void create_obj(uint16_t type, int formPai, float sizeX, float sizeY, int childPai = 0);

	void object_property();
	void create_builder();
	void loading_builder(std::string& file);
	void save_building(std::string& file);

	//void save_building(std::string& file);
	//void loading_builder(std::string& file);
	//void create_builder();
};