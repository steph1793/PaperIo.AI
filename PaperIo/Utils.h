#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>

#define _SDL_TTF_H


using namespace std;
	class Utils
	{
	public:
		Utils();
		~Utils();
	};

	//Texture wrapper class
	class LTexture
	{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile(std::string path);

#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor(Uint8 red, Uint8 green, Uint8 blue);

		//Set blending
		void setBlendMode(SDL_BlendMode blending);

		//Set alpha modulation
		void setAlpha(Uint8 alpha);

		//Renders texture at given point
		void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
	};

	//The application time based timer
	class LTimer
	{
	public:
		//Initializes variables
		LTimer();

		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();

		//Gets the timer's time
		Uint32 getTicks();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

	private:
		//The clock time when the timer started
		Uint32 mStartTicks;

		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;

		//The timer status
		bool mPaused;
		bool mStarted;
	};




	extern const int SCREEN_WIDTH ;
	extern const int SCREEN_HEIGHT ;
	extern const char* windowTitle ;

	extern const int SCREEN_FPS ;
	extern const int SCREEN_TICKS_PER_FRAME;

	extern SDL_Window* window ;
	extern SDL_Surface* screenSurface ;
	extern SDL_Renderer* Renderer;
	extern TTF_Font *gFont;
	extern LTexture gFPSTextTexture;
	extern LTexture gDotTexture;

