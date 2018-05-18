

#ifndef LISTAENC_H
#define LISTAENC_H
#include <iostream>
#include <vector>
#include "Ponto.h"
#include "Linha.h"
#include "Poligono.h"
#include <string>
#include "Curva.h"

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
    double getXP(int pos);
    double getYP(int pos);
    double getXP(std::string n);
    double getYP(std::string n);
    
    //Get X e Y do ponto SCN
    double getUP(int pos);
    double getVP(int pos);
    
    
    //Get nome do objeto
    std::string getNP(int pos);
    std::string getNL(int pos);
    std::string getNPL(int pos);
    
    //Get X e Y da linha
    double getXL(int pos,bool p);
    double getYL(int pos,bool p);
    double getXL(std::string n,bool p);
    double getYL(std::string n,bool p);
    
    //Get X e Y da linha SCN
    double getUL(int pos, bool p);
    double getVL(int pos,bool p);
    
    //Get tamanho da lista
    int getsizeL();
    int getsizeP();
    int getsizePL();
    
    //Get poligono
    int getSPL(int pos);
    int getSPL(std::string n);
    double getXPL(int pos,int a);
    double getYPL(int pos,int a);
    double getXPL(std::string n,int a);
    double getYPL(std::string n,int a);
    
    
    //Get poligono SCN
    double getUPL(int pos,int a);
    double getVPL(int pos,int a);
    
    //Clear lista
    void clearL();
    void clearP();
    void clearPL();
    
    //Set objetos
    void setP(double x,double y, int pos);
    void setP(double x,double y, std::string n);
    void setL(double x,double y, int pos, bool p);
    void setL(double x,double y, std::string n, bool p);
    void setPL(double x,double y,std::string n, int pos);
    
    //Set objetos SCN
    void setPSCN(double x,double y, int pos);
    void setLSCN(double x,double y, int pos, bool i);
    void setPLSCN(double x,double y,int n, int pos);
    
    //Curva
    int getsizeC();
    std::string getNC(int pos);
    int getSC(int pos);
    int getSC(std::string n);
    void addC(double*x,double*y, std::string n, int size,bool t);
    double getXC(int pos,int a);
    double getYC(int pos,int a);
    double getXC(std::string n,int a);
    double getYC(std::string n,int a);

    void clearC();
    double getUC(int pos,int a);
    double getVC(int pos,int a);

    
    
private:
    Ponto pontos[200];
    Linha linhas[200];
    Poligono poligonos[200];
    Curva curvas[200];
    
    int sizePL;
    int sizeP;
    int sizeL;
    int sizeC;

};

#endif /* LISTAENC_H */

	 	  	     	  	      	     	 	    	        	 	
