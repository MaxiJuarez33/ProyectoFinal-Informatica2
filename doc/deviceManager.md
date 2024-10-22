### Cambios

- Ahora los dispositivos se stackean en el archivo, no se sobreescriben
- Ahora el id se obtiene automaticamente en base al anterior
- Ahora no es necesario pasar el "filename" en fileManage

#### Funcionamiento getDevice

Se puede usar para obtener un dispositivo usando el ID, el nombre completo o palabra clave, respetando mayusculas

- En caso de solo nombre
  getDevice("nombre", -1);
- En caso de solo id
  getDevice("", id);

### TODO

- Documentacion del estilo a doc/serialReader.md
- Funcion para eliminar dispositivos y editarlos
