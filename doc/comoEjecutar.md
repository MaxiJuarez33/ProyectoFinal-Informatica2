Para probar el main se tiene que crear el .exe con comandos, mas adelante se hace un makefile.
Para poner el comando fijarse de estar en la posicion de la carpeta raiz, si hicieron todo bien deberia ser ProyectoFinal-Informatica2
Si estan en una carpeta interna no va a funcionar el comando

Comando al dia 23/7 con una sola libreria

g++ -Iinc -o ProyectoFinal src/main.cpp inc/serialReader.cpp

Comando para tankManager

g++ -Iinc -o ProyectoFinal src/main.cpp inc/serialReader.cpp inc/tankManager.cpp

Comando para dataManage con libreria externa en lib

g++ -Iinc -Ilib -o ProyectoFinal test/jsonTest.cpp inc/deviceManager.cpp lib/json.hpp

Comando para electricAlgorithm (en src/main)

g++ -Iinc -Ilib -o ProyectoFinal src/main.cpp inc/serialReader.cpp inc/deviceManager.cpp inc/tankManager.cpp lib/json.hpp

Comando para electricAlgorithm en test en src/main

g++ -Iinc -Ilib -o ProyectoFinal src/main.cpp inc/serialReader.cpp inc/deviceManager.cpp inc/tankManager.cpp inc/electricManager.cpp lib/json.hpp

En caso de probar los codigos para test

g++ -Iinc -o ProyectoFinal test/serialReaderTest.cpp inc/serialReader.cpp
