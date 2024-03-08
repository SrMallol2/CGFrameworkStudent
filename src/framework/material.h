#ifndef material_h
#define material_h

#include "image.h"
#include "shader.h"
#include "camera.h"



struct sLight {
    Vector3 position;
    Vector3 Id;
    Vector3 Is;
};

struct sUniformData {
    Matrix44 modelMatrix;
    Matrix44 viewProjectionMatrix;
    Vector3 cameraPosition;
    Vector3 Ia;
    sLight scenelights;
    Vector3 flag;
};


class Material {

	public:
		Texture* color_texture;
        Texture* normal_texture;
		Shader* shader;
        Vector3 Ka;
        Vector3 Kd;
        Vector3 Ks;
        float shininess;

        
        Material();
 
        void Material::Enable( const sUniformData &uniformData);
        void Material::Disable();




    


};
#endif