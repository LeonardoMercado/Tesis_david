/* PROGRAMA DE CONTROL PARA DISPARO DE LAMPARAS DE XÉNON.
 * 
 * Autor: Leonardo Fabio Mercado Benítez.
 * 
 * Versión: 0.2
 * 
 * Conexiones: 
 *  - Pin D2: Salida de control para el driver del IGBT 1.
 *  - Pin D3: Salida de control para el driver del IGBT 2.
 */
 
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//- - - - - - - - LIBRERIAS - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - - -
#include <SoftwareSerial.h> // Libreria para la comunicación serial.

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//- - - - - - - - VARIABLES - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int disparo = 2;              // Definición del puerto GPIO de disparo de 1.
int disparo_2 = 3;            // Definición del puerto GPIO de disparo de 2.
int tiempo_disparo_1 = 0;     // Variable para almacenar el tiempo de disparo 1.
int delay_disparos = 0;       // Variable para almacenar el delay entre disparos.
int tiempo_disparo_2 = 0;     // Variable para almacenar el tiempo de disparo 2.
int led = 13;                 // Definición del led testico para pruebas.
String objetivo = "";         // Cadena que almacena el tipo de señal entrante por el bluetooth.
String valor_auxiliar_1 = ""; // Almacena temporalmente el valor entrante en forma de cadena {Tiempo disparo 1}.
String valor_auxiliar_2 = ""; // Almacena temporalmente el valor entrante en forma de cadena {Delay entre disparos}.
String valor_auxiliar_3 = ""; // Almacena temporalmente el valor entrante en forma de cadena {Tiempo disparo 2}.
String cadena = "";           // Almacena toda la cadena proveniente del bluetooth.

void setup(){ 
  Serial.begin(38400);      // Inicialización del objeto para la comunicación serial.
  while(!Serial){           // Espera a la inicialización del serial.    
  } 
  pinMode(disparo,OUTPUT);   // Setup del puerto GPIO para disparo 1.
  pinMode(disparo_2,OUTPUT); // Setup del puerto GPIO para disparo 2.
  pinMode(led,OUTPUT);       // Setup del led testigo para pruebas.
}

void loop(){
  Datos();                                            // Llamada a la función Datos().
  if(objetivo == "d"){                                // Si el objetivo enviado en la cadena es d.
    digitalWrite(led,HIGH);                           // Enciende el led integrado en el micro.
    digitalWrite(disparo,HIGH);                       // Pone el disparo (GPIO-2) en alto.
    //Serial.println("d1:1");                           // Para Debug.
    delay(delay_disparos);                            // Espera el tiempo de delay.
    digitalWrite(disparo_2,HIGH);                     // Pone el disparo 2 (GPIO-3) en alto.
    //Serial.println("d2:1");                           // Para Debug.
    int interDelay = delay_disparos-tiempo_disparo_1; // Calcula el tiempo inter-delay.
    delay(interDelay);                                // Espera el tiempo inter-delay.
    digitalWrite(disparo,LOW);                        // Pone el disparo (GPIO-2) en bajo.
    //Serial.println("d1:0");                           // Para Debug.
    int interLamda2 = tiempo_disparo_2-interDelay;    // Calcula el tiempo inter-lambda2.
    delay(interLamda2);                               // Espera el tiempo inter-lambda2.
    digitalWrite(disparo_2,LOW);                      // Pone el disparo 2 (GPIO-3) en bajo.
    //Serial.println("d2:0");                           // Para Debug.
    digitalWrite(led,LOW);                            // Apga el led integrado en el micro.
    objetivo = "";                                    // Limpia el objetivo.
    tiempo_disparo_1 = 0;                             // Limpia el tiempo de disparo 1.
    delay_disparos = 0;                               // Limpia el tiempo de delay.
    tiempo_disparo_2 = 0;                             // Limpia el tiempo de disparo 2.
  }
     
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//- - - - - - - - FUNCION PARA VALIDAR DATOS EN EL BUFFER - - - - - - - - - - - - - - - - - - - - - - - - - 
void Datos(){
  if(Serial.available()>0){ // Si hay datos en el buffer del bluetooth.
    recibirMensaje();       // Llamar a recibir el mensaje.
  }  
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//- - - - - - - - FUNCION PARA RECIBIR EL MENSAJE SERIAL ENTRANTE, EL OBJETIVO Y SU VALOR - - - - - - - - - 
void recibirMensaje(){
  cadena = String("");
  while (Serial.available()) {                                              // Mientras Hayan datos en el Buffer.
    cadena = cadena + char(Serial.read());                                  // Concatena el mensaje y lo almacena en cadena.
    delay(1);                                                               // Delay de estabilidad.
  }  
  if (cadena.indexOf(",") >=0){                                             // Comprobación del formato del mensaje correcto. [objetivo[coma]valor].
    objetivo = cadena.substring(0,(cadena.indexOf(",")));                   // Filtra el objetivo de la cadena.
    int aux_v1 = cadena.indexOf(",")+1;                                     // Variable auxiliar que almacena el indice inicial del tiempo de disparo 1.
    valor_auxiliar_1 = cadena.substring(aux_v1,cadena.indexOf(",",aux_v1)); // Filtra el tiempo de disparo 1 de la cadena.
    int aux_v2 = cadena.indexOf(",",aux_v1)+1;                              // Variable auxiliar que almacena el indice inicial del delay entre disparos.
    valor_auxiliar_2 = cadena.substring(aux_v2,cadena.indexOf(",",aux_v2)); // Filtra el tiempo de delay entre disparos de la cadena.
    int aux_v3 = cadena.indexOf(",",aux_v2)+1;                              // Variable auxiliar que almacena el indice inicial del tiempo de disparo 2.
    valor_auxiliar_3 = cadena.substring(aux_v3);                            // Filtra el valor 1 de la cadena.
    tiempo_disparo_1 = valor_auxiliar_1.toInt();                            // Convierte el valor del tiempo de disparo 1 en entero.
    delay_disparos = valor_auxiliar_2.toInt();                              // Convierte el valor del tiempo de delay entre disparos en entero.
    tiempo_disparo_2 = valor_auxiliar_3.toInt();                            // Convierte el valor del tiempo de disparo 2 en entero.
    Serial.println(objetivo);                                               // Para debug - Comprobación del objeto recibido.
    Serial.println(tiempo_disparo_1);                                       // Para debug - Comprobación del tiempo de disparo 1 recibido.
    Serial.println(delay_disparos);                                         // Para debug - Comprobación del tiempo de delay recibido.
    Serial.println(tiempo_disparo_2);                                       // Para debug - Comprobación del tiempo de disparo 2 recibido.
  }
}
