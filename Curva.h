

#ifndef CURVA_H
#define CURVA_H

#include <iostream>
#include <vector>
#include "ObjGrafico.h"
#include<string>

class Curva : public ObjGrafico {
public:
    Curva(){}
    Curva(const Curva& orig){}
    virtual ~Curva(){}
    Curva(std::string n,bool t){nome = n;estado = t;posicao = 0;tipo =3;}
    
    int getPosicao(){return posicao;}
    int addPosicao(){posicao += 1;}

    
private:
    int posicao;

};

#endif 

	 	  	     	  	      	     	 	    	        	 	