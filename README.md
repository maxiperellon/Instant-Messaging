# Instant-Messaging

- _Es un programa realizado en C con base de datos y un log donde se van a ir almacenando los datos del chat, tanto mensajes, usuarios, fecha y hora._

## Funcionamiento

### [Server](https://github.com/maxiperellon/Instant-Messaging/tree/master/server)

- Una vez ejecutado el `server`, se conecta el socket y queda escuchando hasta que se conecten los clientes a él.
- Cuando un cliente es agregado al servidor, se acepta la conexión. Una vez establecida dicha conexión ya es posible chatear con otro cliente, también se puede ir viendo los 'id' de la conexión establecida y de la conversación entre ellos.

```
    ##########################################
    
            Base de datos conectada...
    
    Table: 'data'
    
    ##########################################
    
    ##########################################
    
            Servidor CONECTADO...
            PUERTO: 8000
    
    ##########################################
            
    Esperando las conexiones...
```

### [Client](https://github.com/maxiperellon/Instant-Messaging/tree/master/client)

- Una vez ejecutado el `client`, se conecta al socket a través del mismo puerto e ip del servidor, y con los siguientes comandos se puede realizar:

    - `ADD <username>`:          agregar un usuario para poder chatear
    - `LIST`:                    podemos ver quienes se encuentran conectados para establecer una conversación
    - `CHAT <username> msg`:     chatear con otra persona que se encuentre conectada y agregar el mensaje que queremos enviarle
    - `BYE`:                     podemos salir de la conversación para poder chatear con otra persona si quisiéramos
 
 ```       
                         >>> BIENVENIDO AL CHAT <<<<
     
             Introduce ADD y tu nombre de usuario 
             ADD: 'Agrega tu nombre de usuario al servidor' 
             LIST: 'Muestra los usuarios conectados y disponibles'
             CHAT <username> msg: 'Permite chatear con un usuario que se encuentre disponible'
             BYE: 'Permite salir del chat y poder chatear con otro usuario disponible'
     
    -> 
```
    
##   
#### Alumnos:
   - [Grasso, Ornella](https://github.com/orneegv/) 
   - [Perellón, Maximiliano](https://github.com/maxiperellon/)
##


