#include <iostream>
#include <fstream>
#include <map>
#include <string.h>
#include <bitset>
#include <utility>
#include <vector>
#include "tree.cpp"
#include "heap.cpp"

using namespace std;

//
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

        string aux;
        int caracteres = -1, i = 0;
        char a;
        while(!f.eof()){
            f.get(a);
            caracteres++;
            aux = a;
            frecuencias[aux]++;
            //cout << "Se ha leido el caracter: " << a << endl;
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
        o << caracteres << ' ';
        o << tam;
        for(i = 0; i < tam; i++){
            //cout << "Se escribe: " << preorderTree[i].first << " " << preorderTree[i].second << endl;
            if(preorderTree[i].second){
                a = 't';
            }else{
                a = 'f';
            }
            o << preorderTree[i].first << a;
            //cout << "Se escribe: " << preorderTree[i].first << " " << a << endl;
        }

        string buffer;
        bitset<8> bset;
        int j;
        string bufferaux;
        bool ultima = true;
        caracteres = 0;
        while(!g.eof()){
            ultima = false;
            g.get(a);
            aux = a;
    
            buffer += tablaCod[aux];
            //Si suman mas de ocho en la ultima letra no la escribira y ultima valdrá true por eso a veces no escribe el ultimo caracter
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
                    caracteres++;
                    o << a;
                }
                ultima = true;
            }
        }
        if(!ultima || buffer.length() > 0){//hacer tambien si buffer es mayor q 0??
            bset.reset();
            for(j=0; j<buffer.length(); j++){
                if(buffer[j]=='1'){
                    bset.set(j);
                }
            }
            a = char(bset.to_ulong());
            if(o.is_open()){
                o << a;
                caracteres++;
            }
        }
        cout << "Se han escrito en .huf " << caracteres << " caracteres.\n";
        g.close();
        o.close();
    }
    /** 
     * DECODIFICACION DEL FICHERO .HUF 
     */
    else if(strcmp(_argv[1],"-d")==0){

        cout << "Se va decodificar el fichero" << _argv[2] << endl;
        ifstream file(_argv[2]);
        if(!file.is_open()){
            cout << "El nombre del archivo no era correcto.\n";
            return -1;
        }
        int tam, caracteres, a;
        char aux; char aux2; 
        //Se lee el num de nodos del arbol

        file >> caracteres;
        file >> tam;
        cout << "el tam del arbol es de: " << tam << endl;
        cout << "el num de caract es de: " << caracteres << endl;
        pair<char, bool> preorderTree[tam];
        //Se leen los nodos del arbol
        for (int i = 0; i < tam; i++)
        {
            file.get(aux);
            file.get(aux2);
            preorderTree[i] = make_pair(aux, (aux2 == 't'));
            /*cout << "Nuevo par: "
                 << "(" << aux << ", " << aux2 << ")" << endl;*/
        }
        a = 1;

        //Se crea la raiz del arbol que es el primer elemento leido
        Tree cod(string(1, preorderTree[0].first), 0);
        //Se construye el arbol que reconoce el codigo Huffman
        cod.treeFromArray(preorderTree, tam, a);
        map<string, string> tablaCod = cod.tablaHuffman();

        string auxName = _argv[2];
        string name = auxName.substr(0, auxName.length() - 4);
        ofstream o;
        o.open(name);

        bitset<8> lectura;
        int i, veces = 0, leidos = 0;
        char decod;

        Tree *busqueda = &cod;
        file.get(decod);
        lectura = bitset<8>(decod);
       
        while (!file.eof() && veces < caracteres)
        {
            for (i = 0; i < 8; i++)
            {
                if (lectura[i] == 0)
                {
                    busqueda = busqueda->getIzq();
                }
                else
                {
                    busqueda = busqueda->getDer();
                }

                if (busqueda->getIzq() == nullptr)
                {
                    veces++;
                    decod = busqueda->getId()[0];
                    //cout << "Letra encontrada " << "(" << decod << ")" << endl;
                    o << decod;
                    busqueda = &cod;
                    if (veces == caracteres)
                    {
                        break;
                    }
                }
            }
            file.get(decod);
            leidos++;
            lectura = bitset<8>(decod);
            if(file.eof()){
                cout << "se ha terminado el fichero" << endl;
                cout << "se han leido de huf: " << leidos << endl;
            }
        }
        file.close();
        o.close();
    }
}