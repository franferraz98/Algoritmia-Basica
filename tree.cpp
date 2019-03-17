#include <iostream>
#include <map>

using namespace std;

#ifndef _TREE_
#define _TREE_

class Tree{
    string id;
    int frecuencia;
    Tree* izq;
    Tree* der;

public:
    Tree(): id(""), frecuencia(99999){
        this->izq = nullptr;
        this->der = nullptr;
    }

    Tree(string _id, int _frecuencia): id(_id), frecuencia(_frecuencia){
        this->izq = nullptr;
        this->der = nullptr;   
    }

    Tree(Tree& x, Tree& y){
        this->id = x.id + y.id;
        this->frecuencia = x.frecuencia + y.frecuencia;
        if(x.frecuencia > y.frecuencia){
            this->izq = &y;            
            this->der = &x;            
        }else{
            this->der = &y;            
            this->izq = &x; 
        }
    }

    map<string,string> tablaHuffman(){
        map<string,string> tabla;
        string codigo = "";
        tablaINM(tabla,codigo);
        return tabla;
    }

    void preorderArray(pair<char,bool> vec[],int tam){
        int i = 0;
        preorderArrayINM(vec,tam,i);
    }

    void treeFromArray(pair<char,bool> vec[], int tam, int& i){
        if(i<tam){
            Tree * aux = new Tree(string(1,vec[i].first),0);
            this->izq = aux;
            if(!vec[i].second){
                i++;
                aux->treeFromArray(vec,tam,i);
            }
            i++;
            Tree * aux = new Tree(string(1,vec[i].first),0);
            this->der = aux;
            if(!vec[i].second){
                i++;
                aux->treeFromArray(vec,tam,i);
            }
        }
    }

    bool operator > (const Tree& rhs) {
        return this->frecuencia > rhs.frecuencia;
    }

    bool operator < (const Tree& rhs) {
        return this->frecuencia < rhs.frecuencia;
    }

    int getFrec(){
        return this->frecuencia;
    }

    string getId(){
        return this->id;
    }
private:
    void preorderArrayINM(pair<char,bool> vec[], int tam, int& i){
        vec[i] = make_pair(char(this->id[0]),(this->id.length == 1));
        if(this->izq != nullptr){
            i++;
            this->izq->preorderArrayINM(vec,tam,i);
            i++;
            this->der->preorderArrayINM(vec,tam,i);
        }
    }
    void tablaINM(map<string,string>& tabla, string cod){
        if(this->izq == nullptr){
            cout << "Caracter: " << this->id << " codigo: " << cod << endl;
            tabla[this->id]=cod;
        }
        else{
            //cout << "bajando por el arbol\n";
            string aux = cod;
            aux = aux + "0";
            this->izq->tablaINM(tabla,aux);
            aux = cod;
            aux = aux +"1";
            this->der->tablaINM(tabla,aux);
        }
    }
};

#endif