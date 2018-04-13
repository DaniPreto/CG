

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
    void setP1(double x , double y);
    void setP2(double x , double y);
    double getP1X();
    double getP1Y();
    double getP2X();
    double getP2Y();
    
    void setP1SCN(double x , double y);
    void setP2SCN(double x , double y);
    double getP1U();
    double getP1V();
    double getP2U();
    double getP2V();
private:
    Ponto ini;
    Ponto fim;
    
};

#endif /* LINHA_H */
	 	  	     	  	      	     	 	    	        	 	