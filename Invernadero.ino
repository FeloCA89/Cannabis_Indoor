#include <Wire.h>       // Librerias para el uso de la lcd y comunicacon SPI
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>        // Necesario descargar la libreria para el sensor de temperatura DHT
#include <DHT_U.h>
#include <RTClib.h>     // Necesario descargar la libreria para el reloj en tiempo real

RTC_DS1307 RTC;         // Objeto para el reloj en tiempo real

LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7);  // Creacion del objeto lcd (DIR, E, RW, RS, D4, D5, D6, D7)
int SENSOR = 2;      // Puerto de conexion del sensor
int LDR = A0;        // Puerto del sensor LDR
int led_azul = 3;    // Puerto de la lampara azul
int led_rojo = 9;    // Puerto de la lampara roja 
int ventilador = 5;  // Puerto del ventilador
int aspersor = 6;    // puerto de los aspersores de agua
int TEMPERATURA;     // Variable para guardar el valor de la temperatura
int HUMEDAD;         // Variable para guardar el valor de la Humedad 
int valor = 0;

DHT dht(SENSOR, DHT22);   // Funcion para configurar el sensor (PIN, TIPO DHT(11/22))

void setup (){
 RTC.begin();                        // Inicializacion del reloj en tiempo real
 lcd.setBacklightPin(3,POSITIVE);    // Funcion para activar la iluminacion de la lcd
 lcd.setBacklight(HIGH);             
 lcd.begin(16,2);                    // Definiendo una lcd de 16 columna por 2 filas  
 lcd.clear();                        // Borrado de la pantalla de inicio
 dht.begin();                        // Inicializacion del sensor
 pinMode(LDR, INPUT);                // Definiendo pines de entrada y salida
 pinMode(led_azul, OUTPUT);
 pinMode(led_rojo, OUTPUT);
 pinMode(aspersor, OUTPUT);
 pinMode(ventilador, OUTPUT);
 RTC.adjust(DateTime(__DATE__,__TIME__)); // Sincronizacion de fecha y hora con el de la computadora
} 

void loop(){
 TEMPERATURA = dht.readTemperature();       // Funcion para leer la temperatura del sensor 
 int lectura = analogRead(A1);
 HUMEDAD = map (lectura, 1023, 0, 0 ,100);  // Guardar el valor mapeado del sensor de temperatura de 0 - 100%
 valor = analogRead(LDR);
 DateTime now = RTC.now();                  // Actualizar el valor de fecha y hora en el RTC

  if(now.hour() >= 8 && now.hour() <= 20)   // Control de la iluminacion entre las 8 Hrs - 20 Hrs -
   if(valor <= 450){                        // Encender los leds si la iluminacion es baja
      analogWrite(led_azul,255);            // Control de led RGB
      analogWrite(led_rojo,240);
   }else{
      analogWrite(led_azul,0);
      analogWrite(led_rojo,0);
   }
  else{
    analogWrite(led_azul,0);
    analogWrite(led_rojo,0);
  }

 if(TEMPERATURA >= 30)         // Control del ventilador de acuerdo a la temperatura
   digitalWrite(ventilador, HIGH);

 else if(TEMPERATURA < 30 && TEMPERATURA >= 25)
   digitalWrite(ventilador, LOW);

 if(HUMEDAD < 30)              // Llamada a la funcion riego() si la Humedad es menor a 30%
   riego();
 
 lcd.setCursor(0,0);           // Informacion a mostrar en la pantalla lcd
 lcd.print("Hora: ");
 lcd.print(now.hour(), DEC);
 lcd.print(":");
 lcd.print(now.minute(), DEC);
 lcd.print(":");
 lcd.print(now.second(), DEC);
 lcd.setCursor(0,1);
 lcd.print("T:"); 
 lcd.print(TEMPERATURA);
 lcd.print(" C ");
 lcd.print("H:");
 lcd.print(HUMEDAD);
 lcd.print(" % ");
 delay(1000);
 lcd.setCursor(0,0);
 lcd.print("                ");
}

void riego(){                     // Funcion para controlar el riego de la planta
  lcd.setCursor(0,0);
  lcd.print("Humedad baja:"); 
  lcd.print(HUMEDAD);
  lcd.print(" % ");
  lcd.setCursor(0,1);
  lcd.print("Regando...      ");
  digitalWrite(aspersor, HIGH);
  delay(1000);
  digitalWrite(aspersor, LOW);
  delay(3000);
  lcd.clear();
}
