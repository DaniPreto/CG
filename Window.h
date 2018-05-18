
#ifndef WINDOW_H
#define WINDOW_H
#include <iostream>
#include <vector>
#include "Ponto.h"
#include "Linha.h"
#include "Poligono.h"

class Window {
public:
    typedef int OutCode;

    Window();
    Window(const Window& orig);
    virtual ~Window();
    Window(double x, double y, double c,double d);
    
    void setTeta(double v);
    void addTeta(double v);
    double getTeta();
    
    void setMAX(double x, double y);
    void setMIN(double x, double y);
    
    double getXmax();
    double getYmax();
    double getXmin();
    double getYmin();
    
    bool getState();
    void rotate();
    
    void calculaCentro();
    double getXcentro();
    double getYcentro();
    
    bool clipPonto(double x, double y);
    Linha cohenSutherland(double x, double y, double a, double b);
    OutCode getQuadrante(double x, double y);
    
    double maxi(double arr[],int n);
    double mini(double arr[], int n);
    Linha liangBarsky(double x, double y, double a, double b);
    
    
private:
    Ponto max;
    Ponto min;
    bool state;
    Ponto centro;
    double teta;

};



#endif /* PONTO_H */

	 	  	     	  	      	     	 	    	        	 	