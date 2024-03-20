# Comandos de git

- git pull - Actualiza la version del programa en nuestro pc segun ultimo commit
- git branch - Crear rama de trabajo
- git branch -l - Lista de la ramas existentes
- git checkout <Nombre de la rama> - Permite cambiar de rama
- git status - Para ver el estado de los archivos del repo
- git fetch - Ver los ultimos cambios de la rama

### Comandos para subir updates locales

- git add <Archivo o .> - Permite añadir los cambios hechos a la cola de subida del repo
- git commit -m "TEXTO" - Añadir una breve descripcion sobre los cambio
- git push origin <Rama> - Subir los archivos con commit a la rama de trabajo (para subir a otra rama cambiarse con checkout)

## Carpetas y archivos

- bin - Archivos .exe o ejecutables
- doc - Archivos documento de investigacion, datasheet, organizacion, etc
- inc - Archivos cabecera / librerias propias, como quieran decirle
- obj - Archivos de pre-creacion de ejecutables, no tocar esta carpeta
- src - Archivos de codigo fuente
- Makefile - Archivo que trabaja con <make> y se encarga de automatizar un test o compilar y depurar el proeycto
- .gitignore - Archivo para github que permite seleccionar que archivos o carpetas no subir
- README.me - Es este archivo que se ve cuando abris el repo y donde esta lo mas frecuente
