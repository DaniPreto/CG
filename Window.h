
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
    void setMAX(double x, double y);
    void setMIN(double x, double y);
    double getXmax();
    double getYmax();
    double getXmin();
    double getYmin();
    
private:
    Ponto max;
    Ponto min;

};



#endif /* PONTO_H */

	 	  	     	  	      	     	 	    	        	 	