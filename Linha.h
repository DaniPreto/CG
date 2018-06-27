

#ifndef LINHA_H
#define LINHA_H

#include "Ponto.h"
#include <iostream>
#include <vector>
#include "ObjGrafico.h"
#include<string>
using std::string;


class Linha : public ObjGrafico {
public:
    Linha();
    Linha(const Linha& orig);
    virtual ~Linha();
    Linha(double a, double b, double c , double d);
    Linha(double a, double b, double c , double d, std::string n);
    Linha(std::string n,Coordenadas c){nome = n;tipo = 1;m_coords = c;}
    
    
    void setP(double x , double y, bool p);
    double getX(bool p);
    double getY(bool p);

    
};

#endif 
	 	  	     	  	      	     	 	    	        	 	