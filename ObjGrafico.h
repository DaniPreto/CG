
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
protected:
    std::string nome;
};

#endif /* OBJGRAFICO_H */

	 	  	     	  	      	     	 	    	        	 	
