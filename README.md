# esp8266_RC
Este proyecto consiste en un programa de ejemplo para esp8266 en entorno arduino para el control via wifi de 
un vehículo de juguete.


Se usan 4 pins del módulo esp8266  para manejar los motores del coche con un típico doble puente en "H".
Dos pins para el movimiento "Adelante-Atrás" y los otros dos pins para la dirección "Izquierda - Derecha"

El programa embebido en el módulo esp8266 establece un punto de acceso al que se puede conectar  
un smartphone, tablet, ... ( Android x ejemplo ).
Después escucha por peticiones web html en el puerto 80 y devuelve una página web con una imágen que simula un 
joystick , y el código para hacer llegar las pulsaciones al módulo esp8266 vía websokets.

El programa en el módulo esp8266 escucha de nuevo por conexiones websocket en el puerto 81 para pasar a los pines de control de los motores los comandos recibidos desde las pulsaciones sobre la pantalla del smartphone cliente.

La página web que se envía al cliente tiene en cuenta los doble-tap para no hacer zoom sobre la imágen.


El archivo index.html es la página que se envía al cliente y que se puede leer fácilmente.
El archivo index.min.html es la misma página sin espacios, retornos de carro etc.
El archivo jota_esp8266_RC.ini es el programa en entorno arduino para cargar en el modulo esp8266


Este proyecto está sujeto a licencia GPL V2.0  (si, se podría considerar anticuada)


Jota.  enero de 2017

