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
    double xMax = max.getX();
    double yMax = max.getY();
    double xMin = min.getX();
    double yMin = min.getY();
    
    if(x < xMax && x > xMin && y < yMax && y > yMin){
        return true;
    }
    
    return false;
}
double* Window::cohensutherland(double x, double y, double a, double b){
    double resp[5];

    double xMax = max.getX();
    double yMax = max.getY();
    double xMin = min.getX();
    double yMin = min.getY();
    //----------------------------------------------------------Passo 1
    //-------------ponto 1
    bool* rcp1 = getQuadrante(x, y);
    
    //-------------ponto 2
    bool* rcp2 = getQuadrante(a, b);
        
    //-------------------------------------------------------Passo 2
    if(!(rcp1[0] || rcp2[0] || rcp1[1] || rcp2[1] || rcp1[2] || rcp2[2] || rcp1[3] || rcp2[3])){
        resp[0] = 0;
        return resp;
    }
    else if((rcp1[0] && rcp2[0]) || (rcp1[1] && rcp2[1])|| (rcp1[2] && rcp2[2]) || (rcp1[3] && rcp2[3])){
        resp[0] = 1;
        return resp;
    }
    else{
        resp[0] = 2;
        //-------------ponto 1
        
        if(rcp1[0] && !rcp1[1] && !rcp1[2] && !rcp1[3]){ //top
            resp[1] = x + (a - x) * (yMax - y) / (b - y);
            resp[2] = yMax;
        }
        else if(!rcp1[0] && rcp1[1] && !rcp1[2] && !rcp1[3]){//bottom
            resp[1] = x + (a - x) * (yMin - y) / (b - y);
            resp[2] = yMin;
        }
        else if(!rcp1[0] && !rcp1[1] && rcp1[2] && !rcp1[3]){//right
            resp[1] = xMax;
            resp[2] = y + (b - y) * (xMax - x) / (a - x);
        }
        else if(!rcp1[0] && !rcp1[1] && !rcp1[2] && rcp1[3]){//left
            resp[1] = xMin;
            resp[2] = y + (b - y) * (xMin - x) / (a - x);
        }
        else{
            resp[1] = x;
            resp[2] = y;
        }
        
        //-------------ponto 2
        
        if(rcp2[0] && !rcp2[1] && !rcp2[2] && !rcp2[3]){ //top
            resp[3] = x + (a - x) * (yMax - y) / (b - y);
            resp[4] = yMax;
        }
        else if(!rcp2[0] && rcp2[1] && !rcp2[2] && !rcp2[3]){//bottom
            resp[3] = x + (a - x) * (yMin - y) / (b - y);
            resp[4] = yMin;
        }
        else if(!rcp2[0] && !rcp2[1] && rcp2[2] && !rcp2[3]){//right
            resp[3] = xMax;
            resp[4] = y + (b - y) * (xMax - x) / (a - x);
        }
        else if(!rcp2[0] && !rcp2[1] && !rcp2[2] && rcp2[3]){//left
            resp[3] = xMin;
            resp[4] = y + (b - y) * (xMin - x) / (a - x);
        }
        else{
            resp[3] = a;
            resp[4] = b;
        }
        
        return resp;
    }
    return resp;
        
}
    
bool* Window::getQuadrante(double x, double y){
    double xMax = max.getX();
    double yMax = max.getY();
    double xMin = min.getX();
    double yMin = min.getY();
    bool rc[4];

    if(x<xMin)
        rc[3] = 1;
    else
        rc[3]  = 0;
    
    if(x>xMax)
        rc[2] = 1;
    else
        rc[2]  = 0;
    
    if(y<yMin)
        rc[1] = 1;
    else
        rc[1]  = 0;
    
    if(y>yMax)
        rc[0] = 1;
    else
        rc[0]  = 0;
        
        
        
    return rc;
}
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    