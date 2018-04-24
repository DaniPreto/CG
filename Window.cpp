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

double* Window::cohenSutherland(double x, double y, double a, double b){
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
    if(!(rcp1[0] || rcp2[0] || rcp1[1] || rcp2[1] || rcp1[2] || rcp2[2] || rcp1[3] || rcp2[3])){                 //Totalmente dentro da window (valor 0)
        resp[0] = 0;
        return resp;
    }
    else if((rcp1[0] && rcp2[0]) || (rcp1[1] && rcp2[1]) || (rcp1[2] && rcp2[2]) || (rcp1[3] && rcp2[3])){     //Totalmente fora da window (valor 1)
        resp[0] = 1;
        return resp;
    }
    else{                                                                                                      //Parcialmente dentro da window (valor 2)
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

double Window::max_min(double* dados, int index,bool status){
    double m;
    if(status)
        double m = 0;
    else
        double m = 1;
        
    for (int i = 0; i < index; ++i)
        if(status){
            if (m < dados[i] && status)
                m = dados[i];
        }
        else{
            if (m > dados[i])
                m = dados[i];
        }
        
    return m;
    
}

double* Window::liangBarsky(double x, double y, double a, double b){
    double resp[5];
    
    double xMax = max.getX();
    double yMax = max.getY();
    double xMin = min.getX();
    double yMin = min.getY();
    
    // defining variables
    double p1 = -(a - x);
    double p2 = -p1;
    double p3 = -(b - y);
    double p4 = -p3;

    double q1 = x - xMin;
    double q2 = xMax - x;
    double q3 = y - yMin;
    double q4 = yMax - y;
    
    double r1;
    double r2;
    double r3;
    double r4;
    double positivo[5];
    double negativo[5];
    
    int index = 1;
    positivo[0] = 1;
    negativo[0] = 0;
    
    if ((p1 == 0 && q1 < 0) || (p3 == 0 && q3 < 0)) { //    Totalmente fora da window (valor 1)
        resp[0] = 1;                  
        return resp;
    }
    if (p1 != 0) {                      //Bordas Laterais
        r1 = q1 / p1;
        r2 = q2 / p2;
        
        if (p1 < 0) {
            negativo[index] = r1; // for negative p1, add it to negative array
            positivo[index] = r2; // and add p2 to positive array
            index++;
        } else {
            negativo[index] = r2;
            positivo[index] = r1;
            index++;
        }
    }
    
    if (p3 != 0) {                 //Bordas Cima e Baixo
        float r3 = q3 / p3;
        float r4 = q4 / p4;
        if (p3 < 0) {
            negativo[index] = r3;
            positivo[index] = r4;
            index++;
        } else {
            negativo[index] = r4;
            positivo[index] = r3;
            index++;
        }
    }
    
    double maximo = max_min(negativo,index,true);
    double minimo = max_min(positivo,index,false);
    
    if (minimo > maximo)  { //               Totalmente Fora da window
        resp[0] = 1;
        return resp;
    }
    
    resp[1] = x + p2 * maximo;
    resp[2] = y + p4 * maximo; // computing new points

    resp[3] = x + p2 * minimo;
    resp[4] = y + p4 * minimo;
    
    return resp;

  
}

//--------------------------------Cliping de Poligono

Ponto* Window::weilerAtherton(Ponto* pontos,int sizePoligono){
    double xMax = max.getX();
    double yMax = max.getY();
    double xMin = min.getX();
    double yMin = min.getY();
    
    double* resp;
    
    Ponto* aux;
    int auxI = 0;
    
    for(int i = 0; i < sizePoligono - 1; i++ ){
        resp = cohenSutherland(pontos[i].getX(),pontos[i].getY(), pontos[i+1].getX(),pontos[i+1].getY());
        if(resp[0] != 1){
            aux[auxI] = Ponto(resp[1],resp[2]);
            auxI++;
            aux[auxI] = Ponto(resp[3],resp[4]);
            auxI++;
        }
    }
    
    resp = cohenSutherland(pontos[sizePoligono-1].getX(),pontos[sizePoligono-1].getY(), pontos[0].getX(),pontos[0].getY());
    if(resp[0] != 1){
        aux[auxI] = Ponto(resp[1],resp[2]);
        auxI++;
        aux[auxI] = Ponto(resp[3],resp[4]);
        auxI++;
    }
    
    return aux;
    
}




























    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    