# Explicación del programa entregado
	
### Sobre el TDA Lista

Al TDA lista se le realizó una modificación y se le agregó una funcionalidad. Como modificación se le agregó un destructor de elementos opcional
que se le es aplicado al elemento cuando se elimine de la lista, si el destructor es ingresado como NULL al crear la lista se dice que la lista no
posee destructor por lo que no le hará nada al elemento luego de borrar los nodos. La funcionalidad que se le agregó es una función llamada
"lista_hacer_copia_invertida", cuya descripción de funcionamiento se encuentra en el header lista.h. Se decidió agregar teniendo en cuenta los
principios de un TDA y la flexibilidad que debe ofrecer para brindar soluciones a soluciones dentro del marco de TDA, en este caso se necesitó
una función que opere con listas de la manera requerida y se implementó.
	
		
### Sobre el TDA Heap

El TDA heap fue implementado teniendo como base a un vector dinámico de elementos donde son guardados todos los punteros a los elementos insertados
en el heap. De esta manera se puede hacer un uso óptimo de la memoria dinámica redimensionando el vector según lo necesario. Las funciones implementadas
son las necesarias para poder llevar a cabo el cometido del TP2 Aventura Pokemon, por lo que no se implementaron funciones como "heap sort" o "heapify".
Como aclaración se tiene que hay dos funciones que operan con la raíz del heap: la primera llamada "heap_extraer_elemento_raiz" elimina el elemento
ubicado en la raíz del heap mientras que lo devuelve como retorno de la función (manteniendo la estructura heap válida haciendo los reordenamientos
correspondientes), si ocurriese un error la función trataría de revertir los cambios; la segunda llamada "heap_elemento_raiz" devuelve el elemento
ubicado en la raíz del heap pero no lo elimina de este, se lo puede tomar como una consulta para saber cuál es el elemento raíz.
Al igual que en el caso del TDA lista, el heap ofrece la posibilidad de usar un destructor opcional para destruir los elementos insertados.
La lógica interna del heap está diseñada para que el resultado de la inserción en el mismo resulte en un heap minimal, respetando las reglas
establecidas para el comparador en el header heap.h
	
		
### Sobre las estructuras

En el header estructuras.h se encuentran las declaraciones de los tipo de dato personalizados utilizados a los largo del TP. Se detallará el motivo
de elección de cada campo a continuación:

- Pokemon: El struct pokemon (pokemon_t) posee un campo con su nombre y un campo por cada estadística esencial (velocidad, defensa y ataque). Estas son las
estadísticas "originales", es decir, las leídas para ese pokemon en el archivo detexto. Para implementar la "evolución" de las estadísticas a medida
que gana combates, es encuentra el campo "bonus", que, empezando desde cero,incrementará hasta 63 para reflejar la máxima progresión del pokemon, como
establecido en la consigna. Por lo tanto, los campos velocidad, ataque y defensano mutarán y representarán las estadísticas originales. Con esto se
tiene la ventaja de que se conservan los datos leidos inicialmente, pero trae ladesventaja de que cada vez que se quiere saber el valor "actual real"
de una estadística, se tiene que realizar la suma "estadística + bonus", lo cualalarga un poco el código.
	
- Personaje: El struct personaje (personaje_t) representará al personaje principal o entrenador principal, posee un campo con su nombre, otro campo para 
almacenar la cantidad de medallas que lleva por ganar gimnasios (1 victoria de ungimnasio equivale a 1 medalla) y un campo booleano para saber si el
personaje fue cargado con datos de un archivo de texto (esta variable es paramotivos de lógica interna). El razonamiento detrás de la implementación
para guardar los pokemones es el siguiente: utilizo una lista como la "caja",donde están todos los pokemones obtenidos, se utiliza esta estructura porque
ofrece flexibilidad para recorrer los pokemones, no ocupa un sólo bloque dememoria contiguo como un vector y es fácil de operar con ella, además de que se
pueden agregar pokemones sin mover todos los datos en contraste con un vectordinámico. Entonces, de esta lista  llamada "lista_caja_pokemones" se obtienen
los primeros MAX_PARTY pokemones (en este caso MAX_PARTY es 6) para tomarlos comola party del personaje principal. De esta manera, la lista contiene todos
los pokemones, y la party apunta a los primeros MAX_PARTY elementos de la lista,se comparte el puntero al mismo elemento. El vector llamado "party" es 
dinámico para que pueda ser redimensionado acorde al tamaño de la party, este esacompañado por un tope "cantidad_en_party" para denotar su cantidad.

- Entrenador: El struct entrenador (entrenador_t) representa a un entrenador de tipo entrenador, este puede ser el líder o un entrenador del gimnasio. Posee un
campo de nombre y la lógica que se utiliza para almacenar los pokemones es lasiguiente: se utiliza un vector dinámico para almacenar punteros a pokemones,
permitiendo redimensionar según sea necesario, es acompañado por un tope"cantidad_en_party" para delimitar su máximo actual. El máximo a almacenar es una
cantidad MAX_PARTY.
	
- Gimnasio: El struct gimnasio (gimnasio_t) posee un campo para el nombre, otro para almacenar la dificultad numérica y otro para conservar el id de la función
de batalla que se utilizará para decidir las victorias. Aquí se contemplaron dosposibilidades: 1) almacenar todos los entrenador en una pila siendo el primer
entrenador insertado el líder, ó 2) tener un campo separado para el líder y teneruna pila exclusiva para los entrenadores. Se optó por la segunda opción para
mantener los entrenadores separados del líder y tener claro si se terminó deciclar por los entrenadores. La elección de la pila es por su funcionalidad LIFO
(Last In First Out), por lo que leyendo el archivo de texto "de arriba haciaabajo" se tendrá una pila donde el entrenador en el tope de la lista será el
último leído, perfecto para respetar la lógica de combate del TP. La dificultades el campo clave que se utilizará para comparar y armar el heap de
carácter minimal.
	
	
### Sobre las funciones de batallas

Cada una de las cinco funciones de batalla tiene la explicación sencilla de quién gana el combate en las firmas de las mismas en batallas.h. En las
implementaciones encontradas en batallas.c se toma a cada pokemon que ingresa como un "void *" como si fuese de tipo "pokemon_t *", haciendo el casteo
correspondiente e incluyendo estructuras.h.


### Aspectos generales sobre el TP2 Aventura Pokemon

Se entrega un programa/juego cuyo main reside en "aventura_pokemon.c". Las bibliotecas utilizadas se encuentran en la carpeta principal, ellas son:
TDA lista (lista.h y lista.c), TDA heap (heap.h y heap.c), definiciones de batallas (batallas.h y batallas.c) y definiciones de estructuras (estructuras.h).
Además, se incluyen las dos consignas (2020_2C_TP2_Algo2.pdf y DiapositivasTP2.pdf), junto a dos carpetas que contienen archivos de personajes principales (pers)
y gimnasios (gims), lo cual se detallará en la siguiente sección sobre casos probados con funcionamiento esperado más adelante. Los nombres de estas dos carpetas
son cortos para agilizar la escritura de los mismos durante las pruebas del programa.
En casos donde hubiera sido fácil abortar o finalizar la ejecución de manera repentina con manejo adecuado de la memoria, se optó por "recuperar" el programa
y llevarlo a un estado exitoso donde pueda seguir funcionando, mejorando la usabilidad y amistosidad para con el usuario. Como desventaja esto trae consigo algunas
funciones llamadas "rescate" o "manejo de errores" que restauran el comportamiento a uno esperado y podrían dificultar el seguimiento estático del programa.
Se utilizó la función system en dos casos: para limpiar la pantalla de la terminal y para pausar por unos segundos la ejecución, con los comandos clear y sleep
respectivamente. Se aclara que esto hace al programa no portable a otros sistemas operativos fuera de los basados en Linux.
Se hizo énfasis en informar al usuario de sus acciones y consecuencias, así como de que la interfaz de comunicación sea clara y placentera a la vista.

<br>

# Casos probados, comportamiento

Ambas cargas de archivos requieren que el archivo sea de extensión '.txt' paraque lo lea. Esta medida fue tomada como precaución. Aunque
limita que no se puedan leer archivos que sean de texto y no utilicen dichaextensión, como la extensión '.csv', por ejemplo. Esta es una
característica que puede ser agregada dependiendo de la extensión que se desee.

### Personaje Principal
	
- Entrenador válido (intentos con 11, 6 y 1 pokemones): carga bien
- Sobrescribir entrenador con entrenador válido: sobrescribe bien
- Intenta cargar entrenador con pokemones pero formato del nombre roto: falla bien
- Intenta cargar entrenador sin pokemones: falla bien
- Intenta cargar archivo con varios entrenadores: carga bien (sólo el primer entrenador como principal)
- Intentar sobrescribir entrenador válido con entrenador con pokemones pero formato del nombre roto sobre
	entrenador valido: falla bien
- Intentar sobrescribir entrenador válido con entrenador sin pokemones: falla bien
- Intenta cargar archivo inexistente: falla bien
- Cargar archivo correcto luego de fallar cargando un archivo inexistente: carga bien
- Intenta cargar archivo inexistente luego de haber cargado un archivo correcto: falla bien
	
Referencias en carpeta 'pers/':
- Ash.txt : válido con 11 pokemones
- Ash1.txt : válido con 1 pokemon
- Ash6.txt : válido con 6 pokemones
- Ashroto.txt : inválido con 11 pokemones. La primera línea tiene mal el formato
- Ashsinpoke.txt : primera línea válida pero no tiene ningún pokemon, por lo que se lo toma como inválido
- Ashyvarios.txt : válido con 4 entrenadores (siendo Ash con 11 pokemones el primero), para comprobar lectura de sólo un entrenador
	
	
### Gimnasios
	
- Archivo de gimnasio válido (gim.txt): carga bien
- Archivo de gimnasio con un entrenador sin pokemones al final (gimEntrenadorSinPoke.txt): carga bien el líder y entrenador con pokemones. 
	Omite entrenador sin pokemones
- Archivo de gimnasio con un entrenador sin pokemones en el medio (gimEntrenadorSinPokeMedio.txt): carga bien el líder y entrenador con pokemones. 
	Omite entrenador sin pokemones
- Archivo de gimnasio con líder sin pokemones (gimLiderSinPoke.txt): falla bien
- Archivo de gimnasio con la línea de un pokemon en el medio rota (gimPokeRoto.txt): carga bien los datos hasta encontrarse con la línea rota y de ahí
	en más no carga lo siguiente.
- Archivo de gimnasio con la primera línea identificatoria del mismo rota (gimRoto.txt): falla bien
- Archivo de 5 gimnasios válidos en el mismo archivo (gims5.txt): carga bien
- Archivo de gimnasio sin líder ni entrenadores, sólo la línea identificatoria (gimSinLiderNiEntrenadores.txt): falla bien
	
Los archivos de los gimnasios se encuentran en la carpeta 'gims/'.

<br>

# Explicación de compilación y ejecución

La línea de compilación es la siguiente:

```bash	
gcc *.c -o aventura_pokemon -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
```
	
Lo que hace esta línea de compilación es llamar al compilador GCC (GNU Compiler Collection) y "decirle" que detecte todos los
archivos de extensión ".c" para la compilación. Luego se expresan diversos flags que le ordenan al compilador respectivamente que:
ensamble todo en un solo archivo binario ejecutable con "-o" y le de el nombre de "aventura_pokemon".
Que genere información de debugging, que use la versión estándar del lenguaje C del año 1999, que active todos los warnings (excepto los
que están en Wextra, que activa extra warnings), que considere como warnings las conversiones implícitas (cuando se convierte entre tipos
de datos distintos sin un casteo explícito), advierta sobre comparaciones que son siempre verdaderas o siempre falsas debido al rango
limitado del tipo de dato, de como advertencia todas las exigidas por el estándar ISO C, que considere todos los warnings como errores
(es decir, que si hay un warning no compilará hasta que se solucione, como si fuera un error), y por último que deshabilite la
optimización hecha por el compilador y sean otorgados resultados esperados a la hora de debuggear.

Para la ejecución del programa basta con ejecutar la siguiente línea:

```bash	
./aventura_pokemon
```
<br>	

# Explicación de ejecución de Valgrind

La línea de ejecución de Valgrind propuesta:

```bash	
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./aventura_pokemon
```

Lo que hace esta línea de comando es ejecutar Valgrind y expresa diversos argumentos que hacen, respectivamente, lo siguiente:
mostrar cada pérdida de memoria individual en detalle, que haga tracking de valores no inicializados útiles para detectar errores de memoria,
a su vez también favorece a la utilidad del resultado analizado en detrimento de la velocidad de ejecución, el último flag muestra la memoria
"alcanzable" y la "perdida indirectamente" del programa además de la "perdida definitivamente" y la "posiblemente perdida". El último argumento
es una orden para que ejecute el binario ejecutable de aventura_pokemon.