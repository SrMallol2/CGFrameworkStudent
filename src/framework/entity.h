#include "mesh.h"
#include "image.h"


class Entity {

	public:
		Mesh* mesh;
		Matrix44 model;
		Image* texture;

		

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
		


	void Entity::Render(Image* framebuffer, Camera* camera, const Color& c0,
		const Color& c1, const Color& c2, FloatImage* zBuffer );

	void Entity::Update(float seconds_elapsed,int type);

};