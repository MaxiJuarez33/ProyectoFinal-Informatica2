# Comandos de git

- git pull - Actualiza la version del programa en nuestro pc segun ultimo commit
- git branch - Crear rama de trabajo
- git branch -l - Lista de la ramas existentes
- git checkout <Nombre de la rama> - Permite cambiar de rama
- git status - Para ver el estado de los archivos del repo
- git fetch - Ver los ultimos cambios de la rama

#### Comandos para subir updates locales al repo remoto

- git add <Archivo/carpeta o .> - Permite añadir los cambios hechos a la cola de subida del repo
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

# Uso de Issues en GitHub

- Issues es una parte que permite hacer seguimiento de tareas, discutir ideas, opiniones, reportar bugs y ese tipo de cosas
- Pueden ser hilos tan largo se quiera pero lo mejor seria cerrarlos cuando consideramos que hay una solucion
- Cuando creen un Issue para lo que sea, traten de que sea dinamico y conciso, asi como algunos que ya deje yo ahi; y usenlo cada vez que encuentren un bug o algo sobre lo que tengan duda, 0 whatsapp
- Cuando comenten un Issue, fijense de que al lado de compartir o publicar comentario, aparece que el comentario que van a poner es para cerrar el Issue, al no ser que eso quieran, selecciones la otra opcion que no lo cierra. Si lo cierran sin querer no pasa nada, se puede volver a abrir
- Todo lo que investiguen desde internet, copien el link y ponganlo en el archivo <investigaciones.md> de la carpeta docs
- Todo lo que se comente y discuta en Issues sera usado para hacer los respectivos informes

### Projects

- Para hacer un mejor seguimiento vamos a usar los projects que esten linkeados a los issues
- Cuando terminen una tarea asignada, sea individual o grupal, si está terminada cambien el estado de la misma
- Si quieren crear una nueva tarea, solicitenlo usando issues y @owner

## Hacer pull request a main

- El Pull Request es para poner en revision los cambios hechos en otra rama, si el cambio es aprobado, los cambios son incorporados a la rama de destino
- Haber testeado todo usando los metodos planteados
- Optimizar al maximo el codigo
- Carpetas con nombres correctos segun lo acordado

## Hacer pull request a pre-main

- Hacer pull request a pre-main es para pedir que otro lo testee u optimice el codigo
- Carpetas con nombres correctos segun lo acordado
