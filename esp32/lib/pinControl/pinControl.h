#ifndef _PINCONTROLH_
#define _PINCONTROLH_

#include <Arduino.h>

class analog // se crea el objeto
{
private:               // se crean atributos no modificables por el usuario, solo se utilizan dentro del objeto
    uint8_t analogpin; // pines de los sensores
public:
    analog(uint8_t analogpin); // funcion constructor, sirve para inicializar el objeto y establecer sus valores

    double recep(); // definicion de la funcion que se utilizara, se le debe agregar los atributos entre parentesis
};

class digital
{
private:
    uint8_t digitalpin;

public:
    digital() : digitalpin(0) {}
    digital(uint8_t digitalpin);

    void emitir(uint8_t digitalval);
};

#endif