#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"

Application::Application(const char *caption, int width, int height)
{
    this->window = createWindow(caption, width, height);

    int w, h;
    SDL_GetWindowSize(window, &w, &h);

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


    myQuad.CreateQuad();
    myQuadShader = Shader::Get("shaders/raster.vs", "shaders/raster.fs");
    myGouraudShader= Shader::Get("shaders/gouraud.vs","shaders/gouraud.fs");
    myPhongShader= Shader::Get("shaders/phong.vs","shaders/phong.fs");


    my_mesh = new Mesh();
    if (!my_mesh->LoadOBJ("meshes/lee.obj"))
    {
        std::cout << "Model not found" << std::endl;
    }


    my_entity1.mesh = my_mesh;

    my_material = new Material();

 
    my_texture = new Texture();
    my_texture->Load("textures/lee_color_specular.tga");

    my_normal_texture = new Texture();
    my_normal_texture->Load("textures/lee_normal.tga");

    my_material->color_texture = my_texture;

    my_material->normal_texture = my_normal_texture;

    my_material->Ka = Vector3(1.0);
    my_material->Kd = Vector3(1.0);
    my_material->Ks = Vector3(0.5);

    my_material->shininess = 100.0;

    zBuffer = FloatImage(framebuffer.width, framebuffer.height);
    zBuffer.Fill(10000);
    
    float aspect = window_width / (float)window_height;
    float near_plane = 0.01f;
    float far_plane = 2.0f;

    my_camera = new Camera();

    my_camera->LookAt(Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

    my_camera->SetOrthographic(-1, 1, 1, -1, near_plane, far_plane);
    my_camera->SetPerspective(45, aspect, near_plane, far_plane);

    uniformData.viewProjectionMatrix = my_camera->viewprojection_matrix;

    uniformData.flag = Vector3(0.0,0.0,0.0);

    //Initialization of  rendering with just one light
    uniformData.numOfLights = 1;

    //LIGHTS DEFINITION

    sLight light1;
    sLight light2;
    sLight light3;
    sLight light4;

    light1.Id = Vector3(0,1,0);
    light1.Is = Vector3(0,1,0);
    light1.position = Vector3(2.0,1.0,1.5);

    light2.Id = Vector3(1,0,0);
    light2.Is = Vector3(1,0,0);
    light2.position = Vector3(-2.0,1.0,1.5);

    light3.Id = Vector3(1,0,0);
    light3.Is = Vector3(1,0,0);
    light3.position = Vector3(2.0,1.0,-1.5);

    light4.Id = Vector3(0,0,1);
    light4.Is = Vector3(0,0,1);
    light4.position = Vector3(-2.0,1.0,-1.5);

    uniformData.scenelights[0] = light1;
    uniformData.scenelights[1] = light2;
    uniformData.scenelights[2] = light3;
    uniformData.scenelights[3] = light4;

    uniformData.Ia = Vector3(1.0,1.0,1.0);


    my_entity1.material = my_material;
    
}

#include <vector>

// Define a struct or class to represent a figure


void Application::Render(void)
{

    uniformData.cameraPosition= my_camera->eye;
    uniformData.viewProjectionMatrix = my_camera->viewprojection_matrix;
    
    
    if(currentTask == 1){
        my_material->shader = myGouraudShader;
    }
    else{
        my_material->shader = myPhongShader;
    }
    my_entity1.Render(uniformData);

    
    
}

// Called after render
void Application::Update(float seconds_elapsed)
{
    // ...

    // if (drawEntity) {

    my_entity1.Render(&framebuffer, my_camera, &zBuffer);

    framebuffer.Fill(Color(0, 0, 0));

    // Present the framebuffer
    framebuffer.Render();
    /*
}

else{
    my_entity1.Update(seconds_elapsed, Entity::ROTATE);

    my_entity2.Update(seconds_elapsed, Entity::TRANSLATE);

    my_entity3.Update(seconds_elapsed, Entity::SCALE);

    // Clear framebuffer
    framebuffer.Fill(Color(0, 0, 0));



    // Present the framebuffer
    framebuffer.Render();
}
*/
}

// Keyboard press event
void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode

    switch (event.keysym.sym)
    {
    case SDLK_ESCAPE:
        exit(0);
        break; // ESC key, kill the app

    case SDLK_1:
        
        if (lab2)
        {
            drawEntity = true;

            break;
        }
        else if(lab4){
            currentShader =1;
            currentSubTask =1;
            break;
        }
        else if(lab5){
            uniformData.numOfLights = 1;
            break;
        }

    case SDLK_2:
       
        if (lab2)
        {

            drawEntity = false;

            break;
        }
        else if(lab4){
            currentShader = 2;
            currentSubTask = 0;
            break;
        }
        else if(lab5){
            uniformData.numOfLights = 2;
            break;
        }

    case SDLK_3:
        
         if (lab4)
        {
            currentShader=3;
            currentSubTask=0;
            break;
        }
        else if(lab5){
            uniformData.numOfLights = 3;
            break;
        }


    case SDLK_4:
       
         if (lab4)
        {
            currentShader=4;
            break;
        }
        else if(lab5){
            uniformData.numOfLights = 4;
            break;
        }

    case SDLK_f:
        
        if (lab2)
        {
            currentProperty = "far";
            break;
        }
        else if (lab4)
        {
            currentSubTask = 6;
            break;
        }

    case SDLK_PLUS:
       
         if (lab2)
        {

            if (strcmp(currentProperty, "fov") == 0)
            {
                my_camera->fov += 5.0f;
                my_camera->SetPerspective(my_camera->fov, my_camera->aspect,
                                          my_camera->near_plane, my_camera->far_plane);
            }
            else if (strcmp(currentProperty, "near") == 0)
            {
                my_camera->near_plane += 0.01f;
                my_camera->UpdateProjectionMatrix();
            }
            else if (strcmp(currentProperty, "far") == 0)
            {
                my_camera->far_plane += 0.01f;
                my_camera->UpdateProjectionMatrix();
            }
            break;
        }

    case SDLK_MINUS:
    
         if (lab2)
        {

            if (strcmp(currentProperty, "fov") == 0)
            {
                my_camera->fov -= 5.0f;
                my_camera->UpdateProjectionMatrix();
            }
            else if (strcmp(currentProperty, "near") == 0)
            {
                my_camera->near_plane -= 0.01f;
                my_camera->UpdateProjectionMatrix();
            }
            else if (strcmp(currentProperty, "far") == 0)
            {
                my_camera->far_plane -= 0.01f;
                my_camera->UpdateProjectionMatrix();
            }
            break;
        }

    case SDLK_o:
         if (lab2)
        {
            my_camera->SetOrthographic(my_camera->left, my_camera->right,
                                       my_camera->top, my_camera->bottom, my_camera->near_plane, my_camera->far_plane);
            break;
        }

    case SDLK_p:
        if (lab2)
        {
            my_camera->SetPerspective(my_camera->fov, my_camera->aspect,
                                      my_camera->near_plane, my_camera->far_plane);
            break;
        }
        else if(lab5){
            currentTask=2;
            break;
        }

    case SDLK_n:
        if (lab2)
        {
            currentProperty = "near";
            break;
        }
        else if (lab5){
            if(currentTask==2){
                if(uniformData.flag.z == 1.0){
                    uniformData.flag.z=0.0;
                    break;
                }
                else{
                    uniformData.flag.z = 1.0;
                    break;
                }

                
            }
            break;
        
        }
    case SDLK_v:
        if (lab2)
        {
            currentProperty = "fov";
            break;
        }

    case SDLK_c:

        if (lab3)
        {

            if (my_entity1.mode != Entity::eRenderMode::PLAIN_COLOR)
            {
                my_entity1.SetRenderMode(Entity::eRenderMode::PLAIN_COLOR);
                break;
            }
            else
            {
                my_entity1.SetRenderMode(Entity::eRenderMode::TRIANGLES_INTERPOLATED);
                break;
            }
        
        }
        else if (lab4)
        {
            currentSubTask = 3;
            break;
        }
        else if(lab5){
            if(currentTask==2){
                if(uniformData.flag.x == 1.0){
                    uniformData.flag.x=0.0;
                    break;
                }
                else{
                    uniformData.flag.x = 1.0;
                    break;
                }

               
            }
            break;
        }

    case SDLK_t:
        if (my_entity1.mode != Entity::eRenderMode::TEXTURES)
        {
            my_entity1.SetRenderMode(Entity::eRenderMode::TEXTURES);
            break;
        }
        else
        {
            my_entity1.SetRenderMode(Entity::eRenderMode::PLAIN_COLOR);
            break;
        }

    case SDLK_z:
        if (my_entity1.mode != Entity::eRenderMode::OCCLUSIONS)
        {
            my_entity1.SetRenderMode(Entity::eRenderMode::OCCLUSIONS);
            break;
        }
        else
        {
            my_entity1.SetRenderMode(Entity::eRenderMode::TRIANGLES_INTERPOLATED);
            break;
        }

    case SDLK_a:
        if (lab4)
        {
            currentSubTask = 1;
            break;
        }

    case SDLK_b:
        if (lab4)
        {
            currentSubTask = 2;
            break;
        }
    case SDLK_d:
        if (lab4)
        {
            currentSubTask = 4;
            break;
        }

    case SDLK_e:
        if (lab4)
        {
            currentSubTask = 5;
            break;
        }
    case SDLK_s:
        {
            if(currentTask==2){
                if(uniformData.flag.y == 1.0){
                    uniformData.flag.y=0.0;
                    break;
                }
                else{
                    uniformData.flag.y = 1.0;
                    break;
                }

            
            }
            break;

        }
    case SDLK_g:
        currentTask = 1;
        break;
    }
}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
    if (event.button == SDL_BUTTON_LEFT)
    {
        // Set the flag to indicate that the left mouse button is pressed
        leftMouseButtonPressed = true;
        prevMouseX = event.x;
        prevMouseY = event.y;
    }
    if (event.button == SDL_BUTTON_RIGHT)
    {
        // Reset the flag when the left mouse button is released
        rightMouseButtonPressed = true;
        prevMouseX = event.x;
        prevMouseY = event.y;
    }
}

void Application::OnMouseButtonUp(SDL_MouseButtonEvent event)
{
    if (event.button == SDL_BUTTON_LEFT)
    {
        // Reset the flag when the left mouse button is released
        leftMouseButtonPressed = false;
    }

    if (event.button == SDL_BUTTON_RIGHT)
    {
        // Reset the flag when the left mouse button is released
        rightMouseButtonPressed = false;
    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    if (leftMouseButtonPressed)
    {
     
        float mouseSensivity = 0.01f;
        my_camera->Orbit(-mouse_delta.x * mouseSensivity, Vector3::UP);
        my_camera->Orbit(-mouse_delta.y * mouseSensivity, Vector3::RIGHT);
    }

    if (rightMouseButtonPressed)
    {
        // Calculate the change in mouse position
        int deltaX = event.x - prevMouseX;
        int deltaY = event.y - prevMouseY;

        // Adjust the sensitivity for each axis if needed
        float sensitivityXY = 0.01f; // Adjust as needed for X and Y axes
        float sensitivityZ = 0.1f;   // Adjust as needed for Z axis

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
    if (event.y > 0)
    { // Positive wheel movement (upwards)
        my_camera->Zoom(zoomFactor);
    }
    else if (event.y < 0)
    { // Negative wheel movement (downwards)
        my_camera->Zoom(-zoomFactor);
    }
}

void Application::OnFileChanged(const char *filename)
{
    Shader::ReloadSingleShader(filename);
}

// THIS IS CODE FROM LAB1:

void Application::DrawingTool(void)
{
    Image *toolbar = new Image;
    toolbar->LoadPNG("images/toolbar.png");
    for (int x = 0; x <= toolbar->width; ++x)
    {
        for (int y = 0; y <= toolbar->height; ++y)
        {
            framebuffer.SetPixelSafe(x, y, Color::GRAY);
        }
    }
    InitButtons();
}

void Application::InitButtons(void)
{

    float x0 = window_width * randomValue();
    float y0 = window_height * randomValue();
    float x1 = window_width * randomValue();
    float y1 = window_height * randomValue();
    Button *toolbar[10];

    toolbar[0] = new Button(4, 10);
    toolbar[0]->SetImage("images/red.png");
    framebuffer.DrawImage(*(toolbar[0]->image), toolbar[0]->x0, toolbar[0]->y0, false);

    toolbar[1] = new Button(toolbar[0]->x0 + 50, 10);
    toolbar[1]->SetImage("images/blue.png");
    framebuffer.DrawImage(*(toolbar[1]->image), toolbar[1]->x0, toolbar[1]->y0, false);

    toolbar[2] = new Button(toolbar[1]->x0 + 50, 10);
    toolbar[2]->SetImage("images/cyan.png");
    framebuffer.DrawImage(*(toolbar[2]->image), toolbar[2]->x0, toolbar[2]->y0, false);

    toolbar[3] = new Button(toolbar[2]->x0 + 50, 10);
    toolbar[3]->SetImage("images/green.png");
    framebuffer.DrawImage(*(toolbar[3]->image), toolbar[3]->x0, toolbar[3]->y0, false);

    toolbar[4] = new Button(toolbar[3]->x0 + 50, 10);
    toolbar[4]->SetImage("images/pink.png");
    framebuffer.DrawImage(*(toolbar[4]->image), toolbar[4]->x0, toolbar[4]->y0, false);

    toolbar[5] = new Button(toolbar[4]->x0 + 50, 10);
    toolbar[5]->SetImage("images/black.png");
    framebuffer.DrawImage(*(toolbar[5]->image), toolbar[5]->x0, toolbar[5]->y0, false);

    toolbar[6] = new Button(toolbar[5]->x0 + 50, 10);
    toolbar[6]->SetImage("images/line.png");
    framebuffer.DrawImage(*(toolbar[6]->image), toolbar[6]->x0, toolbar[6]->y0, false);

    toolbar[7] = new Button(toolbar[6]->x0 + 50, 10);
    toolbar[7]->SetImage("images/rectangle.png");
    framebuffer.DrawImage(*(toolbar[7]->image), toolbar[7]->x0, toolbar[7]->y0, false);

    toolbar[8] = new Button(toolbar[7]->x0 + 50, 10);
    toolbar[8]->SetImage("images/circle.png");
    framebuffer.DrawImage(*(toolbar[8]->image), toolbar[8]->x0, toolbar[8]->y0, false);

    toolbar[9] = new Button(toolbar[8]->x0 + 50, 10);
    toolbar[9]->SetImage("images/eraser.png");
    framebuffer.DrawImage(*(toolbar[9]->image), toolbar[9]->x0, toolbar[9]->y0, false);

    // if (toolbar[3]->IsMouseInside(mouse_position) && mouseRel==true) {

    // green = true;}
}


