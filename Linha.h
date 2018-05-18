

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
    
    
    void setP(double x , double y, bool p);
    double getX(bool p);
    double getY(bool p);

    
    void setSCN(double x , double y, bool p);
    double getU(bool p);
    double getV(bool p);
    
    
private:
    Ponto ini;
    Ponto fim;
    
    
};

#endif /* LINHA_H */
	 	  	     	  	      	     	 	    	        	 	