

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

void Linha::setP(double x , double y, bool p){
    if(p){
        Ponto p(x,y);
        ini = p;
    }
    else{
        Ponto p(x,y);
        fim = p;
    }
}	 	  	     	  	      	     	 	    	        	 	

double Linha::getX(bool p){
    if(p)
        return ini.getX();
    else
        return fim.getX();
}

double Linha::getY(bool p){
    if(p)
        return ini.getY();
    else
        return fim.getY();
}



//----------------------------------------------------------------------SCN

void Linha::setSCN(double x , double y, bool p){
    if(p){
        ini.setU(x);
        ini.setV(y);
    }
    else{
        fim.setU(x);
        fim.setV(y);
    }
}

double Linha::getU(bool p){
    if(p)
        return ini.getU();
    else
        return fim.getU();
}	 	  	     	  	      	     	 	    	        	 	

double Linha::getV(bool p){
    if(p)
        return ini.getV();
    else
        return fim.getV();
}









