
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
    Ponto(int a, int b);
    Ponto(int a, int b,std::string n);
    
    void setX(int v);
    void setY(int v);
    int getX();
    int getY();
    
private:
    int x;
    int y;

};



#endif /* PONTO_H */

	 	  	     	  	      	     	 	    	        	 	
