#include "material.h"



void Material::Enable(const sUniformData& uniformData){
    shader->Enable();
}


void Material::Disable(){
    shader->Disable();
}