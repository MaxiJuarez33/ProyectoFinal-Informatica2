## Cambios

* Se introdujo la clase base abstracta Device para representar dispositivos genéricos con métodos virtuales para activación, desactivación y verificación del estado.
* Se añadió la implementación concreta SpecificDevice que utiliza mapas estáticos para asociar tipos de dispositivos con pines y corrientes específicas.
* Se creó la clase TempAnalyzer para analizar temperaturas y gestionar dispositivos según las condiciones térmicas y el nivel de batería.
* Los dispositivos ahora dependen de un SerialReader para interactuar con los pines (actualmente comentado).


## Funcionamiento

###  Clase Device
####    Métodos principales:
  * bool canActivate() const: Verifica si el dispositivo puede activarse (niveles de batería > 30 y alguien está en casa).
  * bool isPriority() const: Determina si el dispositivo tiene prioridad para activarse según su consumo y estado.
  * bool isPeopleHome() const: Determina si hay personas en casa según los datos del sistema.
###  Clase SpecificDevice
####    Funcionamiento:
  * Usa mapas estáticos (pinMap y currentMap) para asociar cada tipo de dispositivo con un pin y una corriente.
  * Métodos virtuales sobreescritos para obtener el pin (getPin) y la corriente (getCurrent).
  * activate y deactivate controlan el dispositivo con mensajes de serial o en consola.
###  Clase TempAnalyzer
####    Análisis térmico y control:
  * Convierte datos de sensores en temperaturas internas y externas.
  * Evalúa diferencias térmicas para determinar si el aire acondicionado o el ventilador exterior deben activarse.  
###  Métodos:
  * tempConverter(): Calcula las temperaturas a partir de datos del sistema.
  * controlDevices(): Gestiona la activación/desactivación de dispositivos según temperatura y nivel de batería.
  * analyze(): Revisa y ajusta el estado de los dispositivos.
###  main
  * Crea una instancia de TempAnalyzer para iniciar la lógica de control.

## TODO
  * Documentación detallada al estilo de doc/serialReader.md.
  * Implementar la lógica comentada en SpecificDevice::activate y SpecificDevice::deactivate para controlar pines.
  * Agregar métodos para eliminar y editar dispositivos dinámicamente.
  * Expandir TempAnalyzer para incluir más dispositivos y reglas.
