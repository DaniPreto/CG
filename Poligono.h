

#ifndef POLIGONO_H
#define POLIGONO_H

#include "Ponto.h"
#include <iostream>
#include <vector>
#include "ObjGrafico.h"
#include<string>
#include "ObjGrafico.h"

class Poligono : public ObjGrafico {
public:
    Poligono(){tipo = 2;}
    Poligono(std::string n){nome = n;tipo = 2;}
    Poligono(const Poligono& orig){}
    virtual ~Poligono(){}
    Poligono(std::string n,Coordenadas c){nome = n;tipo = 2;m_coords = c;}

};

#endif 

	 	  	     	  	      	     	 	    	        	 	