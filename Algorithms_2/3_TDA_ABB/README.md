# Explicación del programa entregado
	
La explicación del TDA ABB (Árbol Binario de Búsqueda) (existente en el archivo abb.c) presentado funciona de la siguiente manera:

Se trató de realizar el TDA con el mayor uso de recursividad posible para poder hacer más intuitivo el concepto de árbol, además que este de por sí se presta para la recursividad.

Para los elementos duplicados se adoptó la convención de colocarlo a la izquierda.

Es útil aclarar que el comparador y su uso a lo largo de la implementación fue utilizado con valores de retorno análogos al de, por ejemplo, strcmp, donde se devuelve un valor negativo si el segundo elemento es mayor, un valor positivo si el primer elemento es mayor o cero si ambos elementos son iguales.

Se puede notar que en el caso del borrado se dividen los casos en los distintos casos para borrar (sea hoja, raíz hoja, con 1 hijo y con 2 hijos). Para el caso del borrado con 2 hijos se tomó el nodo "mayor de los menores", en términos de clave.

Como explicación de por qué en algunos casos las funciones de búsqueda también recogen el dato "nodo padre" es para evitar gasto innecesario de recursos y poder obtener el nodo en cuestión y su padre en una sola "pasada", evitando un incremento de complejidad en comparación de si se requirieran dos pasadas.

En el caso de las funciones que recorren y llenan un array, se decidió evitar la repetición de código. Esto es, para cada una de estas funciones se implementa el recorrido que representan, pero también la función abb_con_cada_elemento requiere la implementación de funciones que hagan los mismos 3 tipos de recorrido (inorden, preorden y postorden). Este es el motivo por el cual fueron creadas3 funciones que abb_con_cada_elemento puede usar y aplicar la función que recibe como parámetro, así como una función especial que rellena el array, siendo compatible para ser enviada a las funciones de recorrido.

Como nota sobre el conjunto de datos elegido para las pruebas, se seleccionó pensado de tal manera que se tenga un árbol completo de 15 elementos, se proporciona un esquema visual del resultado de dicho árbol con las claves elegidas:

```
                                   (008)                                        
                 +-------------------+-------------------+                      
               (004)                                   (012)                    
       +---------+---------+                   +---------+---------+            
     (002)               (006)               (010)               (014)          
  +----+----+         +----+----+         +----+----+         +----+----+       
(001)     (003)     (005)     (007)     (009)     (011)     (013)     (015)
```

Habiendo dado una explicación sobre el árbol binario de búsqueda, se denota que se agregó un archivo llamado "main_tests.c", donde se encuentran detalladas todas las pruebas (testing) que se le hicieron a las funciones, con el objetivo de detectar posibles fallos mediante testear casos límite, inválidos o casos válidos en una secuencia determinada.

El framework utilizado para realizar las pruebas se proporciona como "test_suite_gd.h", y se podrá encontrar su nombre abreviado como "tsgd".

Las pruebas que mostraron un comportamiento positivo en un grupo, se tomaron como válidas y funcionales para pruebas en grupos siguientes. Se accedió a la estructura interna del ABB para realizar ciertas pruebas y comprobar de manera efectiva que los datos estuviesen donde deberían estar.

Se crearon y ejecutaron un total de 88 pruebas con resultados esperados, correctos y sin problemas de pérdida o corrupción de memoria.

<br>

# Explicación de compilación y ejecución

La línea de compilación es la siguiente:

```bash
gcc *.c -o abb -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
```

Lo que hace esta línea de compilación es llamar al compilador GCC (GNU Compiler Collection) y "decirle" que detecte todos los archivos de extensión ".c" para la compilación. Luego se expresan diversos flags que le ordenan al compilador respectivamente que:ensamble todo en un solo archivo binario ejecutable con "-o" y le de el nombre de "abb" (proveniente de árbol de búsqueda binario).Que genere información de debugging, que use la versión estándar del lenguaje C del año 1999, que active todos los warnings (excepto los que están en Wextra, que activa extra warnings), que considere como warnings las conversiones implícitas (cuando se convierte entre tipos de datos distintos sin un casteo explícito), advierta sobre comparaciones que son siempre verdaderas o siempre falsas debido al rango limitado del tipo de dato, de como advertencia todas las exigidas por el estándar ISO C, que considere todos los warnings como errores(es decir, que si hay un warning no compilará hasta que se solucione, como si fuera un error), y por último que deshabilite la optimización hecha por el compilador y sean otorgados resultados esperados a la hora de debuggear.

Para la ejecución del programa basta con ejecutar la siguiente línea:

```bash
./abb
```

NOTA: se dejó el archivo brindado de "abb_minipruebas.c" en una carpeta dentro del comprimido .zip llamada "minipruebas". Est ocon el fin de que se compile utilizando las pruebas implementadas de "main_tests.c" y al ejecutar el TDA se ejecute dicho archivo con su main.

<br>

# Explicación de ejecución de Valgrind

La línea de ejecución de Valgrind propuesta en la consigna del TP es la siguiente:

```bash
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./abb
```
					
Lo que hace esta línea de comando es ejecutar Valgrind y expresa diversos argumentos que hacen, respectivamente, lo siguiente:mostrar cada pérdida de memoria individual en detalle, que haga tracking de valores no inicializados útiles para detectar errores de memoria,a su vez también favorece a la utilidad del resultado analizado en detrimento de la velocidad de ejecución, el último flag muestra la memoria"alcanzable" y la "perdida indirectamente" del programa además de la "perdida definitivamente" y la "posiblemente perdida". El último argumento es una orden para que ejecute el binario ejecutable del abb.

<br>

# Explicación de conceptos varios

1. Un ABB (Árbol de Búsqueda Binario) es una variante de la estructura de dato árbol, donde se puede tener un máximo de 2 hijos (binario).Hasta aquí se tiene sólo la definición de un árbol binario, el árbol binario de búsqueda posee una condición donde los elementos almacenados en el sub-árbol izquierdo de un determinado nodo siempre son menores al elemento de dicho nodo,y los elementos almacenados en el sub-árbol derecho serán mayores a dicho elemento. Aquí se marca un orden diferenciado. También es posible que esto sea al revés,pero por convención se suele utilizar más esta forma. Se debe adoptar una convención para los elementos con el mismo valor o duplicados.

2. El objetivo de tener una función de destrucción en el TDA es para que el usuario tenga más flexibilidad y control sobre los elementos que inserta en el nodo. Por lo que si están en memoria dinámica, por ejemplo, puede liberar los elementos, osi están en el stack puede no hacer nada con ellos o modificar un campo. Cuando se refiere a elementos se está hablando específicamente de "el objeto" que se inserta en los nodos del árbol y no de los nodos en sí. Si no existiese este destructor, se le quitaría este control al usuario,quedando en manos del usuario el tener un registro de los elementos y el manejo del borrado, lo cual sería poco conveniente.
	
3. Operaciones primitivas del ABB teniendo en cuenta el peor escenario (worst case):

	- Creación: O(1), porque sólo se trata con punteros y se reserva memoria dinámica.

	- Inserción: O(n) en el peor caso porque este ABB no es autobalanceado y podría degenerar en lista, teniendoque ciclar todos los elementos.

	- Eliminación: O(n) en el peor caso porque este ABB no es autobalanceado y podría degenerar en lista,teniendo que ciclar todos los elementos.

	- Búsqueda: O(n) en el peor caso porque este ABB no es autobalanceado y podría degenerar en lista, teniendoque llegar hasta el último para devolver el elemento.

	- Devolver raíz: O(1) porque sólo se trata con punteros.

	- Verificar vacío: O(1) porque sólo se trata con punteros.

	- La idea de las siguientes funciones: "arbol_recorrido_inorden", "arbol_recorrido_preorden","arbol_recorrido_postorden" y "abb_con_cada_elemento" es recorrer todos los nodos y hacerles algo o almacenarlos en alguna parte. Porlo que naturalmente tienen una complejidad algorítmica de O(n).
	
	- Destrucción del árbol: O(n) porque debe eliminar todos los nodos presentes en el árbol, además del árbolmismo.