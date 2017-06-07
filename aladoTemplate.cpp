#include "alado.h"
using namespace std;

template<typename T>
Alado<T>::Alado():Monstro(){}

template<typename T>
Alado<T>::Alado(string nome,
				int vida, 
				int forca, 
				int espirito, 
				int vitalidade,
				int agilidade,
				T* next,
				T* previous)
					:
					Monstro(vida, forca, espirito, vitalidade),
					nome(nome),
					agilidade(agilidade),
					next(next),
					previous(previous)
					{}

template< typename T>
string Alado<T>::getNome(){return this->nome;}
template< typename T>
int Alado<T>::getEspecial(){return this->agilidade;}
template<typename T>
T* Alado<T>::getNext(){ return this->next;}
template<typename T>
T* Alado<T>::getPrevious(){ return this->previous;}


template< typename T>
void Alado<T>::setNome(string nome){this->nome = nome;}
template< typename T>
void Alado<T>::setEspecial(int agilidade){this->agilidade = agilidade;}
template<typename T>
void Alado<T>::setNext(T* next){ this->next = next;}
template<typename T>
void Alado<T>::setPrevious(T* previous){ this->previous = previous;}

//friend ostream& operator<<(ostream&, Alado&);

//friend istream& operator>>(istream&, Alado&);




