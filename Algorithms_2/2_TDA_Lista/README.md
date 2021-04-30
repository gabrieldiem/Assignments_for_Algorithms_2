# Explicación del programa entregado
 
La implementación del TDA Lista Simplemente Enlazada por medio de nodos enlazados (existente en el archivo lista.c) presentada funciona de la siguiente manera:

Para comenzar, se puede decir que en una implementación de una lista simplemente enlazada (desde ahora referida solamente como "lista")existen tres partes de gran importancia: la inserción de elementos, el borrado de elementos, y la liberación de memoria de toda la lista.

Empezando por la parte de inserción, se tiene una inserción exclusivamente al final de la lista y otra donde se puede elegir la posición a insertar. Lo primero que sucede es la creación de la lista en memoria dinámica. Luego, si se eligió la función se inserción al final, el flujo comenzará haciendo una comprobación inicial de varios casos que demuestran que la estructura interna de la lista está rota o que simplemente no existe una lista, estas comprobaciones se decidieron encapsular en una función para poder llevarlas a cabo con una simple llamada en las funciones donde sean requeridas. Después se pasa a comprobar dos casos críticos donde se ejecutará solamente uno de ellos,esos son si se requiere crear e insertar el elemento como primer nodo de la lista, o si este es uno que no es el primero. En cambio, si se eligió insertar un elemento eligiendo una posición determinada, además de los dos casos recién mencionados (a los que podemos referir mejor como insertar el primer nodo e insertar el último nodo, respectivamente) se tiene el caso donde se quiere insertar un nodo como un nuevo inicio, y el caso donde se desea que un nodo sea insertado en medio de otros dos nodos.

Siguiendo con la otra parte de vital importancia, el borrado, nos encontramos con la misma situación que antes, donde podemos borrar un elemento de manera exclusiva al final de la lista, o podemos borrar un nodo ubicado en un lugar determinado de la lista. Análogamente,al elegir la primera opción, el flujo se divide también en dos casos iniciales, que son cuando queremos borramos el único nodo existente que a su vez es el inicio y el fin, y la otra situación donde queremos borrar el último nodo en existencia que no es el primer nodo. Por el otro lado, al borrar de una posición determinada, se le suman casos semejantes que en la inserción en posición, ellos son el borrar el primer nodo que no resulta ser el único, y el borrar un nodo presente en medio de otros dos. El proceso finaliza liberando la memoria que dicho nodo a borrar estaba ocupando.

Como última parte crítica del proceso, se tiene la liberación de la memoria ocupada por toda la lista cuando dicha función es invocada.Aquí se optó por hacer una liberación secuencial de los nodos de la lista, recorriéndolos de inicio a fin para finalmente liberar a la lista en sí.

Como toda lista estándar, también se tienen las funcionalidades de devolver el elemento de una posición determinada, devolver el último elemento, conocer si la lista se encuentra vacía, y conocer la cantidad de elementos de la lista.

Es necesario notar que además de haber implementado una lista simplemente enlazada per se, se agregaron funcionalidades de TDA pila y TDA cola. Se optó por llevar a cabo una implementación encapsulada, es decir, las funciones específicas de pila y cola no llaman a una primitiva exclusiva de lista, cosa que sería posible para reducir la cantidad de líneas de código, pero se eligió respetar una separación entre estos "sub-TDAs".

En el caso de pila, se agrega la funcionalidad de apilar, desapilar y devolver el elemento que se encuentra en el "tope" de la pila,funciones que pueden compartir bloques de código con primitivas específicas de lista, pero estas no son llamadas. Para el caso de cola,está implementado el encolar, desencolar y la devolución del primer elemento.

También se implementaron dos tipos de iteradores, uno externo y otro interno. El primero tiene listo para su uso las primitivas que permiten crearlo, destruirlo, conocer si tiene siguiente, avanzarlo y devolver el elemento en donde está parado.

<br>

Habiendo dado una explicación sobre la lista, se denota que se agregó un archivo llamado "main_tests.c", donde se encuentran detalladas todas las pruebas (testing) que se le hicieron a las funciones, con el objetivo de detectar posibles fallos mediante testear casos límite, inválidos o casos válidos en una secuencia determinada. Aquí también se respetó el encapsulamiento, probando cada parte del trabajo con funciones nativas de dicho TDA.

El framework utilizado para realizar las pruebas se proporciona como "test_suite_gd.h", y se podrá encontrar su nombre abreviado como "tsgd".

En la implementación del testing no se accedió de manera directa a la estructura interna de lista, ejecutando las operaciones y controles con las funciones que son proporcionadas al usuario. Además se aclara que el contenido (los elementos/letras) que se utilizan como base de control para las pruebas, son letras de la A a la Z que se van cargando de manera aleatoria, proporcionando más solidez al saber que pasa varias pruebas efectivamente con datos que no se repiten de ejecución de prueba en ejecución de prueba.Otra cosa a notar es el hecho de que las pruebas que mostraron un comportamiento positivo en un grupo, se tomaron como válidas y funcionales para pruebas en grupos siguientes.

Se crearon y ejecutaron un total de 114 pruebas con resultados esperados, correctos y sin problemas de pérdida o corrupción de memoria.

<br>

# Explicación de compilación y ejecución

La línea de compilación es la siguiente:
	
```bash
gcc *.c -o lista_se -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
```

Lo que hace esta línea de compilación es llamar al compilador GCC (GNU Compiler Collection) y "decirle" que detecte todos los archivos de extensión ".c" para la compilación. Luego se expresan diversos flags que le ordenan al compilador respectivamente que:ensamble todo en un solo archivo binario ejecutable con "-o" y le de el nombre de "lista_se" (proveniente de lista simplemente enlazada).Que genere información de debugging, que use la versión estándar del lenguaje C del año 1999, que active todos los warnings (excepto los que están en Wextra, que activa extra warnings), que considere como warnings las conversiones implícitas (cuando se convierte entre tipos de datos distintos sin un casteo explícito), advierta sobre comparaciones que son siempre verdaderas o siempre falsas debido al rango limitado del tipo de dato, de como advertencia todas las exigidas por el estándar ISO C, ue considere todos los warnings como errores(es decir, que si hay un warning no compilará hasta que se solucione, como si fuera un error), y por último que deshabilite la optimización hecha por el compilador y sean otorgados resultados esperados a la hora de debuggear.

Para la ejecución del programa basta con ejecutar la siguiente línea:

```bash
./lista_se
```

NOTA: se dejó el archivo brindado de "lista_minipruebas.c" en una carpeta dentro del comprimido .zip llamada "minipruebas". Esto con el fin de que se compile utilizando las pruebas implementadas de "main_tests.c" y al ejecutar el TDA se ejecute dicho archivo con su main.

<br>

# Explicación de ejecución de Valgrind

La línea de ejecución de Valgrind propuesta en la consigna del TP es la siguiente:

```bash
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./lista_se
```
	
Lo que hace esta línea de comando es ejecutar Valgrind y expresa diversos argumentos que hacen, respectivamente, lo siguiente:mostrar cada pérdida de memoria individual en detalle, que haga tracking de valores no inicializados útiles para detectar errores de memoria,a su vez también favorece a la utilidad del resultado analizado en detrimento de la velocidad de ejecución, el último flag muestra la memoria"alcanzable" y la "perdida indirectamente" del programa además de la "perdida definitivamente" y la "posiblemente perdida". El último argumento es una orden para que ejecute el binario ejecutable de lista_se.

<br>

# Explicación de conceptos varios

 1. __Lista__: tomando a la implementación de lista como de nodos enlazados, una lista es básicamente un conjunto de nodos donde cada uno conoce al nodo siguiente y guarda un elemento determinado. La lista termina cuando el último nodo es el que apunta a NULL. La implementación de nodos enlazados ofrece la ventaja de que cada no puede estar ubicado en distintas partes de la memoria,sin que sea requisito que esta sea contigua. Por otro lado implementaciones con vectores sí tienen ese problema, pero ofrecen un acceso a un elemento con tiempo de O(1), mientras que la lista por sola tiene O(n) para llegar a un elemento.En una lista simplemente enlazada se tiene que cada nodo apunta siguiente, entonces existe un sentido unidireccional para recorrer la lista. En contraste, en la lista doblemente enlazada se tiene que cada nodo posee un enlace con el nodo siguiente y con el anterior, lo que abre la posibilidad a que la lista sea recorrida en dos direcciones fundamentales.


 2. __Pila y Cola__: Las características fundamentales de ambas, es el estilo u orden de funcionamiento. La pila funciona con un orden LIFO(Last In, First Out), lo que quiere decir que el último elemento que fue apilado será el primero en salir, es decir que sea pila por un extremo, se lo puede pensar como una pila de platos donde sacamos el plato "de más arriba". Por otro lado, la cola posee un orden FIFO (First In, First Out), lo que implica que primer elemento que entró a la cola será el primer elemento en salir de ella, se tiene un "frente" de la cola, se puede hacer analogía a una cola de supermercado, donde el que llegó primero será el primero en salir.

                
 3. __Iterador__: un iterador es un TDA en sí mismo. Aplicando el iterador al contexto de una lista simplemente enlazada, este cumple la función de recorrer secuencialmente dicha lista, es decir, de iterarla. Para nuestro caso, el iterador deja de servir cuando llega al fin de la lista o cuando la lista es modificada. Ofrecer iterador al usuario, tanto externo como interno, le ofrece más posibilidades de recorrido de la lista implementada.
                

 4. __Iterador Externo e Iterador Interno__: un iterador interno es una forma encapsulada de ofrecerle recorrer toda una lista condicionada por una función o parámetro que el usuario decide, en su centro es ciclo que recorre hasta no poder más y procesa los elementos de la lista. El iterador externo en cambio, se lo puede considerar como un iterador "desarmado" por partes, donde el usuario tiene más flexibilidad para controlar el flujo de la iteración y las operaciones que quiere realizar. Además, este último puede reducir el tiempo de ejecución de la iteración al conservar un puntero al nodo "corriente" en que se está parado,aumentando más la flexibilidad y dando beneficios reduciendo el tiempo ejecutando un flujo más eficiente, ya que con el iterador interno debería recorrer toda la lista por cada llamada al mismo.