CREADO, OPTIMIZADO y TESTEADO POR MAXI
23/7 | Al final del archivo se documentan los cambios futuros de esta fecha

### CODIGO MINIMO

```
    const char *portName = "COM5";
    SerialReader serialReader(portName);

    if (!serialReader.openPort())
    {
        system("pause");
        return 1;
    }

    serialReader.configurePort();

    std::thread reader(&SerialReader::startReading, &serialReader);
    reader.detach();
```

### METODOS SIN EXPLICACION

Esta libreria tiene 5 metodos, 3 son obligatorias para que esto funcione

<#> openPort("puerto");
<#> configurePort(); Este metodo no es estrictamente obligatorio pero conviene usarlo, simplemente se pone asi y ya está
<#> readFromPort(); Este es un metodo interno aunque no está declarado como privado, este metodo es llamado por otro asi que no se deberia poner salvo casos especificos
<#> startReading(); Este metodo inicia el hilo para leer datos sin parar, este es el que llama al readFromPort();
<#> setDigitalLow(const int *relayPin); Hecho especificamente para los algoritmos, se envia por UART el pin y el estado del rele a desactivar, el argumento es el numero o nombre del pin (char)
<#> setDigitalHigh(const int *relayPin) Lo mismo que el anterior pero para establecer en alto, mismo argumento (char)

### METODOS EXPLICADOS

##### openPort("puerto)

Este metodo abre y establece los parametros para poder leerlo tanto nosotros como el mismo UART, se abre normalmente a 9600 baudios
Tiene un manejo de errores simple sin la libreria para excepciones

##### configurePort();

Idealmente nunca mas se toca esta configuracion, no es necesaria para que funcione, pero conviene para prevenir pensando en un caso real.
Basicamente establece los baudios, los bytes, bit de stop y la pariedad, ademas de los intervalos de timeout, asi como esta al dia 23/7 funciona perfecto

##### readFromPort();

Lee e imprime en la consola, nada mas, como dice el documento mas arriba, no se necesita llamar individualmente, salvo algun caso especifico en los algoritmos

##### startReading();

Este metodo inicia el hilo que hace correr el metodo anterior, que lee todo el tiempo cada 1000 ms, osea 1s.
Al momento de la prueba final se deberia cambiar el tiempo y ver cual es el adecuado.

##### setDigital(const int \*relayPin);

Explico los dos juntos, no existe un metodo setDigital por las dudas.
Estos dos necesitan que al programarlo se le pase el pin, solamente tienen que poner el nombre,
por ejemplo {
char pinDigital = 33;
setDigitalLow(pinDigital);
}
por dentro, se concatena el pin como string (despues se pasa a int en arduino) y tambien se le agrega el estado
Lo que se transmite/recibe el arduino es "pin,estado"

### TESTEO

Usando el ESP32, y hardcode en algunas partes, se realizaron las siguientes pruebas

- Apertura de UART (FUNCIONA)
- Cierre de UART (FUNCIONA)
- Configuracion de UART (FUNCIONA)
- Configuracion de tiempos de espera (FUNCIONA)
- Lectura de puerto serie (FUNCIONA)
- Enviar señales al esp32 por uart (FUNCIONA)

### PLANTEAMIENTO DE CAMBIOS 23/7

- Una vez terminado los algoritmos, determinar si conviene dejar la escructura con los datos o si se declaran variables globales individuales (esto en caso de hacer los algoritmos de electricidad y tanques como librerias)

- Dependiendo de como se defina el manejo de recepcion desde arduino, cambiar los metodos de setDigital, se puede usar uno solo y que el argumento sea el pin y estado, o dejarlos separados para facil lectura y cambiar el funcionamiento

### ACTUALIZACIONES

26/7 Se cambio el formato minimo para no bloquear el uart y mantener la comunicacion bidireccional
26/7 Se agrego un simple manejo de errores
26/7 Se finalizo el testeo del codigo
