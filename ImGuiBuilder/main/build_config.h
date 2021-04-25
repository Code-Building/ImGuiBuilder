#pragma once 
#include "structs.hpp"
namespace im_config
{
	namespace window_flags
	{
		void save( std::string style, ImGuiStyle custom_gui_style );
		void load( std::string style, ImGuiStyle& custom_gui_style );
		void to_clipboard( ImGuiStyle& custom_gui_style );
	}

	namespace color
	{
		void save( std::string style, ImGuiStyle custom_gui_style );
		void load( std::string style, ImGuiStyle& custom_gui_style );
		void to_clipboard( ImGuiStyle custom_gui_style );
		ImVec4* saved_colors( );
	}

	namespace controls
	{
		void save( std::string& file, std::vector<form> forms, std::vector<basic_obj> objs );
		void load( std::string& file, std::vector<form>& forms, std::vector<basic_obj>& objs, int* the_ids );
		void create_code( std::string file_name, std::vector<form> forms, std::vector<basic_obj> objs );
	}
}

