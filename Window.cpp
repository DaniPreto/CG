#include "Window.h"


Window::Window() {
}

Window::Window(const Window& orig) {
}

Window::~Window() {
}

Window::Window(double x, double y, double c,double d){
    max = Ponto(x,y);
    min = Ponto(c,d);
    
    }

void Window::setMAX(double x, double y){
    max.setX(x);
    max.setY(y);
    }

void Window::setMIN(double x, double y){
    min.setX(x);
    min.setY(y);
    }	 	  	     	  	      	     	 	    	        	 	

double Window::getXmax(){
    return max.getX();
    }

double Window::getXmin(){
    return min.getX();
    }
    
double Window::getYmax(){
    return max.getY();
    }

double Window::getYmin(){
    return min.getY();
    }

    
    
    
    
    