#include "image.h"
#include "shader.h"
#include "framework.h"
#include "camera.h"
#include "light.h"
class Material {

	public:
		Texture* texture;
		Shader* shader;
        Vector3 Ka;
        Vector3 Kd;
        Vector3 Ks;
        float shininess;

        struct sUniformData {
            Matrix44 modelMatrix;
            Matrix44 viewProjectionMatrix;
            Vector3 Ia;
            Material* material;
            Light scenelights;


        };

 
        void Material::Enable(const sUniformData& uniformData);
        void Material::Disable();


    struct sLight {
    Vector3 position;
    Vector3 Id; 
    Vector3 Is; 
    };


    


};