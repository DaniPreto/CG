

#ifndef LISTAENC_H
#define LISTAENC_H
#include <iostream>
#include <vector>
#include "Ponto.h"
#include "Linha.h"
#include <string>

class ListaEnc {
public:
    ListaEnc();
    ListaEnc(const ListaEnc& orig);
    virtual ~ListaEnc();
    
    int getsizeP();
    int getsizeL();
    
    void addP(int x, int y, string n);
    void addL(int a, int b,int c, int d,string n);
    bool temObjeto(std::string n);
    int getpX(int pos);
    int getpY(int pos);
    int getlX(int pos);
    int getlY(int pos);
    int getlX2(int pos);
    int getlY2(int pos);
    
    void clearL();
    void clearP();
private:
    Ponto pontos[200];
    Linha linhas[200];
    int sizeP;
    int sizeL;

};

#endif /* LISTAENC_H */

	 	  	     	  	      	     	 	    	        	 	
