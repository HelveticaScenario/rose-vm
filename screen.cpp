#include "screen.hpp"

Screen::Screen()
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

}

bool Screen::init() {
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

	windowWidth = INITIAL_WINDOW_WIDTH;
	windowHeight = INITIAL_WINDOW_HEIGHT;
	//Create window
	window = SDL_CreateWindow( "Rosebud", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if( window == NULL )
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	//Create renderer for window
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if( renderer == NULL )
	{
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
		

	//Initialize renderer color
	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

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

Screen::~Screen()
{
	//Deallocate
	free();
}


void Screen::free()
{
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
	
}

void Screen::render()
{
	SDL_RenderCopy( renderer, texture, NULL, NULL);
	SDL_RenderPresent( renderer );
}


int Screen::getWindowWidth()
{
	return windowWidth;
}

int Screen::getWindowHeight()
{
	return windowHeight;
}

bool Screen::lockTexture()
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

bool Screen::unlockTexture()
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

void* Screen::getPixels()
{
	return pixels;
}

void Screen::copyPixels( void* newPixels )
{
	//Texture is locked
	if( pixels != NULL )
	{
		//Copy to locked pixels
		memcpy( pixels, newPixels, pitch * SCREEN_HEIGHT );
	}
}

int Screen::getPitch()
{
	return pitch;
}
