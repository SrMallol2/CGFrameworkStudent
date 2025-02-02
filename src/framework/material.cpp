#include "material.h"

Material::Material()
{
}

void Material::Enable(const sUniformData &uniformData, int lightIndex){
    shader->Enable();


    //Material
    shader->SetVector3("Ka",Ka);
    shader->SetVector3("Kd",Kd);
    shader->SetVector3("Ks",Ks);
    shader->SetFloat("shininess",shininess);
    shader->SetTexture("u_color_texture",color_texture);
    shader->SetTexture("u_normal_texture",normal_texture);

    //3D
    shader->SetMatrix44("u_modelMatrix", uniformData.modelMatrix);

    //Camera
    shader->SetMatrix44("u_viewProjection",uniformData.viewProjectionMatrix);
    shader->SetVector3("cameraPosition",uniformData.cameraPosition);

    //Lights
    if (lightIndex==0){
    shader->SetVector3("Ia",uniformData.Ia);
    }
    else {
        shader->SetVector3("Ia", Vector3(0.0));
        }
    shader->SetVector3("lightPosition",uniformData.scenelights[lightIndex].position);
    shader->SetVector3("Id",uniformData.scenelights[lightIndex].Id);
    shader->SetVector3("Is",uniformData.scenelights[lightIndex].Is);

    shader->SetVector3("flag",uniformData.flag);

    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);

    // Z will pass if the Z is LESS or EQUAL to the Z of the pixel
    glDepthFunc(GL_LEQUAL);

}


void Material::Disable(){
    shader->Disable();
}