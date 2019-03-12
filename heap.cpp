#include <iostream>
#include "tree.cpp"

using namespace std;

#define MAX 256

class Heap{
    Tree heap[MAX];
    int numElem;
public:

    Heap(): numElem(0){
        
    }

    Tree cima(){
        return this->heap[0];
    }

    void push(Tree nuevo){
        if(this->numElem == MAX){
            cout << "No ha sido posible añadir el elemnto\n";
        }
        else{
            this->numElem++;
            this->heap[this->numElem]  = nuevo;
            bool debeSubir;
            int i = this->numElem;
            if(i > 0){
                debeSubir = this->heap[i] > this->heap[i/2];
            }else{ debeSubir = false; }
            while(debeSubir){
                Tree aux = this->heap[i];
                this->heap[i] = this->heap[i/2];
                this->heap[i/2] = aux;
                i = i / 2;
                if (i > 0){
                    debeSubir = this->heap[i] > this->heap[i/2];
                }else{ debeSubir = false; }
            }
        }
    }

    void pop(){
        int i, j;
        if((this -> numElem)>0){
            this->heap[0] = this->heap[this->numElem];
            this -> numElem--;
            i = 0;
            while(i<(this->numElem)/2){
                if(2*i==this->numElem || this->heap[2*i]>this->heap[2*i+1]){
                    j = 2*i;
                }else{
                    j = 2*i+1;
                }
                if(this->heap[j] > this->heap[i]){
                    Tree aux = this->heap[i];
                    this->heap[i] = this->heap[j];
                    this->heap[j] = aux;
                    i = j;
                }else{
                    i = this->numElem;
                }
            }
        }

    }
}