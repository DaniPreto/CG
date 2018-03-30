

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
    
    //Add objetos
    void addP(double x, double y, std::string n);
    void addL(double a, double b,double c, double d,std::string n);
    void addPL(int a, double x[], double y[], std::string n);
    
    //Controle da lista
    bool temObjeto(std::string n);
    bool isEmpty();
    int getType(std::string n);
    
    //Get X e Y do ponto
    double getpX(int pos);
    double getpY(int pos);
    double getpX(std::string n);
    double getpY(std::string n);
    
    //Get nome do objeto
    std::string getNP(int pos);
    std::string getNL(int pos);
    std::string getNdoPoligono(int pos);
    
    //Get X e Y da linha
    double getlX(int pos);
    double getlY(int pos);
    double getlX2(int pos);
    double getlY2(int pos);
    double getlX(std::string n);
    double getlY(std::string n);
    double getlX2(std::string n);
    double getlY2(std::string n);
    
    //Get tamanho da lista
    int getsizeL();
    int getsizeP();
    int getsizePL();
    
    //Get poligono
    int getSdoPoligono(int pos);
    int getSdoPoligono(std::string n);
    double getXdoPoligono(int pos,int a);
    double getYdoPoligono(int pos,int a);
    double getXdoPoligono(std::string n,int a);
    double getYdoPoligono(std::string n,int a);
    
    //Clear lista
    void clearL();
    void clearP();
    void clearPL();
    
    //Set objetos
    void setP(double x,double y, int pos);
    void setP(double x,double y, std::string n);
    void setL(double x,double y, int pos);
    void setL1(double x,double y, int pos);
    void setL(double x,double y, std::string n);
    void setL1(double x,double y, std::string n);
    void setPPoligono(double x,double y,std::string n, int pos);
    
    
private:
    Ponto pontos[200];
    Linha linhas[200];
    Poligono poligonos[200];
    int sizePL;
    int sizeP;
    int sizeL;

};

#endif /* LISTAENC_H */

	 	  	     	  	      	     	 	    	        	 	
