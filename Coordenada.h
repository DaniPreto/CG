#ifndef COORDENADA_H
#define COORDENADA_H


class Coordenada
{
    public:
        Coordenada(){}
        Coordenada(double cx, double cy, double cz) :
            x(cx), y(cy), z(cz){}
        virtual ~Coordenada(){};
        double x = 0, y = 0, z = 0;
        
    private:
};

#endif
