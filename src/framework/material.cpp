#include "material.h"



void Material::Enable(){
    shader->Enable();
}


void Material::Disable(){
    shader->Disable();
}