#pragma once
#include "structs.hpp"
class imgui_builder
{
private:


	int						m_id				= -1;
	int						m_child_id			= -1; 
	int						m_obj_id			= -1;
	bool					m_color_menu		= false; 
	bool					m_style_menu		= false;
	bool					m_font_menu			= false;
	std::vector<form>		m_forms				= { };
	std::vector<basic_obj>	m_objs				= { };
	ImGuiStyle				m_dark_style		= { };
	ImGuiStyle				m_custom_gui_style	= { };
	int						m_active_window_id	= 0;
	
	// variables used in the property of the selected object.
	std::string				m_current_item		= { };
	int						m_index				= 0;
	int						m_family			= 0;
	int						m_grandchild		= -1;
	int						m_type				= -1; // none types
	std::string				m_name				= { };
	ImVec2					m_form_pos			= { };
	ImVec2					m_item_size			= { };
	ImVec2					m_pos_obj			= { };

	// for resize
	resize_opt				m_resize_opt		= resize_opt::off;
	bool					m_no_move			= false;
	int						m_in_resize_id		= 0;
	uint64_t				m_tick_resize		= 0;


	bool					m_my_forms_active	= false;			

	//cursor
	struct s_cursor
	{
		HCURSOR m_arrow_all							= nullptr;
		HCURSOR m_arrow_top_or_bottom				= nullptr;
		HCURSOR m_arrow_left_or_right				= nullptr;
		HCURSOR m_arrow_northwest_and_southeast		= nullptr;
		HCURSOR m_arrow_northeast_and_southwest		= nullptr;
		HCURSOR m_current_icon						= nullptr;
	}cursor;


	void form_window_flag( );

	void form_color_editor( );

	void form_font_editor( );

	void draw( );

	void show_form( );

	void delete_form( int form_id );

	void render_obj( basic_obj& obj, int current_form_id );

	void create_form( );

	void create_child( );

	void create_obj( uint16_t type );

	void paste_obj( );

	void copy_obj( const int type, const int child, const ImVec2 size, const ImVec2 pos, const bool border, bool selected, bool pass_key_check = false );

	void object_property( );

	void draw_dialogs_save_open( );

	void resize_obj( basic_obj& obj, bool selected );

	void resize_obj( ImVec2& obj_pos, ImVec2& obj_size, bool hover, bool selected );
public:
	imgui_builder();
	static void routine_draw( );
};