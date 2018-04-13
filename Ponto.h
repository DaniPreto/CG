
#ifndef PONTO_H
#define PONTO_H
#include <iostream>
#include <vector>
#include "ObjGrafico.h"
#include<string>

class Ponto : public ObjGrafico {
public:
    Ponto();
    Ponto(const Ponto& orig);
    virtual ~Ponto();
    Ponto(double a, double b);
    Ponto(double a, double b,std::string n);
    
    void setX(double valor);
    void setY(double valor);
    double getX();
    double getY();
    
    void setU(double valor);
    void setV(double valor);
    double getU();
    double getV();
    
private:
    double x;
    double y;
    double u;
    double v;

};



#endif /* PONTO_H */

	 	  	     	  	      	     	 	    	        	 	