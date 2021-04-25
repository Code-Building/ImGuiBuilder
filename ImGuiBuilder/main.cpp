#include "header.h"
#include "main/the_window.h"
#include "main/imgui_builder.h"



int main(int, char**)
{
	window win( imgui_builder::routine_draw );

	win.routine( );

	return 0;
}