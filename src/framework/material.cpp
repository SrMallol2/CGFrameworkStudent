#include "material.h"

Material::Material()
{
}

void Material::Enable(const sUniformData &uniformData){
    shader->Enable();
    glEnable(GL_DEPTH_TEST);

    // Z will pass if the Z is LESS or EQUAL to the Z of the pixel
    glDepthFunc(GL_LEQUAL);

    //Material
    shader->SetVector3("Ka",Ka);
    shader->SetVector3("Kd",Kd);
    shader->SetVector3("Ks",Ks);
    shader->SetFloat("shininess",shininess);
    shader->SetTexture("u_texture", texture);

    //3D
    shader->SetMatrix44("u_modelMatrix", uniformData.modelMatrix);

    //Camera
    shader->SetMatrix44("u_viewProjection",uniformData.viewProjectionMatrix);
    shader->SetVector3("cameraPosition",uniformData.cameraPosition);

    //Lights
    shader->SetVector3("Ia",uniformData.Ia);
    shader->SetVector3("lightPosition",uniformData.scenelights.position);
    shader->SetVector3("Id",uniformData.scenelights.Id);
    shader->SetVector3("Is",uniformData.scenelights.Is);

}


void Material::Disable(){
    shader->Disable();
}