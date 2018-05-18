
#ifndef OBJGRAFICO_H
#define OBJGRAFICO_H
#include <string>
class ObjGrafico {
public:
    ObjGrafico();
    ObjGrafico(const ObjGrafico& orig);
    virtual ~ObjGrafico();
    std::string getNome();
    void setNome(std::string dado);
    
    bool getEstado();
    bool setEstado(bool e);
protected:
    std::string nome;
    bool estado;
};

#endif /* OBJGRAFICO_H */

	 	  	     	  	      	     	 	    	        	 	