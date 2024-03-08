#include "entity.h"


#include <iostream>



void Entity::Render(Image* framebuffer, Camera* camera, FloatImage * zBuffer ) {
    const auto& vertices = mesh->GetVertices();
    const auto& UVs = mesh->GetUVs();
    

    for (size_t i = 0; i < vertices.size(); i += 3) {
        // Transform vertices from local space to world space using the model matrix
        Vector3 screenVertices[3];
        bool insideFrustum = true;

        for (int j = 0; j < 3; ++j) {
            // Transform vertices from local space to world space using the model matrix
            Vector3 worldVertex = model * vertices[i + j];

            // Project vertex to clip space using the camera
            bool negZ;
            screenVertices[j] = camera->ProjectVector(worldVertex, negZ);

            // Check if the vertex is inside the camera frustum
            insideFrustum = insideFrustum && !negZ;

            

        }
        
        float screenWidth = static_cast<float>(framebuffer->width);

        float screenHeight = static_cast<float>(framebuffer->height);

        Vector3 screen0 = Vector3((screenVertices[0].x + 1.0f) * 0.5f * screenWidth, (1.0f + screenVertices[0].y) * 0.5f * screenHeight, screenVertices[0].z);
        Vector3 screen1 = Vector3((screenVertices[1].x + 1.0f) * 0.5f * screenWidth, (1.0f + screenVertices[1].y) * 0.5f * screenHeight, screenVertices[1].z);
        Vector3 screen2 = Vector3((screenVertices[2].x + 1.0f) * 0.5f * screenWidth, (1.0f + screenVertices[2].y) * 0.5f * screenHeight, screenVertices[2].z);

        
        Vector2 uv0 = Vector2(UVs[i].x, UVs[i].y);
        Vector2 uv1 = Vector2(UVs[i+1].x, UVs[i+1].y);
        Vector2 uv2 = Vector2(UVs[i+2].x, UVs[i+2].y);

        Image:: TriangleInfo triangleinfo;
        triangleinfo.p0 = Vector3(screen0.x, screen0.y, screen0.z);
        triangleinfo.p1 = Vector3(screen1.x, screen1.y, screen1.z);
        triangleinfo.p2 = Vector3(screen2.x, screen2.y, screen2.z);
        triangleinfo.uv0 = uv0;
        triangleinfo.uv1 = uv1;
        triangleinfo.uv2 = uv2;
        triangleinfo.c0 = Color::RED;
        triangleinfo.c1 = Color::GREEN;
        triangleinfo.c2 = Color::BLUE;
        //triangleinfo. = image;
        triangleinfo.renderMode = int(mode);


        if (insideFrustum) {
            
            // Draw the triangle using the screen space vertices and color
            
            if (mode == Entity::eRenderMode::PLAIN_COLOR) {
                Color plainColor = triangleinfo.c1;
                framebuffer->DrawTriangle(
                    Vector2(screen0.x, screen0.y),
                    Vector2(screen1.x, screen1.y),
                    Vector2(screen2.x, screen2.y),
                    plainColor, true, plainColor);
            }
             

            else {

                
                framebuffer->DrawTriangleInterpolated(triangleinfo ,zBuffer);
            }
            
            //framebuffer->DrawLineDDA(screenVertices[0].x, screenVertices[0].y, screenVertices[1].x, screenVertices[1].y, c);
            //framebuffer->DrawLineDDA(screenVertices[1].x, screenVertices[1].y, screenVertices[2].x, screenVertices[2].y, c);
            //framebuffer->DrawLineDDA(screenVertices[2].x, screenVertices[2].y, screenVertices[0].x, screenVertices[0].y, c);
        }
    }
}

/*
void Entity::Render(Camera* camera) {   

    shader->Enable();
    glEnable( GL_DEPTH_TEST ); 

    // Z will pass if the Z is LESS or EQUAL to the Z of the pixel
    glDepthFunc(GL_LEQUAL); 

    
    shader->SetMatrix44("u_model", model);
    shader->SetMatrix44("u_viewprojection", camera->viewprojection_matrix);
    shader->SetTexture("u_texture", texture);
    mesh->Render();

    shader->Disable();

}*/

void Entity::Render(sUniformData uniformData) {   

    uniformData.modelMatrix = model;

    // Change Depth Test function from LESS to LEQUAL
    glDepthFunc(GL_LEQUAL);


    // Draw the mesh (using the first light uniforms) without blending enabled
    glDisable(GL_BLEND);
    for (int light_index = 0; light_index < uniformData.numOfLights; ++light_index) {
        this->material->Enable(uniformData, light_index);
        this->mesh->Render();
    }


    // Add current light to the previous one with additive blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    for (int light_index = 0; light_index < uniformData.numOfLights; ++light_index) {
        // Important: change uniforms to represent the additional light contribution
        this->material->Enable(uniformData, light_index);
        this->mesh->Render();
    }


    // Disable blending and reset depth function
    glDisable(GL_BLEND);
    glDepthFunc(GL_LESS);


    material->Disable();

}




void Entity::Update(float seconds_elapsed, int type) {
    static float time_elapsed = 0.0f;

    float rotation_speed = 0.05f;
    float translation_speed = 1.0f;
    float scaling_speed = 0.2f;

    float scaling_factor = 1.0f;

    time_elapsed += seconds_elapsed;

    if (type == ROTATE) {
        float rotation_factor = sin(time_elapsed * rotation_speed);
        model.Rotate(rotation_factor, Vector3(0.0f, 1.0f, 0.0f));
    }
    else if (type == TRANSLATE) {
        //float translation_factor = sin(time_elapsed * translation_speed);
        //model.Translate(6, 0.0f, 0.0f);

        
        float translation_factor = 0.5f * sin(time_elapsed * translation_speed);
        model.Translate(translation_factor * seconds_elapsed, 0.0f, 0.0f);
     
    }
    else if (type == SCALE) {
        
        scaling_factor += 0.1f * sin(time_elapsed * scaling_speed) * seconds_elapsed; // Adjusted scaling factor

        // Reset scaling after reaching a certain threshold
        if (scaling_factor >= 2.0f) {
            scaling_factor = 1.0f;
        }

        model.Scale(scaling_factor, scaling_factor, scaling_factor);
    }
}


