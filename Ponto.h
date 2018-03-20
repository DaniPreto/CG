
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
    
    void setX(double v);
    void setY(double v);
    double getX();
    double getY();
    
private:
    double x;
    double y;

};



#endif /* PONTO_H */

	 	  	     	  	      	     	 	    	        	 	
