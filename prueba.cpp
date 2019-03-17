#include <iostream>
#include <fstream>
#include <map>
#include <string.h>
#include <bitset>
#include <utility>
#include "tree.cpp"
#include "heap.cpp"

using namespace std;

// Hola
Tree Huffman(map<string,int> conjunto){
    Heap Q;
    for(std::map<string,int>::iterator it=conjunto.begin(); it!=conjunto.end(); ++it){
        Tree aux(it->first,it->second);
        //cout << "Se introduce ID: " << aux.getId() << " frec: " << aux.getFrec() << endl;
        Q.push(aux);
    }
    int n = conjunto.size(), i = 0;
    for(i = 0; i < n-1; i++){
        Tree * x = new Tree(Q.cima()); Q.pop();
        cout << "Se elimina ID: " << x->getId() << " frec: " << x->getFrec() << endl;
        Tree * y = new Tree(Q.cima()); Q.pop();
        cout << "Se elimina ID: " << y->getId() << " frec: " << y->getFrec() << endl;
        Tree z(*x,*y);
        Q.push(z);
        //cout << "ID: " << z.getId() << " frec: " << z.getFrec() << endl;
    }
    return Q.cima();
}


int main(int _argc, char ** _argv){
    char a;
    for(int i = 0; i<_argc; i++){
        cout << i << _argv[i] << endl;
    }

    if(strcmp(_argv[1],"-c")==0){
        // Crear el árbol y la tabla de códigos
        map<string,int> frecuencias;
        ifstream f(_argv[2]);
        string name = _argv[2];
        name += ".huf";
        ofstream o;
        o.open(name);
        if(f.is_open()){
            cout << "Se ha abierto el fichero" << endl;
        }
        int i = 0;
        string aux;
        while(!f.eof()){
            f.get(a);
            aux = a;
            frecuencias[aux]++;
            //cout << "Se ha leido el caracter: " << a << endl;
            //i++;
        }
        f.close();

        Tree arbolHuffman = Huffman(frecuencias);

        int tam = frecuencias.size();
        pair<char,bool> preorderTree[tam];
        arbolHuffman.preorderArray(preorderTree,tam);
        
        map<string,string> tablaCod = arbolHuffman.tablaHuffman();

        // Codificar el fichero
        ifstream g(_argv[2]);
        if(g.is_open()){
            cout << "Se ha abierto el fichero" << endl;
        }
        o << tam;
        for(i = 0; i < tam; i++){
            o << preorderTree[i].first << preorderTree[i].second;
        }
        aux;
        string buffer;
        bitset<8> bset;
        int j;
        string bufferaux;
        bool ultima = true;
        while(!g.eof()){
            ultima = false;
            g.get(a);
            aux = a;
            //cout << "Se ha leido el caracter: " << a << endl;

            buffer += tablaCod[aux];
            if(buffer.length()>=8){
                bset.reset();
                for(j=0; j<8; j++){
                    if(buffer[j]=='1'){
                        bset.set(j);
                    }
                }
                bufferaux = "";
                for(j=8; j<buffer.length(); j++){
                    bufferaux += buffer[j];
                }
                buffer = bufferaux;
                if(o.is_open()){
                    o << bset;
                }
                ultima = true;
            }
        }
        if(!ultima){
            bset.reset();
            for(j=0; j<buffer.length(); j++){
                if(buffer[j]=='1'){
                    bset.set(j);
                }
            }
            o << bset;
        }
        g.close();
        o.close();
    }
    else if(strcmp(_argv[1],"-d")==0){
        ifstream f(_argv[2]);
        if(!f.is_open()){
            cout << "El nombre del archivo no era correcto.\n";
        }
        int tam, a = 1;
        char aux; bool aux2;
        pair<char,bool> preorderTree[tam];
        f >> tam;
        for(int i = 0; i < tam; i++){
            f >> aux >> aux2;
            preorderTree[i] = make_pair(aux,aux2);
        }
        Tree cod(string(1,preorderTree[0].first),0);
        cod.treeFromArray(preorderTree,tam,a);
    }
}