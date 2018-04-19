
#ifndef WINDOW_H
#define WINDOW_H
#include <iostream>
#include <vector>
#include "Ponto.h"

class Window {
public:
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
    double * cohensutherland(double x, double y, double a, double b);
    bool* getQuadrante(double x, double y);
    
private:
    Ponto max;
    Ponto min;
    bool state;
    Ponto centro;
    double teta;

};



#endif /* PONTO_H */

	 	  	     	  	      	     	 	    	        	 	