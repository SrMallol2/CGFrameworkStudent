#include "image.h"
#include "shader.h"


class Material {

	public:
		Texture* texture;
		Shader* shader;

        float shininess;

 
        void Material::Enable();
        void Material::Disable();


    struct sLight {
    float position[3];
    float diffuseIntensity; 
    float specularIntensity; 
    };


};