#ifndef __MODULE_FONTS_H__
#define __MODULE_FONTS_H__

#include "Module.h"
#include "SDL\include\SDL_pixels.h"

#define MAX_FONTS 10
#define MAX_FONT_CHARS 256 //Estos dos valores se pueden cambiar dependiendo de las fonts

struct SDL_Texture;

struct Font {
	char table[MAX_FONT_CHARS];

	SDL_Texture* texture = nullptr;

	uint totalLength;
	uint rows, columns;
	uint char_w, char_h;
};

class ModuleFonts : public Module {
public:
	ModuleFonts(Application* app, bool start_enabled = true);

	~ModuleFonts();

	// Loads a font file from a texture
	// Returns a font index from the fonts array
	// Param texturePath	- The path to the texture file
	// Param characters		- The lookup table. All characters displayed in the same order as the texture
	// Param rows			- The amount of character rows in the texture (se puede cambiar dependiendo del archivo de las texturas de las fonts)

	int Load(const char* texturePath, const char* characters, uint rows = 1);

	void Unload(int fontIndex);

	void BlitText(int x, int y, int fontIndex, const char* text) const;

private:

	Font fonts[MAX_FONTS];

};

#endif // __MODULE_FONTS_H__
