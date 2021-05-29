#pragma once


enum type_obj
{
	button = 1,
	label,
	slider_i,
	slider_f,
	checkbox,
	radio,
	toggle
}; 
enum class resize_opt
{
	off,
	bottom_right,
	top_left,
	top_right,
	bottom_left,
	top,
	bottom,
	left,
	right
};

struct basic_obj
{
	int			id			= 0;
	int			form		= 0;
	int			child		= -1;
	std::string name		= "obj";
	int32_t		my_type		= 0;

	ImVec2		size		= { };
	ImVec2		pos			= { 10, 10 };
	ImVec2		size_obj	= { };

	bool		change_pos	= false;
	bool		hover		= false;
	bool		delete_me	= false;
	bool		selected	= false;
	bool		locked		= false;
};

struct child
{
	int			id			= 0;
	std::string name		= "form";
	int			father		= 0;
	bool		border		= true;
	ImVec2		size		= { };
	ImVec2		pos			= { };

	// AGAIN REPEAT FIX THAT!
	bool		change_pos	= false;
	bool		hover		= false;
	bool		delete_me	= false;
	bool		selected	= false;
	bool		locked		= false;
	//std::vector<simple_obj> obj_render_me; // is very unstable that
};

struct form
{
	int			id			= 0;
	std::string name		= "form";
	ImVec2		size		= { };
	ImVec2		pos			= { };
	uint32_t	type_me		= 0;

	// AGAIN REPEAT FIX THAT!
	bool		change_pos	= false;
	bool		hover		= false;
	bool		delete_me	= false;
	std::vector<child> child{ };
};