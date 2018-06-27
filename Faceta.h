#ifndef FACETA_H
#define FACETA_H


class Faceta
{
    public:
        Faceta(){}
        Faceta(std::vector<int> m){m_vertices = m;}
        virtual ~Faceta(){};

        int getVertice(int index){return m_vertices[index];}
        void setVertice(int v,int index){m_vertices[index] = v;}
        void addVertice(int v){m_vertices.push_back(v);}
        int getSize() const { return m_vertices.size(); }
        

    private:
        std::vector<int> m_vertices;
        
        
};

#endif 
