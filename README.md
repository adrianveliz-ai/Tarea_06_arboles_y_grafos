# 🫐 Tarea 06 - Árboles y Grafos

### 🟥 Integrante (Grupo 12):

- Veliz Quispe, Adrián Jesús Ángel

## 📄 Ejercicios:

- Ejercicio 1: ¿Por qué un BST puede llegar a O(n)?
- Ejercicio 2: ¿Por qué el Red-Black Tree tiene altura O(log n)?
- Ejercicio 3: ¿Por qué DFS es adecuado?
- Ejercicio 4: ¿Por qué BFS es lo natural?

Convención: la **altura** es la cantidad de niveles (árbol vacío = 0, un solo nodo = 1).

---

## Ejercicio 1: ¿Por qué un BST puede llegar a O(n)?

**Idea clave:** el costo de insertar, buscar o eliminar es proporcional a la altura `h` del árbol, es decir, `O(h)`.

**Caso malo: insertar 1, 2, 3, …, n en orden.**

Demostración por inducción:

- Con 1 nodo, el árbol es una cadena de 1 nodo.
- Supón que después de insertar `k` datos, el árbol es una cadena que solo va hacia la derecha. El dato nuevo `k+1` es mayor que todos los anteriores, así que en cada paso va a la derecha, hasta llegar al final de la cadena. Ahí se queda como hijo derecho.
- Entonces el árbol con `k+1` nodos sigue siendo una cadena. ∎

Por lo tanto, la altura es `h = n`.

**Costo de insertar los n datos:** el dato número `k` hace `k − 1` comparaciones.

```
Total = 0 + 1 + 2 + ... + (n-1) = n(n-1)/2 = O(n²)
```

**Costo de una sola operación:** buscar el último dato (el `n`) recorre `n` nodos, es decir `O(n)`.

**En general:** un BST con `n` nodos tiene altura `h` con `⌈log₂(n+1)⌉ ≤ h ≤ n`. El mínimo se logra cuando el árbol está lleno y el máximo cuando es una cadena. Como no hay nada que controle la forma, el peor caso es `O(n)`.

---

## Problema 2: ¿Por qué el Red-Black Tree tiene altura O(log n)?

**Resultado:** un Red-Black Tree con `n` nodos tiene altura `h ≤ 2·log₂(n+1)`.

**Definición:** `bh(x)` (altura negra) = cantidad de nodos negros en el camino desde `x` (sin contarlo) hasta una hoja nula (contándola). Por la propiedad 4, este número es el mismo para todos los caminos hacia abajo.

**Paso 1: el subárbol de `x` tiene al menos `2^bh(x) − 1` nodos.** (Inducción)

- Base: si `x` es una hoja nula, `bh = 0` y `2⁰ − 1 = 0` nodos. ✓
- Paso: `x` tiene dos hijos. Cada hijo tiene `bh` igual a `bh(x)` (si es rojo) o `bh(x) − 1` (si es negro). En el peor caso, cada hijo tiene `bh(x) − 1`. Por hipótesis, cada hijo tiene al menos `2^(bh(x)−1) − 1` nodos. Entonces:

```
nodos(x) ≥ 2·(2^(bh(x)-1) - 1) + 1 = 2^bh(x) - 1  ✓
```

**Paso 2: al menos la mitad de los nodos de cualquier camino son negros.** Por la propiedad 3, un nodo rojo no puede tener hijo rojo, así que después de cada rojo viene un negro. Por eso, en un camino de la raíz a una hoja de largo `h`, hay al menos `h/2` nodos negros. Entonces `bh(raíz) ≥ h/2`.

**Paso 3: juntar todo.**

```
n ≥ 2^bh(raíz) - 1 ≥ 2^(h/2) - 1
n + 1 ≥ 2^(h/2)
log₂(n+1) ≥ h/2
h ≤ 2·log₂(n+1) = O(log n)   ∎
```

**Resultado experimental (datos ordenados):**

| n | Altura BST | Altura Red-Black |
| --- | --- | --- |
| 20 | 20 | 6 |
| 1000 | 1000 | 17 |

---

## Problema 3: ¿Por qué DFS es adecuado?

- Un camino raíz-hoja es **un solo camino completo hacia abajo**. DFS justamente baja por un camino hasta el final antes de probar otro.
- El `stack` guarda **exactamente el camino actual** (de la raíz al nodo donde estamos). Cuando llegamos a una hoja con suma `K`, el stack ya tiene la respuesta y solo hay que copiarlo.
- Si la suma no es `K`, hacemos `pop` y volvemos atrás (retroceso o *backtracking*), que es lo natural en DFS.
- **Costo:** tiempo `O(n)` (cada nodo se visita una vez) y memoria `O(h)` (el stack solo guarda un camino).
- Con BFS tendríamos que guardar un camino completo por cada nodo de la cola, lo cual gasta mucha más memoria.

---

## Problema 4: ¿Por qué BFS es lo natural?

**Idea clave:** la cola es FIFO (el primero que entra es el primero que sale).

**Invariante (se demuestra por inducción):** en todo momento, la cola contiene primero nodos del nivel `d` y después nodos del nivel `d+1`, nunca más.

- Al inicio, la cola solo tiene la raíz (nivel 1). ✓
- Cuando sacamos un nodo del nivel `d`, metemos sus hijos (nivel `d+1`) al final. Por ser FIFO, todos los del nivel `d` salen antes que los del `d+1`. ✓

Entonces BFS visita los nodos **nivel por nivel**, y las tres métricas se leen directo de esa idea:

- **Profundidad mínima:** la primera hoja que sale de la cola está en el nivel más bajo posible, porque todos los niveles anteriores ya se revisaron completos. Se puede parar ahí.
- **Ancho máximo:** el tamaño de la cola al empezar cada nivel es la cantidad de nodos de ese nivel. Nos quedamos con el mayor.
- **Árbol completo:** un árbol completo, leído por niveles, no tiene huecos en el medio. Si vemos un hueco (`nullptr`) y después aparece un nodo real, no es completo.

**Costo:** tiempo `O(n)`, memoria `O(w)`, donde `w` es el ancho máximo.
