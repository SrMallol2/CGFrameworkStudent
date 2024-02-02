#include "mesh.h"
#include "image.h"


class Entity {

	public:
		Mesh* mesh;
		Matrix44 model;

		Entity() {};


		enum AnimationType {
			ROTATE,
			TRANSLATE,
			SCALE,

		};

		struct Animation {
			int type;
		};



	void Entity::Render(Image* framebuffer, Camera* camera, const Color& c);

	void Entity::Update(float seconds_elapsed,int type);

};