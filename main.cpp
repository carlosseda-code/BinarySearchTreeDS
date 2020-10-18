//
//  main.cpp
//  proyecto3Sem3
//
//  Created by Carlos Seda and Rodrigo on 17/10/20.
//  Copyright © 2020 Carlos Seda. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

// Nodo del arbol binario
class node{
public:
    int data;
    int repeticiones;
    node* left;
    node* right;
    node(int data){
        this->data = data;
        repeticiones = 1;
        left = NULL;
        right = NULL;
    }
};

// Arbol binario
class BST{
public:
    node* root;
    BST(){
        root = NULL;
    }
    
    void add(vector<node>& v, node* n){
        bool found = false;
        if(root == NULL)
            root = n;
        else{
            node* temp = root;
            node* father = root;
            while(temp != NULL){
                if(n->data == temp->data){
                    found = true;
                }
                if(n->data < temp->data){
                    father = temp;
                    temp = temp->left;
                }
                else{
                    father = temp;
                    temp = temp->right;
                }
            }
            if(!found){
                if(n->data < father->data)
                    father->left = n;
                else
                    father->right = n;
            } else {
                search(v,n->data);
            }
        }
    }
    
    void del(int num){
        bool found = false;
        if(root == NULL)
            cout << "BST is Empty" << endl;
        else{
            node* temp = root;
            node* father = root;
            while(temp != NULL){
                if(num == temp->data){
                    found = true;
                    break;
                }
                if(num < temp->data){
                    father = temp;
                    temp = temp->left;
                }
                else{
                    father = temp;
                    temp = temp->right;
                }
            }
            if(found){
                if(father->left == NULL && father->right == NULL){ // Leaf node
                if(num < father->data){
                    delete father->left;
                    father->left = NULL;
                }
                else{
                    delete father->right;
                    father->right = NULL;
                }
                } else if(temp->left == NULL || temp->right == NULL){ // Has a child
                    if(num < father->data){
                        if(temp->left != NULL){
                            node* child = temp->left;
                            delete father->left;
                            father->left = child;
                        } else{
                            node* child = temp->right;
                            delete father->left;
                            father->left = child;
                        }
                    }
                    else{
                        if(temp->left != NULL){
                            node* child = temp->left;
                            delete father->right;
                            father->right = child;
                        } else{
                            node* child = temp->right;
                            delete father->right;
                            father->right = child;
                        }
                    }
                } else {
                    node* pre = temp->left;
                    node* fatherpre = temp;
                    while(pre->right != NULL){
                        fatherpre = pre;
                        pre = pre->right;
                    }
                    if(num == fatherpre->data){
                        temp->data = pre->data;
                        delete fatherpre->left;
                        fatherpre->left = NULL;
                    } else {
                        temp->data = pre->data;
                        delete fatherpre->right;
                        fatherpre->right = NULL;
                    }
                }
            } else cout << "Element is NOT in the BST" << endl;
            }
    }
    
    bool search(vector<node>& v, int num){
        bool found = false;
        if(root == NULL)
            cout << "BST is Empty" << endl;
        else{
            node* temp = root;
            while(temp != NULL){
                if(num == temp->data){
                    temp->repeticiones++;
                    v.push_back(*temp);
                    found = true;
                    break;
                }
                if(num < temp->data){
                    temp = temp->left;
                }
                else{
                    temp = temp->right;
                }
            }
        }
        return found;
    }
    
    void postorder(node* r){
       if(r != NULL){
            postorder(r->left);
            postorder(r->right);
           //cout << r->data << " Accesos: " << r->repeticiones << endl;
        }
    }
};

// Declaracion de funciones
void lecturaVariables(vector<node>& v, BST& arbol);
int dividirLinea(string linea);
int crearIP(string ip);
void ordenaMerge(vector<node>& v);
void merge(vector<node>& izquierda, vector<node>& derecha, vector<node>& v);

// Menu principal
int main(int argc, const char * argv[]) {
    BST arbol;
    vector<node> v;
    map<int, int> mymap;
    lecturaVariables(v, arbol);
    arbol.postorder(arbol.root);
    ordenaMerge(v);
    int n = v.size() - 1, i = 1;
    if(n > 5){
        cout << "Puerto: " << v[n].data << " Accesos: "<< v[n].repeticiones << endl;
        mymap[v[n].data] = 1;
        n--;
        while(i < 5){
            if(mymap[v[n].data] == 0){
            cout << "Puerto: " << v[n].data << " Accesos: "<< v[n].repeticiones << endl;
            arbol.del(v[n].data);
            i++;
            }
            mymap[v[n].data]++;
            n--;
            if(n < 0)
                break;
        }
    } else {
        cout << "Puerto: " << v[n].data << " Accesos: "<< v[n].repeticiones << endl;
        mymap[v[n].data] = 1;
        n--;
        while(n > 0){
            if(mymap[v[n].data] == 0){
            cout << "Puerto: " << v[n].data << " Accesos: "<< v[n].repeticiones << endl;
            arbol.del(v[n].data);
            }
            mymap[v[n].data]++;
            n--;
        }
    }
}

// Leer todo el archivo y agregar los puertos de cada registro
void lecturaVariables(vector<node>& v, BST& arbol){
    string linea;
    ifstream archivo;
    archivo.open("bitacora.txt");
    int dato;
    while (getline(archivo, linea)){
        dato = dividirLinea(linea);
        node* n= new node(dato);
        arbol.add(v, n);
    }
        archivo.close();
}

// Divir la linea del archivo y guardar en variables
int dividirLinea(string linea) {
  vector<string> variables;
  stringstream ss(linea);
  string dato;
  string ip;
  int nuevo;
  char delimiter = ' ';
  while(getline(ss, dato, delimiter))
    variables.push_back(dato);
    ip = variables[3];
    nuevo = crearIP(ip);
    return nuevo;
}

// Crear el ID del usuario recibiendo los datos necesarios
int crearIP(string ip){
    vector<string> variables;
    stringstream ss(ip);
    string dato;
    int ipnuevo;
    char delimiter = ':';
    while(getline(ss, dato, delimiter))
        variables.push_back(dato);
    ipnuevo = stoi(variables[1]);
    return ipnuevo;
}

// Funcion 2 de ordenamiento merge
void merge(vector<node>& izquierda, vector<node>& derecha, vector<node>& v){
    int i = 0, j = 0, k = 0;
    while (i < izquierda.size() && j < derecha.size()) {
        if (izquierda[i].repeticiones <= derecha[j].repeticiones) {
            v[k] = izquierda[i];
            i++;
            k++;
        } else {
            v[k] = derecha[j];
            j++;
            k++;
        }
    }
    while (i < izquierda.size()) {
        v[k] = izquierda[i];
        k++;
        i++;
    }
    while (j < derecha.size()) {
        v[k] = derecha[j];
        k++;
        j++;
    }
}

// Con esta funcion ordenaremos todos los ids de forma ascendente
void ordenaMerge(vector<node>& v){
    int c = 0;
    if (v.size() < 2)
        return;
    int len = v.size();
    int first = 0;
    int mitad = first + (len - first) / 2;
    vector<node> izquierda,derecha;
    for (int i = 0; i < mitad; i++){
        node* u;
        izquierda.push_back(*u);
        izquierda[i] = v[i];
    }
    for (int i = mitad; i < len; i++){
        node* u;
        derecha.push_back(*u);
        derecha[c] = v[i];
        c++;
    }
    ordenaMerge(izquierda);
    ordenaMerge(derecha);
    merge(izquierda, derecha, v);
}

