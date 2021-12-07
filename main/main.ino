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
int disparo_2 = 3;              // Definición del puerto GPIO de disparo de 2.
int tiempo_disparo_1 = 0;     // Variable para almacenar el tiempo de disparo 1.
int delay_disparos = 0;       // Variable para almacenar el delay entre disparos.
int tiempo_disparo_2 = 0;     // Variable para almacenar el tiempo de disparo 2.
int led = 13;                 // Definición del led testico para pruebas.
int valor;                    // Variable para almacenar el valor dirigido al objetivo.
String objetivo = "";         // Cadena que almacena el tipo de señal entrante por el bluetooth.
String valor_auxiliar_1 = ""; // Almacena temporalmente el valor entrante en forma de cadena {Tiempo disparo 1}.
String valor_auxiliar_2 = ""; // Almacena temporalmente el valor entrante en forma de cadena {Delay entre disparos}.
String valor_auxiliar_3 = ""; // Almacena temporalmente el valor entrante en forma de cadena {Tiempo disparo 2}.
String cadena = "";           // Almacena toda la cadena proveniente del bluetooth.

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
  if (cadena.indexOf(",") >=0){                               // Comprobación del formato del mensaje correcto. [objetivo[coma]valor]
    objetivo = cadena.substring(0,(cadena.indexOf(",")));     // Filtra el objetivo de la cadena.
    int aux_v1 = cadena.indexOf(",")+1;
    valor_auxiliar_1 = cadena.substring(aux_v1,cadena.indexOf(",",aux_v1)); // Filtra el valor 1 de la cadena.
    int aux_v2 = cadena.indexOf(",",aux_v1)+1;
    valor_auxiliar_2 = cadena.substring(aux_v2,cadena.indexOf(",",aux_v2)); // Filtra el valor 1 de la cadena.
    int aux_v3 = cadena.indexOf(",",aux_v2)+1;
    valor_auxiliar_3 = cadena.substring(aux_v3); // Filtra el valor 1 de la cadena.
    tiempo_disparo_1 = valor_auxiliar_1.toInt();                           // Convierte el valor del objetivo a entero.
    delay_disparos = valor_auxiliar_2.toInt();
    tiempo_disparo_2 = valor_auxiliar_3.toInt();
    Serial.println(objetivo);                               // Para debug - Comprobación del objeto recibido.
    Serial.println(tiempo_disparo_1);                                  // Para debug - Comprobación del valor para el objeto recibido.
    Serial.println(delay_disparos);                                  // Para debug - Comprobación del valor para el objeto recibido.
    Serial.println(tiempo_disparo_2);                                  // Para debug - Comprobación del valor para el objeto recibido.
  }
}
