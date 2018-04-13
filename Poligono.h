

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
    int getSize();
    void addP(double x, double y);
    void setP(double x, double y, int a);
    double getX(int pos);
    double getY(int pos);
    
    void setPSCN(double x, double y, int a);
    double getU(int pos);
    double getV(int pos);

private:
    Ponto pontos[100];
    int numeroPontos;
    int posicao;

};

#endif /* POLIGONO_H */

	 	  	     	  	      	     	 	    	        	 	