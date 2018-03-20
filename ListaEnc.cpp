

#include "ListaEnc.h"

ListaEnc::ListaEnc() {
    sizeL=0;
    sizeP=0;
}

ListaEnc::ListaEnc(const ListaEnc& orig) {
}

ListaEnc::~ListaEnc() {
}

int ListaEnc::getsizeP(){
    return sizeP;
}
int ListaEnc::getsizeL(){
    return sizeL;
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
    return false;
}

void ListaEnc::addP(int x, int y,string n){
    pontos[sizeP] = Ponto(x,y,n);
    sizeP = sizeP + 1;

}	 	  	     	  	      	     	 	    	        	 	
void ListaEnc::addL(int a, int b,int c, int d, string n){
    linhas[sizeL] = Linha(a,b,c,d,n);
    sizeL = sizeL + 1;
    
}
    
int ListaEnc::getpX(int pos){
    int x = pontos[pos].getX();
    return x;
}

int ListaEnc::getpY(int pos){
    int y = pontos[pos].getY();
    return y;
}
int ListaEnc::getlX(int pos){
    int x = linhas[pos].getP1X();
    return x;
}
int ListaEnc::getlY(int pos){
    int y = linhas[pos].getP1Y();
    return y;
}
int ListaEnc::getlX2(int pos){
    int x = linhas[pos].getP2X();
    return x;
}
int ListaEnc::getlY2(int pos){
    int y = linhas[pos].getP2Y();
    return y;
}
    
void ListaEnc::clearL(){
    sizeL = 0;
}
void ListaEnc::clearP(){	 	  	     	  	      	     	 	    	        	 	
    sizeP = 0;
}

