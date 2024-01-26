/*  
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "particle.h"


class Application
{
private:
	bool red = false;
	bool green = false;
	bool mouseRel;     
	bool drawingTool = false;
	bool drawingMode;      // Flag to indicate whether drawing mode is active
	bool drawLine;         // Flag to indicate whether to draw a line
	bool drawRectangle;    // Flag to indicate whether to draw a rectangle
	bool drawCircle;       // Flag to indicate whether to draw a circle
	bool drawTriangle;     // Flag to indicate whether to draw a triangle
	bool isfilled;
	int borderWith=1;

public:

	// Window

	SDL_Window* window = nullptr;
	int window_width;
	int window_height;

	float time;

	// Input
	const Uint8* keystate;
	int mouse_state; // Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta; // Mouse movement in the last frame
	void Application::HandleKeyPress1();

	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnWheel(SDL_MouseWheelEvent event);
	void OnFileChanged(const char* filename);
	void DrawingTool(void);
	void InitButtons(void);

	// CPU Global framebuffer
	Image framebuffer;


	ParticleSystem particleSystem;

	// Constructor and main methods
	Application(const char* caption, int width, int height);
	~Application();

	void Init( void );
	void Render( void );
	void Update( float dt );

	// Other methods to control the app
	void SetWindowSize(int width, int height) {
		glViewport( 0,0, width, height );
		this->window_width = width;
		this->window_height = height;
	}

	Vector2 GetWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(float(w), float(h));
	}
};
