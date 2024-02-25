#include "mesh.h"
#include "image.h"
#include "shader.h"


class Entity {

	public:
		Mesh* mesh;
		Matrix44 model;
		Image* image;
		Texture* texture;
		Shader* shader;

		

		enum AnimationType {
			ROTATE,
			TRANSLATE,
			SCALE,

		};
		struct Animation {
			int type;
		};


		enum class eRenderMode {
			PLAIN_COLOR,
			OCCLUSIONS,
			TEXTURES,
			TRIANGLES_INTERPOLATED
		};

		eRenderMode mode;

public:
	// Constructor with default render mode
	Entity() : mode(eRenderMode::PLAIN_COLOR), mesh(nullptr) {}

	// Method to set the render mode
	void SetRenderMode(eRenderMode renderMode) {
		mode = renderMode;
		
	}
		


	void Entity::Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer );
	void Entity::Render(Camera* camera);

	void Entity::Update(float seconds_elapsed,int type);

};