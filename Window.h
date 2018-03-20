
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
    Window(int x, int y, int c,int d);
    void setMAX(int x, int y);
    void setMIN(int x, int y);
    int getXmax();
    int getYmax();
    int getXmin();
    int getYmin();
    
private:
    Ponto max;
    Ponto min;

};



#endif /* PONTO_H */

	 	  	     	  	      	     	 	    	        	 	