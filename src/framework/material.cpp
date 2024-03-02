#include "material.h"

Material::Material()
{
}

void Material::Enable(const sUniformData& uniformData){
    shader->Enable();
    glEnable(GL_DEPTH_TEST);

    // Z will pass if the Z is LESS or EQUAL to the Z of the pixel
    glDepthFunc(GL_LEQUAL);

    shader->SetMatrix44("u_modelMatrix", uniformData.modelMatrix);
    shader->SetMatrix44("u_viewprojection",uniformData.viewProjectionMatrix);
    shader->SetTexture("u_texture", texture);

}


void Material::Disable(){
    shader->Disable();
}