#ifndef _EMTRPINH_
#define _EMTRPINH_

#include <arduino.h>

class analog // se crea el objeto llamado analog
{
private:                // se crean atributos no modificables por el usuario, solo se utilizan dentro del objeto
    uint16_t analogpin; // pines de los sensores
public:
    analog(uint16_t analogpin); // funcion constructor, sirve para inicializar el objeto y establecer sus valores
    ~analog();                  // funcion decostructor, se llama cuando se quiere liberar memeoria, debe tener el mismo nombre que la clase

    void recep(uint16_t analogpin); // definicion de la funcion que se utilizara, se le debe agregar los atributos entre parentesis
};

class digital
{
private:
    uint8_t digitalpin;

public:
    digital(uint8_t digitalpin);
    ~digital();

    void emitir(uint8_t digitalpin, uint8_t digitalval);
};

#endif
