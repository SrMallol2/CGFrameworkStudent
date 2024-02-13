#include "entity.h"


#include <iostream>



void Entity::Render(Image* framebuffer, Camera* camera, const Color& c0,
    const Color& c1, const Color& c2 ) {
    const auto& vertices = mesh->GetVertices();

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

        Vector2 screen0 = Vector2((screenVertices[0].x + 1.0f) * 0.5f * screenWidth, (1.0f + screenVertices[0].y) * 0.5f * screenHeight);
        Vector2 screen1 = Vector2((screenVertices[1].x + 1.0f) * 0.5f * screenWidth, (1.0f + screenVertices[1].y) * 0.5f * screenHeight);
        Vector2 screen2 = Vector2((screenVertices[2].x + 1.0f) * 0.5f * screenWidth, (1.0f + screenVertices[2].y) * 0.5f * screenHeight);

        if (insideFrustum) {
            
            // Draw the triangle using the screen space vertices and color
            /*
            framebuffer->DrawTriangle(
                Vector2(screen0.x, screen0.y),
                Vector2(screen1.x, screen1.y),
                Vector2(screen2.x, screen2.y),
                c0, true, c0);
                
             */
            framebuffer->DrawTriangleInterpolated(
                Vector3(screen0.x, screen0.y,1),
                Vector3(screen1.x, screen1.y,1),
                Vector3(screen2.x, screen2.y,1),
                c0, c1, c2);
            
            //framebuffer->DrawLineDDA(screenVertices[0].x, screenVertices[0].y, screenVertices[1].x, screenVertices[1].y, c);
            //framebuffer->DrawLineDDA(screenVertices[1].x, screenVertices[1].y, screenVertices[2].x, screenVertices[2].y, c);
            //framebuffer->DrawLineDDA(screenVertices[2].x, screenVertices[2].y, screenVertices[0].x, screenVertices[0].y, c);
        }
    }
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


