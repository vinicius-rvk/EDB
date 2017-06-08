//
//  HashTable.cpp
//
//  Created by Eiji Adachi Medeiros Barbosa
//

#include "HashTable.h"
#include <string>
#include <iostream>

using namespace std;

// Variável pra registrarmos que uma posição foi deletada
// Fazemos um cast de (-1) para um ponteiro para HashEntry,
// ou seja, fazemos apontar para uma região inválida
HashEntry<std::string, std::string> *ENTRY_DELETED = (HashEntry<std::string,std::string>*)(-1);

/**
 Construtor que usa tamanho default da tabela.
 */
HashTable::HashTable()
{
    this->size = TABLE_SIZE;
    init();
}

/**
 Construtor que recebe tamanho como parâmetro.
 */
HashTable::HashTable(const int size)
{
    this->size = size;
    init();
}

/**
 Método auxiliar que é chamado pelos dois construtores.
 Este método quem de fato inicia o array interno da tabela.
 */
void HashTable::init()
{
    this->data = new HashEntry<std::string, string>*[this->size];
    this->quantity = 0;
    for( int i = 0; i < this->getSize(); i++ )
    {
        // Atribui-se nulo às posições da tabela para termos certeza de que estão vazias
        this->data[i] = nullptr;
    }
}

/**
 Destroi todas as entradas da tabela e a própria tabela.
 */
HashTable::~HashTable()
{
    for( int i = 0; i < this->getSize(); i++ )
    {
        HashEntry<string, string>* entry = this->data[i];
        
        if( entry != nullptr && entry != ENTRY_DELETED )
        {
            delete entry;
        }
    }
    delete [] data;
}

/**
 Este método deve retornar o 'value' da HashEntry cuja atributo 'key' == parâmetro key.
 Caso contrário, deve retornar 'nullptr'.
 Quantidade de itens na tabela não é modificada.
 */
string HashTable::get(const string key)
{
    if(this->isEmpty())
    {
        return "";
    }
    
    unsigned long base = hash(key);

    string result = "";

    for( auto delta = 0; delta < this->getSize(); delta++)
    {
        long index = (base+delta) % this->getSize();
        HashEntry<string, string>* entry = this->data[index];

        if( entry == nullptr )
        {
            break;
        }
        else if( entry != ENTRY_DELETED && entry->getKey() == key )
        {
            result = entry->getValue();
            break;
        }
    }
    
    return result;

}

/**
 Este método deve inserir na tabela um novo HashEntry com 'key' e 'value' recebidos como parâmetros. Neste caso, a quantidade de itens na tabela deve ser incrementada e retorna-se 'true'.
 Caso já exista um HashEntry com atributo 'key' == parâmetro 'key', deve apenas atualizar o atributo 'value' do HashEntry. Neste caso, a quantidade de itens na tabela não é modificada e retorna-se 'true'.
 Caso nã
 */
bool HashTable::put(const string key, const string value)
{
    if(this->isFull())
    {
        return false;
    }
    else if( this->getAlpha() >= MAX_ALPHA )
    {
        this->expand();
    }
    
    unsigned long base = hash(key);
    
    long deletedIndex = -1;
    
    bool result = false;
    
    for(int delta = 0; delta < this->getSize(); delta++)
    {
        long index = (base + delta) % this->getSize();
        HashEntry<string, string>* entry = this->data[index];
        
        if( entry == ENTRY_DELETED )
        {
            deletedIndex = index;
        }
        else if( entry == nullptr )
        {
            HashEntry<string, string>* newEntry = new HashEntry<string, string>(key, value);
            long indexToUse = deletedIndex == -1 ? index : deletedIndex;
            this->data[indexToUse] = newEntry;
            ++this->quantity;
            result = true;
            break;
        }
        else if( entry->getKey() == key )
        {
            entry->setValue(value);
            result = true;
            break;
        }
    }
    
    // Este if checa o caso em que não se encontrou espaços nulos, mas encontrou pelo menos um espaço 'deleted'
    if(deletedIndex != -1)
    {
        HashEntry<string, string>* newEntry = new HashEntry<string, string>(key, value);
        this->data[deletedIndex] = newEntry;
        ++this->quantity;
        result = true;
    }
    
    return result;
}

/**
 Este método deve remover da tabela o HashEntry cujo atributo 'key' == parâmetro 'key'. Neste caso, a quantidade de itens na tabela deve ser dencrementada e retorna-se 'true'.
 Caso não exista um HashEntry com atributo 'key' == parâmetro 'key',a quantidade de itens na tabela não é modificada e retorna-se 'false'.
 */
bool HashTable::remove(const string key)
{
    if(this->isEmpty())
    {
        return false;
    }
    else if( this->getAlpha() <= MIN_ALPHA )
    {
        this->reduce();
    }
    
    unsigned long base = hash(key);
    
    bool result = false;

    for( long delta = 0; delta < this->getSize(); delta++)
    {
        long index = (base + delta) % this->getSize();
        HashEntry<string, string>* entry = this->data[index];

        if( entry == nullptr )
        {
            break;
        }
        else if( entry != ENTRY_DELETED && entry->getKey() == key )
        {
            delete entry;
            this->data[index] = ENTRY_DELETED;
            --this->quantity;
            result = true;
            break;
        }
    }
    
    return result;
}

/**
 Este método calcular um valor numérico para a string 'key'.
 */
unsigned long HashTable::preHash(const string key)
{
    unsigned long x = 0;
    for(unsigned int i = 0; i < key.size(); i++)
    {
        x = x*101 + key.at(i);
    }
    return x;
}

/**
 Este método calcular o hash para a string 'key'.
 */
unsigned long HashTable::hash(const string key)
{
    unsigned long hashValue = this->preHash(key);
    return hashValue % this->getSize();
}

void HashTable::expand()
{
    int newSize = this->getSize() * 2 + 1;
    this->resize(newSize);
}

void HashTable::reduce()
{
    int half = this->getSize() / 2;
    int newSize = half % 2 != 0 ? half : half+1;
    this->resize(newSize);
}

void HashTable::resize(int newSize)
{
    // SIGA OS COMENTÁRIOS ABAIXO.    
    
    // Cria novo array de ponteiros para HashEntry
    // Coloquei para lembrar de iniciar todas posições com 'nulo'.
    HashEntry<std::string, string>** newData = new HashEntry<std::string, string>*[newSize];
    for( int i = 0; i < newSize; i++ )
    {
        newData[i] = nullptr;
    }
    
    // Atualize o tamanho da tabela (this->size)
    int aux = this->size;
    this->size = newSize;

    // Transifra as entradas válidas que estão no array antigo para o novo
    // BEGIN-FOR     Percorra cada nó do array antigo
    for(int i = 0; i < aux ; i++){
    //  BEGIN-IF    Se entry é válida
        if(data[i] != nullptr && data[i] != ENTRY_DELETED){
    //      Insira no Novo Array, tratando possíveis colisões
            int base = hash(data[i]->getKey());
                int j = 0;
                int indice;
            do{
                indice = (base + j)% this->size;
                j++;
                //cout << indice <<" " << size << endl;
            }while(newData[indice] != nullptr );
            newData[indice] = data[i];
    //  END-IF
        }
    // END-FOR
    }
    // Delete o antigo array
    delete[] data;   
    // Atualize o this->data para apontar para o novo array
    this->data = newData;
    
    //std::cout << "ERRO: Método resize(int) ainda não foi implementado.\n" << std::endl;
    //std::exit(1);
}

inline float HashTable::getAlpha()
{
    return (float)this->getQuantity()/this->getSize();
}

/**
 Método que imprime o conteúdo da tabela. Use para ajudar a depurar o programa.
 */
void HashTable::print()
{
    for(int i = 0; i < this->getSize(); i++)
    {
        HashEntry<string, string> * entry = data[i];
        if( entry == ENTRY_DELETED )
        {
            std::cout << i << ": DELETED" << std::endl;
        }
        else if( entry != nullptr )
        {
            std::cout << i << ": " << entry->getKey() << ":" << entry->getValue() << std::endl;
        }
        else
        {
            std::cout << i << ": []" << std::endl;
        }
    }
}

int HashTable::getSize()
{
    return this->size;
}

int HashTable::getQuantity()
{
    return this->quantity;
}

bool HashTable::isEmpty()
{
    return this->quantity == 0;
}

bool HashTable::isFull()
{
    return this->quantity == this->getSize();
}
