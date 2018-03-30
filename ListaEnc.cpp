

#include "ListaEnc.h"

ListaEnc::ListaEnc() {
    sizeL=0;
    sizeP=0;
    sizePL = 0;
}

ListaEnc::ListaEnc(const ListaEnc& orig) {
}

ListaEnc::~ListaEnc() {
}


bool ListaEnc::temObjeto(std::string n){
    for(int i = 0; i<sizeP; i++){
        if(n == pontos[i].getNome())
            return true;
    }
    
    for(int i = 0; i<sizeL; i++){
        if(n == linhas[i].getNome())
            return true;
    }
    for(int i = 0; i<sizePL; i++){
        if(n == poligonos[i].getNome())
           return true;
    }
    return false;
}

int ListaEnc::getType(std::string n){
    for(int i = 0; i<sizeP; i++){
        if(n == pontos[i].getNome())
            return 0;
    }
    
    for(int i = 0; i<sizeL; i++){
        if(n == linhas[i].getNome())
            return 1;
    }
    for(int i = 0; i<sizePL; i++){
        if(n == poligonos[i].getNome())
           return 2;
    }
    return 0;
}

bool ListaEnc::isEmpty(){
    if(sizeP > 0 || sizeL > 0 || sizePL > 0){
        return false;
    }
    return true;
}


//------------------------------------------------------Ponto

int ListaEnc::getsizeP(){
    return sizeP;
}
    

std::string ListaEnc::getNP(int pos){
    return pontos[pos].getNome();
}

void ListaEnc::addP(double x, double y,std::string n){
    pontos[sizeP] = Ponto(x,y,n);
    sizeP = sizeP + 1;

}	 	  	     	  	      	     	 	    	        	 	
    
double ListaEnc::getpX(int pos){
    double x = pontos[pos].getX();
    return x;
}

double ListaEnc::getpY(int pos){
    double y = pontos[pos].getY();
    return y;
}

double ListaEnc::getpX(std::string n){
    double x;
    for(int i = 0; i<sizeP; i++){
        if(n == pontos[i].getNome())
            x = pontos[i].getX();
            return x;
    }
}

double ListaEnc::getpY(std::string n){
    double y;
    for(int i = 0; i<sizeP; i++){
        if(n == pontos[i].getNome())
            y = pontos[i].getY();
            return y;
    }
}


void ListaEnc::setP(double x,double y, int pos){
    pontos[pos].setX(x);
    pontos[pos].setY(y);
}

void ListaEnc::setP(double x,double y, std::string n){
    for(int i = 0; i<sizeP; i++){
        if(n == pontos[i].getNome()){
            pontos[i].setX(x);
            pontos[i].setY(y);
        }
    }
}

void ListaEnc::clearP(){
    sizeP = 0;
}

//--------------------------------------------------------------------------Linha
int ListaEnc::getsizeL(){
    return sizeL;
}	 	

std::string ListaEnc::getNL(int pos){
    return linhas[pos].getNome();
}

void ListaEnc::addL(double a, double b,double c, double d, std::string n){
    linhas[sizeL] = Linha(a,b,c,d,n);
    sizeL = sizeL + 1;
    
}

double ListaEnc::getlX(int pos){
    double x = linhas[pos].getP1X();
    return x;
}
double ListaEnc::getlY(int pos){
    double y = linhas[pos].getP1Y();
    return y;
}
double ListaEnc::getlX2(int pos){
    double x = linhas[pos].getP2X();
    return x;
}
double ListaEnc::getlY2(int pos){
    double y = linhas[pos].getP2Y();
    return y;
}
double ListaEnc::getlX(std::string n){
    double x;
    for(int i = 0; i<sizeL; i++){
        if(n == linhas[i].getNome()){
            x = linhas[i].getP1X();
            return x;
        }
    }
}
double ListaEnc::getlY(std::string n){
    double y;
    for(int i = 0; i<sizeL; i++){
        if(n == linhas[i].getNome()){
            y = linhas[i].getP1Y();
            return y;
        }
    }
}
double ListaEnc::getlX2(std::string n){
    double x;
    for(int i = 0; i<sizeL; i++){
        if(n == linhas[i].getNome()){
            x = linhas[i].getP2X();
            return x;
        }
    }
}
double ListaEnc::getlY2(std::string n){
    double y;
    for(int i = 0; i<sizeL; i++){
        if(n == linhas[i].getNome()){
            y = linhas[i].getP2Y();
            return y;
        }
    }
}


void ListaEnc::setL(double x,double y, int pos){
    linhas[pos].setP1(x,y);
}
void ListaEnc::setL1(double x,double y, int pos){
    linhas[pos].setP2(x,y);
}

void ListaEnc::setL(double x,double y, std::string n){
    for(int i = 0; i<sizeL; i++){
        if(n == linhas[i].getNome()){
            linhas[i].setP1(x,y);
        }
    }
}
void ListaEnc::setL1(double x,double y, std::string n){
    for(int i = 0; i<sizeL; i++){
        if(n == linhas[i].getNome()){
            linhas[i].setP2(x,y);
        }
    }
}

void ListaEnc::clearL(){
    sizeL = 0;
}

//------------------------------------------------------------------------Poligono
int ListaEnc::getsizePL(){
    return sizePL;
}

std::string ListaEnc::getNdoPoligono(int pos){
    return poligonos[pos].getNome();
}

int ListaEnc::getSdoPoligono(int pos){
    int s = poligonos[pos].getSize();
    return s;
}

int ListaEnc::getSdoPoligono(std::string n){
    
    double r;
    for(int i = 0; i<sizePL; i++){
        if(n == poligonos[i].getNome()){
            r = poligonos[i].getSize();
            return r;
        }
    }
}

void ListaEnc::addPL(int a, double x[], double y[], std::string n){
    Poligono pl(a,n);
    for(int i =0; i<a;i++){
        pl.addP(Ponto(x[i],y[i]));
    }
    poligonos[sizePL] = pl;
    sizePL += 1;
}

double ListaEnc::getXdoPoligono(int pos,int a){
    double x = poligonos[pos].getX(a);
    return x;
}

double ListaEnc::getYdoPoligono(int pos,int a){
    double y = poligonos[pos].getY(a);
    return y;
}

double ListaEnc::getXdoPoligono(std::string n,int a){
    double r;
    for(int i = 0; i<sizePL; i++){
        if(n == poligonos[i].getNome()){
            r = poligonos[i].getX(a);
            return r;
        }
    }
}

double ListaEnc::getYdoPoligono(std::string n,int a){
    double r;
    for(int i = 0; i<sizePL; i++){
        if(n == poligonos[i].getNome()){
            r = poligonos[i].getY(a);
            return r;
        }
    }
}

void ListaEnc::setPPoligono(double x,double y,std::string n, int pos){
    for(int i = 0; i<sizePL; i++){
        if(n == poligonos[i].getNome()){
            poligonos[i].setP(Ponto(x,y),pos);
        }
    }
}

void ListaEnc::clearPL(){
    sizePL = 0;
}













	 	  	     	  	      	     	 	    	        	 	
