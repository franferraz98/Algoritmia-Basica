#include <iostream>
#include <map>

using namespace std;

class Tree{
    string id;
    int frecuencia;
    Tree* izq;
    Tree* der;

public:
    Tree::Tree(): id(""), frecuencia(0){
        this->izq = nullptr;
        this->der = nullptr;
    }

    Tree::Tree(string _id, int _frecuencia): id(_id), frecuencia(_frecuencia){
        this->izq = nullptr;
        this->der = nullptr;   
    }

    Tree::Tree(Tree x, Tree y){
        this->id = x.id + y.id;
        this->frecuencia = x.frecuencia + y.frecuencia;
        if(x.frecuencia > y.frecuencia){
            this->izq = &y;            
            this->der = &x;            
        }
    }

    map<string,string> tablaHuffman(){
        map<string,string> tabla;
        string codigo = "";
        tablaINM(tabla,codigo);

    }

    bool Tree::operator > (const Tree& rhs) {
        return this->frecuencia > rhs.frecuencia;
    }

    bool Tree::operator < (const Tree& rhs) {
        return this->frecuencia < rhs.frecuencia;
    }

    private:
        void tablaINM(map<string,string>& tabla, string cod){
            if(this->izq == nullptr){
                tabla[this->id]=cod;
            }
            else{
                string aux = cod;
                aux = aux + "0";
                tablaINM(tabla,aux);
                aux = cod;
                aux = aux +"1";
                tablaINM(tabla,aux);
            }
        }
}