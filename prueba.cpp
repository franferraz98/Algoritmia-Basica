#include <iostream>
#include <fstream>
#include <map>
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
    /*char a;
    for(int i = 0; i<_argc; i++){
        cout << _argv[i] << endl;
    }

    if(_argv[1] == "-c"){
        map<string,int> frecuencias;
        ifstream f(_argv[2]);
        if(f.is_open()){
            cout << "Se ha abierto el fichero" << endl;
        }
        int i = 0;
        string aux;
        while(!f.eof() && i < 200){
            f.read(reinterpret_cast<char*>(a), 1);
            //f.get(a);
            aux = a;
            frecuencias[aux]=frecuencias[aux]+1;
            cout << "Se ha leido el caracter: " << a << endl;
            i++;
        }
        f.close();

        Tree arbolHuffman = Huffman(frecuencias);

        map<string,string> tablaCod = arbolHuffman.tablaHuffman();

    }
    else{

    }*/
    map<string,int> frec;
    frec["a"] = 45;
    frec["f"] = 5;
    frec["e"] = 9;
    frec["c"] = 12;
    frec["b"] = 13;
    frec["d"] = 16;

    for(std::map<string,int>::iterator it=frec.begin(); it!=frec.end(); ++it){
        cout << "clave: " << it->first << " frecuencia: " << it->second << endl;
    }

    Tree arbolHuffman = Huffman(frec);
    cout << "ID: " << arbolHuffman.getId() << " frec: " << arbolHuffman.getFrec() << endl;
    map<string,string> tablaCod = arbolHuffman.tablaHuffman();

    for(std::map<string,string>::iterator it=tablaCod.begin(); it!=tablaCod.end(); ++it){
        cout << "clave: " << it->first << " codigo: " << it->second << endl;
    }

}