

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
    Linha(int a, int b, int c , int d);
    Linha(int a, int b, int c , int d, std::string n);
    void setP1(int x , int y);
    void setP2(int x , int y);
    int getP1X();
    int getP1Y();
    int getP2X();
    int getP2Y();
private:
    Ponto ini;
    Ponto fim;
    
};

#endif /* LINHA_H */
	 	  	     	  	      	     	 	    	        	 	
