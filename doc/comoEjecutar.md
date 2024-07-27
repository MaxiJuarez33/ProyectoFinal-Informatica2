Para probar el main se tiene que crear el .exe con comandos, mas adelante se hace un makefile.
Para poner el comando fijarse de estar en la posicion de la carpeta raiz, si hicieron todo bien deberia ser ProyectoFinal-Informatica2
Si estan en una carpeta interna no va a funcionar el comando

Comando al dia 23/7 con una sola libreria

g++ -Iinc -o ProyectoFinal src/main.cpp inc/serialReader.cpp

En caso de probar los codigos para test

g++ -Iinc -o ProyectoFinal test/serialReaderTest.cpp inc/serialReader.cpp
