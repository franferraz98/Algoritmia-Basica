#include <iostream>
#include <fstream>
#include <map>
#include <string.h>
#include <bitset>
#include <utility>
#include "tree.cpp"
#include "heap.cpp"

using namespace std;

//https://stackoverflow.com/questions/3061721/concatenate-boostdynamic-bitset-or-stdbitset
template <size_t N1, size_t N2>
bitset<N1 + N2> concat(const bitset<N1> &b1, const bitset<N2> &b2)
{
    string s1 = b1.to_string();
    string s2 = b2.to_string();
    return bitset<N1 + N2>(s1 + s2);
}

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

        int tam = frecuencias.size();
        tam = tam + tam - 1;
        Tree arbolHuffman = Huffman(frecuencias);

        pair<char,bool> preorderTree[tam];
        //Se construye un vector con el arbol recorrido en pre-order
        arbolHuffman.preorderArray(preorderTree,tam);
        
        //Se construye la tabla de codificacion
        map<string,string> tablaCod = arbolHuffman.tablaHuffman();

        // Codificar el fichero
        ifstream g(_argv[2]);
        if(g.is_open()){
            cout << "Se ha abierto el fichero" << endl;
        }

        //Se almacenan tanto el numero de nodos como el vector del arbol
        o << tam;
        for(i = 0; i < tam; i++){
            //cout << "Se escribe: " << preorderTree[i].first << " " << preorderTree[i].second << endl;;
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
                    a = char(bset.to_ulong());
                    o << a;
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
            a = char(bset.to_ulong());
            o << a;
        }
        g.close();
        o.close();
    }
    else if(strcmp(_argv[1],"-d")==0){

        cout << "Se va decodificar el fichero" << _argv[2] << endl;
        ifstream f(_argv[2]);
        if(!f.is_open()){
            cout << "El nombre del archivo no era correcto.\n";
        }
        int tam, a;
        char aux; bool aux2;
        //Se lee el num de nodos del arbol
        //f.read(reinterpret_cast<char *>(&tam), sizeof(int));
        f >> tam;
        cout << "el tam del arbol es de: " << tam << endl;
        pair<char,bool> preorderTree[tam];
        //Se leen los nodos del arbol
        for(int i = 0; i < tam; i++){
            f.get(aux);
            f >> aux2;
            //f.read(reinterpret_cast<char *>(&aux), sizeof(char));
            //f.read(reinterpret_cast<char *>(&aux2), sizeof(bool));
            preorderTree[i] = make_pair(aux,aux2);
            cout << "Nuevo par: " << "(" << aux << ", " << aux2 << ")" << endl;
        }
        a = 1;
        
        //Se crea la raiz del arbol que es el primer elemento leido
        Tree cod(string(1,preorderTree[0].first),0);
        //Se construye el arbol que reconoce el codigo Huffman
        cod.treeFromArray(preorderTree,tam,a);
        map<string, string> tablaCod = cod.tablaHuffman();

        string auxName = _argv[2];
        string name = auxName.substr(0, auxName.length()-4);
        ofstream o;
        o.open(name);

        bitset<8> lectura;
        int i, veces=0;
        char decod;

        Tree * busqueda = &cod;
        f.get(decod);
        //f.read(reinterpret_cast<char *>(&decod), sizeof(char));
        lectura = bitset<8>(decod);
        cout << "Letra leida: " << decod << " binario de la letra: " << lectura << '\n';
        while(!f.eof() && veces < 4){
            for(i = 0; i < 8; i++){
                if(lectura[i] == 0){
                    busqueda = busqueda->getIzq();
                }
                else{
                    busqueda = busqueda->getDer();
                }

                if(busqueda->getIzq() == nullptr)
                {
                    decod = busqueda->getId()[0];
                    cout << "Letra encontrada " << "(" << decod << ")" << endl;
                    o << decod;
                    busqueda = &cod;
                }
            }
            veces++;
            //f >> lectura;
            f.get(decod);
            //f.read(reinterpret_cast<char *>(&decod), sizeof(char));
            lectura = bitset<8>(decod);
        }

        f.close(); o.close();

    }
}