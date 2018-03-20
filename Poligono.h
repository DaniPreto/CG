

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
    Poligono();
    Poligono(int size, std::string n);
    Poligono(const Poligono& orig);
    virtual ~Poligono();
    void addP(Ponto p1);
    void setP(Ponto ponto, int a);
    double getX(int pos);
    double getY(int pos);
    int getSize();

private:
    Ponto pontos[100];
    int numeroPontos;
    int posicao;

};

#endif /* POLIGONO_H */

	 	  	     	  	      	     	 	    	        	 	