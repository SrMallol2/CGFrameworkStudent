#include "mesh.h"
#include "image.h"


class Entity {

	public:
		Mesh* mesh;
		Matrix44 model;

		Entity() {};



	void Entity::Render(Image* framebuffer, Camera* camera, const Color& c);


};