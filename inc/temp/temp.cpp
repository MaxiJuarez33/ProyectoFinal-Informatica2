#include "temp.h"

temp::temp(uint16_t pintemp){

    this->pintemp = pintemp;
}


temp::~temp(){

}

void temp::recep(uint16_t pintemp){
    pinMode (pintemp, INPUT);
    int valtemp = analogRead(pintemp);
    Serial.println(valtemp);
}


