

#ifndef LISTAENC_H
#define LISTAENC_H
#include <iostream>
#include <vector>
#include "Ponto.h"
#include "Linha.h"
#include "Poligono.h"
#include <string>

class ListaEnc {
public:
    ListaEnc();
    ListaEnc(const ListaEnc& orig);
    virtual ~ListaEnc();
    
    void addP(double x, double y, std::string n);
    void addL(double a, double b,double c, double d,std::string n);
    void addPL(int a, double x[], double y[], std::string n);
    
    bool temObjeto(std::string n);
    bool isEmpty();
    
    double getpX(int pos);
    double getpY(int pos);
    std::string getNP(int pos);
    
    std::string getNL(int pos);
    double getlX(int pos);
    double getlY(int pos);
    double getlX2(int pos);
    double getlY2(int pos);
    
    int getsizeL();
    int getsizeP();
    int getsizePL();
    
    int getSdoPoligono(int pos);
    std::string getNdoPoligono(int pos);
    double getXdoPoligono(int pos,int a);
    double getYdoPoligono(int pos,int a);
    
    void clearL();
    void clearP();
    void clearPL();
    
    void setP(double x,double y, int pos);
    void setL(double x,double y, int pos);
    void setL1(double x,double y, int pos);
    
    
private:
    Ponto pontos[200];
    Linha linhas[200];
    Poligono poligonos[200];
    int sizePL;
    int sizeP;
    int sizeL;

};

#endif /* LISTAENC_H */

	 	  	     	  	      	     	 	    	        	 	
