#include "../header.h"
#include "the_window.h"


#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

void glfw_error_callback( int error, const char* description )
{
	fprintf( stderr, "Glfw Error %d: %s\n", error, description );
}

window* c_window = nullptr;


/// <summary>
/// creation builder window
/// </summary>
window::window( void* func_draw ) : m_func_draw( func_draw )
{
	c_window = this;

	glfwSetErrorCallback( glfw_error_callback );
	if ( !glfwInit( ) )
		return;

	const char* glsl_version = "#version 130";
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );

	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );

	m_glfwindow = glfwCreateWindow( 1280, 720, "ImGuiBuilder!", nullptr, nullptr );

	if ( !m_glfwindow ) return;

	glfwMakeContextCurrent( m_glfwindow );

	glfwSwapInterval( 1 );

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit( ) != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit( ) != GLEW_OK;
#endif
	if ( err )
	{
		fprintf( stderr, "Failed to initialize OpenGL loader!\n" );
		return;
	}

	IMGUI_CHECKVERSION( );

	ImGui::CreateContext( );

	auto& io = ImGui::GetIO( );

	io.ConfigWindowsMoveFromTitleBarOnly = true;

	ImGui::StyleColorsDark( );

	ImGui_ImplGlfw_InitForOpenGL( m_glfwindow, true );

	ImGui_ImplOpenGL3_Init( glsl_version );

	auto clear_color = ImVec4( 0.30f, 0.30f, 0.30f, 1.00f );

	m_window = GetForegroundWindow( );
}

/// <summary>
/// check key pressed in current window
/// </summary>
/// <param name="key">key val</param>
/// <returns></returns>
bool window::pressed_key( int key )
{
	return ( GetFocus( ) == m_window ) && ( GetAsyncKeyState( key ) & 1 );
}

/// <summary>
/// checks if the key is being held 
/// </summary>
/// <param name="key">key val</param>
/// <returns></returns>
bool window::holding_key( int key )
{
	return ( GetFocus( ) == m_window ) && GetAsyncKeyState( key );
}

/// <summary>
/// check bind keys pressed in current window
/// </summary>
/// <param name="key_1">key val 1</param>
/// <param name="key_2">key val 2</param>
/// <returns></returns>
bool window::pressed_bind_keys( int key_1, int key_2 )
{
	return ( GetFocus( ) == m_window ) && ( GetAsyncKeyState( key_1 ) ) && ( GetAsyncKeyState( key_2 ) & 1 );
}

/// <summary>
/// get window instance
/// </summary>
/// <returns>current instance </returns>
window* window::i( )
{
	return c_window;
}


POINT window::get_relative_cursor_pos( )
{
	POINT pos;
	GetCursorPos( &pos );
	ScreenToClient( m_window, &pos );
	return pos;
}

/// <summary>
/// render window routine
/// </summary>
void window::routine( )
{
	auto clear_color = ImVec4( 0.30f, 0.30f, 0.30f, 1.00f );
	while ( !glfwWindowShouldClose( m_glfwindow ) )
	{
		glfwPollEvents( );

		ImGui_ImplOpenGL3_NewFrame( );

		ImGui_ImplGlfw_NewFrame( );

		ImGui::NewFrame( );

		if ( m_func_draw )
			reinterpret_cast<void( __stdcall* )( void )>( m_func_draw )( );

		ImGui::Render( );

		int display_w, display_h;
		glfwGetFramebufferSize( m_glfwindow, &display_w, &display_h );

		glViewport( 0, 0, display_w, display_h );

		glClearColor( clear_color.x, clear_color.y, clear_color.z, clear_color.w );

		glClear( GL_COLOR_BUFFER_BIT );

		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );

		glfwSwapBuffers( m_glfwindow );
	}
}


/// <summary>
/// get created builder window handle
/// </summary>
/// <returns>handle window</returns>
HWND window::get_win32_window( )
{
	return m_window;
}

/// <summary>
/// cleanup
/// </summary>
window::~window( )
{
	ImGui_ImplOpenGL3_Shutdown( );

	ImGui_ImplGlfw_Shutdown( );

	ImGui::DestroyContext( );

	glfwDestroyWindow( m_glfwindow );

	glfwTerminate( );
}
