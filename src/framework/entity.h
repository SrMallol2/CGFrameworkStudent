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
		//We define plain_color as predetermined 
		Entity() : mode(eRenderMode::PLAIN_COLOR) {}
		

		void SetRenderMode(eRenderMode renderMode) {
			mode = renderMode;
		}
		


	void Entity::Render(Image* framebuffer, Camera* camera, const Color& c0,
		const Color& c1, const Color& c2, FloatImage* zBuffer );

	void Entity::Update(float seconds_elapsed,int type);

};