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

//----------------------------------------------------------------------------------------------------Rotação window
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

typedef int OutCode;
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

OutCode Window::getQuadrante(double x, double y){
    double xmax = max.getX();
    double ymax = max.getY();
    double xmin = min.getX();
    double ymin = min.getY();
    
    OutCode code;

	code = INSIDE;          // initialised as being inside of [[clip window]]

	if (x < xmin)           // to the left of clip window
		code |= LEFT;
	else if (x > xmax)      // to the right of clip window
		code |= RIGHT;
	if (y < ymin)           // below the clip window
		code |= BOTTOM;
	else if (y > ymax)      // above the clip window
		code |= TOP;

	return code;
}

Linha Window::cohenSutherland(double x0, double y0, double x1, double y1){

    double xmax = max.getX();
    double ymax = max.getY();
    double xmin = min.getX();
    double ymin = min.getY();
    
    Linha l = Linha(x0, y0, x1 ,y1);
    l.setEstado(false);
    
    	// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
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

			if (outcodeOut & TOP) {           // point is above the clip window
				x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
				y = ymax;
			} else if (outcodeOut & BOTTOM) { // point is below the clip window
				x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
				y = ymin;
			} else if (outcodeOut & RIGHT) {  // point is to the right of clip window
				y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
				x = xmax;
			} else if (outcodeOut & LEFT) {   // point is to the left of clip window
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

// this function gives the minimum
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
    
    double xmax = max.getX();
    double ymax = max.getY();
    double xmin = min.getX();
    double ymin = min.getY();
    
    // defining variables
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
            negarr[negind++] = r1; // for negative p1, add it to negative array
            posarr[posind++] = r2; // and add p2 to positive array
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
    rn1 = maxi(negarr, negind); // maximum of negative array
    rn2 = mini(posarr, posind); // minimum of positive array

    if (rn1 > rn2)  { // reject
        return l;
    }
    
    l.setEstado(true);
    
    xn1 = x1 + p2 * rn1;
    yn1 = y1 + p4 * rn1; // computing new points
    l.setP(xn1,yn1,true);

    xn2 = x1 + p2 * rn2;
    yn2 = y1 + p4 * rn2;
    l.setP(xn2,yn2,false);
    
    return l;
  
}





























    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    