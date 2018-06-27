#ifndef DESCRITOROBJ_H
#define DESCRITOROBJ_H

#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <map>
#include <string>

#include "Coordenada.h"
#include "Faceta.h"
#include "ObjGrafico.h"


class DescritorObj{
    public:
        DescritorObj(std::string& filename);
        std::vector<ObjGrafico*>& getObjs(){ return m_objs; }

    private:
        void loadObjs();
        void setName(std::stringstream& line);
        void addCoord(std::stringstream& line);
        void addPonto(std::stringstream& line);
        void addPoligono(std::stringstream& line);
        void addFaceta(std::stringstream& line);
        void addWindow(std::stringstream& line);
        void addObj3D();
        void loadCoordsIndexes(std::stringstream& line, Coordenadas& objCoords);
        void loadIndexes(std::stringstream& line, std::vector<int>& objCoords);


    private:
        std::vector<ObjGrafico*> m_objs;
        Coordenadas m_coords;// Todas as coordenadas lidas do arquivo
        Facetas m_faces;
        int m_numSubObjs = 0; // Numero de objetos em seguida com o mesmo nome
        std::string m_name;// Nome do arquivo, sem a extensao
        std::fstream m_objsFile;
};

DescritorObj::DescritorObj(std::string& filename){	 	  	     	  	      	     	 	    	        	 	
    
    std::string arquivo;
    
    m_name = filename;
    arquivo = filename + ".obj" ;
        
    m_objsFile.open(arquivo);
    

    loadObjs();
}


void DescritorObj::addObj3D(){
    
    m_objs.push_back(new Objeto3D(m_name,m_coords, m_faces));
    m_faces.clear();
}

void DescritorObj::loadObjs(){
    std::string tmp, keyWord;
    while(std::getline(m_objsFile, tmp)){
        if(tmp.size() <= 1) continue;
        
        std::stringstream line(tmp);
        line >> keyWord;

        if(keyWord == "#")              { /* Não faz nada... */ }
        else if(keyWord == "mtllib")    { /* Não faz nada... */}
        else if(keyWord == "usemtl")    { /* Não faz nada... */}
        else if(keyWord == "v")         { addCoord(line); }                          // v x y z
        else if(keyWord == "o")         { setName(line); }                           // o obj_name
        else if(keyWord == "w")         { addWindow(line); }
        else if(keyWord == "p")         { addPonto(line); }                            // p v1 v2 v3 ...
        else if(keyWord == "l")         { addPoligono(line); }                         // l v1 v2 v3 ...
        else if(keyWord == "f")         { addFaceta(line); }                           // f v1/vt1/vn1 v2/vt2/vn2 ...
        else if(keyWord == "curv")      { /* Não faz nada... */ }                          // curv u1 u2 v1 v2 v3 ...
        else if(keyWord == "cstype")    {/* Não faz nada... */}	 	  	     	  	      	     	 	    	        	 	

        else if(keyWord == "end")       { /* Não faz nada... */}
        else if(keyWord == "deg")       { /* Não faz nada... */ }                    // deg degu
        else if(keyWord == "g")         { /* Não faz nada... */ }                    // g group_name
        else if(keyWord == "vt")        { /* Não faz nada... */ }                    // vt u v w
        else if(keyWord == "vn")        { /* Não faz nada... */ }                    // vn i j k
        else if(keyWord == "vp")        { /* Não faz nada... */ }                    // vp u v w
    }
    // Se chegar ao final e tiver alguma Face salva, ela pertence ao ultimo objeto 3D
    if(m_faces.size() != 0)
        addObj3D();
}

void DescritorObj::setName(std::stringstream& line){
    // Caso o nome do objeto mude, deve-se checar se ja foi carregado alguma Face. Caso tenha sido, deve-se então criar o objeto 3D com as Faces carregadas até agora
    if(m_faces.size() != 0)
        addObj3D();

    line >> m_name;
    m_numSubObjs = 0;
}


void DescritorObj::addCoord(std::stringstream& line){
    double x=0;
    double y=0;
    double z=0;
    line >> x >> y >> z;
    m_coords.push_back(Coordenada(x,y,z));
}

void DescritorObj::addPonto(std::stringstream& line){
    if(m_faces.size() != 0)
        addObj3D();

    Coordenadas objCoords;
    loadCoordsIndexes(line, objCoords);

    std::string name = m_numSubObjs == 0 ? m_name :
        m_name+"_ponto"+std::to_string(m_numSubObjs);

    // Pode-se declarar varios pontos em uma mesma linha 'p'
    for(auto &c : objCoords){	 	  	     	  	      	     	 	    	        	 	
        m_objs.push_back(new Ponto(name, c));
        m_numSubObjs++;
        name = m_name+"_ponto"+std::to_string(m_numSubObjs);
    }
}

void DescritorObj::addPoligono(std::stringstream& line){
    if(m_faces.size() != 0)
        addObj3D();

    Coordenadas objCoords;
    loadCoordsIndexes(line, objCoords);

    std::string name = m_numSubObjs == 0 ? m_name :
        m_name+"_"+std::to_string(m_numSubObjs);

    if(objCoords.size() == 2)
        m_objs.push_back(new Linha(name, objCoords));
    else
        m_objs.push_back(new Poligono(name,objCoords));
    
    m_numSubObjs++;
}

void DescritorObj::addFaceta(std::stringstream& line){
    std:vector<int> objCoords;
    loadIndexes(line, objCoords);
    m_faces.push_back(Faceta(objCoords));
}

void DescritorObj::addWindow(std::stringstream& line){
    if(m_faces.size() != 0)
        addObj3D();

    Coordenadas objCoords;
    loadCoordsIndexes(line, objCoords);
    
    m_objs.push_back(new Window(objCoords));


}	 	  	     	  	      	     	 	    	        	 	


void DescritorObj::loadCoordsIndexes(std::stringstream& line, Coordenadas& objCoords){
    std::string pointString;
    int index = 0;
    int size = m_coords.size();
    std::size_t found;
    std::string point;

    while(line >> pointString){
        // Algoritmo vai pegar o vertice 'v' e vai ignorar os outros [v/vt/vn]
        point = pointString;
        found = pointString.find_first_of('/');
        if(found != std::string::npos){
            point = pointString.substr(0, found - 1);
        }
        found = pointString.find_first_of('/');
        index = atoi(point.c_str());

        

        // Processa o index do vertice
        if(index < 0)
            index = size + index;
        else
            index--;
        if(index < 0 || index >= size){
            break;
        }
        objCoords.push_back(m_coords[index]);
    }
}

void DescritorObj::loadIndexes(std::stringstream& line, std::vector<int>& objCoords){
    std::string pointString;
    int index = 0;
    int size = m_coords.size();
    std::size_t found;
    std::string point;
    
    while(line >> pointString){	 	  	     	  	      	     	 	    	        	 	
        found = pointString.find_first_of('/');
        point = pointString;
        if(found != std::string::npos){
            point = pointString.substr(0, found - 1);
        }
        index = atoi(point.c_str());

        // Processa o index do vertice
        if(index < 0)
            index = size + index + 1;
        
        if(0 < index < size){
            objCoords.push_back(index);
        }
           
    }
 }

#endif
