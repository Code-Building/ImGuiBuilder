#pragma once

// ===========================
// Windows IMPORTS
// ===========================

#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include <regex>
#include <fstream>
#include <sstream>

// ===========================
// Windows IMPORTS /
// ===========================


// ===========================
// OPENGL Framework
// ===========================

#ifndef _OPENGL2
	#include <GL/gl3w.h>
#endif
#include <GLFW/glfw3.h>

// ===========================
// OPENGL Framework /
// ===========================


// ===========================
// IMGUI Framework
// ===========================

#include <imgui.h>
#include <opengl/imgui_impl_glfw.h>

#ifdef _OPENGL2
#include <opengl/imgui_impl_opengl2.h>
#else
#include <opengl/imgui_impl_opengl3.h>
#endif

// ===========================
// IMGUI Framework /
// ===========================