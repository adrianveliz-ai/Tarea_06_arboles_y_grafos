
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

// Convencion de altura: cantidad de NIVELES (arbol vacio = 0, un solo nodo = 1).

// ============================================================
// EJERCICIO 1: BST ITERATIVO
// ============================================================
struct Nodo {
    int dato;
    Nodo* izq;
    Nodo* der;
    Nodo(int d) : dato(d), izq(nullptr), der(nullptr) {}
};

class BST {
public:
    Nodo* raiz = nullptr;

    ~BST() {
        // Liberar memoria sin recursion
        stack<Nodo*> pila;
        if (raiz) pila.push(raiz);
        while (!pila.empty()) {
            Nodo* n = pila.top();
            pila.pop();
            if (n->izq) pila.push(n->izq);
            if (n->der) pila.push(n->der);
            delete n;
        }
    }

    // Insercion iterativa (ignora repetidos)
    void insertar(int x) {
        Nodo* nuevo = new Nodo(x);
        if (!raiz) { raiz = nuevo; return; }
        Nodo* act = raiz;
        while (true) {
            if (x == act->dato) { delete nuevo; return; }
            if (x < act->dato) {
                if (!act->izq) { act->izq = nuevo; return; }
                act = act->izq;
            } else {
                if (!act->der) { act->der = nuevo; return; }
                act = act->der;
            }
        }
    }

    // Eliminacion iterativa
    bool eliminar(int x) {
        Nodo* padre = nullptr;
        Nodo* act = raiz;
        while (act && act->dato != x) {
            padre = act;
            act = (x < act->dato) ? act->izq : act->der;
        }
        if (!act) return false;  // no existe

        // Caso 2 hijos: copiar el sucesor (el menor de la derecha) y borrar ese nodo
        if (act->izq && act->der) {
            Nodo* padreSuc = act;
            Nodo* suc = act->der;
            while (suc->izq) { padreSuc = suc; suc = suc->izq; }
            act->dato = suc->dato;
            padre = padreSuc;
            act = suc;
        }

        // Ahora act tiene 0 o 1 hijo
        Nodo* hijo = act->izq ? act->izq : act->der;
        if (!padre) raiz = hijo;
        else if (padre->izq == act) padre->izq = hijo;
        else padre->der = hijo;
        delete act;
        return true;
    }

    // Recorrido in-order con stack
    vector<int> inorder() {
        vector<int> res;
        stack<Nodo*> pila;
        Nodo* act = raiz;
        while (act || !pila.empty()) {
            while (act) { pila.push(act); act = act->izq; }
            act = pila.top();
            pila.pop();
            res.push_back(act->dato);
            act = act->der;
        }
        return res;
    }

    // Pre-orden con stack (para dibujar el arbol)
    vector<int> preorden() {
        vector<int> res;
        stack<Nodo*> pila;
        if (raiz) pila.push(raiz);
        while (!pila.empty()) {
            Nodo* n = pila.top();
            pila.pop();
            res.push_back(n->dato);
            if (n->der) pila.push(n->der);  // primero der, para que salga izq primero
            if (n->izq) pila.push(n->izq);
        }
        return res;
    }

    // Altura iterativa: contamos niveles con una cola
    int altura() { return alturaDe(raiz); }

    static int alturaDe(Nodo* r) {
        if (!r) return 0;
        queue<Nodo*> q;
        q.push(r);
        int niveles = 0;
        while (!q.empty()) {
            int cant = q.size();
            niveles++;
            for (int i = 0; i < cant; i++) {
                Nodo* n = q.front();
                q.pop();
                if (n->izq) q.push(n->izq);
                if (n->der) q.push(n->der);
            }
        }
        return niveles;
    }
};

// ============================================================
// PROBLEMA 2: RED-BLACK TREE
// ============================================================
struct NodoRB {
    int dato;
    bool rojo;  // true = rojo, false = negro
    NodoRB* izq;
    NodoRB* der;
    NodoRB* padre;
    NodoRB(int d) : dato(d), rojo(true), izq(nullptr), der(nullptr), padre(nullptr) {}
};

class RedBlackTree {
    NodoRB* raiz = nullptr;  // nullptr representa las hojas nulas (negras)

    void rotarIzq(NodoRB* x) {
        NodoRB* y = x->der;
        x->der = y->izq;
        if (y->izq) y->izq->padre = x;
        y->padre = x->padre;
        if (!x->padre) raiz = y;
        else if (x == x->padre->izq) x->padre->izq = y;
        else x->padre->der = y;
        y->izq = x;
        x->padre = y;
    }

    void rotarDer(NodoRB* x) {
        NodoRB* y = x->izq;
        x->izq = y->der;
        if (y->der) y->der->padre = x;
        y->padre = x->padre;
        if (!x->padre) raiz = y;
        else if (x == x->padre->der) x->padre->der = y;
        else x->padre->izq = y;
        y->der = x;
        x->padre = y;
    }

    // Arregla los problemas que deja una insercion (nodo rojo con padre rojo)
    void corregir(NodoRB* z) {
        while (z != raiz && z->padre->rojo) {
            NodoRB* padre = z->padre;
            NodoRB* abuelo = padre->padre;  // existe: el padre es rojo, entonces no es la raiz
            if (padre == abuelo->izq) {
                NodoRB* tio = abuelo->der;
                if (tio && tio->rojo) {
                    // Caso 1: tio rojo -> solo recolorear y subir
                    padre->rojo = false;
                    tio->rojo = false;
                    abuelo->rojo = true;
                    z = abuelo;
                } else {
                    if (z == padre->der) {  // Caso 2: triangulo -> rotar para volverlo linea
                        z = padre;
                        rotarIzq(z);
                        padre = z->padre;
                    }
                    // Caso 3: linea -> recolorear y rotar el abuelo
                    padre->rojo = false;
                    abuelo->rojo = true;
                    rotarDer(abuelo);
                }
            } else {  // lo mismo pero en espejo
                NodoRB* tio = abuelo->izq;
                if (tio && tio->rojo) {
                    padre->rojo = false;
                    tio->rojo = false;
                    abuelo->rojo = true;
                    z = abuelo;
                } else {
                    if (z == padre->izq) {
                        z = padre;
                        rotarDer(z);
                        padre = z->padre;
                    }
                    padre->rojo = false;
                    abuelo->rojo = true;
                    rotarIzq(abuelo);
                }
            }
        }
        raiz->rojo = false;  // Propiedad 2: la raiz siempre es negra
    }

    // Devuelve false si falla alguna regla. bh = altura negra del subarbol
    bool revisar(NodoRB* n, long long minimo, long long maximo, int& bh) {
        if (!n) { bh = 1; return true; }  // Propiedad 5: hoja nula es negra
        if (n->dato <= minimo || n->dato >= maximo) {
            cout << "  Falla: no es BST en el nodo " << n->dato << endl;
            return false;
        }
        if (n->rojo) {  // Propiedad 3
            if ((n->izq && n->izq->rojo) || (n->der && n->der->rojo)) {
                cout << "  Falla: nodo rojo " << n->dato << " tiene hijo rojo" << endl;
                return false;
            }
        }
        int bhIzq, bhDer;
        if (!revisar(n->izq, minimo, n->dato, bhIzq)) return false;
        if (!revisar(n->der, n->dato, maximo, bhDer)) return false;
        if (bhIzq != bhDer) {  // Propiedad 4
            cout << "  Falla: distinta cantidad de negros bajo el nodo " << n->dato << endl;
            return false;
        }
        bh = bhIzq + (n->rojo ? 0 : 1);
        return true;
    }

    int alturaRec(NodoRB* n) {
        if (!n) return 0;
        return 1 + max(alturaRec(n->izq), alturaRec(n->der));
    }

    void liberar(NodoRB* n) {
        if (!n) return;
        liberar(n->izq);
        liberar(n->der);
        delete n;
    }

public:
    ~RedBlackTree() { liberar(raiz); }

    void insertar(int x) {
        NodoRB* padre = nullptr;
        NodoRB* act = raiz;
        while (act) {
            if (x == act->dato) return;  // ignorar repetidos
            padre = act;
            act = (x < act->dato) ? act->izq : act->der;
        }
        NodoRB* z = new NodoRB(x);  // los nodos nuevos son rojos
        z->padre = padre;
        if (!padre) raiz = z;
        else if (x < padre->dato) padre->izq = z;
        else padre->der = z;
        corregir(z);
    }

    // Revisa las 5 propiedades
    bool verificar() {
        if (!raiz) return true;
        if (raiz->rojo) {
            cout << "  Falla: la raiz es roja" << endl;
            return false;
        }
        int bh;
        return revisar(raiz, LLONG_MIN, LLONG_MAX, bh);
    }

    int altura() { return alturaRec(raiz); }

    // Pre-orden con stack. Muestra dato(R) o dato(N)
    void imprimirPreorden() {
        stack<NodoRB*> pila;
        if (raiz) pila.push(raiz);
        while (!pila.empty()) {
            NodoRB* n = pila.top();
            pila.pop();
            cout << n->dato << (n->rojo ? "(R) " : "(N) ");
            if (n->der) pila.push(n->der);
            if (n->izq) pila.push(n->izq);
        }
        cout << endl;
    }
};

// ============================================================
// PROBLEMA 3: DFS ITERATIVO (camino raiz-hoja con suma K)
// ============================================================
// Cada elemento del stack es un nodo del camino actual.
// "estado" dice que hijo toca visitar: 0 = izq, 1 = der, 2 = ya termine (hacer pop)
struct Marco {
    Nodo* nodo;
    int suma;  // suma desde la raiz hasta este nodo
    int estado;
};

bool caminoConSuma(Nodo* raiz, int K, vector<int>& camino) {
    camino.clear();
    if (!raiz) return false;
    stack<Marco> pila;
    pila.push({raiz, raiz->dato, 0});

    while (!pila.empty()) {
        Nodo* n = pila.top().nodo;
        int suma = pila.top().suma;
        int estado = pila.top().estado;

        if (estado == 0) {
            pila.top().estado = 1;
            if (!n->izq && !n->der && suma == K) {
                // Encontrado: el stack tiene justo el camino raiz -> hoja
                while (!pila.empty()) {
                    camino.push_back(pila.top().nodo->dato);
                    pila.pop();
                }
                reverse(camino.begin(), camino.end());
                return true;
            }
            if (n->izq) pila.push({n->izq, suma + n->izq->dato, 0});
        } else if (estado == 1) {
            pila.top().estado = 2;
            if (n->der) pila.push({n->der, suma + n->der->dato, 0});
        } else {
            pila.pop();  // ya revise ambos hijos: retroceder
        }
    }
    return false;
}

// ============================================================
// PROBLEMA 4: BFS ITERATIVO
// ============================================================

// Un arbol es completo si, en el recorrido por niveles,
// despues de ver un hueco (nullptr) ya no aparece ningun nodo.
bool esCompleto(Nodo* raiz) {
    if (!raiz) return true;
    queue<Nodo*> q;
    q.push(raiz);
    bool vimosHueco = false;
    while (!q.empty()) {
        Nodo* n = q.front();
        q.pop();
        if (!n) {
            vimosHueco = true;
        } else {
            if (vimosHueco) return false;  // nodo despues de un hueco
            q.push(n->izq);
            q.push(n->der);
        }
    }
    return true;
}

// Ancho maximo = la mayor cantidad de nodos en un mismo nivel
int anchoMaximo(Nodo* raiz) {
    if (!raiz) return 0;
    queue<Nodo*> q;
    q.push(raiz);
    int mejor = 0;
    while (!q.empty()) {
        int cant = q.size();
        mejor = max(mejor, cant);
        for (int i = 0; i < cant; i++) {
            Nodo* n = q.front();
            q.pop();
            if (n->izq) q.push(n->izq);
            if (n->der) q.push(n->der);
        }
    }
    return mejor;
}

// Profundidad minima = nivel de la primera hoja que aparece (la raiz es nivel 1)
int profundidadMinima(Nodo* raiz) {
    if (!raiz) return 0;
    queue<Nodo*> q;
    q.push(raiz);
    int nivel = 0;
    while (!q.empty()) {
        int cant = q.size();
        nivel++;
        for (int i = 0; i < cant; i++) {
            Nodo* n = q.front();
            q.pop();
            if (!n->izq && !n->der) return nivel;  // primera hoja
            if (n->izq) q.push(n->izq);
            if (n->der) q.push(n->der);
        }
    }
    return nivel;
}

// ============================================================
// MAIN: pruebas
// ============================================================
void imprimirVector(const vector<int>& v) {
    for (int x : v) cout << x << " ";
    cout << endl;
}

int main() {
    // ---------- Ejercicio 1 ----------
    cout << "===== EJERCICIO 1: BST iterativo =====" << endl;
    vector<int> datos = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35,
                         45, 55, 65, 75, 85, 5, 15, 90, 1, 95};
    BST bst;
    for (int x : datos) bst.insertar(x);
    cout << "Altura despues de insertar 20: " << bst.altura() << endl;

    bst.eliminar(20);  // nodo con 2 hijos
    bst.eliminar(95);  // hoja
    cout << "Elimine 20 y 95" << endl;
    cout << "In-order: ";
    imprimirVector(bst.inorder());
    cout << "Altura final: " << bst.altura() << endl;

    // ---------- Problema 2 ----------
    cout << "\n===== PROBLEMA 2: BST vs Red-Black (1..20 en orden) =====" << endl;
    BST bstOrdenado;
    RedBlackTree rbt;
    for (int i = 1; i <= 20; i++) {
        bstOrdenado.insertar(i);
        rbt.insertar(i);
    }
    cout << "BST        -> altura: " << bstOrdenado.altura() << endl;
    cout << "Pre-orden BST: ";
    imprimirVector(bstOrdenado.preorden());
    cout << "Red-Black  -> altura: " << rbt.altura() << endl;
    cout << "Pre-orden RBT (R = rojo, N = negro): ";
    rbt.imprimirPreorden();
    cout << "Propiedades del Red-Black: " << (rbt.verificar() ? "OK" : "FALLA") << endl;

    // Prueba extra: 1000 datos ordenados
    BST bstGrande;
    RedBlackTree rbtGrande;
    for (int i = 1; i <= 1000; i++) {
        bstGrande.insertar(i);
        rbtGrande.insertar(i);
    }
    cout << "Con n=1000 ordenados: BST altura = " << bstGrande.altura()
         << ", RBT altura = " << rbtGrande.altura()
         << ", RBT valido = " << (rbtGrande.verificar() ? "OK" : "FALLA") << endl;

    // ---------- Problema 3 ----------
    cout << "\n===== PROBLEMA 3: DFS camino raiz-hoja con suma K =====" << endl;
    BST arbol;
    for (int x : {10, 5, 15, 3, 7, 12, 18}) arbol.insertar(x);
    vector<int> camino;
    for (int K : {22, 45, 100}) {
        cout << "K = " << K << ": ";
        if (caminoConSuma(arbol.raiz, K, camino)) {
            cout << "SI existe. Camino: ";
            imprimirVector(camino);
        } else {
            cout << "NO existe" << endl;
        }
    }

    // ---------- Problema 4 ----------
    cout << "\n===== PROBLEMA 4: BFS =====" << endl;
    BST completo;  // 4 2 6 1 3 5 7 -> completo
    for (int x : {4, 2, 6, 1, 3, 5, 7}) completo.insertar(x);
    BST noCompleto;  // le falta el hijo izq del 2 pero tiene el der del 2
    for (int x : {4, 2, 6, 3, 5, 7}) noCompleto.insertar(x);
    BST inclinado;
    for (int x : {1, 2, 3, 4, 5}) inclinado.insertar(x);

    struct Caso { const char* nombre; BST* a; };
    Caso casos[] = {{"Arbol completo", &completo},
                    {"Arbol no completo", &noCompleto},
                    {"Arbol inclinado", &inclinado}};
    for (auto& c : casos) {
        cout << c.nombre << " -> completo: " << (esCompleto(c.a->raiz) ? "si" : "no")
             << ", ancho maximo: " << anchoMaximo(c.a->raiz)
             << ", profundidad minima: " << profundidadMinima(c.a->raiz) << endl;
    }
    return 0;
}
