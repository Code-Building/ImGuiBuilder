#include "../header.h"
#include "imgui_builder.h"
#include "build_config.h"
#include "additional.h"
#include "the_window.h"
#include "../utils/utils.h"
#include "../ImGuiFileDialog/ImGuiFileDialogConfig.h"
#include "../ImGuiFileDialog/ImGuiFileDialog.h"
#include <imgui_internal.h>

/// PENSE BEM ANTES DE POR UMA VARIAVEL GLOBAL, SE FOR USAR APENAS EM MEMBRO DE CLASS, NÃO NECESSIDADE PARA SER GLOBAL

// Proprity formulary
bool			g_moving_obj	= false;
bool			g_resize_obj	= false;

//Move obj with cursor

uint64_t		g_tick_move		= 0;

struct move_obj
{
	int index{ };
	ImVec2 pos{ };
};

std::string get_name_type( const int type )
{
	switch ( type )
	{
	case 1: return "button";
	case 2: return "label";
	case 3: return "edit";
	case 4: return "sliderI";
	case 5: return "sliderF";
	case 6: return "checkbox";
	case 7: return "radio";
	case 8: return "toggle";
	default:
		break;
	}
	return "";
}

resize_opt limit_bordering_control( ImVec2 obj_pos, ImVec2 obj_size, float thickness = 3.f )
{
	auto current_win_pos	= ImGui::GetWindowPos( );

	auto control_win_pos	= ImVec2( current_win_pos.x + obj_pos.x, current_win_pos.y + obj_pos.y );

	auto pos				= window::i( )->get_relative_cursor_pos( );

	auto top				= ( pos.y >= (long)( control_win_pos.y - thickness ) && pos.y <= (long)( control_win_pos.y ) );

	auto bottom				= ( pos.y >= (long)( control_win_pos.y + obj_size.y ) && pos.y <= (long)( control_win_pos.y + obj_size.y + thickness ) );

	auto left				= ( pos.x >= (long)( control_win_pos.x - thickness ) && pos.x <= (long)( control_win_pos.x ) );

	auto right				= ( pos.x >= (long)( control_win_pos.x + obj_size.x ) && pos.x <= (long)( control_win_pos.x + obj_size.x + thickness ) );

	if ( ( bottom && right ) || ( top && left ) )
	{
		return ( bottom && right ) ? resize_opt::bottom_right : resize_opt::top_left;
	}
	else
		if ( ( top && right ) || ( bottom && left ) )
		{
			return ( top && right ) ? resize_opt::top_right : resize_opt::bottom_left;
		}
		else
		{
			if ( top || bottom )
				return ( top ) ? resize_opt::top : resize_opt::bottom;
			else
				if ( left || right )
					return ( left ) ? resize_opt::left : resize_opt::right;
				
		}
	return resize_opt::off;
}

bool my_IsItemHovered(ImVec2 obj_pos, ImVec2 obj_size, float distance )
{
	auto current_win_pos	= ImGui::GetWindowPos( );
	auto control_win_pos	= ImVec2( current_win_pos.x + ( obj_pos.x - distance ), current_win_pos.y + ( obj_pos.y - distance ) );
	auto pos				= window::i( )->get_relative_cursor_pos( );
	return ( pos.y >= control_win_pos.y && pos.y <= ( control_win_pos.y + (obj_size.y  + ( distance * 2 ) ) ) && pos.x >= control_win_pos.x && pos.x <= ( control_win_pos.x + obj_size.x + ( distance * 2 ) ) );
}

void move_item( ImVec2& obj_pos, bool& continue_edt );
void move_items( std::vector<move_obj>& mto, bool& continue_edt );

imgui_builder::imgui_builder( )
{
	

	this->cursor.m_arrow_top_or_bottom				= LoadCursor( NULL, IDC_SIZENS );
	this->cursor.m_arrow_left_or_right				= LoadCursor( NULL, IDC_SIZEWE );
	this->cursor.m_arrow_northwest_and_southeast	= LoadCursor( NULL, IDC_SIZENWSE );
	this->cursor.m_arrow_northeast_and_southwest	= LoadCursor( NULL, IDC_SIZENESW );
	this->cursor.m_arrow_all						= LoadCursor( NULL, IDC_SIZEALL );

	ImGui::StyleColorsDark( &m_dark_style );
	for ( auto i = 0; i < ImGuiCol_COUNT; i++ )
		m_custom_gui_style.Colors[ i ] = m_dark_style.Colors[ i ];

	m_forms.clear( );
	m_objs.clear( );
}

/// <summary>
/// here will show the dialogs for saving or uploading files 
/// </summary>
/// 
void imgui_builder::draw_dialogs_save_open( )
{

	static std::vector<std::string> dialogs_keys = {
		"SaveProjectFileDlgKey",
		"OpenProjectFileDlgKey",
		"GenCodeProjectFileDlgKey",
		"SaveColorsDlgKey",
		"OpenColorsDlgKey",
		"SaveFlagsDlgKey",
		"OpenFlagsDlgKey"
	};

	auto DeleteKey = [ & ] ( const std::string& keyname ) -> void {
		HKEY hKey;
		RegOpenKeyExA( HKEY_CURRENT_USER, NULL, 0, KEY_SET_VALUE, &hKey );

		RegDeleteKeyExA( hKey, keyname.c_str( ), KEY_WOW64_64KEY, NULL );

		RegCloseKey( hKey );
	};

	auto SetValue = [ & ] ( const std::string& keyname, const std::string& value ) -> void {
		DeleteKey( keyname );

		HKEY hKey;
		RegOpenKeyExA( HKEY_CURRENT_USER, NULL, 0, KEY_SET_VALUE, &hKey );

		auto status = RegSetValueExA( hKey, keyname.c_str( ), NULL, REG_SZ, (LPBYTE)value.c_str( ), value.size( ) + 1 );

		printf_s( "RegSetValueEx: %d\n", status );

		RegCloseKey( hKey );
	};

	auto i = 0;
	for ( auto key : dialogs_keys )
	{
		if ( ImGuiFileDialog::Instance( )->Display( key.c_str(), 32, { 350.f, 300.f } ) )
		{
			if ( ImGuiFileDialog::Instance( )->IsOk( ) )
			{
				std::string file_full_path	= ImGuiFileDialog::Instance( )->GetFilePathName( );
				std::string full_path		= ImGuiFileDialog::Instance( )->GetCurrentPath( );

				SetValue( "ImGuiBuilderPath", full_path );

				switch ( i )
				{
				case 0:
				{
					if ( im_config::controls::save( file_full_path, m_forms, m_objs ) )
						MessageBoxA( nullptr, "Project saved!", "ImGui Builder", MB_OK | MB_ICONINFORMATION );
					break;
				}
				case 1:
				{
					m_forms.clear( );
					m_objs.clear( );
					if ( im_config::controls::load( file_full_path, m_forms, m_objs, &m_id ) )
						MessageBoxA( nullptr, "Project loaded!", "ImGui Builder", MB_OK | MB_ICONINFORMATION );
					break;
				}
				case 2:
				{
					if ( im_config::controls::create_code( file_full_path, m_forms, m_objs ) )
						MessageBoxA( nullptr, "Code been generated!", "ImGui Builder", MB_OK | MB_ICONINFORMATION );
					break;
				}
				case 3:
				{
					if ( im_config::color::save( file_full_path, m_custom_gui_style ) )
						MessageBoxA( nullptr, "Colors saved!", "ImGui Builder", MB_OK | MB_ICONINFORMATION );
					break;
				}
				case 4:
				{
					if ( im_config::color::load( file_full_path, m_custom_gui_style ) )
						MessageBoxA( nullptr, "Colors loaded!", "ImGui Builder", MB_OK | MB_ICONINFORMATION );
					break;
				}
				case 5:
				{
					if ( im_config::window_flags::save( file_full_path, m_custom_gui_style ) )
						MessageBoxA( nullptr, "Flags saved!", "ImGui Builder", MB_OK | MB_ICONINFORMATION );
					break;
				}
				case 6:
				{
					if ( im_config::window_flags::load( file_full_path, m_custom_gui_style ) )
						MessageBoxA( nullptr, "Flags loaded!", "ImGui Builder", MB_OK | MB_ICONINFORMATION );
					break;
				}
				default:
					break;
				}
			}

			ImGuiFileDialog::Instance( )->Close( );
		}
		++i;
	}
}

auto RegeditGetPath ( const std::string& keyname ) -> std::string {
	HKEY hKey;
	RegOpenKeyExA( HKEY_CURRENT_USER, NULL, 0, KEY_SET_VALUE, &hKey );

	char buffer[ MAX_PATH ];
	DWORD dwBufferSize = sizeof( buffer );

	auto status = RegQueryValueExA( hKey, keyname.c_str( ), NULL, NULL, (LPBYTE)buffer, &dwBufferSize );

	RegCloseKey( hKey );

	return std::string( buffer );
}

/// <summary>
/// this function is for displaying the project's flags editing window. 
/// </summary>
void imgui_builder::form_window_flag( )
{
	ImGui::Begin( "Style Window Editor", &m_style_menu );
	if ( ImGui::Button( "Export" ) )
	{
		im_config::window_flags::to_clipboard( m_custom_gui_style );
		MessageBoxA( nullptr, "Code exported to clipboard !", "ImGui Builder", MB_OK | MB_ICONINFORMATION );
	}

	ImGui::SameLine( );

	if ( ImGui::Button( "Load" ) )
	{
		ImGuiFileDialog::Instance( )->OpenDialog( "OpenFlagsDlgKey", "Open File", ".flags", RegeditGetPath( "ImGuiBuilderPath" ), "style_flags" );
	}

	ImGui::SameLine( );
	if ( ImGui::Button( "Save" ) )
	{
		ImGuiFileDialog::Instance( )->OpenDialog( "SaveFlagsDlgKey", "Save File", ".flags", RegeditGetPath( "ImGuiBuilderPath" ), "style_flags" );
	}

	ImGui::Text( "First" );
	if ( ImGui::SliderFloat( "FrameRounding", &m_custom_gui_style.FrameRounding, 0.0f, 12.0f, "%.0f" ) )
		m_custom_gui_style.GrabRounding = m_custom_gui_style.FrameRounding;
	// Make GrabRounding always the same value as FrameRounding
	{
		auto border = ( m_custom_gui_style.WindowBorderSize > 0.0f );
		if ( ImGui::Checkbox( "WindowBorder", &border ) ) { m_custom_gui_style.WindowBorderSize = border ? 1.0f : 0.0f; }
	}
	ImGui::SameLine( );
	{
		auto border = ( m_custom_gui_style.FrameBorderSize > 0.0f );
		if ( ImGui::Checkbox( "FrameBorder", &border ) ) { m_custom_gui_style.FrameBorderSize = border ? 1.0f : 0.0f; }
	}
	ImGui::SameLine( );
	{
		auto border = ( m_custom_gui_style.PopupBorderSize > 0.0f );
		if ( ImGui::Checkbox( "PopupBorder", &border ) ) { m_custom_gui_style.PopupBorderSize = border ? 1.0f : 0.0f; }
	}

	ImGui::Text( "Main" );
	ImGui::SliderFloat2( "WindowPadding",		reinterpret_cast<float*>( &m_custom_gui_style.WindowPadding		), 0.0f, 20.0f, "%.0f" );
	ImGui::SliderFloat2( "FramePadding",		reinterpret_cast<float*>( &m_custom_gui_style.FramePadding		), 0.0f, 20.0f, "%.0f" );
	ImGui::SliderFloat2( "CellPadding",			reinterpret_cast<float*>( &m_custom_gui_style.CellPadding		), 0.0f, 20.0f, "%.0f" );
	ImGui::SliderFloat2( "ItemSpacing",			reinterpret_cast<float*>( &m_custom_gui_style.ItemSpacing		), 0.0f, 20.0f, "%.0f" );
	ImGui::SliderFloat2( "ItemInnerSpacing",	reinterpret_cast<float*>( &m_custom_gui_style.ItemInnerSpacing	), 0.0f, 20.0f, "%.0f" );
	ImGui::SliderFloat2( "TouchExtraPadding",	reinterpret_cast<float*>( &m_custom_gui_style.TouchExtraPadding ), 0.0f, 10.0f, "%.0f" );
	ImGui::SliderFloat( "IndentSpacing",		&m_custom_gui_style.IndentSpacing,		0.0f, 30.0f, "%.0f" );
	ImGui::SliderFloat( "ScrollbarSize",		&m_custom_gui_style.ScrollbarSize,		1.0f, 20.0f, "%.0f" );
	ImGui::SliderFloat( "GrabMinSize",			&m_custom_gui_style.GrabMinSize,		1.0f, 20.0f, "%.0f" );
	ImGui::Text( "Borders" );
	ImGui::SliderFloat( "WindowBorderSize",		&m_custom_gui_style.WindowBorderSize,	0.0f, 1.0f, "%.0f" );
	ImGui::SliderFloat( "ChildBorderSize",		&m_custom_gui_style.ChildBorderSize,	0.0f, 1.0f, "%.0f" );
	ImGui::SliderFloat( "PopupBorderSize",		&m_custom_gui_style.PopupBorderSize,	0.0f, 1.0f, "%.0f" );
	ImGui::SliderFloat( "FrameBorderSize",		&m_custom_gui_style.FrameBorderSize,	0.0f, 1.0f, "%.0f" );
	ImGui::SliderFloat( "TabBorderSize",		&m_custom_gui_style.TabBorderSize,		0.0f, 1.0f, "%.0f" );
	ImGui::Text( "Rounding" );
	ImGui::SliderFloat( "WindowRounding",		&m_custom_gui_style.WindowRounding,		0.0f, 12.0f, "%.0f" );
	ImGui::SliderFloat( "ChildRounding",		&m_custom_gui_style.ChildRounding,		0.0f, 12.0f, "%.0f" );
	ImGui::SliderFloat( "FrameRounding",		&m_custom_gui_style.FrameRounding,		0.0f, 12.0f, "%.0f" );
	ImGui::SliderFloat( "PopupRounding",		&m_custom_gui_style.PopupRounding,		0.0f, 12.0f, "%.0f" );
	ImGui::SliderFloat( "ScrollbarRounding",	&m_custom_gui_style.ScrollbarRounding,	0.0f, 12.0f, "%.0f" );
	ImGui::SliderFloat( "GrabRounding",			&m_custom_gui_style.GrabRounding,		0.0f, 12.0f, "%.0f" );
	ImGui::SliderFloat( "LogSliderDeadzone",	&m_custom_gui_style.LogSliderDeadzone,	0.0f, 12.0f, "%.0f" );
	ImGui::SliderFloat( "TabRounding",			&m_custom_gui_style.TabRounding,		0.0f, 12.0f, "%.0f" );
	ImGui::Text( "Alignment" );
	ImGui::SliderFloat2( "WindowTitleAlign",	reinterpret_cast<float*>( &m_custom_gui_style.WindowTitleAlign ), 0.0f, 1.0f, "%.2f" );
	auto window_menu_button_position = m_custom_gui_style.WindowMenuButtonPosition + 1;
	if ( ImGui::Combo( "WindowMenuButtonPosition", static_cast<int*>( &window_menu_button_position ), "None\0Left\0Right\0" ) )
		m_custom_gui_style.WindowMenuButtonPosition = window_menu_button_position - 1;
	ImGui::Combo( "ColorButtonPosition",		static_cast<int*>( &m_custom_gui_style.ColorButtonPosition ), "Left\0Right\0" );
	ImGui::SliderFloat2( "ButtonTextAlign",		reinterpret_cast<float*>( &m_custom_gui_style.ButtonTextAlign ), 0.0f, 1.0f, "%.2f" );

	ImGui::SliderFloat2( "SelectableTextAlign", reinterpret_cast<float*>( &m_custom_gui_style.SelectableTextAlign ), 0.0f, 1.0f, "%.2f" );
	ImGui::Text( "Safe Area Padding" );
	ImGui::SliderFloat2( "DisplaySafeAreaPadding", reinterpret_cast<float*>( &m_custom_gui_style.DisplaySafeAreaPadding ), 0.0f, 30.0f, "%.0f" );

	ImGui::End( );
}

/// <summary>
/// this function is for displaying the project's color editing window. 
/// </summary>
void imgui_builder::form_color_editor( )
{
	auto& style = ImGui::GetStyle( );

	ImGui::SetNextWindowSize( { 400.f, 500.f }, ImGuiCond_Once );

	ImGui::Begin( "Gui Builder color export/import ", &m_color_menu );

	if ( ImGui::Button( "Export" ) )
	{
		im_config::color::to_clipboard( m_custom_gui_style );
		MessageBoxA( nullptr, "Code exported to clipboard !", "ImGui Builder", MB_OK | MB_ICONINFORMATION );
	}
	
	ImGui::SameLine( );

	if ( ImGui::Button( "Load" ) )
	{
		ImGuiFileDialog::Instance( )->OpenDialog( "OpenColorsDlgKey", "Open File", ".colors", RegeditGetPath( "ImGuiBuilderPath" ), "style_colors" );
	}

	ImGui::SameLine( );
	if ( ImGui::Button( "Save" ) )
	{
		ImGuiFileDialog::Instance( )->OpenDialog( "SaveColorsDlgKey", "Save File", ".colors", RegeditGetPath( "ImGuiBuilderPath" ), "style_colors" );
	}
	

	static ImGuiTextFilter filter;
	filter.Draw( "Filter colors", ImGui::GetFontSize( ) * 16 );

	static auto alpha_flags = 0;
	if ( ImGui::RadioButton( "Opaque", alpha_flags == ImGuiColorEditFlags_None ) )
		alpha_flags = ImGuiColorEditFlags_None;
	

	ImGui::SameLine( );
	if ( ImGui::RadioButton( "Alpha", alpha_flags == ImGuiColorEditFlags_AlphaPreview ) )
		alpha_flags = ImGuiColorEditFlags_AlphaPreview;
	

	ImGui::SameLine( );
	if ( ImGui::RadioButton( "Both", alpha_flags == ImGuiColorEditFlags_AlphaPreviewHalf ) )
		alpha_flags = ImGuiColorEditFlags_AlphaPreviewHalf;


	ImGui::BeginChild( "##colors", ImVec2( 0, 0 ), true,
		ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar |
		ImGuiWindowFlags_NavFlattened );

	ImGui::PushItemWidth( -160 );
	for ( auto i = 0; i < ImGuiCol_COUNT; i++ )
	{
		const auto* name = ImGui::GetStyleColorName( i );
		//std::cout << "Name: " << name << " Index: " << i << std::endl;
		if ( !filter.PassFilter( name ) )
			continue;
		ImGui::PushID( i );
		ImGui::ColorEdit4( "##color", reinterpret_cast<float*>( &m_custom_gui_style.Colors[ i ] ),
			ImGuiColorEditFlags_AlphaBar | alpha_flags );
		if ( memcmp( &m_custom_gui_style.Colors[ i ], &im_config::color::saved_colors()[ i ], sizeof( ImVec4 ) ) != 0 )
		{
			ImGui::SameLine( 0.0f, style.ItemInnerSpacing.x );
			if ( ImGui::Button( "Save" ) ) { im_config::color::saved_colors( )[ i ] = m_custom_gui_style.Colors[ i ]; }
			ImGui::SameLine( 0.0f, style.ItemInnerSpacing.x );
			if ( ImGui::Button( "Revert" ) ) { m_custom_gui_style.Colors[ i ] = im_config::color::saved_colors( )[ i ]; }
		}
		ImGui::SameLine( 0.0f, style.ItemInnerSpacing.x );
		ImGui::TextUnformatted( name );
		ImGui::PopID( );
	}
	ImGui::PopItemWidth( );
	ImGui::EndChild( );

	ImGui::End( );
}

/// <summary>
/// this function is for displaying the project's font editing window. 
/// </summary>
void imgui_builder::form_font_editor( )
{
	ImGui::Begin( "Font Editor", &m_font_menu );

	if ( ImGui::Button( "Import font from file" ) )
	{

	}

	ImGui::Text( std::string( "Current font: " + std::string( "123" ) ).c_str( ) );

	ImGui::End( );
}

/// <summary>
/// will draw everything ... 
/// </summary>
void imgui_builder::draw( )
{
	ImGui::PushAllColorsDark( m_dark_style );
	auto width = 1280;
	RECT rect;
	if ( GetWindowRect( window::i()->get_win32_window(), &rect ) )
	{
		width = rect.right - rect.left;
	}

	draw_dialogs_save_open( );

	if ( m_color_menu )
		form_color_editor( );

	if ( m_style_menu )
		form_window_flag( );

	if ( m_font_menu )
		form_font_editor( );

	paste_obj( );

	ImGui::SetNextWindowSize( { static_cast<float>( width - 16 ), 100 } );
	ImGui::SetNextWindowPos( { 0, 0 } );
	ImGui::Begin( "BUILDER", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar );
	m_my_forms_active = ImGui::IsWindowFocused( );
	if ( ImGui::BeginMenuBar( ) )
	{

		if ( ImGui::BeginMenu( "Project" ) )
		{
			if ( ImGui::MenuItem( "Save" ) )
			{
				ImGuiFileDialog::Instance( )->OpenDialog( "SaveProjectFileDlgKey", "Save File", ".builder", RegeditGetPath( "ImGuiBuilderPath" ), "project" );
			}

			if ( ImGui::MenuItem( "Open" ) )
			{
				ImGuiFileDialog::Instance( )->OpenDialog( "OpenProjectFileDlgKey", "Open File", ".builder", RegeditGetPath( "ImGuiBuilderPath" ), "project" );
			}

			if ( ImGui::MenuItem( "Generate Code" ) )
			{
				ImGuiFileDialog::Instance( )->OpenDialog( "GenCodeProjectFileDlgKey", "Open File", ".cpp,.h,.hpp", RegeditGetPath( "ImGuiBuilderPath" ), "imgui_builder" );
			}

			ImGui::EndMenu( );
		}
		if ( ImGui::BeginMenu( "Editor" ) )
		{
			if ( ImGui::MenuItem( "Color" ) )
				m_color_menu = !m_color_menu;

			if ( ImGui::MenuItem( "Style" ) )
				m_style_menu = !m_style_menu;

			if ( ImGui::MenuItem( "Font" ) )
				m_font_menu = !m_font_menu;

			ImGui::EndMenu( );
		}
		ImGui::EndMenuBar( );
	}

	if ( ImGui::Button( "New Form" ) )
	{
		create_form( );
	}
	ImGui::SameLine( );
	if ( ImGui::Button( "New Child" ) )
	{
		create_child( );
	}
	ImGui::SameLine( );
	if ( ImGui::Button( "New Button" ) )
	{
		create_obj( 1 );
	}
	ImGui::SameLine( );
	if ( ImGui::Button( "New Label" ) )
	{
		create_obj( 2 );
	}
	ImGui::SameLine( );
	if ( ImGui::Button( "New Text" ) )
	{
		create_obj( 3 );
	}
	ImGui::SameLine( );

	if ( ImGui::Button( "New Slider Int" ) )
	{
		create_obj( 4 );
	}
	ImGui::SameLine( );
	if ( ImGui::Button( "New Slider float" ) )
	{
		create_obj( 5 );
	}

	ImGui::SameLine( );
	if ( ImGui::Button( "New CheckBox" ) )
	{
		create_obj( 6 );
	}
	ImGui::SameLine( );
	if ( ImGui::Button( "New radio" ) )
	{
		create_obj( 7 );
	}
	ImGui::SameLine( );
	if ( ImGui::Button( "New toggle" ) )
	{
		create_obj( 8 );
	}

	object_property( );
	ImGui::PopAllColorsCustom( );

	ImGui::PushAllColorsCustom( m_custom_gui_style );

	show_form( );

	ImGui::PopAllColorsCustom( );

	ImGui::End( );
}

/// <summary>
/// creation of a new form.
/// </summary>
void imgui_builder::create_form( )
{
	// that give warning...
	++m_id;
	form frm = { m_id, "form" + std::to_string( m_id ), { 50.f, 50.f } };
	m_forms.push_back( frm );
}

/// <summary>
/// creation of a new beginchild, it's a container
/// </summary>
void imgui_builder::create_child( )
{
	if ( m_forms.empty( ) ) //no crash pls
		return;
	// fixed in update...
	// child id is dinamic to form
	m_child_id = static_cast<int>( m_forms[ m_id ].child.size( ) );
	m_forms[ m_id ].child.push_back( { m_child_id, "child" + std::to_string( m_child_id ), m_id, true, { 50.f, 50.f }, { 15, 15 } } );
}

/// <summary>
/// creation any object
/// </summary>
/// <param name="type">type for the object</param>
void imgui_builder::create_obj( uint16_t type )
{
	if ( m_forms.empty( ) )
		return;

	m_obj_id++;
	auto name = get_name_type( type );

	name += std::to_string( m_obj_id );

	auto child_id = [ & ] ( ) -> int {
		for ( auto& chl : m_forms[ m_active_window_id ].child )
			if ( chl.selected )
				return chl.id;
		return -1;
	};

	const basic_obj new_obj = { m_obj_id, m_active_window_id, child_id(), name, type, { }, { 30, 30 } };
	//form_[id_].obj_render_me.push_back(new_obj);
	m_objs.push_back( new_obj );
}

/// <summary>
/// paste an object that is in the clipboard. 
/// </summary>
void imgui_builder::paste_obj( ) //NOT NEED  OVERLOAD FOR THAT!
{
	if ( m_my_forms_active ) return;
	if ( window::i( )->pressed_bind_keys( VK_LCONTROL, 'V' )  )
	{
		const auto* psz_text = ImGui::GetClipboardText( );

		const std::string text( psz_text );
		auto m_copy = utils::split( text, '\n' );
		ImVec2 pos = { 30, 30 };
		for ( const auto& n_text : m_copy )
		{
			auto o = utils::split( n_text, ',' );

			if ( o.size( ) != 7 )
				return;

			for ( auto tx : o )
			{
				if ( !utils::is_number( tx ) )
					return;
			}

			auto name = get_name_type( std::stoi( o[ 0 ] ) );

			if ( std::stoi( o[ 0 ] ) == 10 )
			{
				m_child_id = static_cast<int>( m_forms[ m_active_window_id ].child.size( ) );
				m_forms[ m_active_window_id ].child.push_back( {
					m_child_id, "child" + std::to_string( m_child_id ), m_active_window_id,
					std::stoi( o[ 4 ] ) != 0, { std::stof( o[ 2 ] ), std::stof( o[ 3 ] ) },
					{ std::stof( o[ 4 ] ), std::stof( o[ 5 ] ) }
					} );
				std::cout << "child obj\n";
			}
			else if ( !name.empty( ) )
			{
				m_obj_id++;
				name += std::to_string( m_obj_id );
				const basic_obj new_obj = {
					m_obj_id, m_active_window_id, std::stoi( o[ 1 ] ), name, std::stoi( o[ 0 ] ), { std::stof( o[ 3 ] ), std::stof( o[ 4 ] ) },
					{ std::stof( o[ 5 ] ), std::stof( o[ 6 ] ) }
				};
				m_objs.push_back( new_obj );
				std::cout << "paste obj\n";
			}
			pos.x += 15;
			pos.y += 15;
		}
	}
}

void imgui_builder::copy_obj( const int type, const int child, const ImVec2 size, const ImVec2 pos, const bool border, bool selected, bool pass_key_check )
{

	if ( ( !m_my_forms_active && window::i( )->pressed_bind_keys( VK_CONTROL, 'C' ) ) || pass_key_check )
	{
		std::string buffer;
		ImGui::LogToClipboard( );
		if ( !selected )
		{
			if ( type == 10 ) // is child
				buffer = std::to_string( type ) + ",0," + std::to_string( size.x ) + "," + std::to_string( size.y ) + "," +
				std::to_string( pos.x ) + "," + std::to_string( pos.y ) + "," + std::to_string( border );
			else
				buffer = std::to_string( type ) + "," + std::to_string( child ) + ",0," + std::to_string( size.x ) + "," +
				std::to_string( size.y ) + "," + std::to_string( pos.x ) + "," + std::to_string( pos.y );
		}
		else
		{
			for ( auto& copy : m_objs )
			{
				if ( copy.selected )
				{
					//if (type == 10) // is child
					//	buffer += std::to_string(type) + ",0," + std::to_string(size.x) + "," + std::to_string(size.y) + "," + std::to_string(border) + "\n";
					//else // other obj
					buffer += std::to_string( copy.my_type ) + "," + std::to_string( copy.child ) + ",0," +
						std::to_string( copy.size.x ) + "," + std::to_string( copy.size.y ) +
						", " + std::to_string( copy.pos.x ) + ", " + std::to_string( copy.pos.y ) + "\n";
				}
			}
		}
		ImGui::LogText( buffer.c_str( ) );
		ImGui::LogFinish( );
		std::cout << "copy that " << buffer << std::endl;
	}
}

/// <summary>
/// show created forms.
/// </summary>
void imgui_builder::show_form( )
{
	for ( auto& form : m_forms )
	{
		ImGui::SetNextWindowSize( form.size );
		//	if any other obj is moving position
		//freezer form because if obj dont have much area for hover like label form move too
		//

		if ( form.delete_me )
		{
			delete_form( form.id );
			break;
		}
		ImGui::Begin( form.name.c_str( ), nullptr, g_moving_obj ? 
			( ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove ) : ( ImGuiWindowFlags_NoCollapse ) );

		form.pos = ImGui::GetWindowPos( ); // get value position form
		form.size = ImGui::GetWindowSize( ); // get value size form

		// get propriety of form with user double click mouse
		if ( ImGui::IsWindowHovered( ) && ImGui::IsMouseDoubleClicked( 0 ) )
		{
			m_name = const_cast<char*>( form.name.c_str( ) );
			m_current_item = form.name + ":" + std::to_string( form.id );
			m_family = form.id;
			m_type = 0;
		}
		m_my_forms_active = !ImGui::IsWindowFocused( );
		if ( ImGui::IsWindowFocused( ) || ImGui::IsWindowAppearing( ) || ImGui::IsWindowHovered( ) )
		{
			m_active_window_id = form.id;

		}

		//Render obj save him
		for ( auto& obj : m_objs )
		{
			if ( obj.form == form.id && obj.child < 0 ) // if really obj for this form and not include in child
			{
				// render obj
				render_obj( obj, form.id );
			}
		}
		for ( auto& container : form.child )
		{
			ImGui::SetCursorPos( container.pos );

			// if signal of delete object
			if ( container.delete_me )
			{
				// delete objs
				for ( auto& obj : m_objs )
				{
					if ( obj.child == container.id )
					{
						//obj.name = "";
						obj.my_type = -1;
						obj.delete_me = true;
						obj.child = -1;
						// obj.delete_me = true;
					}
				}
				// delete child

				form.child.erase( form.child.begin( ) + container.id );
				m_child_id = form.child.size( ) - 1;
				// if obj not is last
				/*if ( container.id != m_objs.size( ) )
				{*/
					// previous object, before rendering the others
					for ( auto new_id = container.id - 1; new_id < form.child.size( ); ++new_id )
					{
						form.child[new_id].name = ("child" + std::to_string(new_id));
						//form.child[new_id].name += "#new" + new_id;
						form.child[ new_id ].id = new_id;
					}
				//}
				break; //not to render the object
			}

			bool hover = false;

			auto normal_select = ( m_current_item == ( container.name + ":" + std::to_string( container.id ) ) );

			ImGui::BeginChild( container.name.c_str( ), container.size, container.border );

			bool scrollEnableY = ImGui::GetScrollMaxY( ) > 0.f;
			auto scrollPosY = ImGui::GetScrollY( );

			for ( auto& obj : m_objs )
			{
				if ( obj.form == form.id && obj.child == container.id ) // render obj with child
				{
					render_obj( obj, form.id );

					ImVec2 oldPos = obj.pos;

					if ( scrollEnableY )
					{
						obj.pos.y -= scrollPosY;
					}
					
					if ( my_IsItemHovered( obj.pos, obj.size, 5.f ) )
						hover = true;

					obj.pos = oldPos;
				}
			}
			ImGui::EndChild( );

			// check if selected
			// container.hover = ImGui::IsItemHovered( );
			container.hover = my_IsItemHovered( container.pos, container.size, 5.f ) && !hover;
			//if ( container.hover && limit_bordering_control( container.pos, container.size, -15.f ) != resize_opt::off ) //I don't know if you were good with that 
			//	container.hover = false;

			auto left_clicked = ImGui::IsMouseClicked( 0, false );
			auto right_clicked = ImGui::IsMouseClicked( 1, false );
			auto show_context = normal_select;

			// thats is shame but... work good....
			if ( container.hover && ( left_clicked || right_clicked ) && show_context == false )
			{
				show_context = !left_clicked;
				if ( GetKeyState( VK_CONTROL ) & 0x8000 )
					container.selected = !container.selected;
				else
				{
					for ( auto& o_obj : m_objs )
						o_obj.selected = false;
				}
				// container.selected	= true;
				m_current_item		= container.name + ':' + std::to_string( form.id );
				m_family			= container.father;
				m_index				= container.id;
				m_type				= 10;
			}
			if ( container.hover )
				SetCursor( this->cursor.m_arrow_all );

			container.selected = ( m_current_item == ( container.name + ":" + std::to_string( form.id ) ) );
			if ( container.selected && form.id == m_active_window_id )
			{
				ImGui::DrawObjBorder( container.pos, container.size );
			}
			if ( !container.locked )
				resize_obj( container.pos, container.size, container.hover, container.selected );

			ImGui::PushAllColorsDark( m_dark_style );

			if ( show_context && ImGui::BeginPopupContextItem( "##obj_context" ) )
			{
				//style.ButtonTextAlign
				auto& g = *GImGui;
				auto backup1_y = g.Style.ButtonTextAlign.y;
				auto backup2_y = g.Style.FramePadding.y;
				g.Style.FramePadding.y = -1.3f;
				g.Style.ButtonTextAlign.y = 0.f;
				ImVec2 btn_size = { 60.f, 12.f };
				if ( ImGui::Button( "delete", btn_size ) )
				{
					container.delete_me = true;
					m_current_item = "";
					m_type = -1;
				}
				//if ( ImGui::Button( "copy", btn_size ) )
				//{
				//	copy_obj( container.my_type, obj.child, obj.size, obj.pos, false, false, true );
				//}
				ImGui::Checkbox( "lock", &container.locked );
				g.Style.ButtonTextAlign.y = backup1_y;
				g.Style.FramePadding.y = backup2_y;
				ImGui::EndPopup( );
			}

			ImGui::PopAllColorsCustom( );
		}
		ImGui::End( );
	}
}

void imgui_builder::delete_form( int form_id )
{
	m_forms.erase( m_forms.begin( ) + form_id );
	m_id = m_forms.size( ) - 1;
	std::cout << "size id " << m_id << std::endl;

	if ( !m_objs.empty( ) && !m_forms.empty( ) )
	{
		for ( auto i = m_objs.size( ) - 1; i > -1; --i )
		{
			std::cout << i << std::endl;
			if ( m_objs[ i ].form == form_id )
			{
				std::cout << "Obj id: " << m_objs[ i ].id << " \t form id: " << m_objs[ i ].form << std::endl;
				m_objs.erase( m_objs.begin( ) + i );
				m_obj_id = m_objs.size( ) - 1;

				for ( auto x = i - 1; x < m_objs.size( ); ++x )
					m_objs[ x ].id = x;
			}
		}

		std::cout << "delete file finish\n";
	}
	else
	{
		m_objs.clear( );
		m_obj_id = -1;
		std::cout << "delete all\n";
	}


	for ( auto id = ( form_id - 1 ); m_id >= id; ++id )
	{
		if ( id < 0 )
			continue;

		for ( auto& obj : m_objs )
		{
			if ( obj.form == form_id )
				obj.delete_me = true;

			if ( obj.form == m_forms[ id ].id )
				obj.form = id;
		}
		m_forms[ id ].id = id;
	}
}

void imgui_builder::resize_obj( basic_obj& current_obj, bool selected )
{
	if ( current_obj.locked ) return;
	resize_obj( current_obj.pos, current_obj.size_obj, current_obj.hover, selected );
	current_obj.size = current_obj.size_obj;
}

void imgui_builder::resize_obj( ImVec2& obj_pos, ImVec2& obj_size, bool hover, bool selected )
{
	//if ( obj.my_type != button ) return;
	//printf( "pos { %.f, %.f }, size { %.f, %.f }, hover %d, selected %d\n", obj_pos.x, obj_pos.y, obj_size.x, obj_size.y, hover, selected );
	auto resze_opt			= resize_opt::off;

	bool scrollEnableY = ImGui::GetScrollMaxY( ) > 0.f;
	auto scrollPosY = ImGui::GetScrollY( );

	if ( hover && !m_tick_resize )
	{
		ImVec2 oldPos = obj_pos;

		if ( scrollEnableY )
		{
			obj_pos.y -= scrollPosY;
		}

		resze_opt = limit_bordering_control( obj_pos, obj_size, 3.f );

		obj_pos = oldPos;

		switch ( resze_opt )
		{
		case resize_opt::bottom_right:
		case resize_opt::top_left:
			this->cursor.m_current_icon	= this->cursor.m_arrow_northwest_and_southeast;
			break;
		case resize_opt::top_right:
		case resize_opt::bottom_left:
			this->cursor.m_current_icon	= this->cursor.m_arrow_northeast_and_southwest;
			break;
		case resize_opt::top:
		case resize_opt::bottom:
			this->cursor.m_current_icon	= this->cursor.m_arrow_top_or_bottom;
			break;
		case resize_opt::left:
		case resize_opt::right:
			this->cursor.m_current_icon	= this->cursor.m_arrow_left_or_right;
			break;
		default:
			break;
		}
		if ( resze_opt != resize_opt::off )
		{
			SetCursor( this->cursor.m_current_icon );
		}
	}

	m_no_move = (m_tick_resize ) || ( resze_opt != resize_opt::off );

	//printf( "pos { %.f, %.f }, size { %.f, %.f }, hover %d, selected %d, moving %d\n", obj_pos.x, obj_pos.y, obj_size.x, obj_size.y, hover, selected, g_moving_obj );
	if ( g_moving_obj || !window::i( )->holding_key( VK_LBUTTON )  )
	{
		g_resize_obj	= false;
		m_tick_resize	= 0;
		m_resize_opt	= resize_opt::off;
		//m_in_resize_id	= 0;
		return;
		
	}

	if ( !selected  ) return;
	auto current_pos	= window::i( )->get_relative_cursor_pos( );
	auto tick_now		= GetTickCount64( );

	if ( m_tick_resize == 0 && resze_opt != resize_opt::off )
	{
		m_resize_opt	= resze_opt;
		m_tick_resize	= tick_now + 80;
		//m_in_resize_id	= obj.id + obj.my_type;
		return;
	}	
	

	//printf( "pos { %.f, %.f }, size { %.f, %.f }, hover %d, selected %d\n", obj_pos.x, obj_pos.y, obj_size.x, obj_size.y, hover, selected );

	if ( m_tick_resize && tick_now > m_tick_resize )
	{
		g_resize_obj			= true;
		auto current_win_pos	= ImGui::GetWindowPos( );
		SetCursor( this->cursor.m_current_icon );

		auto normalize_diff		= []( float diff, float val = 100.f ) -> float //at some point it retains an exorbitant value, it will help to control that 
		{
			if ( diff > val )
				diff = val;
			else if ( diff < -val )
				diff = -val;
			return diff;
		};

		auto basic_margins = [&]( resize_opt rs_opt ) -> void
		{
			switch ( rs_opt )
			{
			case resize_opt::right:
			{
				auto end_pos_x	= current_win_pos.x + obj_pos.x + obj_size.x;
				auto dif		= normalize_diff( current_pos.x - end_pos_x );
				obj_size.x		+= dif;
				break;
			}
			case resize_opt::left:
			{
				auto end_pos_x	= current_win_pos.x + obj_pos.x;
				auto dif		= normalize_diff( end_pos_x - current_pos.x );
				obj_pos.x		-= dif;
				obj_size.x		+= dif;
				break;
			}
			case resize_opt::top:
			{
				auto end_pos_y	= ( current_win_pos.y + obj_pos.y ) - scrollPosY;
				auto dif		= normalize_diff( end_pos_y - current_pos.y );
				obj_pos.y		-= dif;
				obj_size.y		+= dif;
				break;
			}
			case resize_opt::bottom:
			{
				auto end_pos_y	= current_win_pos.y + obj_pos.y + obj_size.y;
				auto dif		= normalize_diff( current_pos.y - end_pos_y );
				obj_size.y		+= dif;
				break;
			}
			default:
				break;
			}		
		};

		switch ( m_resize_opt )
		{
		case resize_opt::bottom_right:
		{
			basic_margins( resize_opt::bottom );
			basic_margins( resize_opt::right );
			break;
		}
		case resize_opt::top_left:
		{
			basic_margins( resize_opt::top );
			basic_margins( resize_opt::left );
			break;
		}
		case resize_opt::top_right:
		{
			basic_margins( resize_opt::top );
			basic_margins( resize_opt::right );
			break;
		}
		case resize_opt::bottom_left:
		{
			basic_margins( resize_opt::bottom );
			basic_margins( resize_opt::left );
			break;
		}
		default:
			basic_margins( m_resize_opt );
			break;
		}
	}

}

void imgui_builder::render_obj( basic_obj& obj, int current_form_id )
{
	// set pos for next obj render
	ImGui::SetCursorPos( obj.pos );

	// if signal of delete object
	if ( obj.delete_me )
	{
		// delete obj
		if (obj.id < m_obj_id)
		{
			m_objs.erase(m_objs.begin() + obj.id);
			// reform id objs
			m_obj_id = m_objs.size() - 1;
		}

		// previous object, before rendering the others
		for ( auto new_id = obj.id - 1; new_id < m_objs.size( ); ++new_id )
		{
			m_objs[ new_id ].id = new_id;
			//std::cout << obj_render_me[new_id].id << std::endl;
		}
		return; //not to render the object
	}

	// To type any obj for render set in case
	// to render all obj for 1 time in loop

	// buffer for inputs
	std::string		buffer			= "text here";
	int				value_i			= 0;
	float			value_f			= 0;
	static bool		true_bool		= false;
	auto			normal_select	= ( m_current_item == ( obj.name + ":" + std::to_string( obj.id ) ) );


	auto relative_for_resize = []( basic_obj& obj ) -> float
	{ 
		auto&		g			= *GImGui;
		auto*		window		= g.CurrentWindow;
		const auto&	style		= g.Style;
		const auto	id			= window->GetID( obj.name.c_str( ) );
		const auto	label_size	= ImGui::CalcTextSize( obj.name.c_str( ), nullptr, true );
		const auto	frame_size	= ImGui::CalcItemSize( ImVec2( 0, 0 ), ImGui::CalcItemWidth( ), ( label_size.y ) + style.FramePadding.y * 2.0f );
		const auto	label_dif	= ( label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f );
		if ( obj.size.x == 0.f && obj.size.y == 0.f )
			obj.size = ImVec2( frame_size.x + label_dif, frame_size.y );
		return obj.size.x - label_dif;
	};

	// render obj
	switch ( obj.my_type )
	{
	case 1:
		ImGui::Button( obj.name.c_str( ), obj.size );
		break;
	case 2:
		ImGui::Text( obj.name.c_str( ) );
		break;
	case 3:
	{
		ImGui::PushItemWidth(relative_for_resize(obj));
		ImGui::InputText( obj.name.c_str( ), const_cast<char*>( buffer.c_str( ) ), 254 );
		ImGui::PopItemWidth( );
		break;
	}
	case 4:
	{
		ImGui::PushItemWidth( relative_for_resize( obj ) );
		ImGui::SliderInt( obj.name.c_str( ), &value_i, 0, 100 );
		ImGui::PopItemWidth( );
		break;
	}
	case 5:
	{
		ImGui::PushItemWidth( relative_for_resize( obj ) );
		ImGui::SliderFloat( obj.name.c_str( ), &value_f, 0, 100 );
		ImGui::PopItemWidth( );
		break;
	}
	case 6:
		ImGui::Checkbox( obj.name.c_str( ), &true_bool );

		break;
	case 7:
		ImGui::RadioButton( obj.name.c_str( ), true_bool );

		break;
	case 8:
		ImGui::ToggleButton( obj.name.c_str( ), &true_bool );

		break;
	default:
		break;
	}
	obj.size_obj		= ImGui::GetItemRectSize( );

	bool scrollEnableY	= ImGui::GetScrollMaxY( ) > 0.f;

	auto scrollPosY		= ImGui::GetScrollY( );

	ImVec2 oldPos = obj.pos;

	if ( scrollEnableY )
	{
		obj.pos.y -= scrollPosY;
	}

	if ( ( obj.selected || normal_select ) && current_form_id == m_active_window_id )
	{
		ImGui::DrawObjBorder( obj );
	}

	// get size and hover of object
	//obj.hover		= ImGui::IsItemHovered( );
	obj.hover		= my_IsItemHovered( obj.pos, obj.size_obj, 5.f );

	// Set family and type child etc for propri and execution modification on type
	auto left_clicked	= ImGui::IsMouseClicked( 0, false );
	auto right_clicked	= ImGui::IsMouseClicked( 1, false );
	auto show_context	= normal_select;
	if ( obj.hover && (left_clicked || right_clicked) && show_context == false )
	{
		show_context = !left_clicked;
		if ( GetKeyState( VK_CONTROL ) & 0x8000 )
			obj.selected = !obj.selected;
		else
		{
			for ( auto& o_obj : m_objs )
				o_obj.selected = false;
		}

		m_current_item	= obj.name + ':' + std::to_string( obj.id );
		m_family		= obj.form;
		m_grandchild	= obj.child;
		m_index			= obj.id;
		m_type			= obj.my_type;
	}

	ImGui::PushAllColorsDark(m_dark_style);

	if ( show_context && ImGui::BeginPopupContextItem( "##obj_context" ) )
	{
		//style.ButtonTextAlign
		auto& g = *GImGui;
		auto backup1_y = g.Style.ButtonTextAlign.y;
		auto backup2_y = g.Style.FramePadding.y;
		g.Style.FramePadding.y		= -1.3f;
		g.Style.ButtonTextAlign.y	= 0.f;
		ImVec2 btn_size				= { 60.f, 12.f };
		if ( ImGui::Button( "delete", btn_size ) )
		{
			obj.delete_me	= true;
			m_current_item	= "";
			m_type			= -1;
		}
		if ( ImGui::Button( "copy", btn_size ) )
		{
			copy_obj( obj.my_type, obj.child, obj.size, obj.pos, false, false, true );
		}
		ImGui::Checkbox( "lock", &obj.locked );
		g.Style.ButtonTextAlign.y	= backup1_y;
		g.Style.FramePadding.y		= backup2_y;
		ImGui::EndPopup( );
	}

	ImGui::PopAllColorsCustom( );

	if ( obj.hover )
		SetCursor( this->cursor.m_arrow_all );

	obj.pos = oldPos;

	resize_obj( obj, normal_select  );
}

void imgui_builder::object_property( )
{
	static std::vector<move_obj> mto{ };
	ImGui::SetNextWindowPos( { 0, 100 } );
	ImGui::SetNextWindowSize( { 300, 700 - 100 } );
	ImGui::Begin( "property", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus );
	m_my_forms_active = ImGui::IsWindowFocused( );
	if ( ImGui::BeginCombo( "##itens", m_current_item.c_str( ) ) )
	{
		// list all obj render in array child and form

		for ( auto& n : m_forms )
		{
			if ( n.delete_me )
			{
				break;
			}

			// it is simply possible to simplify this please do this
			auto item = n.name + ":" + std::to_string( n.id );
			const auto is_selected = ( m_current_item == item );

			if ( ImGui::Selectable( item.c_str( ), is_selected ) )
			{
				m_name = const_cast<char*>( n.name.c_str( ) );
				m_current_item = item;
				m_type = 0;
				m_family = n.id;
			}

			for ( auto& c : n.child )
			{
				item = c.name + ":" + std::to_string( n.id );
				if ( ImGui::Selectable( item.c_str( ), is_selected ) )
				{
					m_family		= n.id;
					m_index			= c.id;
					m_type			= 10;
					m_current_item	= item;
				}
			}

			item = "";

			if ( is_selected )
				ImGui::SetItemDefaultFocus( );
		}

		for ( auto& o : m_objs )
		{
			if ( o.delete_me )
			{
				break;
			}

			auto item = o.name + ":" + std::to_string( o.id );
			const auto is_selected = ( m_current_item == item );

			item = o.name + ":" + std::to_string( o.id );
			if ( ImGui::Selectable( item.c_str( ), is_selected ) )
			{
				m_family		= o.form;
				m_grandchild	= o.child;
				m_index			= o.id;
				m_type			= o.my_type;
				m_current_item	= item;
			}

			if ( is_selected )
				ImGui::SetItemDefaultFocus( );
		}

		ImGui::EndCombo( );
	}

	// vars for simplification functions less line length
	child		chl{ };
	basic_obj	obj{ };
	form		fm{ };

	switch ( m_type )
	{
	case -1: // none

		break;

	case 0: // form
		fm = m_forms[ m_family ];
		ImGui::InputInt( "ID", &fm.id, 0 );

		ImGui::InputTextEx( "Name form", &m_name, 0 );
		//ImGui::InputText("name form", name, 255);
		if ( ImGui::Button( "Apply name" ) )
		{
			if ( !m_name.empty( ) ) fm.name = m_name;
		}
		ImGui::InputFloat( "SizeX", &fm.size.x, 1 );
		ImGui::InputFloat( "SizeY", &fm.size.y, 1 );
		ImGui::InputFloat( "PosX", &fm.pos.x, 1 );
		ImGui::InputFloat( "PosY", &fm.pos.y, 1 );

		if ( ImGui::Button( "DELETE" ) || window::i( )->pressed_key( VK_DELETE ) )
		{
			fm.delete_me	= true;
			m_current_item	= "";
			m_type			= -1;
		}

		m_forms[ m_family ] = fm;
		break;

	case 10: // child
		chl = m_forms[ m_family ].child[ m_index ];
		m_form_pos = m_forms[ m_family ].pos;
		ImGui::InputInt( "ID", &chl.id, 0 );

		if ( ImGui::InputInt( "Form Father", &chl.father, 1 ) )
		{
			m_forms[ chl.father ].child.push_back( chl );
			chl.delete_me	= true;
			m_current_item	= "";
			m_type			= -1;
		}

		ImGui::InputFloat( "SizeX", &chl.size.x, 1 );
		ImGui::InputFloat( "SizeY", &chl.size.y, 1 );
		ImGui::InputFloat( "PosX", &chl.pos.x, 1 );
		ImGui::InputFloat( "PosY", &chl.pos.y, 1 );
		ImGui::Checkbox( "Border", &chl.border );
		ImGui::SameLine( );
		ImGui::Checkbox( "Lock", &chl.locked );
		m_item_size = chl.size;
		if ( chl.hover && !chl.locked )
			chl.change_pos = true;

		if ( !m_no_move )
		move_item( chl.pos, chl.change_pos );

		copy_obj( 10, 0, chl.size, chl.pos, chl.border, obj.selected );

		if ( ImGui::Button( "DELETE" ) || window::i( )->pressed_key( VK_DELETE ) )
		{
			chl.delete_me	= true;
			m_current_item	= "";
			m_type			= -1;
		}

		m_forms[ m_family ].child[ m_index ] = chl;
		break;
	default: // another obj

		if ( m_grandchild > -1 )
		{
			obj = m_objs[ m_index ];
			m_form_pos	= m_forms[ m_family ].child[ m_grandchild ].pos;
			m_form_pos.x += m_forms[ m_family ].pos.x;
			m_form_pos.y += m_forms[ m_family ].pos.y;
		}
		else
		{
			if ( m_objs.size( ) > static_cast<unsigned>( m_index ) )
			{
				obj = m_objs[ m_index ];
				m_form_pos = m_forms[ m_family ].pos;
			}
		}

		m_item_size = obj.size_obj;
		ImGui::InputInt( "ID", &obj.id, 0 );
		ImGui::InputInt( "Form Father", &obj.form, 1 );
		ImGui::InputInt( "Child Father", &obj.child, 1 );
		ImGui::InputTextEx( "Name", &obj.name, 0 );
		//ImGui::InputText("Name", name, 255);
		ImGui::InputFloat( "PosX", &obj.pos.x, 1, 1 );
		ImGui::InputFloat( "PosY", &obj.pos.y, 1, 1 );
		ImGui::InputFloat( "SizeX", &obj.size.x, 1, 1 );
		ImGui::InputFloat( "SizeY", &obj.size.y, 1, 1 );
		ImGui::Checkbox( "Lock", &obj.locked );

		//obj.name = name;
		// check if hover because need for change position
		if ( obj.hover )
			obj.change_pos = true;

		if ( obj.selected && !m_no_move )
		{
			mto.clear( );
			for ( auto& r_obj : m_objs )
			{
				if ( r_obj.selected == true && !r_obj.locked )
				{
					mto.push_back( { r_obj.id, r_obj.pos } );
				}
			}

			move_items( mto, obj.change_pos );

			for ( auto& teste : mto )
			{
				if ( obj.id == teste.index )
					obj.pos = teste.pos;
				else
					m_objs[ teste.index ].pos = teste.pos;
			}
		}
		else if ( !m_no_move )
			if (!obj.locked )
				move_item( obj.pos, obj.change_pos );

		copy_obj( obj.my_type, obj.child, obj.size, obj.pos, false, obj.selected );

		// dont delete here!
		if ( ImGui::Button( "DELETE" ) || window::i( )->pressed_key( VK_DELETE ) )
		{
			obj.delete_me	= true;
			m_current_item	= "";
			m_type			= -1;
		}

		m_objs[ m_index ] = obj;

		break;
	}

	ImGui::End( );
}

void imgui_builder::routine_draw( )
{
	static imgui_builder* instance = nullptr;
	if ( !instance )
		instance = new imgui_builder( );
	instance->draw( );
}

void move_items( std::vector<move_obj>& mto, bool& continue_edt )
{
	static POINT old_pos = { 0, 0 };
	static std::vector<ImVec2> old_pos_obj{ };
	if ( !window::i( )->holding_key( VK_LBUTTON ) || !continue_edt )
	{
		old_pos_obj.clear( );
		g_moving_obj	= false;
		continue_edt	= false;
		old_pos			= { 0, 0 };
		g_tick_move		= 0;
		return;
	}
	auto tick_now		= GetTickCount64( );
	auto current_pos	= window::i( )->get_relative_cursor_pos( );
	if ( g_tick_move == 0 )
	{
		g_tick_move		= tick_now + 100;
		old_pos			= current_pos;
		for ( auto& i : mto )
			old_pos_obj.push_back( i.pos );
		return;
	}
	if ( mto.size( ) != old_pos_obj.size( ) )
	{
		old_pos_obj.clear( );
		g_moving_obj	= false;
		continue_edt	= false;
		old_pos			= { 0, 0 };
		g_tick_move		= 0;
		return;
	}
	if ( tick_now > g_tick_move )
	{
		g_moving_obj	= true;
		auto i			= 0;
		for ( auto& m : mto )
		{
			const auto x_pos = old_pos_obj[ i ].x + current_pos.x - old_pos.x;
			const auto y_pos = old_pos_obj[ i ].y + current_pos.y - old_pos.y;
			m.pos.x = x_pos;
			m.pos.y = y_pos;
			++i;
		}
	}
	return;
}

void move_item( ImVec2& obj_pos, bool& continue_edt )
{
	static ImVec2 old_pos{ };
	if ( !window::i( )->holding_key( VK_LBUTTON ) || !continue_edt  )
	{
		old_pos				= { 0, 0 };
		g_moving_obj		= false;
		continue_edt		= false;
		g_tick_move			= 0;
		return;
	}
	auto tick_now			= GetTickCount64( );
	auto current_pos		= window::i( )->get_relative_cursor_pos( );
	if ( g_tick_move == 0 )
	{
		old_pos				= { current_pos.x - obj_pos.x, current_pos.y - obj_pos.y };
		g_tick_move			= tick_now + 50;
		return;
	}
	auto current_win_pos = ImGui::GetWindowPos( );
	if ( tick_now > g_tick_move )
	{
		g_moving_obj		= true;
		const auto x_pos	=  current_pos.x - old_pos.x;
		const auto y_pos	=  current_pos.y - old_pos.y;
		obj_pos.x = x_pos;
		obj_pos.y = y_pos;
	}
}