

#include "ListaEnc.h"

ListaEnc::ListaEnc() {
    sizeL=0;
    sizeP=0;
    sizePL = 0;
    sizeC = 0;
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
    if(sizeP > 0 || sizeL > 0 || sizePL > 0 || sizeC > 0){
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

void ListaEnc::setPSCN(double x,double y, int pos){
    pontos[pos].setU(x);
    pontos[pos].setV(y);
}

double ListaEnc::getpU(int pos){
    double x = pontos[pos].getU();
    return x;
}

double ListaEnc::getpV(int pos){
    double y = pontos[pos].getV();
    return y;
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

double ListaEnc::getlX(int pos, bool p){
    double x;
    x = linhas[pos].getX(p);
    return x;
}

double ListaEnc::getlY(int pos,bool p){
    double y;
    y = linhas[pos].getY(p);
    
    return y;
}

double ListaEnc::getlX(std::string n, bool p){
    double x;
    for(int i = 0; i<sizeL; i++){
        if(n == linhas[i].getNome()){
            x = linhas[i].getX(p);
            return x;
        }
    }
}
double ListaEnc::getlY(std::string n,bool p){
    double y;
    for(int i = 0; i<sizeL; i++){
        if(n == linhas[i].getNome()){
            y = linhas[i].getY(p);
            return y;
        }
    }
}


void ListaEnc::setL(double x,double y, int pos,bool p){
    linhas[pos].setP(x,y,p);
}

void ListaEnc::setL(double x,double y, std::string n,bool p){
    for(int i = 0; i<sizeL; i++){
        if(n == linhas[i].getNome()){
            linhas[i].setP(x,y,p);
        }
    }
}

void ListaEnc::clearL(){
    sizeL = 0;
}

void ListaEnc::setLSCN(double x,double y, int pos,bool p){
    linhas[pos].setSCN(x,y,p);
}

double ListaEnc::getlU(int pos,bool p){
    double x = linhas[pos].getU(p);
    return x;
}
double ListaEnc::getlV(int pos,bool p){
    double y = linhas[pos].getV(p);
    return y;
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
        pl.addP(x[i],y[i]);
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
            poligonos[i].setP(x,y,pos);
        }
    }
}

void ListaEnc::setPPoligonoSCN(double x,double y,int n, int pos){
    poligonos[n].setPSCN(x,y,pos);
}

void ListaEnc::clearPL(){
    sizePL = 0;
}

double ListaEnc::getUdoPoligono(int pos,int a){
    double x = poligonos[pos].getU(a);
    return x;
}

double ListaEnc::getVdoPoligono(int pos,int a){
    double y = poligonos[pos].getV(a);
    return y;
}

Ponto* ListaEnc::getPontosPoligono(int pos){
    return poligonos[pos].getPontos();
}





//------------------------------------------------------------------------Curva
int ListaEnc::getsizeC(){
    return sizeC;
}

std::string ListaEnc::getNCurva(int pos){
    return curvas[pos].getNome();
}

int ListaEnc::getSCurva(int pos){
    int s = curvas[pos].getSize();
    return s;
}

int ListaEnc::getSCurva(std::string n){
    
    int r;
    for(int i = 0; i<sizeC; i++){
        if(n == curvas[i].getNome()){
            r = curvas[i].getSize();
            return r;
        }
    }
}

void ListaEnc::addC(double*x,double*y, std::string n, int size){
    

    curvas[sizeC] = Curva(x,y,n,size);
    sizeC += 1;
}

double ListaEnc::getXCurva(int pos,int a){
    double x = curvas[pos].getX(a);
    return x;
}

double ListaEnc::getYCurva(int pos,int a){
    double y = curvas[pos].getY(a);
    return y;
}

double ListaEnc::getXCurva(std::string n,int a){
    double r;
    for(int i = 0; i<sizeC; i++){
        if(n == curvas[i].getNome()){
            r = curvas[i].getX(a);
            return r;
        }
    }
}

double ListaEnc::getYCurva(std::string n,int a){
    double r;
    for(int i = 0; i<sizeC; i++){
        if(n == curvas[i].getNome()){
            r = curvas[i].getY(a);
            return r;
        }
    }
}


void ListaEnc::clearC(){
    sizeC = 0;
}

double ListaEnc::getUCurva(int pos,int a){
    double x = curvas[pos].getU(a);
    return x;
}

double ListaEnc::getVCurva(int pos,int a){
    double y = curvas[pos].getV(a);
    return y;
}













	 	  	     	  	      	     	 	    	        	 	
