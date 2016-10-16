#ifndef SYSTEM_SDL2_HPP
#define SYSTEM_SDL2_HPP

#include "config.hpp"
#include "system_interface.hpp"
#include "runtime.hpp"
#include "editor.hpp"
#include "cartridge.hpp"
#include <SDL.h>

//Texture wrapper class
class SystemSdl2: public SystemInterface
{
	public:
		//Initializes variables
		SystemSdl2();

		//Deallocates memory
		~SystemSdl2();

		bool init(int argc, char *argv[]);

		void run();
		
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
		unsigned int widthMult;
		unsigned int heightMult;

		Cartridge* cartridge;

		Editor* editor;

		Runtime* runtime;

		ScreenMode screenMode;

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
};

#endif