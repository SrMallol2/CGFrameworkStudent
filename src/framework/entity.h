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



	void Entity::Render(Image* framebuffer, Camera* camera, const Color& c0,
		const Color& c1, const Color& c2);

	void Entity::Update(float seconds_elapsed,int type);

};