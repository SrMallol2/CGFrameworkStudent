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
    /*
	drawingMode = false;
	drawLine = false;
	drawRectangle = false;
	drawCircle = false;
	drawTriangle = false;
   */

    //particleSystem.Init();

    
    
    my_mesh = new Mesh();
    if (!my_mesh->LoadOBJ("meshes/lee.obj")) {
        std::cout << "Model not found" << std::endl;
    }

    my_mesh2 = new Mesh();
    if (!my_mesh2->LoadOBJ("meshes/anna.obj")) {
        std::cout << "Model not found" << std::endl;
    }

    my_mesh3 = new Mesh();
    if (!my_mesh3->LoadOBJ("meshes/cleo.obj")) {
        std::cout << "Model not found" << std::endl;
    }

    
    zBuffer = FloatImage(framebuffer.width,framebuffer.height);
    zBuffer.Fill(10000);

    //my_model.Rotate(3.14, Vector3(1, 0, 0));

    //my_model.Translate(1.0, 2.0, 0.0);

    my_entity2.model.Translate(0.5, 0.0, 0.0);

    

    my_entity3.model.Translate(-0.5, 0.0, 0.0);



    //my_model.Rotate(3.14, Vector3(1, 0, 0));
   // my_entity.model = my_model;

    my_entity.mesh = my_mesh;

    my_entity2.mesh = my_mesh2;

    my_entity3.mesh = my_mesh3;

    //my_camera->LookAt(Vector3(0, 0.2, 0.75), Vector3(0, 0.2, 0.0), Vector3::UP);
    float aspect = window_width / (float)window_height;
    float near_plane = 0.01f;
    float far_plane = 2.0f;
    my_camera = new Camera();

   

    my_camera->LookAt(Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));
    
    my_camera->SetOrthographic(-1, 1, 1, -1, near_plane, far_plane);
    my_camera->SetPerspective(45, aspect, near_plane, far_plane);
  
    
    

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
    /*
    int x = 400;
    int y = 200;
    Vector2 p0 = { 300, 300 };
    Vector2 p1 = { 400, 200 };
    Vector2 p2 = { 600, 350 };

    //particleSystem.Render(&framebuffer);
    //framebuffer.Render();
    */

    zBuffer.Fill(10000);

    if (drawEntity) {
        my_entity.Render(&framebuffer, my_camera, Color::RED, Color::GREEN, Color::BLUE, &zBuffer);
    }
    else{
        my_entity.Render(&framebuffer, my_camera, Color::BLUE, Color::RED, Color::YELLOW, &zBuffer);

        my_entity2.Render(&framebuffer, my_camera, Color::RED, Color::YELLOW, Color::BLUE, &zBuffer);

        my_entity3.Render(&framebuffer, my_camera, Color::YELLOW, Color::RED, Color::BLUE, &zBuffer);
    }
    framebuffer.Render();

    

    /*
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
    */
}


// Called after render
void Application::Update(float seconds_elapsed)
{
    // ...
    if (drawEntity) {
        my_entity.Render(&framebuffer, my_camera, Color::RED, Color::GREEN, Color::BLUE, &zBuffer);

        framebuffer.Fill(Color(0, 0, 0));



        // Present the framebuffer
        framebuffer.Render();
    }

    else{
        my_entity.Update(seconds_elapsed, Entity::ROTATE);

        my_entity2.Update(seconds_elapsed, Entity::TRANSLATE);

        my_entity3.Update(seconds_elapsed, Entity::SCALE);

        // Clear framebuffer
        framebuffer.Fill(Color(0, 0, 0));

        

        // Present the framebuffer
        framebuffer.Render();
    }

    
}

// Keyboard press event
void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode

    switch (event.keysym.sym) {
    if (lab1){
    case SDLK_ESCAPE:
        exit(0);
        break; // ESC key, kill the app

    case SDLK_1:
        if (lab1) {
            drawingMode = true;
            Figure lineFigure;
            lineFigure.type = LINE;
            drawnFigures.push_back(lineFigure);
            break;
        }
        else if (lab2) {
            
            drawEntity = true;
        
            break;

        }

    case SDLK_2:
        if (lab1) {
            drawingMode = true;
            Figure rectangleFigure;
            rectangleFigure.type = RECTANGLE;
            drawnFigures.push_back(rectangleFigure);
            break;
        }
        else if (lab2) {
           
                drawEntity = false;
           
            break;

        }

    case SDLK_3:
        if (lab1) {
            drawingMode = true;
            Figure circleFigure;
            circleFigure.type = CIRCLE;
            drawnFigures.push_back(circleFigure);
            break;
        }
        else if (lab2) {

            
        }

    case SDLK_4:
        if (lab1) {
            drawingMode = true;
            Figure triangleFigure;
            triangleFigure.type = TRIANGLE;
            drawnFigures.push_back(triangleFigure);
            break;
        }
        else if (lab2) {

        }

    case SDLK_5:
        if (lab1) {
            drawingMode = true;
            Figure paint;
            paint.type = PAINT;
            drawnFigures.push_back(paint);
            break;
        }
        else if (lab2) {

        }

    case SDLK_6:
        if (lab1) {
            drawingMode = true;
            Figure particle;
            particle.type = PARTICLE;
            drawnFigures.push_back(particle);
            break;
        }
        else if (lab2) {

        }

    case SDLK_f:
        if (lab1) {
            isfilled = true;
            break;
        }
        else if (lab2) {
            currentProperty = "far";
            break;
        }
            
    case SDLK_PLUS:
        if (lab1) {
            borderWith++;
            break;
        }
        else if (lab2) {

            if (strcmp(currentProperty, "fov") == 0) {
                my_camera->fov += 5.0f;
                my_camera->SetPerspective(my_camera->fov, my_camera->aspect,
                my_camera->near_plane, my_camera->far_plane);
            }
            else if (strcmp(currentProperty, "near") == 0) {
                my_camera->near_plane += 0.01f;
                my_camera->UpdateProjectionMatrix();
   
            }
            else if (strcmp(currentProperty, "far") == 0) {
                my_camera->far_plane += 0.01f;
                my_camera->UpdateProjectionMatrix();
            }
            break;
        
        }

    case SDLK_MINUS:
        if (lab1) {
            borderWith--;
            break;
        }
        else if (lab2) {

            if (strcmp(currentProperty, "fov") == 0) {
                my_camera->fov -= 5.0f;
                my_camera->UpdateProjectionMatrix();
            }
            else if (strcmp(currentProperty, "near") == 0) {
                my_camera->near_plane -= 0.01f;
                my_camera->UpdateProjectionMatrix();

            }
            else if (strcmp(currentProperty, "far") == 0) {
                my_camera->far_plane -= 0.01f;
                my_camera->UpdateProjectionMatrix();
            }
            break;
         
        }

    case SDLK_o:
        if (lab1) {

        }
        else if (lab2) {
            my_camera->SetOrthographic(my_camera->left, my_camera->right,
            my_camera->top, my_camera->bottom, my_camera->near_plane, my_camera->far_plane);
            break;
            
        }

    case SDLK_p:
        if (lab1) {

        }
        else if (lab2) {
            my_camera->SetPerspective(my_camera->fov, my_camera->aspect,
            my_camera->near_plane, my_camera->far_plane);
            break;
        }

    case SDLK_n:
        if (lab1) {

        }
        else if (lab2) {
            currentProperty = "near";
            break;
        }
    case SDLK_v:
        if (lab1) {

        }
        else if (lab2) {
            currentProperty = "fov";
                break;
        }

    }


    }
}




void Application::OnMouseButtonDown(SDL_MouseButtonEvent event) {
    if (event.button == SDL_BUTTON_LEFT) {
        // Set the flag to indicate that the left mouse button is pressed
        leftMouseButtonPressed = true;
        prevMouseX = event.x;
        prevMouseY = event.y;
    }
    if (event.button == SDL_BUTTON_RIGHT) {
        // Reset the flag when the left mouse button is released
        rightMouseButtonPressed = true;
        prevMouseX = event.x;
        prevMouseY = event.y;
    }
}

void Application::OnMouseButtonUp(SDL_MouseButtonEvent event) {
    if (event.button == SDL_BUTTON_LEFT) {
        // Reset the flag when the left mouse button is released
        leftMouseButtonPressed = false;
    }

    if (event.button == SDL_BUTTON_RIGHT) {
        // Reset the flag when the left mouse button is released
        rightMouseButtonPressed = false;
    }


}

void Application::OnMouseMove(SDL_MouseButtonEvent event) {
    if (leftMouseButtonPressed) {
        float mouseSensivity = 0.01f;
        my_camera->Orbit(-mouse_delta.x *mouseSensivity , Vector3::UP);
        my_camera->Orbit(-mouse_delta.y *mouseSensivity  , Vector3::RIGHT);
    }

    if (rightMouseButtonPressed) {
        // Calculate the change in mouse position
        int deltaX = event.x - prevMouseX;
        int deltaY = event.y - prevMouseY;

        // Adjust the sensitivity for each axis if needed
        float sensitivityXY = 0.01f;  // Adjust as needed for X and Y axes
        float sensitivityZ = 0.1f;     // Adjust as needed for Z axis

        // Update the camera's center position
        my_camera->MoveCenter(deltaX * sensitivityXY, deltaY * sensitivityXY, 0.0f /* No movement along Z axis */);

        // Update previous mouse position for the next frame
        prevMouseX = event.x;
        prevMouseY = event.y;
    }
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;

	// ...
    float zoomFactor = 0.1f; // Adjust as needed

    // Determine the direction of zoom based on the mouse wheel movement
    if (event.y > 0) { // Positive wheel movement (upwards)
        my_camera->Zoom(zoomFactor);
    }
    else if (event.y < 0) { // Negative wheel movement (downwards)
        my_camera->Zoom(-zoomFactor);
    }
    
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}



//LAB 1 CODE

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


	//if (toolbar[3]->IsMouseInside(mouse_position) && mouseRel==true) {

        //green = true;}
      

	
	
	}


