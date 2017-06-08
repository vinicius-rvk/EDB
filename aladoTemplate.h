#ifndef _ALADO_HPP_
#define _ALADO_HPP_

#include <iostream>
#include "monstro.h"

using namespace std;

template<typename T>
class Alado: public Monstro{
private:
	string nome;
	int agilidade;
	T* next;
	T* previous;
public:
	Alado();
	Alado(string, int, int, int, int, int, T*, T*);
	
	string getNome();
	int getEspecial();
	T* getNext();
	T* getPrevious();

	void setNome(string );
	void setEspecial(int);
	void setNext(T*);
	void setPrevious(T*);

	//friend ostream& operator<<(ostream&, Alado&);

	//friend istream& operator>>(istream&, Alado&);

};

#endif //_ALADO_HPP_