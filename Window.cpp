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
    state = false;
    centro = Ponto();
    calculaCentro();
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

//----------------------------------------------------------------------------------------------------ROtação window
void Window::rotate(){
    state = true;
}

bool Window::getState(){
    return state;
}   
    
void Window::calculaCentro(){
    double x = (max.getX() + min.getX())/2;
    double y = (max.getY() + min.getY())/2;
    centro.setX(x);
    centro.setY(y);
    }	 	  	     	  	      	     	 	    	        	 	


double Window::getXcentro(){
    return centro.getX();
    }

double Window::getYcentro(){
    return centro.getY();
    }
    
    
void Window::setTeta(double v){
    teta = v;
}
void Window::addTeta(double v){
    teta += v;
}
double Window::getTeta(){
    return teta;
}   

//--------------------------------------------------------------------------------Clipagem-----------------------------------
bool Window::clipPonto(double x, double y){
    xMax = max.getX();
    yMax = max.getY();
    xMin = min.getX();
    yMin = min.getY();
    
    if(x < xMax && x > xMin && y < yMax && y > yMin){
        return true;
    }
    
    return false;
} 
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    