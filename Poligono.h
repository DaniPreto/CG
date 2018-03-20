

#ifndef POLIGONO_H
#define POLIGONO_H

#include "Ponto.h"
#include <iostream>
#include <vector>
#include "ObjGrafico.h"

class Poligono : public ObjGrafico {
public:
    Poligono(std::vector<Ponto> p);
    Poligono(const Poligono& orig);
    virtual ~Poligono();
    void addP(Ponto p1);
    void setP(Ponto ponto, int a);
    Ponto getP(int a);
    int getSize();

private:
    std::vector<Ponto> pontos;

};

#endif /* POLIGONO_H */

	 	  	     	  	      	     	 	    	        	 	
