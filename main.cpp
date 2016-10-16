#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include "system_interface.hpp"
#include "system_sdl2.hpp"
#include "sol.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
	SystemInterface*  system = new SystemSdl2();
	if (system->init(argc, argv)) {
		system->run();
	}


    // sol::state lua;
    // sol::optional<sol::function> init = lua["_init"];
    // sol::optional<sol::function> update = lua["_update"];
    // sol::optional<sol::function> draw = lua["_draw"];
	// Screen* screen = new Screen();

	// //Start up SDL and create window
	// if( screen->init() )
	// {
	// 	screen->run();

	// 	Uint32 windowID = SDL_GetWindowID(screen->window);
	// 	//Main loop flag
	// 	bool quit = false;

	// 	//Event handler
	// 	SDL_Event e;
    //     int m = 0;
	// 	//While application is running
	// 	while( !quit )
	// 	{
	// 		//Handle events on queue
	// 		while( SDL_PollEvent( &e ) != 0 )
	// 		{
	// 			switch (e.type) {
	// 				case SDL_QUIT: {
	// 					quit = true;
	// 					break;
	// 				}
	// 				case SDL_WINDOWEVENT: {
	// 					if (event.window.windowID == windowID)  {
	// 						switch (event.window.event)  {

	// 						case SDL_WINDOWEVENT_SIZE_CHANGED:  {
	// 							width = event.window.data1;
	// 							height = event.window.data2;
	// 							break;
	// 						}

	// 						case SDL_WINDOWEVENT_CLOSE:  {
	// 							event.type = SDL_QUIT;
	// 							SDL_PushEvent(&event);
	// 							break;
	// 						}

	// 						}
	// 					}
	// 					break;
	// 				}
	// 			}
	// 			//User requests quit
	// 			if( e.type == SDL_QUIT )
	// 			{
	// 				quit = true;
	// 			}
	// 		}

	// 		if (screen->lockTexture()) {
	// 			Uint8* pixels = (Uint8*)screen->getPixels();
	// 			int pitch = screen->getPitch();
                
	// 			int pixelCount = ( pitch ) * screen->getScreenHeight();
	// 			for (int y = 0; y<SCREEN_HEIGHT; y++) {
	// 				for (int x = 0; x<SCREEN_WIDTH; x++) {
    //                     pixels[((x + (y*SCREEN_WIDTH)) * 3) + 0] = ((x%2)*255) + ((y%2)*255);
    //                     pixels[((x + (y*SCREEN_WIDTH)) * 3) + 1] = ((x%2)*255) + ((y%2)*255);
    //                     pixels[((x + (y*SCREEN_WIDTH)) * 3) + 2] = ((x%2)*255) + ((y%2)*255);
    //                 }
	// 			}
	// 			screen->unlockTexture();
	// 		}
	// 		screen->render();
    //         m = (m+1)%255;

    //         // update();
            
    //         printf("%d\n", thing);
	// 	}
	// }
	
	delete system;
	return 0;
}
