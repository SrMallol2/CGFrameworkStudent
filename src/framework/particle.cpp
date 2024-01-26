#include "particle.h"
#include <cstdlib>  )

void ParticleSystem::Init() {
    // Initialize particles to random positions
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        particles[i].position = Vector2(std::rand()%1400, std::rand()%700);
        particles[i].velocity = Vector2((rand() % 5 - 2), rand()%5-2).normalize();
        particles[i].color = Color::WHITE;
        particles[i].acceleration = 0.1f;
        particles[i].ttl = 100;//static_cast<float>(std::rand() % 100 / 10.0f);
        particles[i].inactive = false;
    }
}

void ParticleSystem::Render(Image* framebuffer) {
    // Draw particles in the framebuffer
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        if (!particles[i].inactive) {
            // Assuming DrawPoint is a function to draw a pixel at a given position with a specified color
            framebuffer->SetPixelSafe(static_cast<int>(particles[i].position.x), static_cast<int>(particles[i].position.y), particles[i].color);
        }
    }
}

void ParticleSystem::Update(float dt) {
    // Update particle positions
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        if (!particles[i].inactive) {
            particles[i].position.x += particles[i].velocity.x * dt;
            particles[i].position.y += particles[i].velocity.y * dt;
            particles[i].velocity.x += particles[i].acceleration * dt;
            particles[i].velocity.y += particles[i].acceleration * dt;
            particles[i].ttl -= dt;

            if (particles[i].ttl <= 0.0f) {
                particles[i].inactive = true;
            }
        }
    }
}
