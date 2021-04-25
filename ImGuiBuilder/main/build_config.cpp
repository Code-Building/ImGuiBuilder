#include "../header.h"
#include "build_config.h"
#include "../utils/utils.h"


ImGuiStyle	g_ref_saved_style		= { };
auto		g_output_only_modified	= false;

void im_config::window_flags::to_clipboard( ImGuiStyle& custom_gui_style )
{
	ImGui::LogToClipboard( );

	ImGui::LogText( "namespace ImGui {\n void CustomStyle() {\n" );
	ImGui::LogText( "ImGuiStyle& style = ImGui::GetStyle();\n" );

	// Padding
	ImGui::LogText( "style.WindowPadding = ImVec2(%.0f, %.0f);\n", custom_gui_style.WindowPadding.x,
		custom_gui_style.WindowPadding.y );
	ImGui::LogText( "style.FramePadding = ImVec2(%.0f, %.0f);\n", custom_gui_style.FramePadding.x,
		custom_gui_style.FramePadding.y );
	ImGui::LogText( "style.CellPadding = ImVec2(%.0f, %.0f);\n", custom_gui_style.CellPadding.x,
		custom_gui_style.CellPadding.y );
	ImGui::LogText( "style.ItemSpacing = ImVec2(%.0f, %.0f);\n", custom_gui_style.ItemSpacing.x,
		custom_gui_style.ItemSpacing.y );
	ImGui::LogText( "style.ItemInnerSpacing = ImVec2(%.0f, %.0f);\n", custom_gui_style.ItemInnerSpacing.x,
		custom_gui_style.ItemInnerSpacing.y );
	ImGui::LogText( "style.TouchExtraPadding = ImVec2(%.0f, %.0f);\n", custom_gui_style.TouchExtraPadding.x,
		custom_gui_style.TouchExtraPadding.y );
	ImGui::LogText( "style.IndentSpacing = %.0f\n", custom_gui_style.IndentSpacing );
	ImGui::LogText( "style.ScrollbarSize = %.0f\n", custom_gui_style.ScrollbarSize );
	ImGui::LogText( "style.GrabMinSize = %.0f\n", custom_gui_style.GrabMinSize );
	ImGui::LogText( "style.WindowBorderSize = %.0f\n", custom_gui_style.WindowBorderSize );
	ImGui::LogText( "style.ChildBorderSize = %.0f\n", custom_gui_style.ChildBorderSize );
	ImGui::LogText( "style.PopupBorderSize = %.0f\n", custom_gui_style.PopupBorderSize );
	ImGui::LogText( "style.FrameBorderSize = %.0f\n", custom_gui_style.FrameBorderSize );
	ImGui::LogText( "style.TabBorderSize = %.0f\n", custom_gui_style.TabBorderSize );

	// Rounding
	ImGui::LogText( "style.WindowRounding = %.0f\n", custom_gui_style.WindowRounding );
	ImGui::LogText( "style.ChildRounding = %.0f\n", custom_gui_style.ChildRounding );
	ImGui::LogText( "style.FrameRounding = %.0f\n", custom_gui_style.FrameRounding );
	ImGui::LogText( "style.PopupRounding = %.0f\n", custom_gui_style.PopupRounding );
	ImGui::LogText( "style.ScrollbarRounding = %.0f\n", custom_gui_style.ScrollbarRounding );
	ImGui::LogText( "style.GrabRounding = %.0f\n", custom_gui_style.GrabRounding );
	ImGui::LogText( "style.LogSliderDeadzone = %.0f\n", custom_gui_style.LogSliderDeadzone );
	ImGui::LogText( "style.TabRounding = %.0f\n", custom_gui_style.TabRounding );

	// Position
	ImGui::LogText( "style.WindowTitleAlign = ImVec2(%.0f, %.0f);\n", custom_gui_style.WindowTitleAlign.x,
		custom_gui_style.WindowTitleAlign.y );
	ImGui::LogText( "style.WindowMenuButtonPosition = %d\n", custom_gui_style.WindowMenuButtonPosition );
	ImGui::LogText( "style.ColorButtonPosition = %d\n", custom_gui_style.ColorButtonPosition );
	ImGui::LogText( "style.ButtonTextAlign = ImVec2(%.0f, %.0f);\n", custom_gui_style.ButtonTextAlign.x,
		custom_gui_style.ButtonTextAlign.y );
	ImGui::LogText( "style.SelectableTextAlign = ImVec2(%.0f, %.0f);\n", custom_gui_style.SelectableTextAlign.x,
		custom_gui_style.SelectableTextAlign.y );

	// Nani??
	ImGui::LogText( "style.DisplaySafeAreaPadding = ImVec2(%.0f, %.0f);\n",
		custom_gui_style.DisplaySafeAreaPadding.x, custom_gui_style.DisplaySafeAreaPadding.y );

	ImGui::LogText( "}\n}\n" );

	ImGui::LogFinish( );
}

void im_config::window_flags::save( std::string style, ImGuiStyle custom_gui_style )
{
	std::ofstream f_save;
	f_save.open( style );
	if ( f_save.is_open( ) )
	{
		//Padding
		f_save << custom_gui_style.WindowPadding.x			<< "," << custom_gui_style.WindowPadding.y			<< "\n";
		f_save << custom_gui_style.FramePadding.x			<< "," << custom_gui_style.FramePadding.y			<< "\n";
		f_save << custom_gui_style.CellPadding.x			<< "," << custom_gui_style.CellPadding.y			<< "\n";
		f_save << custom_gui_style.ItemSpacing.x			<< "," << custom_gui_style.ItemSpacing.y			<< "\n";
		f_save << custom_gui_style.ItemInnerSpacing.x		<< "," << custom_gui_style.ItemInnerSpacing.y		<< "\n";
		f_save << custom_gui_style.TouchExtraPadding.x		<< "," << custom_gui_style.TouchExtraPadding.y		<< "\n";
		f_save << custom_gui_style.IndentSpacing			<< "\n";
		f_save << custom_gui_style.ScrollbarSize			<< "\n";
		f_save << custom_gui_style.GrabMinSize				<< "\n";
		f_save << custom_gui_style.WindowBorderSize			<< "\n";
		f_save << custom_gui_style.ChildBorderSize			<< "\n";
		f_save << custom_gui_style.PopupBorderSize			<< "\n";
		f_save << custom_gui_style.FrameBorderSize			<< "\n";
		f_save << custom_gui_style.TabBorderSize			<< "\n";

		//Rounding
		f_save << custom_gui_style.WindowRounding			<< "\n";
		f_save << custom_gui_style.ChildRounding			<< "\n";
		f_save << custom_gui_style.FrameRounding			<< "\n";
		f_save << custom_gui_style.PopupRounding			<< "\n";
		f_save << custom_gui_style.ScrollbarRounding		<< "\n";
		f_save << custom_gui_style.GrabRounding				<< "\n";
		f_save << custom_gui_style.LogSliderDeadzone		<< "\n";
		f_save << custom_gui_style.TabRounding				<< "\n";

		//Position
		f_save << custom_gui_style.WindowTitleAlign.x		<< "," << custom_gui_style.WindowTitleAlign.y		<< "\n";
		f_save << custom_gui_style.WindowMenuButtonPosition << "\n";
		f_save << custom_gui_style.ColorButtonPosition		<< "\n";
		f_save << custom_gui_style.ButtonTextAlign.x		<< "," << custom_gui_style.ButtonTextAlign.y		<< "\n";
		f_save << custom_gui_style.SelectableTextAlign.x	<< "," << custom_gui_style.SelectableTextAlign.y	<< "\n";

		//NANI???
		f_save << custom_gui_style.DisplaySafeAreaPadding.x << "," << custom_gui_style.DisplaySafeAreaPadding.y << "\n";

		f_save.close( );
	}
}

void im_config::window_flags::load( std::string style, ImGuiStyle& custom_gui_style )
{
	std::ifstream r_file( style );
	std::string line;

	int fileindex = 0;

	if ( r_file.is_open( ) )
	{
		while ( !r_file.eof( ) )
		{
			std::getline( r_file, line );

			if ( line.find( ',' ) != std::string::npos )
			{
				auto b_line = utils::split( line, ',' );

				switch ( fileindex )
				{
				case 0:
					custom_gui_style.WindowPadding.x			= std::stof( b_line[ 0 ] );
					custom_gui_style.WindowPadding.y			= std::stof( b_line[ 1 ] );
					break;
				case 1:
					custom_gui_style.FramePadding.x				= std::stof( b_line[ 0 ] );
					custom_gui_style.FramePadding.y				= std::stof( b_line[ 1 ] );
					break;
				case 2:
					custom_gui_style.CellPadding.x				= std::stof( b_line[ 0 ] );
					custom_gui_style.CellPadding.y				= std::stof( b_line[ 1 ] );
					break;
				case 3:
					custom_gui_style.ItemSpacing.x				= std::stof( b_line[ 0 ] );
					custom_gui_style.ItemSpacing.y				= std::stof( b_line[ 1 ] );
					break;
				case 4:
					custom_gui_style.ItemInnerSpacing.x			= std::stof( b_line[ 0 ] );
					custom_gui_style.ItemInnerSpacing.y			= std::stof( b_line[ 1 ] );
					break;
				case 5:
					custom_gui_style.TouchExtraPadding.x		= std::stof( b_line[ 0 ] );
					custom_gui_style.TouchExtraPadding.y		= std::stof( b_line[ 1 ] );
					break;
				case 22:
					custom_gui_style.WindowTitleAlign.x			= std::stof( b_line[ 0 ] );
					custom_gui_style.WindowTitleAlign.y			= std::stof( b_line[ 1 ] );
					break;
				case 25:
					custom_gui_style.ButtonTextAlign.x			= std::stof( b_line[ 0 ] );
					custom_gui_style.ButtonTextAlign.y			= std::stof( b_line[ 1 ] );
					break;
				case 26:
					custom_gui_style.SelectableTextAlign.x		= std::stof( b_line[ 0 ] );
					custom_gui_style.SelectableTextAlign.y		= std::stof( b_line[ 1 ] );
					break;
				case 27:
					custom_gui_style.DisplaySafeAreaPadding.x	= std::stof( b_line[ 0 ] );
					custom_gui_style.DisplaySafeAreaPadding.y	= std::stof( b_line[ 1 ] );
					break;
				}
			}

			switch ( fileindex )
			{
			case 6:
				custom_gui_style.IndentSpacing				= std::stof( line );
				break;
			case 7:
				custom_gui_style.ScrollbarSize				= std::stof( line );
				break;
			case 8:
				custom_gui_style.GrabMinSize				= std::stof( line );
				break;
			case 9:
				custom_gui_style.WindowBorderSize			= std::stof( line );
				break;
			case 10:
				custom_gui_style.ChildBorderSize			= std::stof( line );
				break;
			case 11:
				custom_gui_style.PopupBorderSize			= std::stof( line );
				break;
			case 12:
				custom_gui_style.FrameBorderSize			= std::stof( line );
				break;
			case 13:
				custom_gui_style.TabBorderSize				= std::stof( line );
				break;
			case 14:
				custom_gui_style.WindowRounding				= std::stof( line );
				break;
			case 15:
				custom_gui_style.ChildRounding				= std::stof( line );
				break;
			case 16:
				custom_gui_style.FrameRounding				= std::stof( line );
				break;
			case 17:
				custom_gui_style.PopupRounding				= std::stof( line );
				break;
			case 18:
				custom_gui_style.ScrollbarRounding			= std::stof( line );
				break;
			case 19:
				custom_gui_style.GrabRounding				= std::stof( line );
				break;
			case 20:
				custom_gui_style.LogSliderDeadzone			= std::stof( line );
				break;
			case 21:
				custom_gui_style.TabRounding				= std::stof( line );
				break;
			case 23:
				custom_gui_style.WindowMenuButtonPosition	= static_cast<ImGuiDir_>( std::stoi( line ) );
				break;
			case 24:
				custom_gui_style.ColorButtonPosition		= static_cast<ImGuiDir_>( std::stoi( line ) );
				break;
			}

			fileindex++;
		}
		r_file.close( );
	}
}

ImVec4* im_config::color::saved_colors( )
{
	return g_ref_saved_style.Colors;
}

void im_config::color::to_clipboard( ImGuiStyle custom_gui_style )
{
	ImGui::LogToClipboard( );

	ImGui::LogText( "namespace ImGui {\n void CustomColor() {\n" );
	ImGui::LogText( "auto* colors = ImGui::GetStyle().Colors;\n" );
	for ( auto i = 0; i < ImGuiCol_COUNT; i++ )
	{
		const auto& col = custom_gui_style.Colors[ i ];
		const auto* name = ImGui::GetStyleColorName( i );
		if ( !g_output_only_modified || memcmp( &col, &g_ref_saved_style.Colors[ i ], sizeof( ImVec4 ) ) != 0 )
			ImGui::LogText( "colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);\n", name, 23 - static_cast<int>( strlen( name ) ), "", col.x, col.y, col.z, col.w );
	}
	ImGui::LogText( "}\n}\n" );
	ImGui::LogFinish( );
}

void im_config::color::save( std::string style, ImGuiStyle custom_gui_style )
{
	remove( style.c_str( ) );

	std::ofstream filewrite( style );

	for ( const auto col : custom_gui_style.Colors )
		filewrite << col.x << "," << col.y << "," << col.z << "," << col.w << std::endl;

	filewrite.close( );
}

void im_config::color::load( std::string style, ImGuiStyle& custom_gui_style )
{

	std::ifstream file_import( style );

	if ( file_import.is_open( ) )
	{
		for ( auto& i : custom_gui_style.Colors )
		{
			std::string line{ };

			std::getline( file_import, line );

			auto colors = utils::split( line, ',' );

			i = ImVec4( std::stof( colors[ 0 ] ), std::stof( colors[ 1 ] ), std::stof( colors[ 2 ] ), std::stof( colors[ 3 ] ) );

		}

		file_import.close( );
	}
}


void im_config::controls::create_code( std::string file_name, std::vector<form> forms, std::vector<basic_obj> objs )
{
	static int fctn = 0;
	std::string file_builder = ( "void ToggleButton(const char* str_id, bool* v)\n" );
	file_builder.append(
		"{ \nImVec2 p = ImGui::GetCursorScreenPos(); \nImDrawList* draw_list = ImGui::GetWindowDrawList();\nfloat height = ImGui::GetFrameHeight();\n" );
	file_builder.append(
		"float width = height * 1.55f;\nfloat radius = height * 0.50f;\n if (ImGui::InvisibleButton(str_id, ImVec2(width, height)))\n " );
	file_builder.append(
		"  *v = !*v;\nImU32 col_bg;\nif (ImGui::IsItemHovered())\n\t  col_bg = *v ? IM_COL32(145 + 20, 211, 68 + 20, 255) : IM_COL32(218 - 20, 218 - 20, 218 - 20, 255);\n" );
	file_builder.append(
		" else\n\t   col_bg = *v ? IM_COL32(145, 211, 68, 255) : IM_COL32(218, 218, 218, 255);\n  draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);\n   draw_list->AddCircleFilled(ImVec2(*v ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));\n}\n\n\n" );

	for ( const auto& form : forms )
	{
		file_builder.append( "void gui_builder" + std::to_string( fctn ) + "()\n{\n" );
		file_builder.append(
			"ImGui::SetNextWindowSize({" + std::to_string( static_cast<int>( form.size.x ) ) + ".f," + std::to_string(
				static_cast<int>( form.size.y ) ) + ".f});\n" );
		file_builder.append( "\nImGui::Begin( \"" + form.name + "\");\n" );

		for ( const auto& obj : objs )
		{
			for ( const auto& chl : form.child )
			{
				//obj with child
				if ( obj.child == chl.id )
				{
					file_builder.append(
						"ImGui::SetCursorPos({" + std::to_string( static_cast<int>( chl.pos.x ) ) + ".f," + std::to_string(
							static_cast<int>( chl.pos.y ) ) + ".f});\n" );
					file_builder.append(
						"ImGui::BeginChild(\"" + chl.name + "\",{" + std::to_string( static_cast<int>( chl.size.x ) ) +
						".f," + std::to_string( static_cast<int>( chl.size.y ) ) + ".f},true );" );
					if ( obj.child == chl.id && obj.form == form.id && chl.father == form.id )
					{
						file_builder.append(
							"ImGui::SetCursorPos({" + std::to_string( static_cast<int>( obj.pos.x ) ) + ".f," +
							std::to_string( static_cast<int>( obj.pos.y ) ) + ".f});\n" );
						switch ( obj.my_type )
						{
						case 1:
							file_builder.append(
								"if(ImGui::Button(\"" + obj.name + "\", {" + std::to_string( static_cast<int>( obj.size.x ) )
								+ ".f," + std::to_string( static_cast<int>( obj.size.y ) ) + ".f }))\n{\n\n}\n" );
							break;
						case 2:
							file_builder.append( "ImGui::Text(\"" + obj.name + "\");\n" );
							break;
						case 3:
							file_builder.append( "ImGui::InputText(\"" + obj.name + "\", buffer, 255);\n" );
							break;
						case 4:
							file_builder.append( "ImGui::SliderInt(\"" + obj.name + "\", &valueI,0,100);\n" );
							break;
						case 5:
							file_builder.append( "ImGui::SliderFloat(\"" + obj.name + "\", &valueF,0,100);\n" );
							break;
						case 6:
							file_builder.append( "ImGui::Checkbox(\"" + obj.name + "\", &the_bool);\n" );
							break;
						case 7:
							file_builder.append( "ImGui::RadioButton(\"" + obj.name + "\", the_bool);\n" );
							break;
						case 8:
							file_builder.append( "ToggleButton(\"" + obj.name + "\", the_bool);\n" );
							break;
						default:
							break;
						}
					}
					file_builder.append( "\nImGui::EndChild();" );
				}
			}

			//obj none child
			if ( obj.child == -1 && obj.form == form.id )
			{
				file_builder.append(
					"ImGui::SetCursorPos({" + std::to_string( static_cast<int>( obj.pos.x ) ) + ".f," + std::to_string(
						static_cast<int>( obj.pos.y ) ) + ".f});\n" );
				//file_builder.append("ImGui::PushItemWidth(" + std::to_string(obj.size.x) + ".f);\n");
				switch ( obj.my_type )
				{
				case 1:
					file_builder.append(
						"if(ImGui::Button(\"" + obj.name + "\", {" + std::to_string( static_cast<int>( obj.size.x ) ) + ".f,"
						+ std::to_string( static_cast<int>( obj.size.y ) ) + ".f}))\n{\n\n}\n" );
					break;
				case 2:
					file_builder.append( "ImGui::Text(\"" + obj.name + "\");\n" );
					break;
				case 3:
					file_builder.append( "ImGui::InputText(\"" + obj.name + "\", buffer, 255);\n" );
					break;
				case 4:
					file_builder.append( "ImGui::SliderInt(\"" + obj.name + "\", &valueI,0,100);\n" );
					break;
				case 5:
					file_builder.append( "ImGui::SliderFloat(\"" + obj.name + "\", &valueF,0,100);\n" );
					break;
				case 6:
					file_builder.append( "ImGui::Checkbox(\"" + obj.name + "\", &the_bool);\n" );
					break;
				case 7:
					file_builder.append( "ImGui::RadioButton(\"" + obj.name + "\", the_bool);\n" );
					break;
				case 8:
					file_builder.append( "ToggleButton(\"" + obj.name + "\", the_bool);\n" );
					break;
				default:
					break;
				}
			}
		}

		file_builder.append( "\n\nImGui::End();\n}\n\n\n" );
		fctn += 1;
	}

	std::ofstream file_to_save;
	DeleteFile( file_name.c_str( ) );
	//name_file.append( "0.cpp" );
	file_to_save.open( file_name.c_str( ) );
	for ( auto i : file_builder )
		file_to_save << i;

	file_to_save.close( );
}


void im_config::controls::load( std::string& file, std::vector<form> & forms, std::vector<basic_obj> & objs, int * last_ids )
{
	if ( !last_ids ) return;

	std::ifstream f_read( file );
	if ( f_read.is_open( ) )
	{
		std::string line;
		auto menu = 0;

		while ( !f_read.eof( ) )
		{
			form		form_load;
			child		child_load;
			basic_obj	obj_load;

			std::getline( f_read, line );
			if ( !line.compare( "#forms" ) )
			{
				menu = 0;
				continue;
			}
			if ( !line.compare( "#child" ) )
			{
				menu = 1;
				continue;
			}
			if ( !line.compare( "#obj" ) )
			{
				menu = 2;
				continue;
			}
			if ( !line.compare( "" ) )
			{
				menu = 5;
				continue;
			}

			// VAR STRING
			auto vstr = utils::split( line, ',' );
			switch ( menu )
			{
			case 0:

				form_load.id		= std::stoi( vstr[ 0 ] );
				last_ids[0]			= form_load.id;
				//id_ = form_load.id;
				form_load.name		= vstr[ 1 ];
				form_load.size.x	= std::stof( vstr[ 2 ] );
				form_load.size.y	= std::stof( vstr[ 3 ] );

				forms.push_back( form_load );

				printf( "Loading form\n" );
				break;

			case 1:
				child_load.id		= std::stoi( vstr[ 0 ] );
				last_ids[ 1 ]		= child_load.id;
				//child_id = child_load.id;
				child_load.father	= std::stoi( vstr[ 1 ] );
				child_load.name		= vstr[ 2 ];
				child_load.size.x	= std::stof( vstr[ 3 ] );
				child_load.size.y	= std::stof( vstr[ 4 ] );
				child_load.pos.x	= std::stof( vstr[ 5 ] );
				child_load.pos.y	= std::stof( vstr[ 6 ] );
				forms[ child_load.father ].child.push_back( child_load );

				printf( "Loading child\n" );
				break;

			case 2:
				obj_load.id			= std::stoi( vstr[ 0 ] );
				last_ids[ 2 ]		= obj_load.id;
				//obj_id = obj_load.id;
				obj_load.form		= std::stoi( vstr[ 1 ] );
				obj_load.child		= std::stoi( vstr[ 2 ] );
				obj_load.name		= vstr[ 3 ];
				obj_load.my_type	= std::stoi( vstr[ 4 ] );
				obj_load.size.x		= std::stof( vstr[ 5 ] );
				obj_load.size.y		= std::stof( vstr[ 6 ] );
				obj_load.pos.x		= std::stof( vstr[ 7 ] );
				obj_load.pos.y		= std::stof( vstr[ 8 ] );

				objs.push_back( obj_load );

				printf( "Loading obj\n" );
				break;
			default:

				break;
			}
		}
		f_read.close( );
	}
}

void im_config::controls::save( std::string& file, std::vector<form> forms, std::vector<basic_obj> objs )
{
	remove( file.c_str( ) );
	std::ofstream f_write( file );

	if ( f_write.is_open( ) )
	{
		for ( const auto& form : forms )
		{
			f_write << "#forms\n";
			f_write << form.id << "," << form.name << "," << form.size.x << "," << form.size.y << "\n";

			for ( const auto& ch : form.child )
			{
				f_write << "#child\n";
				f_write << ch.id << "," << ch.father << "," << ch.name << "," << ch.size.x << "," << ch.size.y << "," <<
					ch.pos.x << "," << ch.pos.y << "\n";
			}

			for ( const auto& obj : objs )
			{
				if ( form.id == obj.form )
				{
					f_write << "#obj\n";
					f_write << obj.id << "," << obj.form << "," << obj.child << "," << obj.name << "," << obj.my_type <<
						"," << obj.size.x << "," << obj.size.y << "," << obj.pos.x << "," << obj.pos.y << "\n";
				}
			}
		}

		f_write.close( );
	}
}