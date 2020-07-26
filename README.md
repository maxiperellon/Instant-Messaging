# Instant-Messaging

- _Es un programa realizado en C con base de datos y un log donde se van a ir almacenando los datos del chat, tanto mensajes, usuarios, fecha y hora._

## Funcionamiento

### Server

- Una vez ejecutado el `server`, se conecta el socket y queda escuchando hasta que se conecten los clientes a él.
- Cuando el cliente se agrega al servidor se acepta la conexión, una vez establecida dicha conexión puede chatear con otro cliente. También podemos ir viendo los id de la conexión establecida y de la conversación entre ellos.

### Client 

- Una vez ejecutado el `client`, se conecta al socket a través del mismo puerto e ip del servidor, y con los siguientes comandos se puede realizar:

    - `ADD username `         El usuario se encuentra conectado listo para chatear
    - `LIST`                  Podemos ver quienes se encuentran conectados para establecer conversación
    - `CHAT username msg`     Chatear con otra persona que se encuentre conectada y agregar el mensaje que queremos enviarle
    - `BYE`                   Podemos salir de la conversación para poder chatear con otra persona si quisieramos
 
    
    
#### Alumnos:
        Grasso, Ornella - Perellón, Maximiliano
