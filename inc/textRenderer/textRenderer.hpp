#pragma once

// Include Freetpye libraries
#include <ft2build.h>
#include FT_FREETYPE_H

// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Standard includes
#include <iostream>
#include <map>

class TextRenderer
{
public:
	friend class TextShader;
	/**
	* Text renderer constructor
	* 
	*/

	/**
	* Function to initialize freetype library
	* 
	* Function initializes library, reads font from passed path
	* and loads first 128 ASCII characters to local storage.
	* 
	* @param: path to font file
	* @return 0: success, -1: error.
	*/
	auto initFreetype(const char* path) -> int;
protected:
	struct Character
	{
		unsigned int	TextureID;		// ID handle to glyph text
		glm::ivec2		Size;			// Size of glyph
		glm::ivec2		Bearing;		// Offset form base line to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};
	std::map<char, Character> Characters;
};
