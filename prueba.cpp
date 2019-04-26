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

/* Pre: true
 * Post: dado un conjunto de pares clave-valor, devuelve el árbol de Huffman
 * resultante de insertar dichos pares
 */
Tree Huffman(map<string,int> conjunto){
    Heap Q;
    for(std::map<string,int>::iterator it=conjunto.begin(); it!=conjunto.end(); ++it){
        Tree aux(it->first,it->second);
        Q.push(aux);
    }
    int n = conjunto.size(), i = 0;
    for(i = 0; i < n-1; i++){
        Tree * x = new Tree(Q.cima()); Q.pop();
        Tree * y = new Tree(Q.cima()); Q.pop();
        Tree z(*x,*y);
        Q.push(z);
    }
    return Q.cima();
}

/* Pre: o.isOpen() AND name es el nombre de un fichero existente (de texto o binario)
 * Post: codifica el fichero de nombre 'name' mediante una tabla de datos por
 * codificar y un vector de pares que representa el árbol de Huffman
 */
void codificar(ofstream& o, map<string,string> tablaCod, char * name, pair<char,bool> preorderTree[], int tam, int caracteres){
        /* Declaración de variables */
        string aux;
        char a;
        ifstream g(name);

        if(!g.is_open()){
            cout << "No existe el fichero " << name << endl;
        } else{
            cout << "Se ha abierto el fichero " << name << endl;
        
            /* Escritura de los datos de guía: número de caracteres distintos, 
             * tamaño del fichero (en caracteres) y estructura del árbol */
            o << caracteres << ' ';
            o << tam;
            for(int i = 0; i < tam; i++){
                if(preorderTree[i].second){
                    a = 't';
                }else{
                    a = 'f';
                }
                o << preorderTree[i].first << a;
            }

            /* Declaración de variables */
            string buffer;
            bitset<8> bset;
            int j;
            string bufferaux;
            bool ultima = true;
            caracteres = 0;

            /* Codificación del fichero en binario según la estructura del
             * árbol de Huffman. Los bits se escriben de 8 en 8 dado el tamaño
             * del bitset. */
            while(!g.eof()){
                ultima = false;
                g.get(a);
                aux = a;
        
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
                        caracteres++;
                        o << a;
                    }
                    ultima = true;
                }
            }

            /* Módulo que escribe los bits sobrantes (cuando quedan menos
             * de 8) para no perder información */
            if(!ultima || buffer.length() > 0){
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
            cout << "Se han escrito en " << name << ".huf " << caracteres << " caracteres." << endl;
        }
        g.close();
}

/* Pre: file!=null AND o!=null AND file está asociado a un fichero previamente
 * codificado por este mismo programa.
 * Post: Decodifica el fichero asociado a file según la estructura del árbol 
 * 'cod'
 */
void decodificar(Tree cod, ifstream& file, ofstream& o, int caracteres){
    /* Declaración de variables */
    bitset<8> lectura;
    int i, veces = 0, leidos = 0;
    char decod;
    Tree *busqueda = &cod;
    file.get(decod);
    lectura = bitset<8>(decod);
    
    /* Lectura de los carácteres binarios codificados en el fichero 'file' para
     * su posterior conversión a carácteres */
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

            /* Caracter encontrado */
            if (busqueda->getIzq() == nullptr)
            {
                veces++;
                decod = busqueda->getId()[0];
                o << decod;
                busqueda = &cod;
                if (veces == caracteres)
                {
                    break;
                }
            }
        }

        /*Actualización de carácter buscado*/
        file.get(decod);
        leidos++;
        lectura = bitset<8>(decod);
        if(file.eof()){
            cout << "Se ha terminado el fichero" << endl;
            cout << "Se han leido de huf: " << leidos << endl;
        }
    }
}

int main(int _argc, char ** _argv){
    /** 
     * CODIFICACION DEL FICHERO .HUF 
     */
    if(strcmp(_argv[1],"-c")==0){
        /* Declaración de variables */
        ifstream f(_argv[2]);
        string name = _argv[2];
        name += ".huf";
        string aux;
        int caracteres = -1, i = 0;
        char a;
        ofstream o;

        o.open(name);
        if(f.is_open()){
            cout << "Se ha creado el fichero " << name << endl;
        } else{
            cout << "El nombre del archivo no era correcto." << endl;
            return -1;
        }

        /* Poblado de la tabla de frecuencias */
        map<string,int> frecuencias;
        while(!f.eof()){
            f.get(a);
            caracteres++;
            aux = a;
            frecuencias[aux]++;
        }
        f.close();

        /* Creación de la tabla Huffman */
        int tam = frecuencias.size();
        tam = tam + tam - 1;
        Tree arbolHuffman = Huffman(frecuencias);

        /* Construcción de un vector con el árbol recorrido en pre-orden */
        pair<char,bool> preorderTree[tam];
        arbolHuffman.preorderArray(preorderTree,tam);
        
        /* Construcción de la tabla de codificación */
        map<string,string> tablaCod = arbolHuffman.tablaHuffman();

        /* Codificación del fichero */
        codificar(o,tablaCod,_argv[2],preorderTree, tam, caracteres);
       
        o.close();
    }
    /** 
     * DECODIFICACION DEL FICHERO .HUF 
     */
    else if(strcmp(_argv[1],"-d")==0){
        /* Declaración de variables */
        int tam, caracteres, a;
        char aux; char aux2; 
        ifstream file(_argv[2]);

        cout << "Se va decodificar el fichero " << _argv[2] << endl;
        if(!file.is_open()){
            cout << "El nombre del archivo no era correcto." << endl;
            return -1;
        }
        
        /* Recuperación del número de nodos del arbol, lectura e inserción en
         * un árbol en pre-orden de los nodos */
        file >> caracteres;
        file >> tam;
        pair<char, bool> preorderTree[tam];
        for (int i = 0; i < tam; i++)
        {
            file.get(aux);
            file.get(aux2);
            preorderTree[i] = make_pair(aux, (aux2 == 't'));
        }

        /* Creación de la raíz del árbol (primer elemenro leido) y construcción
         * del árbol que reconoce el código Huffman*/
        a = 1;
        Tree cod(string(1, preorderTree[0].first), 0);
        cod.treeFromArray(preorderTree, tam, a);
        map<string, string> tablaCod = cod.tablaHuffman();

        /* Recuperación del nombre original del fichero y creación del mismo*/
        string auxName = _argv[2];
        string name = auxName.substr(0, auxName.length() - 4);
        ofstream o;
        o.open(name);
        
        /* Decodificación  del fichero */
        decodificar(cod,file,o,caracteres);
        
        file.close();
        o.close();
    }
}