#include "Window.h"


Window::Window() {
}

Window::Window(const Window& orig) {
}

Window::~Window() {
}

Window::Window(double x, double y, double c,double d){
    Coordenada b1(c,y,0);
    Coordenada max(x,y,0);
    Coordenada min(c,d,0);
    Coordenada b2(x,d,0);
    
    addCoordenada(b1);
    addCoordenada(max);
    addCoordenada(min);
    addCoordenada(b2);
    

    estado = false;
    teta = 0;
    tipo = 5;

    }
    
Window::Window(Coordenadas c){
    m_coords = c;
    tipo = 5;
}

void Window::setMAX(double x, double y){
    m_coords[1].x = x;
    m_coords[1].y = y;
    
    m_coords[3].x = x;
    m_coords[0].y = y;
    }	 	  	     	  	      	     	 	    	        	 	

void Window::setMIN(double x, double y){
    m_coords[2].x = x;
    m_coords[2].y = y;
    
    m_coords[0].x = x;
    m_coords[3].y = y;
    }	 

double Window::getXmax(){
    return m_coords[1].x;
    }

double Window::getXmin(){
    return m_coords[2].x;
    }
    
double Window::getYmax(){
    return m_coords[1].y;
    }

double Window::getYmin(){
    return m_coords[2].y;
    }

//----------------------------------------------------------------------------------------------------Rotação window
void Window::rotate(){
    estado = true;
}

    

double Window::getXcentro(){
    double x = (m_coords[0].x + m_coords[1].x + m_coords[2].x + m_coords[3].x)/4;
    return x;
    }	 	  	     	  	      	     	 	    	        	 	

double Window::getYcentro(){
    double y = (m_coords[0].y + m_coords[1].y + m_coords[2].y + m_coords[3].y)/4;
    return y;
    }
    
double Window::getZcentro(){
    double z = (m_coords[0].z + m_coords[1].z + m_coords[2].z + m_coords[3].z)/4;
    return z;
    }
    
    
void Window::addTeta(double v){
    teta += v;
    
    if(teta == 0)
        estado = false;
    else
        estado = true;
}
double Window::getTeta(){
    return teta;
}   

//--------------------------------------------------------------------------------Clipagem-----------------------------------
bool Window::clipPonto(double x, double y){
    double xmax = m_coords[1].x;
    double ymax = m_coords[1].y;
    double xmin = m_coords[2].x;
    double ymin = m_coords[2].y;
    
    if(estado){
        xmax = 1;
        ymax = 1;
        xmin = -1;
        ymin = -1;
    }	 	  	     	  	      	     	 	    	        	 	
    
    if(x < xmax && x > xmin && y < ymax && y > ymin){
        return true;
    }
    
    return false;
}

typedef int OutCode;
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

OutCode Window::getQuadrante(double x, double y){
    double xmax = m_coords[1].x;
    double ymax = m_coords[1].y;
    double xmin = m_coords[2].x;
    double ymin = m_coords[2].y;
    
    if(estado){
        xmax = 1;
        ymax = 1;
        xmin = -1;
        ymin = -1;
        
    }
    
    OutCode code;

	code = INSIDE;          // iinicializa em 0, como se estivesse dentro da window

	if (x < xmin)           // esquerda
		code |= LEFT;
	else if (x > xmax)      // direita
		code |= RIGHT;
	if (y < ymin)           // abaixo
		code |= BOTTOM;
	else if (y > ymax)      // cima
		code |= TOP;

	return code;
}	 	  	     	  	      	     	 	    	        	 	

Linha Window::cohenSutherland(double x0, double y0, double x1, double y1){
    
    double xmax = m_coords[1].x;
    double ymax = m_coords[1].y;
    double xmin = m_coords[2].x;
    double ymin = m_coords[2].y;
    
    if(estado){
        xmax = 1;
        ymax = 1;
        xmin = -1;
        ymin = -1;
        
    }
    
    Linha l = Linha(x0, y0, x1 ,y1);
    l.setEstado(false);
    
    	// pega os outcodes dos pontos
	OutCode outcode0 = getQuadrante(x0, y0);
	OutCode outcode1 = getQuadrante(x1, y1);
	bool accept = false;

	while (true) {
		if (!(outcode0 | outcode1)) {
			l.setEstado(true);
			break;
		} else if (outcode0 & outcode1) {
			break;
		} else {
			double x, y;

			OutCode outcodeOut = outcode0 ? outcode0 : outcode1;

			if (outcodeOut & TOP) {           //cima
				x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
				y = ymax;
			} else if (outcodeOut & BOTTOM) { // baixo
				x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
				y = ymin;
			} else if (outcodeOut & RIGHT) {  // direita
				y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
				x = xmax;
			} else if (outcodeOut & LEFT) {   // esquerda
				y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
				x = xmin;
			}	 	  	     	  	      	     	 	    	        	 	

			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == outcode0) {
				l.setP(x,y,true);
				x0 = x;
				y0 = y;
				outcode0 = getQuadrante(x0, y0);
			} else {
			    l.setP(x,y,false);
				x1 = x;
				y1 = y;
				outcode1 = getQuadrante(x1, y1);
			}
		}
	}
	    
	return l;
        
}


double Window::maxi(double arr[],int n) {
  double m = 0;
  for (int i = 0; i < n; ++i)
    if (m < arr[i])
        m = arr[i];
  return m;
}


double Window::mini(double arr[], int n) {
  double m = 1;
  for (int i = 0; i < n; ++i)
    if (m > arr[i])
        m = arr[i];
  return m;
}	 	  	     	  	      	     	 	    	        	 	


Linha Window::liangBarsky(double x1, double y1, double x2, double y2){
    
    Linha l = Linha(x1, y1, x2 ,y2);
    l.setEstado(false);
    
    double xmax = m_coords[1].x;
    double ymax = m_coords[1].y;
    double xmin = m_coords[2].x;
    double ymin = m_coords[2].y;
    
    if(estado){
        xmax = 1;
        ymax = 1;
        xmin = -1;
        ymin = -1;
    }
    
    // define variaveis
    double p1 = -(x2 - x1);
    double p2 = -p1;
    double p3 = -(y2 - y1);
    double p4 = -p3;
  
    double q1 = x1 - xmin;
    double q2 = xmax - x1;
    double q3 = y1 - ymin;
    double q4 = ymax - y1;

    double posarr[5], negarr[5];
    int posind = 1, negind = 1;
    posarr[0] = 1;
    negarr[0] = 0;
    
    if ((p1 == 0 && q1 < 0) || (p3 == 0 && q3 < 0)) {	 	  	     	  	      	     	 	    	        	 	
      return l;
    }
    if (p1 != 0) {
        double r1 = q1 / p1;
        double r2 = q2 / p2;
        if (p1 < 0) {
            negarr[negind++] = r1; // para p1 negativo, adiciona no vetor de negativos
            posarr[posind++] = r2; // e adiciona p2 nos positivos
        } else {
            negarr[negind++] = r2;
            posarr[posind++] = r1;
        }
    }
    if (p3 != 0) {
        double r3 = q3 / p3;
        double r4 = q4 / p4;
        if (p3 < 0) {
            negarr[negind++] = r3;
            posarr[posind++] = r4;
        } else {
            negarr[negind++] = r4;
            posarr[posind++] = r3;
        }
    }

    double xn1, yn1, xn2, yn2;
    double rn1, rn2;
    rn1 = maxi(negarr, negind); // maximo negativo
    rn2 = mini(posarr, posind); // minimo positivo

    if (rn1 > rn2)  { // rejeita
        return l;
    }
    
    l.setEstado(true);
    
    xn1 = x1 + p2 * rn1;
    yn1 = y1 + p4 * rn1; // calcula novos pontos
    l.setP(xn1,yn1,true);

    xn2 = x1 + p2 * rn2;
    yn2 = y1 + p4 * rn2;
    l.setP(xn2,yn2,false);
    
    return l;
  
}	 	  	     	  	      	     	 	    	        	 	

















    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    	 	  	     	  	      	     	 	    	        	 	
