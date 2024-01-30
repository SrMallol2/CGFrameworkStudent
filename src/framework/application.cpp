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
	
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
	drawingMode = false;
	drawLine = false;
	drawRectangle = false;
	drawCircle = false;
	drawTriangle = false;
   

    particleSystem.Init();


    my_mesh = new Mesh();
    if (!my_mesh->LoadOBJ("meshes/lee.obj")) {
        std::cout << "Model not found" << std::endl;
    }

    my_model.Rotate(3.14, Vector3(1, 0, 0));

    my_model.Translate(1.0, 2.0, 0.0);



    my_model.Rotate(3.14, Vector3(1, 0, 0));
    my_entity.model = my_model;
    my_entity.mesh = my_mesh;

    my_camera = new Camera();
    my_camera->LookAt(Vector3(0, 0.2, 0.75), Vector3(0, 0.2, 0.0), Vector3::UP);

    my_camera->SetPerspective(60, window_width/(float)window_height,0.01,100);


}

#include <vector>

// ... (your existing code)

// Define a struct or class to represent a figure

enum FigureType {
    LINE,
    RECTANGLE,
    CIRCLE,
    TRIANGLE,
    PAINT,
    PARTICLE,

};
struct Figure {
    int type;  

};

struct Fill {
    bool fill;
};

// Maintain a list of drawn figures
std::vector<Figure> drawnFigures;

void Application::Render(void)
{
    // ...
    int x = 400;
    int y = 200;
    Vector2 p0 = { 300, 300 };
    Vector2 p1 = { 400, 200 };
    Vector2 p2 = { 600, 350 };

    //particleSystem.Render(&framebuffer);
    //framebuffer.Render();

    if (drawingMode) {
        // Iterate over the drawn figures and render each one
        for (const auto& figure : drawnFigures) {
            if (figure.type == LINE) {
                framebuffer.DrawLineDDA(x, y, x + 100, y + 100, Color::WHITE);
            }
            else if (figure.type == RECTANGLE) {
                framebuffer.DrawRect(x, y, 100, 200, Color::YELLOW, borderWith, isfilled, Color::GREEN);
            }
            else if (figure.type == CIRCLE) {
                framebuffer.DrawCircle(x, y, 100, Color::YELLOW, borderWith, isfilled, Color::CYAN);
            }
            else if (figure.type == TRIANGLE) {
                framebuffer.DrawTriangle(p0, p1, p2, Color::YELLOW, isfilled, Color::GREEN);
            }
            else if (figure.type == PAINT) {
                DrawingTool();
            }
            else if (figure.type == PARTICLE) {
                particleSystem.Render(&framebuffer);
            }
        }
        if (mousePre && green) {

           framebuffer.SetPixel(mouse_position.x, mouse_position.y, Color::GREEN);
        }
        framebuffer.Render();


    }
}

// Called after render
void Application::Update(float seconds_elapsed)
{
    // ...
    particleSystem.Update(0.01f);
}

// Keyboard press event
void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode

    switch (event.keysym.sym) {
    case SDLK_ESCAPE:
        exit(0);
        break; // ESC key, kill the app

    case SDLK_1:
        drawingMode = true;
        Figure lineFigure;
        lineFigure.type = LINE;
        drawnFigures.push_back(lineFigure);
        break;

    case SDLK_2:
        drawingMode = true;
        Figure rectangleFigure;
        rectangleFigure.type = RECTANGLE;
        drawnFigures.push_back(rectangleFigure);
        break;

    case SDLK_3:
        drawingMode = true;
        Figure circleFigure;
        circleFigure.type = CIRCLE;
        drawnFigures.push_back(circleFigure);
        break;

    case SDLK_4:
        drawingMode = true;
        Figure triangleFigure;
        triangleFigure.type = TRIANGLE;
        drawnFigures.push_back(triangleFigure);
        break;

    case SDLK_5:
		drawingMode = true;
        Figure paint;
        paint.type = PAINT;
        drawnFigures.push_back(paint);
        break;
        
    case SDLK_6:
        drawingMode = true;
        Figure particle;
        particle.type = PARTICLE;
        drawnFigures.push_back(particle);
        break;

    case SDLK_f:
        isfilled = true;
        break;

    case SDLK_PLUS:
        borderWith++;
        break;

    case SDLK_MINUS:
        borderWith--;
        break;
    }


    

}


void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
		
        mouseRel = true;
	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    if (event.button == SDL_BUTTON_LEFT) {

        mousePre = true;

    }
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

void Application::DrawingTool(void) {
	Image* toolbar = new Image;
	toolbar->LoadPNG("images/toolbar.png");
	for (int x = 0; x <= toolbar->width; ++x) {
		for (int y = 0; y <= toolbar->height; ++y) {
			framebuffer.SetPixelSafe(x, y, Color::GRAY);
		}
	}
	InitButtons();
}
void Application::InitButtons(void) {

	float x0 = window_width * randomValue();
	float y0 = window_height * randomValue();
	float x1 = window_width * randomValue();
	float y1 = window_height * randomValue();
	Button *toolbar [10];

	toolbar[0] = new Button(4, 10);
	toolbar[0]->SetImage("images/red.png");
	framebuffer.DrawImage(*(toolbar[0]->image), toolbar[0]->x0, toolbar[0]->y0, false);

	toolbar[1] = new Button(toolbar[0]->x0 + 50,10 );
	toolbar[1]->SetImage("images/blue.png");
	framebuffer.DrawImage(*(toolbar[1]->image), toolbar[1]->x0, toolbar[1]->y0, false);

	toolbar[2] = new Button(toolbar[1]->x0 + 50,10 );
	toolbar[2]->SetImage("images/cyan.png");
	framebuffer.DrawImage(*(toolbar[2]->image), toolbar[2]->x0, toolbar[2]->y0, false);

	toolbar[3] = new Button(toolbar[2]->x0 + 50,10 );
	toolbar[3]->SetImage("images/green.png");
	framebuffer.DrawImage(*(toolbar[3]->image), toolbar[3]->x0, toolbar[3]->y0, false);

	toolbar[4] = new Button(toolbar[3]->x0 + 50, 10);
	toolbar[4]->SetImage("images/pink.png");
	framebuffer.DrawImage(*(toolbar[4]->image), toolbar[4]->x0, toolbar[4]->y0, false);

	toolbar[5] = new Button(toolbar[4]->x0 + 50,10 );
	toolbar[5]->SetImage("images/black.png");
	framebuffer.DrawImage(*(toolbar[5]->image), toolbar[5]->x0, toolbar[5]->y0, false);

	toolbar[6] = new Button(toolbar[5]->x0 + 50,10);
	toolbar[6]->SetImage("images/line.png");
	framebuffer.DrawImage(*(toolbar[6]->image), toolbar[6]->x0, toolbar[6]->y0, false);

	toolbar[7] = new Button(toolbar[6]->x0 + 50,10 );
	toolbar[7]->SetImage("images/rectangle.png");
	framebuffer.DrawImage(*(toolbar[7]->image), toolbar[7]->x0, toolbar[7]->y0, false);

	toolbar[8] = new Button(toolbar[7]->x0 + 50,10 );
	toolbar[8]->SetImage("images/circle.png");
	framebuffer.DrawImage(*(toolbar[8]->image), toolbar[8]->x0, toolbar[8]->y0, false);

	toolbar[9] = new Button(toolbar[8]->x0 + 50, 10);
	toolbar[9]->SetImage("images/eraser.png");
	framebuffer.DrawImage(*(toolbar[9]->image), toolbar[9]->x0, toolbar[9]->y0, false);


	if (toolbar[3]->IsMouseInside(mouse_position) && mouseRel==true) {

        green = true;
      

	}
	
	
	}


