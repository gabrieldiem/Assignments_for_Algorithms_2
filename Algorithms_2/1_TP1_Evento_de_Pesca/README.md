# Breve Introducción

El programa presentado para evento_pesca funciona de la siguiente manera:

Primero se consiguen las rutas de los archivos, siendo estrictamente necesaria la del arrecife para el funcionamiento y siendo opcional la de guardado del acuario, contando con una ruta predeterminada. 
Luego se crea el arrecife, donde se revisa si la extensión del archivo contenido en la ruta recibida es realmente ".txt", cosa necesaria para detectar el tipo de archivo. Posteriormente se cargan los pokemones teniendo como primera dirección de memoria al puntero de "pokemon" de arrecife, haciendo una reserva en el heap inicial yluego una por cada pokemon. 

Se inicializa en la memoria dinámica un acuario, dejándolo listo para copiar elementos. 

Una vez que los pasos previos se completan y la ejecución sigue (fallando y retornando -1 si hubiese sucedidoalgún error), se pueden usar las funciones para trasladar pokemon y censar el arrecife, esta última recibe unafunción como parámetro y hace de intermediario para iterar y mostrar todos los pokemones. La función paratrasladar posee el flujo condicionado por una función booleana que utilizando la función de selección recibidacomo parámetro, determina si las condiciones son aptas para trasladar (hay cantidad suficiente para el traslado). Luego, el flujo de "trasladar" continúa buscando la posición de un pokemon apto para el traslado y copiándolo alacuario, si el copiado fuese exitoso, la posicion se agregaría a un vector de posiciones que tienen que ser eliminadasdel arrecife, si hubiese fallado, revertiría las copias ya hechas en el acuario y retornaría para poder hacer un posiblesiguiente traslado. Continuando, al copiar todos los pokemones respetando la cant_seleccion, se procedería a eliminardel arrecife los pokemones copiados con éxito al acuario. En caso de que el arrecife se quedara sin pokemones paratrasladar, liberaría la memoria de su puntero de pokemones y lo haría apuntar a NULL, quedando su campo de cantidad encero. 

Para finalizar, guarda los pokemones del acuario en archivo de texto. Y luego libera la memoria del heap reservadautilizada previamente.

<br>

# Explicación de compilación


La línea de compilación es la siguiente:

```bash
gcc *.c -Wall -Werror -Wconversion -std=c99 -o evento_pesca
```

Lo que hace esta línea de compilación es llamar al compilador GCC (GNU Compiler Collection) y "decirle" que detecte todos losarchivos de extensión ".c" para la compilación. Luego se expresan diversos flags que le ordenan al compilador respectivamente que:active todos los warnings (excepto los que están en Wextra, que activa extra warnings), que considere todos los warnings como errores(es decir, que si hay un warning no compilará hasta que se solucione, como si fuera un error), que considere como warnings lasconversiones implícitas (cuando se convierte entre tipos de datos distintos sin un casteo), y que use la versión estándar dellenguaje C del año 1999. Luego se utiliza "-o" para ensamblar todo en un sólo archivo binario ejecutable y le da el nombre de "evento_pesca".

Para la ejecución del programa se debe ingresar como parámetro fundamental por línea de comando la ruta (o nombre si está en elmismo directorio que el ejecutable) del archivo del arrecife para que lo cargue, de otra manera se imprimirá un mensaje de error que indique eso. El parámetro de la ruta de guardado del acuario es opcional, se presenta cómo el programa detecta los parámetros con un ejemplo:

```bash
./evento_pesca arrecife.txt un_acuario.txt
```
También funcionaría:

```bash
./evento_pesca arrecife.txt
```

En este último caso, guardándose el acuario en un archivo ubicado en el mismo directorio del ejecutable con el nombre "acuario.txt".

NOTA: En el .zip entregado se encuentra un archivo arrecife de pruebas llamado "arrecife_test.txt".


<br>

# Explicación de ejecución de Valgrind

La línea de ejecución de Valgrind propuesta en la consigna del TP es la siguiente:

```bash
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./evento_pesca
```

mostrar cada pérdida de memoria individual en detalle, que haga tracking de valores no inicializados útiles para detectar errores de memoria,a su vez también favorece a la utilidad del resultado analizado en detrimento de la velocidad de ejecución, el último flag muestra la memoria"alcanzable" y la "perdida indirectamente" del programa además de la "perdida definitivamente" y la "posiblemente perdida". El último argumentoes una orden para que ejecute el binario ejecutable de evento_pesca. Aquí es donde sugiero que en vez de esta línea de ejecución de Valgrind se utilice la mostrada a continuación:

```bash
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./evento_pesca un_arrecife.txt
```
					
Esta sugerencia viene de la mano que es necesario un archivo arrecife para poder hacer un testeo más a fondo y ver los traslados de lospokemones en acción. Sin el argumento extra de la ruta del arrecife se ejecutaría finalizando en que no se pudo obtener un archivo para leer(caso que está cubierto con fin de ejecución correcta). Opcionalmente, al igual que en la línea de compilación, se puede colocar el argumentode guardado de acuario, pero ya se posee una ruta predeterminada.

<br>

# Explicación de conceptos varios

1. Punteros: Un puntero de tipo de dato "x" es una variable que contiene la dirección de memoria de otra variable del mismo tipo de dato "x". Se propone un ejemplo:

```c
		int un_entero;
		int* puntero_a_entero = &un_entero;
```
												
2. Aritmética de punteros: Entendiendo que un puntero contiene una dirección de memoria, lo que es posible hacer es ir moviéndosea través de un bloque de memoria (siempre y cuando se tenga permiso y acceso adecuado a dicha memoria) como si fueraun vector, porque lo es, (teniendo en cuenta que diferentes tipos de variable tienen diferentes tamaños por unidadde variable) considerando que cada dirección/celda de memoria almacena un byte. Los vectores declarados como vector[MAX], poseen su valor de puntero como la dirección de la primer celda de memoria del mismo.
                

 3. Punteros a funciones: Es un puntero capaz de apuntar a la dirección de memoria de inicio del código de cualquier función, la cualestá almacenada en el apartado de memoria TEXT, encontrado durante la ejecución del programa. Cel uso de estospunteros se es capaz de llamar a una función desde cualquier parte del programa. Pueden sutilizadas paraparametrizar determinadas funciones y evitar repetición constante de código cuando sólo unpartecitas cambian, porejemplo, que sólo cambiase una comparación.
                

 4. Malloc y Realloc: Malloc reserva una determinada cantidad de bytes en la memoria dinámica (heap), la cual es controladaexclusivamente por el programador y debe pedir su acceso al sistema operativo y liberarla una vque ya no esté enuso antes del fin de ejecución del programa. Devuelve un puntero a la primera dirección de memordel bloque dememoria reservado o NULL si no pudo hacerlo. Puede ayudar a entender el concepto, expandir nombre MALLOC en MemoryALLOCation. Realloc por su parte, modifica el tamaño de un bloque de memoria apuntado por puntero dejándolo en unadeterminada nueva cantidad de bytes. Puede devolver un puntero a un nuevo bloque de memoria qcontiene una copia delcontenido del puntero anterior (lo que entra en la nueva cantidad de bytes), es decir, realocó contenido en memoria;puede que devuelva la misma dirección de memoria que el puntero que intenta realoc, es decimodificó el tamaño delmismo bloque; o puede devolver NULL en caso de no poder realocar.