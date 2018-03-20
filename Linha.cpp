

#include "Linha.h"

Linha::Linha() {
}

Linha::Linha(const Linha& orig) {
}

Linha::~Linha() {
}

Linha::Linha(int a, int b, int c , int d){
    Ponto p(a,b);
    Ponto p1(c,d);
    ini = p;
    fim = p1;
}
Linha::Linha(int a, int b, int c , int d,std::string n){
    Ponto p(a,b);
    Ponto p1(c,d);
    ini = p;
    fim = p1;
    nome = n;
}

void Linha::setP1(int x, int y){
    Ponto p(x,y);
    ini = p;
}
void Linha::setP2(int x, int y){
    Ponto p(x,y);
    fim = p;
}
int Linha::getP1X(){
    return ini.getX();
}	 	  	     	  	      	     	 	    	        	 	
int Linha::getP1Y(){
    return ini.getY();
}

int Linha::getP2X(){
    return fim.getX();
}

int Linha::getP2Y(){
    return fim.getY();
}

