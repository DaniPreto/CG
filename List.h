#ifndef LIST_H
#define LIST_H

#include "ListEnc.hpp"
#include "ObjGrafico.h"
#include <string>
class List : public ListEnc<ObjGrafico*>
{
    public:
        List(){}
        virtual ~List() {}

        ObjGrafico* getObj(int pos){ return getFromPos(pos); }
        ObjGrafico* getObj(ObjGrafico *obj){ return *posicaoMem(obj); }
        Elemento<ObjGrafico*>* getFirstElement(){ return getHead(); }

        void addObj(ObjGrafico *obj){ adiciona(obj); }
        void removeObj(ObjGrafico *obj){ delete retiraEspecifico(obj); }
        int size() const { return getSize(); }
        bool contains(ObjGrafico *obj){ return contem(obj); }
        void clearL(){destroiLista();}
        
};

#endif // Lista_H