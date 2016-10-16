#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "config.hpp"
#include "runtime.hpp"
#include "editor.hpp"
#include "cartridge.hpp"
#include <SDL.h>

enum ScreenMode {
	RuntimeMode,
	EditorMode
};

//Texture wrapper class
class Screen
{
	public:
		//Initializes variables
		Screen();

		//Deallocates memory
		~Screen();

		bool init();

		void run();

		//Deallocates texture
		void free();
		
		//Renders screen
		void render();

		//Gets screen dimensions
		int getWindowWidth();
		int getWindowHeight();

		//Pixel manipulators
		bool lockTexture();
		bool unlockTexture();
		void* getPixels();
		void copyPixels( void* pixels );
		int getPitch();
		
	private:
		//The window we'll be rendering to
		SDL_Window* window;

		//The window renderer
		SDL_Renderer* renderer;

		//The actual hardware texture
		SDL_Texture* texture;

		void* pixels;
		int pitch;
		int windowWidth;
		int windowHeight;
		//Image dimensions
		unsigned int screenMult;

		Cartridge* cartridge;

		Editor* editor;

		Runtime* runtime;

		ScreenMode screenMode;
};

#endif