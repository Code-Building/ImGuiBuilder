#pragma once


/// <summary>
/// main window for editor
/// </summary>
class window
{
public:
	window( void* func_draw );
	~window( );
	static window* i( );
	void routine( );
	HWND get_win32_window( );
	bool pressed_key( int key );
	bool holding_key( int key );
	bool pressed_bind_keys( int key_1, int key_2 );
	POINT get_relative_cursor_pos( );
private:
	void*		m_func_draw = nullptr;
	HWND		m_window	= nullptr;
	GLFWwindow* m_glfwindow = nullptr;
};
