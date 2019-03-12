#include <iostream>
#include <fstream>
#include <map>
#include "tree.cpp"
#include "heap.cpp"

using namespace std;

Tree Huffman(map<string,int> conjunto){
    Heap Q;
    for(std::map<string,int>::iterator it=conjunto.begin(); it!=conjunto.end(); ++it){
        Tree aux(it->first,it->second);
        Q.push(aux);
    }
    int n = conjunto.size(), i = 0;
    for(i = 0; i < n; i++){
        Tree x = Q.cima(); Q.pop();
        Tree y = Q.cima(); Q.pop();
        Tree z(x,y);
        Q.push(z);
    }
    return Q.cima();
}

int main(int _argc, char ** _argv){
    char a[4];
    for(int i = 0; i<_argc; i++){
        cout << _argv[i] << endl;
    }

    map<string,int> frecuencias;
    ifstream f("fich.exe");
    if(f.is_open()){
        cout << "Se ha abierto el fichero" << endl;
    }
    int i = 0;
    while(!f.eof() && i < 200){
        f.read(reinterpret_cast<char*>(a), 1);
        //f.get(a);
        frecuencias[a]=frecuencias[a]+1;
        cout << "Se ha leido el caracter: " << a << endl;
        i++;
    }
    int n = frecuencias.size();
    int sum = 0;
    for(std::map<string,int>::iterator it=frecuencias.begin(); it!=frecuencias.end(); ++it){
        cout << it->first << " => " << it->second << '\n';
        sum += it->second;
    }
    cout << "Suma total: " << sum << endl;
    f.close();
}