
#ifndef WINDOW_H
#define WINDOW_H
#include <iostream>
#include <vector>
#include "Linha.h"

class Window : public ObjGrafico {
public:
    typedef int OutCode; 

    Window();
    Window(const Window& orig);
    virtual ~Window();
    Window(double x, double y, double c,double d);
    Window(Coordenadas c);
    
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
    
    double getXcentro();
    double getYcentro();
    double getZcentro();
    
    bool clipPonto(double x, double y);
    Linha cohenSutherland(double x, double y, double a, double b);
    OutCode getQuadrante(double x, double y);
    
    double maxi(double arr[],int n);
    double mini(double arr[], int n);
    Linha liangBarsky(double x, double y, double a, double b);
    
    void setTransforms(Coordenadas c){m_transforms = c;}
    Coordenada getTransform(int i){return m_transforms[i];}
    
    Coordenada getCOP(){return cop;}
  
    
private:
    double teta;
    Coordenadas m_transforms;
    Coordenada cop;
    

};



#endif 

	 	  	     	  	      	     	 	    	        	 	