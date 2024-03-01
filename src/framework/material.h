#include "image.h"
#include "shader.h"
#include "framework.h"
#include "camera.h"
class Material {

	public:
		Texture* texture;
		Shader* shader;
        Vector3 Ka;
        Vector3 Kd;
        Vector3 Ks;
        float shininess;


 
        void Material::Enable(const sUniformData& uniformData);
        void Material::Disable();


    struct sLight {
    Vector3 position;
    Vector3 Id; 
    Vector3 Is; 
    };


    struct sUniformData {
        Entity::model modelMatrix;
        Camera::viewprojection_matrix viewprojectionMatrix;
        Application::Ia Ia;
        Material material;
        Light scenelights;
        

    };


};