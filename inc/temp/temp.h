#ifndef _TEMPH_
#define _TEMPH_

#include <arduino.h>

class temp // se crea el objeto llamado temp
{
private: // se crean atributos no modificables por el usuario, solo se utilizan dentro del objeto
    uint16_t pintemp, pintemp1; // pines de los sensores
public:
    temp(uint16_t pintemp); // funcion constructor, sirve para inicializar el objeto y establecer sus valores
    ~temp(); // funcion decostructor, se llama cuando se quiere liberar memeoria, debe tener el mismo nombre que la clase

    void recep(uint16_t pintemp); // definicion de la funcion que se utilizara, se le debe agregar los atributos entre parentesis
};

#endif