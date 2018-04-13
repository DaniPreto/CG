

#include "Linha.h"

Linha::Linha() {
}

Linha::Linha(const Linha& orig) {
}

Linha::~Linha() {
}

Linha::Linha(double a, double b, double c , double d){
    Ponto p(a,b);
    Ponto p1(c,d);
    ini = p;
    fim = p1;
}
Linha::Linha(double a, double b, double c , double d,std::string n){
    Ponto p(a,b);
    Ponto p1(c,d);
    ini = p;
    fim = p1;
    nome = n;
}

void Linha::setP1(double x , double y){
    Ponto p(x,y);
    ini = p;
}
void Linha::setP2(double x , double y){
    Ponto p(x,y);
    fim = p;
}
double Linha::getP1X(){
    return ini.getX();
}	 	  	     	  	      	     	 	    	        	 	
double Linha::getP1Y(){
    return ini.getY();
}

double Linha::getP2X(){
    return fim.getX();
}

double Linha::getP2Y(){
    return fim.getY();
}


//----------------------------------------------------------------------PPT

void Linha::setP1SCN(double x , double y){
    ini.setU(x);
    ini.setV(y);
}
void Linha::setP2SCN(double x , double y){
    fim.setU(x);
    fim.setV(y);
}
double Linha::getP1U(){
    return ini.getU();
}	 	  	     	  	      	     	 	    	        	 	
double Linha::getP1V(){
    return ini.getV();
}

double Linha::getP2U(){
    return fim.getU();
}

double Linha::getP2V(){
    return fim.getV();
}
