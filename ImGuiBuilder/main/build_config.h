#pragma once

#include "structs.hpp"

namespace im_config
{
	namespace window_flags
	{
		bool save( std::string style, ImGuiStyle custom_gui_style );
		bool load( std::string style, ImGuiStyle& custom_gui_style );
		void to_clipboard( ImGuiStyle& custom_gui_style );
	}

	namespace color
	{
		bool save( std::string style, ImGuiStyle custom_gui_style );
		bool load( std::string style, ImGuiStyle& custom_gui_style );
		void to_clipboard( ImGuiStyle custom_gui_style );
		ImVec4* saved_colors( );
	}

	namespace controls
	{
		bool save( std::string& file, std::vector<form> forms, std::vector<basic_obj> objs );
		bool load( std::string& file, std::vector<form>& forms, std::vector<basic_obj>& objs, int* the_ids );
		bool create_code( std::string file_name, std::vector<form> forms, std::vector<basic_obj> objs );
	}
}