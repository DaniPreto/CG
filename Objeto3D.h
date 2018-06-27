

#ifndef OBJETO3D_H
#define OBJETO3D_H

#include <iostream>
#include <vector>
#include "ObjGrafico.h"
#include<string>
#include "ObjGrafico.h"

class Objeto3D : public ObjGrafico {
public:
    Objeto3D(){}
    Objeto3D(std::string n){nome = n;tipo = 4;}
    Objeto3D(std::string n,Coordenadas coords, Facetas facets){nome = n;tipo = 4;m_coords = coords;m_facets = facets;}
    Objeto3D(const Objeto3D& orig){}
    virtual ~Objeto3D(){}


};
#endif

	 	  	     	  	      	     	 	    	        	 	