#ifndef LISTENC_HPP
#define LISTENC_HPP

#include "Elemento.hpp"

template<typename T>
class ListEnc {

public:
	ListEnc();
	~ListEnc();
	// inicio
	void adicionaNoInicio(const T& dado);
	T retiraDoInicio();
	void eliminaDoInicio();
	// posicao
	void adicionaNaPosicao(const T& dado, int pos);
	int posicao(const T& dado) const;
	T* posicaoMem(const T& dado) const;
	bool contem(const T& dado);
	T retiraDaPosicao(int pos);
	//fim
	void adiciona(const T& dado);
	T retira();
	// especifico
	T retiraEspecifico(const T& dado);
	void adicionaEmOrdem(const T& data);
	bool listaVazia() const;
	bool igual(T dado1, T dado2);
	bool igual(T dado1, T dado2) const; // Necessaria internamente
	bool maior(T dado1, T dado2);
	bool menor(T dado1, T dado2);
	void destroiLista();

protected:
    // Necessarias para percorrer a lista
	Elemento<T>* getHead(){ return head; }	 	  	     	  	      	     	 	    	        	 	
	int getSize() const { return this->size; }
	T getFromPos(int pos);

private:
	Elemento<T>* head;
	int size;
};

template <typename T>
T ListEnc<T>::getFromPos(int pos){
    if(pos >= 0 && pos < this->size){
        if(pos == 0)
            return this->head->getInfo();
    
        int i;
        Elemento<T> *temporario = this->head;
        for (i = 0; i < pos; i++) {
            temporario = temporario->getProximo();
        }
        return temporario->getInfo();
    }
}

template <typename T>
ListEnc<T>::ListEnc() {
    this->head = NULL;
    this->size = 0;
}

template <typename T>
ListEnc<T>::~ListEnc() {
    this->destroiLista();
}

template <typename T>
bool ListEnc<T>::listaVazia() const {	 	  	     	  	      	     	 	    	        	 	
    return this->size == 0;
}

template <typename T>
void ListEnc<T>::adicionaNoInicio(const T& valor) {
    Elemento<T> *ult = this->head;
    this->head = new Elemento<T>(valor, ult);
    this->size++;
}


template <typename T>
void ListEnc<T>::adiciona(const T& valor) {
    if (this->listaVazia()) {
        return this->adicionaNoInicio(valor);
    }
    int i, max;
    Elemento<T> *temporario = this->head;
    max = this->size - 1;
    for (i = 0; i < max; i++) {
        temporario = temporario->getProximo();
    }
    Elemento<T> *novo = new Elemento<T>(valor, NULL);
    temporario->setProximo(novo);
    this->size++;
}

template <typename T>
void ListEnc<T>::adicionaNaPosicao(const T& valor, int posicao) {
    if(posicao >= 0 && posicao < this->size){
        if (posicao == 0) {
            return this->adicionaNoInicio(valor);
        }
    
        Elemento<T> *temporario = this->head;
        Elemento<T> *novo;
        posicao = posicao - 1;
        int h;
        for (h = 0; h < posicao; h++) {	 	  	     	  	      	     	 	    	        	 	
            temporario = temporario->getProximo();
        }
        
        novo = temporario->getProximo();
        temporario->setProximo(new Elemento<T>(valor, novo));
        this->size++;
    }
}


template <typename T>
void ListEnc<T>::adicionaEmOrdem(const T& data) {
    if (this->listaVazia()) {
        return this->adicionaNoInicio(data);
    }
    Elemento<T> *temporario = this->head;
    int max, posicao;
    max = this->size - 1;
    posicao = 0;
    while (posicao < max && this->maior(data, temporario->getInfo())) {
        temporario = temporario->getProximo();
        posicao++;
    }
    if (this->maior(data, temporario->getInfo())) {
        this->adicionaNaPosicao(data, posicao+1);
    } else {
        this->adicionaNaPosicao(data, posicao);
    }
}

template <typename T>
void ListEnc<T>::destroiLista() {
    Elemento<T> *atual = this->head;
    int i, max;
    max = this->size;
    for (i = 0; i < max; i++) {	 	  	     	  	      	     	 	    	        	 	
        Elemento<T> *anterior = atual;
        atual = atual->getProximo();
        delete anterior;
    }
    this->head = NULL;
    this->size = 0;
}

template <typename T>
T ListEnc<T>::retiraDaPosicao(int pos) {
    int n = this->size;
    if((pos >= 0) && (pos < n)){
        if (pos == 0) {
            return this->retiraDoInicio();
        }
    
        int i;
        Elemento<T> *temporario = this->head;
        pos -= 1;
        for (i = 0; i < pos; i++) {
            temporario = temporario->getProximo();
        }
        Elemento<T> *proximo = temporario->getProximo();
        T valor = proximo->getInfo();
        temporario->setProximo(proximo->getProximo());
        delete proximo;
        this->size--;
        return valor;
    }
}

template <typename T>
T ListEnc<T>::retiraEspecifico(const T& dado) {
    int posicao = this->posicao(dado);
    return this->retiraDaPosicao(posicao);
}	 	  	     	  	      	     	 	    	        	 	

template <typename T>
int ListEnc<T>::posicao(const T& dado) const {
    int i;
    Elemento<T> *temporario = this->head;
    for (i = 0; i < this->size; i++) {
        if (igual(temporario->getInfo(), dado)) {
            return i;
        }
        temporario = temporario->getProximo();
    }
    
}


template <typename T>
T* ListEnc<T>::posicaoMem(const T& dado) const {
    int i;
    Elemento<T> *temporario = this->head;
    for (i = 0; i < this->size; i++) {
        if (igual(temporario->getInfo(), dado)) {
            return temporario->getInfoPointer();
        }
        temporario = temporario->getProximo();
    }
}


template <typename T>
T ListEnc<T>::retiraDoInicio() {
    if(this->size > 0){
        Elemento<T> *inicio = this->head;
        T valor = inicio->getInfo();
        this->head = inicio->getProximo();
        this->size--;
        delete inicio;
        return valor;
    }	 	  	     	  	      	     	 	    	        	 	
}

template <typename T>
T ListEnc<T>::retira() {
    
    if(this->size > 0){
        int i, max;
        Elemento<T> *temporario = this->head;
        max = this->size - 2;
        if (max < 0) {
            return this->retiraDoInicio();
        }
        
        for (i = 0; i < max; i++) {
            temporario = temporario->getProximo();
        }
        Elemento<T> *proximo = temporario->getProximo();
        T valor = proximo->getInfo();
        temporario->setProximo(proximo->getProximo());
        delete proximo;
        this->size--;
        return valor;
    }
}

template <typename T>
void ListEnc<T>::eliminaDoInicio() {
    if(this->size > 0){
        Elemento<T> *inicio = this->head;
        Elemento<T> *atual = inicio->getProximo();
        this->head = atual;
        this->size--;
        delete inicio;
    }
}

template <typename T>
bool ListEnc<T>::contem(const T& dado) {	 	  	     	  	      	     	 	    	        	 	
    int i;
    Elemento<T> *temporario = this->head;
    for (i = 0; i < this->size; i++) {
        if (igual(temporario->getInfo(), dado)) {
            return true;
        }
        temporario = temporario->getProximo();
    }
    return false;
}


/*
template <typename T>
bool ListaEnc<T>::igual(T dado1, T dado2) {
    if(dado1 != NULL && dado2 == NULL) return false;
    if(dado2 != NULL && dado1 == NULL) return false;

    // Melhor jeito que encontrei para proteger o codigo
    // caso o T seja ou nao um ponteiro.
    // [Classe T precisa implementar operador '*']
    return *dado1 == *dado2;
}

template <typename T>
bool ListaEnc<T>::igual(T dado1, T dado2) const {
    if(dado1 != NULL && dado2 == NULL) return false;
    if(dado2 != NULL && dado1 == NULL) return false;

    // Melhor jeito que encontrei para proteger o codigo
    // caso o T seja ou nao um ponteiro.
    // [Classe T precisa implementar operador '*']
    return *dado1 == *dado2;
}
*/

template <typename T>
bool ListEnc<T>::maior(T dado1, T dado2) {	 	  	     	  	      	     	 	    	        	 	
    if (dado1 > dado2) {
        return true;
    }
    return false;
}

template <typename T>
bool ListEnc<T>::menor(T dado1, T dado2) {
    if (dado1 < dado2) {
        return true;
    }
    return false;
}

#endif