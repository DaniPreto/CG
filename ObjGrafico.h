
#ifndef OBJGRAFICO_H
#define OBJGRAFICO_H
#include <string>
#include <vector>
#include "Coordenada.h"

#include "Faceta.h"

typedef std::vector<Coordenada> Coordenadas;
typedef std::vector<Faceta> Facetas;

class ObjGrafico {
public:
    ObjGrafico(){}
    ObjGrafico(std::string n){}
    ObjGrafico(const ObjGrafico& orig){}
    virtual ~ObjGrafico(){}
    
    std::string getNome(){return nome;}
    void setNome(std::string dado){nome = dado;}
    
    bool getEstado(){return estado;}
    bool setEstado(bool e){estado = e;}
    
    int getTipo(){return tipo;}
    
    Coordenadas& getCoords() {return m_coords;}
    Coordenada& getCoord(int index) { return m_coords[index]; }
    int getCoordsSize() const { return m_coords.size(); }
    void addCoordenada(const Coordenada& p) { m_coords.push_back(p); }
    void setCoordenada(const Coordenada& p,int index) { m_coords[index] = p; }
    
    void addFaceta(const Faceta& f) { m_facets.push_back(f); }
    Faceta& getFaceta(int index) {return m_facets[index];}
    int getFacetsSize() { return m_facets.size(); }
    
    Coordenada getCentro(){
        double x =0;double y=0;double z=0;
        for(int i = 0;i<m_coords.size();i++){
            x += m_coords[i].x;
            y += m_coords[i].y;
            z += m_coords[i].z;
        }
        x = x/m_coords.size();
        y = y/m_coords.size();
        z = z/m_coords.size(); 
        Coordenada c(x,y,z);
        return c;
    }
    
    
protected:
    std::string nome;
    bool estado;
    int tipo;
    
    Coordenadas m_coords;
    Facetas m_facets;

};

#endif 

	 	  	     	  	      	     	 	    	        	 	