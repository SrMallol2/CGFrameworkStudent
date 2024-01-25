#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 



Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->mouse_state = 0;
	this->time = 0.f;
	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(nullptr);

	this->framebuffer.Resize(w, h);


}

Application::~Application()
{
	drawingMode = false;
	drawLine = false;
	drawRectangle = false;
	drawCircle = false;
	drawTriangle = false;
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
	std::cout << "Hola Albertito" << std::endl;
	std::cout << "Hola Markitos" << std::endl;
	std::cout << "Hola" << std::endl;
	drawingMode = false;
	drawLine = false;
	drawRectangle = false;
	drawCircle = false;
	drawTriangle = false;
}

// Render one frame
void Application::Render(void)
{
	// ...
	int x = 400;
	int y = 200;		
	Vector2 p0 = { 100, 100 };
	Vector2 p1 = { 300, 100 };
	Vector2 p2 = { 200, 300 };
	framebuffer.Fill(Color::BLACK);
	//framebuffer.DrawLineDDA(x, y, x + 100 * cos(time), y + 100 * sin(time), Color ::WHITE);
	//framebuffer.DrawRect(x, y, 100, 200, Color::YELLOW, 5, true, Color::GREEN);
	//framebuffer.DrawCircle(x, y,100, Color::YELLOW,10,true, Color::CYAN);
	framebuffer.DrawTriangle(p0, p1,p2, Color::YELLOW,true,Color::GREEN);


	framebuffer.Render();


}

// Called after render
void Application::Update(float seconds_elapsed)
{

}

//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode

	switch(event.keysym.sym) {

		case SDLK_ESCAPE: 
			exit(0);
			break; // ESC key, kill the app

		case SDLK_1:
			drawingMode = true;
			drawLine = true;
			break;

		case SDLK_2:
			drawingMode = true;
			drawRectangle = true;
			break;

		case SDLK_3:
			drawingMode = true;
			drawCircle = true;
			break;

		case SDLK_4:
			drawingMode = true;
			drawTriangle = true;
			break;
	}
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;

	// ...
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}