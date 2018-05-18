

#ifndef CURVA_H
#define CURVA_H

#include "Ponto.h"
#include <iostream>
#include <vector>
#include "ObjGrafico.h"
#include<string>

class Curva : public ObjGrafico {
public:
    Curva();
    Curva(const Curva& orig);
    virtual ~Curva();
    Curva(double*x,double*y,std::string n,int s,bool t);
    
    double getX(int i);
    double getY(int i);
    double getU(int i);
    double getV(int i);
    int getSize();

    
private:
    Ponto ponto[200];
    int size;

};

#endif /* POLIGONO_H */

	 	  	     	  	      	     	 	    	        	 	