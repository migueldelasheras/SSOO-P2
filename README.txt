PARTICIPANTES
    
    - MIGUEL DE LAS HERAS FUENTES
    - FRANCISCO JESÚS DÍAZ PELLEJERO
    - JAVIER VILLAR ASENSIO


EXPLICACIÓN DETALLADA DEL PROYECTO

    El proyecto está dividido en un total de 4 ficheros; dos ficheros .cpp y otros dos ficheros .h. El fichero
    "colors.h" alojado en el directorio "include" se utiliza para definir los colores que más tarde usaremos 
    en la impresión de los resultados obtenidos. El otro archivo .h es "Searcher.h" que utilizaremos para las 
    definiciones tanto de la estructura "Result" que usaremos para dar forma a los resultados; como de la clase 
    "Searcher" cuyos métodos serán implementados en el archivo "Searcher.cpp" alojado en el directorio /src.
    Por último, el archivo SSOOIIGLE será quien contenga el main del programa.
    Además, tendremos un archivo Makefile que utilizaremos para compilar y ejecutar el programa.

MODO DE EJECUCIÓN

    1. Compilar el programa mediante el comando "make all"
    2. Ejecutar cualquiera de los ejemplos definidos en el makefile mediante make test<1,2 o 3> o realizar una 
        ejecución manual mediante ./exec/SSOOIIGLE <file> <word> <num_threads>