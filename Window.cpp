#include "Window.h"


Window::Window() {
}

Window::Window(const Window& orig) {
}

Window::~Window() {
}

Window::Window(int x, int y, int c,int d){
    max = Ponto(x,y);
    min = Ponto(c,d);
    
    }

void Window::setMAX(int x, int y){
    max.setX(x);
    max.setY(y);
    }

void Window::setMIN(int x, int y){
    min.setX(x);
    min.setY(y);
    }	 	  	     	  	      	     	 	    	        	 	

int Window::getXmax(){
    return max.getX();
    }

int Window::getXmin(){
    return min.getX();
    }
    
int Window::getYmax(){
    return max.getY();
    }

int Window::getYmin(){
    return min.getY();
    }

    
    
    
    
    