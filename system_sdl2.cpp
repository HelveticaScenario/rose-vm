#include "system_sdl2.hpp"

SystemSdl2::SystemSdl2()
{

	//Initialize
	window = NULL;
	renderer = NULL;
	texture = NULL;
	cartridge = NULL;
	editor = NULL;
	runtime = NULL;
	screenMode = RuntimeMode;
	pixels = NULL;
	pitch = 0;
	windowWidth = 0;
	windowHeight = 0;
	widthMult = 0;
	heightMult = 0;

}

bool SystemSdl2::init(int argc, char *argv[]) {
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	//Set texture filtering to linear
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" ) )
	{
		printf( "Warning: Nearest Neighbor texture filtering not enabled!" );
	}

	//Seed random
	srand( SDL_GetTicks() );
	
	//Create window
	window = SDL_CreateWindow( "Rosebud", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	if( window == NULL )
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	SDL_GL_GetDrawableSize(this->window, &windowWidth, &windowHeight);
	widthMult = (windowWidth * 2) / SCREEN_WIDTH;
	heightMult = (windowHeight * 2) / SCREEN_HEIGHT;

	//Create renderer for window
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if( renderer == NULL )
	{
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
		

	//Initialize renderer color
	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0xFF );

	//Create uninitialized texture
	texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT );
	if( texture == NULL )
	{
		printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	cartridge = new Cartridge();
	runtime = new Runtime(cartridge);
	editor = new Editor(cartridge);

	return true;
}

void SystemSdl2::run() {
	Uint32 windowID = SDL_GetWindowID(this->window);

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event event;
	//While application is running
	while( !quit )
	{
		//Handle events on queue
		while( SDL_PollEvent( &event ) != 0 )
		{
			switch (event.type) {
				case SDL_QUIT: {
					quit = true;
					break;
				}
				case SDL_WINDOWEVENT: {
					if (event.window.windowID == windowID)  {
						switch (event.window.event)  {

						case SDL_WINDOWEVENT_SIZE_CHANGED:  {
							// windowWidth = event.window.data1;
							// windowHeight = event.window.data2;
							SDL_GL_GetDrawableSize(this->window, &windowWidth, &windowHeight);
							widthMult = (windowWidth * 2 ) / SCREEN_WIDTH;
							heightMult = (windowHeight * 2) / SCREEN_HEIGHT;
							break;
						}

						case SDL_WINDOWEVENT_CLOSE:  {
							event.type = SDL_QUIT;
							SDL_PushEvent(&event);
							break;
						}

						}
					}
					break;
				}
			}
			//User requests quit
			if( event.type == SDL_QUIT )
			{
				quit = true;
			}
		}

		if (this->lockTexture()) {
			Uint8* pixels = (Uint8*)this->getPixels();
			int pitch = this->getPitch();
			
			int pixelCount = ( pitch ) * SCREEN_HEIGHT;
			for (int y = 0; y<SCREEN_HEIGHT; y++) {
				for (int x = 0; x<SCREEN_WIDTH; x++) {
					pixels[((x + (y*SCREEN_WIDTH)) * 3) + 0] = ((x%2)*255) + ((y%2)*255);
					pixels[((x + (y*SCREEN_WIDTH)) * 3) + 1] = ((x%2)*255) + ((y%2)*255);
					pixels[((x + (y*SCREEN_WIDTH)) * 3) + 2] = ((x%2)*255) + ((y%2)*255);
				}
			}
			this->unlockTexture();
		}
		this->render();

	}

}

SystemSdl2::~SystemSdl2()
{
	//Deallocate

	//Free texture if it exists
	if( texture != NULL )
	{
		SDL_DestroyTexture( texture );
		texture = NULL;
		pixels = NULL;
		pitch = 0;
	}

	if( renderer != NULL ) {
		SDL_DestroyRenderer( renderer );
		renderer = NULL;
	}

	if( window != NULL ) {
		SDL_DestroyWindow( window );
		window = NULL;
	}
	SDL_Quit();
}

void SystemSdl2::render()
{
	SDL_RenderClear(renderer);
	unsigned int mult = widthMult < heightMult ? widthMult : heightMult;
	
	if (mult < 30) {
		mult -= (mult % 2);
	}

	if (mult < 2) {
		mult = 2;
	}
	
	SDL_Rect rect;
	
	rect.w = (SCREEN_WIDTH * mult) / 2;
	rect.h = (SCREEN_HEIGHT * mult) / 2;
	rect.x = (windowWidth - rect.w) / 2;
	rect.y = (windowHeight - rect.h) / 2;
	SDL_RenderCopy( renderer, texture, NULL, &rect);
	SDL_RenderPresent( renderer );
}


int SystemSdl2::getWindowWidth()
{
	return windowWidth;
}

int SystemSdl2::getWindowHeight()
{
	return windowHeight;
}

bool SystemSdl2::lockTexture()
{
	//Texture is already locked
	if( pixels != NULL )
	{
		printf( "Texture is already locked!\n" );
		return false;
	}

	if( SDL_LockTexture( texture, NULL, &pixels, &pitch ) != 0 )
	{
		printf( "Unable to lock texture! %s\n", SDL_GetError() );
		return false;
	}

	return true;
}

bool SystemSdl2::unlockTexture()
{
	//Texture is not locked
	if( pixels == NULL )
	{
		printf( "Texture is not locked!\n" );
		return false;
	}

	SDL_UnlockTexture( texture );
	pixels = NULL;
	pitch = 0;

	return true;
}

void* SystemSdl2::getPixels()
{
	return pixels;
}

void SystemSdl2::copyPixels( void* newPixels )
{
	//Texture is locked
	if( pixels != NULL )
	{
		//Copy to locked pixels
		memcpy( pixels, newPixels, pitch * SCREEN_HEIGHT );
	}
}

int SystemSdl2::getPitch()
{
	return pitch;
}
