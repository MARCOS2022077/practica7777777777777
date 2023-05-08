#include <Wire.h>//se llama la libreria Wire que nos ayudara a comunicarnos con el dispositivos de I2C
#include <LiquidCrystal_I2C.h>//se instala la libreria LiquidCrystal_I2C para trabajar con el i2c del lcd junto con el MÃ³dulo serial I2C
#include <Keypad.h>//se crea la libreria para iniciar el teclado matricial
#include <RTClib.h>//se instala y llama la libreria RTClib para poder utilizar el modulo RTC
#include <Servo.h>//se instala esta libreria para el servo motor

const int FILAS = 4;//se crean el numero de filas
const int COLUMNAS= 4;//se crean el numero de filas
int lee;
char keys[FILAS][COLUMNAS]={ //se ajustan como van las columnas y filas del teclado matricial
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte pinesFila[FILAS]={9,8,7,6};//se indica que pines llevan las filas
byte pinesColumna[COLUMNAS]={5,4,3,2};//se indican que pines llevan las columnas
Keypad teclado=Keypad(makeKeymap(keys), pinesFila, pinesColumna,FILAS,COLUMNAS);//se le indica como van las filas y columas del teclado matricial
char teclaPrecionada;//se crea una variable de tipo caracter para depues leer lo que se envie en el teclado matricial
char pass[6]="90210"; //Poner en el arreglo 1 posicion mas a la que tendra la contraseÃ±a correcta
char ingresaPass[6]; // tiene que tener el mismo tamaÃ±o del arreglo de la contraseÃ±a correcta
int indice=0;//se crea esta variable para que cuando se de un codigo para ingresar siempre se inicie desde 0

#define lcdAddress 0x27  //se indica la direcciÃ³n que toma el modulo i2c
#define filas 2//se indica que numero de filas que tiene el lcd que son dos para despues utilizarlo
#define columnas 16//se indica el numero de columnas que tiene el lcd que son 16 

LiquidCrystal_I2C pantallalcd(lcdAddress, columnas, filas);//se llama la libreria LiquidCrystal_I2C se le indica el nombre de pantallalcd se le indica la direcciÃ³n del lcd,la variable columnas y filas
Servo dota;//SE CREA LA VARIABLE PARA EL SERVO MOTOR
RTC_DS3231 THC;//crea objeto del tipo RTC_DS3231


void setup() {
 pantallalcd.init();//se indica que la pantallalcd que en eso se iniciara
 pantallalcd.backlight();//se indica que la pantallalcd para su iluminaciÃ³n
 THC.begin();//se inicializa el THC
 THC.adjust(DateTime(__DATE__, __TIME__));//funcion que permite establecer fecha y horario 
 dota.attach(10);//SE INDICA QUE QUE PIN SE VA A ENCONTRAR

}

void loop() {
  pantallalcd.setCursor(8,0);//se dise que en la columna 0 se imprima lo siguiente y que se coloque en la fila 0
  DateTime fecha = THC.now();//funcion que devuelve fecha y horario en formato
  pantallalcd.print(   fecha.hour ());//funcion que obtiene la hora de la fecha completa
  pantallalcd.print(":");//caracter dos puntos como separador
  pantallalcd.print( fecha.minute ());//funcion que obtiene los minutos de la fecha completa
  pantallalcd.print(":");//caracter dos puntos como separador
  pantallalcd.print( fecha.second());//funcion que obtiene los segundos de la fecha completa
  pantallalcd.setCursor(0,0);//en la posición 0 y la fila 0 este ponga lo siguientr
  pantallalcd.print("cerrado");//imprima cerrado
  delay(100);

  teclaPrecionada=teclado.getKey(); //detecta que en el teclado se pulso un boton
  pantallalcd.setCursor(0,1);//se coloque en en la columna cero y se imprima en la fila
  pantallalcd.print("contra:");
  if(teclaPrecionada){//se crea la condiciÃ³n
    ingresaPass[indice]=teclaPrecionada; // le asigna lo que ingresamos a el arreglo que va a ser comparado
    indice++; //cambiara la posicion del arreglo para poder asignar los caracteres
    pantallalcd.setCursor(7,1);//en la posición 7 y la fila 0 este ponga lo siguientr
    pantallalcd.print(ingresaPass);//imrpima la ingresaPass
    
  }
    if(indice==5){ //cuando se llegue al limite del arreglo verificara si esta correcta o no la contraseÃ±a
    if(strcmp(pass,ingresaPass)==0){ //Verifica que la contraseÃ±a esta correcta
      pantallalcd.setCursor(0,1);//se coloque en en la columna cero y se imprima en la fila
      pantallalcd.print("abrir las puertas");//imprima lo que esta entre parentesis
      delay(2000);
      pantallalcd.clear();//luego que pase el tiempo se limpie   
      dota.write(180);//se indica los grados a moverse el servo motor
      delay(2000);
      pantallalcd.setCursor(1,1);//se coloque en en la columna cero y se imprima en la fila
      pantallalcd.print("bienvenido pa");//imprima lo que esta entre parentesis
      delay(5000);
      pantallalcd.clear();//luego que pase el tiempo se limpie
      pantallalcd.setCursor(1,1);//que ponga en la posición 1 y fila 1 lo siguiente
      pantallalcd.print("cerrara en 10s");//imprima lo que esta entre parentesis
      delay(10000);
      pantallalcd.clear();//luego que pase el tiempo se limpie
      dota.write(0);//que mueva el servo motor a 0 grado de regreso
      delay(2000);
      pantallalcd.setCursor(1,1);//que ponga en la posición 1 y fila 1 lo siguiente
      pantallalcd.print("puerta cerrada");//imprima lo que esta entre parentesis
      delay(1000);
      pantallalcd.setCursor(1,1);//que ponga en la posición 1 y fila 1 lo siguiente
      pantallalcd.print("cerrado");//imprima lo que esta entre parentesis
      pantallalcd.clear();//luego se pase a el tiempo se limpie
    } 
    else{
      pantallalcd.setCursor(0,1);//que ponga en la posición 0 y fila 1 lo siguiente
      pantallalcd.print("acceso no permitido");//imprima lo que esta entre parentesis
      delay(3000);
      pantallalcd.clear();//luego que pase el tiempo se limpie  
    }indice=0;//que el indice se vuelva cero despues de todo
  }
}

