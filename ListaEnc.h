

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
    double getpX(int pos);
    double getpY(int pos);
    double getpX(std::string n);
    double getpY(std::string n);
    
    //Get X e Y do ponto SCN
    double getpU(int pos);
    double getpV(int pos);
    
    
    //Get nome do objeto
    std::string getNP(int pos);
    std::string getNL(int pos);
    std::string getNdoPoligono(int pos);
    
    //Get X e Y da linha
    double getlX(int pos,bool p);
    double getlY(int pos,bool p);
    double getlX(std::string n,bool p);
    double getlY(std::string n,bool p);
    
    //Get X e Y da linha SCN
    double getlU(int pos, bool p);
    double getlV(int pos,bool p);
    
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
    Ponto* getPontosPoligono(int pos);
    
    //Get poligono SCN
    double getUdoPoligono(int pos,int a);
    double getVdoPoligono(int pos,int a);
    
    //Clear lista
    void clearL();
    void clearP();
    void clearPL();
    
    //Set objetos
    void setP(double x,double y, int pos);
    void setP(double x,double y, std::string n);
    void setL(double x,double y, int pos, bool p);
    void setL(double x,double y, std::string n, bool p);
    void setPPoligono(double x,double y,std::string n, int pos);
    
    //Set objetos SCN
    void setPSCN(double x,double y, int pos);
    void setLSCN(double x,double y, int pos, bool i);
    void setPPoligonoSCN(double x,double y,int n, int pos);
    
    //Curva
    int getsizeC();
    std::string getNCurva(int pos);
    int getSCurva(int pos);
    int getSCurva(std::string n);
    void addC(double*x,double*y, std::string n, int size);
    double getXCurva(int pos,int a);
    double getYCurva(int pos,int a);
    double getXCurva(std::string n,int a);
    double getYCurva(std::string n,int a);

    void clearC();
    double getUCurva(int pos,int a);
    double getVCurva(int pos,int a);

    
    
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

	 	  	     	  	      	     	 	    	        	 	
