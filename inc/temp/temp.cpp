#include "temp.h" // inclusion de la cabecera 


    // El temp:: designa la clase con la que se trabajata

temp::temp(uint16_t pintemp){  // Esta funcion tomara el pin que se ingrese y guardarlo en el atributo

    this->pintemp = pintemp;    // el this--> sirve para establecer la primera variable como el atributo del objeto
}

temp::~temp(){
}

void temp::recep(uint16_t pintemp){ // defino el metodo en el cual trabajara nuestra funcion
    pinMode (pintemp, INPUT);
    float valtemp = analogRead(pintemp);
    Serial.println(valtemp);
}


