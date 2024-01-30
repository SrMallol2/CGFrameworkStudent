#include "entity.h"


#include <iostream>

std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}


void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
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
            framebuffer->DrawTriangle(
                Vector2(screen0.x, screen0.y),
                Vector2(screen1.x, screen1.y),
                Vector2(screen2.x, screen2.y),
                c, false, c);
            //framebuffer->DrawLineDDA(screenVertices[0].x, screenVertices[0].y, screenVertices[1].x, screenVertices[1].y, c);
            //framebuffer->DrawLineDDA(screenVertices[1].x, screenVertices[1].y, screenVertices[2].x, screenVertices[2].y, c);
            //framebuffer->DrawLineDDA(screenVertices[2].x, screenVertices[2].y, screenVertices[0].x, screenVertices[0].y, c);
        }
    }
}

