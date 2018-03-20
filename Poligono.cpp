
#include "Poligono.h"

Poligono::Poligono(std::vector<Ponto> p) {
    int size = p.size();
    
    pontos.reserve(size);
    for(int i = 0; i<size; i++)
        pontos.push_back(p.at(i));
    
}

Poligono::Poligono(const Poligono& orig) {
}

Poligono::~Poligono() {
}

void Poligono::addP(Ponto p){
    int s = pontos.size();
    pontos.resize(s+1);
    pontos.push_back(p);
    
}
void Poligono::setP(Ponto ponto, int a){	 	  	     	  	      	     	 	    	        	 	
    if(a<pontos.size())
        pontos.at(a) = ponto;
}
Ponto Poligono::getP(int a){
    if(a<pontos.size())
        return pontos.at(a); 
}
int Poligono::getSize(){
    return pontos.size();
}

