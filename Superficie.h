

#ifndef SUPERFICIE_H
#define SUPERFICIE_H


#include <iostream>
#include <vector>
#include "ObjGrafico.h"
#include<string>

class Superficie : public ObjGrafico {
public:
    Superficie(const Superficie& orig){}
    virtual ~Superficie(){}
    Superficie(std::string n,bool t){nome = n;estado = t;posicao = 0;tipo =6;}
    
    int getPosicao(){return posicao;}
    int addPosicao(){posicao += 1;}

    
private:
    int posicao;

};

#endif 

	 	  	     	  	      	     	 	    	        	 	