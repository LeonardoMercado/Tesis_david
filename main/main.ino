/* PROGRAMA DE CONTROL PARA DISPARO DE LAMPARAS DE XÉNON.
 * 
 * Autor: Leonardo Fabio Mercado Benítez.
 * 
 * Versión: 0.1
 * 
 * Conexiones: 
 *  - Pin  D2: Salida de control para driver del IGBT. 
 */
 
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//- - - - - - - - LIBRERIAS - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - - - -
#include <SoftwareSerial.h> // Libreria para la comunicación serial.

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//- - - - - - - - VARIABLES - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int disparo = 2;            // Definición del puerto GPIO de disparo.
int tiempo_disparo = 0;     // Variable para almacenar el tiempo de disparo.
int led = 13;               // Definición del led testico para pruebas.
int valor;                  // Variable para almacenar el valor dirigido al objetivo.
String objetivo = "";       // Cadena que almacena el tipo de señal entrante por el bluetooth.
String valor_auxiliar = ""; // Almacena temporalmente el valor entrante en forma de cadena.
String cadena = "";         // Almacena toda la cadena proveniente del bluetooth.

void setup(){ 
  Serial.begin(38400);      // Inicialización del objeto para la comunicación serial.
  while(!Serial){           // Espera a la inicialización del serial.    
  } 
  pinMode(disparo,OUTPUT);  // Setup del puerto GPIO para disparo de la lamparas.
  pinMode(led,OUTPUT);      // Setup del led testigo para pruebas.
}

void loop(){
  Datos();                      // Llamada a la función Datos().
  if(objetivo == "d"){
    digitalWrite(led,HIGH);
    digitalWrite(disparo,HIGH); //
    delay(valor);
    digitalWrite(led,LOW);
    digitalWrite(disparo,LOW);
    objetivo = "";
    valor = 0;
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
  while (Serial.available()) {                                // Mientras Haya dato en el Buffer.
    cadena = cadena + char(Serial.read());                    // Concatena el mensaje y lo almacena en cadena.
    delay(1);                                                 // Delay de estabilidad.
  }  
  if (cadena.indexOf(" ") >=0){                               // Comprobación del formato del mensaje correcto. [objetivo[espacio]valor]
    objetivo = cadena.substring(0,(cadena.indexOf(" ")));     // Filtra el objetivo de la cadena.
    valor_auxiliar = cadena.substring(cadena.indexOf(" ")+1); // Filtra el valor de la cadena.
    valor = valor_auxiliar.toInt();                           // Convierte el valor del objetivo a entero.
    //Serial.println(objetivo);                               // Para debug - Comprobación del objeto recibido.
    //Serial.println(valor);                                  // Para debug - Comprobación del valor para el objeto recibido.
  }
}
