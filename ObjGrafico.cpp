

#include "ObjGrafico.h"

ObjGrafico::ObjGrafico() {
}

ObjGrafico::ObjGrafico(const ObjGrafico& orig) {
}

ObjGrafico::~ObjGrafico() {
}

std::string ObjGrafico::getNome(){
    return nome;
}
void ObjGrafico::setNome(std::string dado){
    nome = dado;
}

bool ObjGrafico::getEstado(){
    return estado;
}
bool ObjGrafico::setEstado(bool e){
    estado = e;
}