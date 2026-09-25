//librerias
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MQUnifiedsensor.h>
#include <BluetoothSerial.h>
#include <soc/soc.h>
#include <soc/rtc_cntl_reg.h>
//pines
int boton1 = 16;
int boton2 = 17;
int boton3 = 18;
int boton4 = 19;
int rele1 = 13;
int rele2 = 14;
int rele3 = 27;
int rele4 = 26;
int buzzer = 25;
int alimentacion = 23;
//control de botones
bool boton1_anterior = 0;
bool boton2_anterior = 0;
bool boton3_anterior = 0;
bool boton4_anterior = 0;
//control de reles
bool estado_rele1 = 0;
bool estado_rele2 = 0;
bool estado_rele3 = 0;
bool estado_rele4 = 0;
//registro de alimentacion
int caidas = 0;
bool alimentacion_anterior = 1;
String alimentacion_estado = "cable";
//control mediciones
int contador2 = 0;
const long intervalo = 100;
unsigned long tiempoActual = 0;
unsigned long tiempoAnterior = 0;
//mediciones
float s1_ppm = 0.0;
float s2_ppm = 0.0;
float s3_ppm = 0.0;
float s4_ppm = 0.0;
float s1_pre_ppm = 0.0;
float s2_pre_ppm = 0.0;
float s3_pre_ppm = 0.0;
float s4_pre_ppm = 0.0;
//control reles y buzzer
int ppm_max_1 = 20;
int ppm_max_2 = 20;
int ppm_max_3 = 20;
int ppm_max_4 = 20;
bool estado_buzzer = 0;
const int intervalo1 = 1000;
unsigned long tiempoAnterior1 = 0;
//calibracion de sensores
int rl = 2;
float a = 0;
float b = 0;
float R0 = 0;
int lim1 = 0;
int lim2 = 0;
int lim3 = 0;
int lim4 = 0;
float calibrador = 0;
//contadores
int contador1 = 0;
int menu = 0;
int menu_anterior = 0;
//menu e interfaz
int y = 0;
int alto = 12;
int ancho = 110;
int ancho1 = 16;
int max_num = 0;
int largo1 = 0;
int largo2 = 0;
int largo3 = 0;
int largo4 = 0;
int puerto_num = 0;
String sensor1 = "0";
String sensor2 = "0";
String sensor3 = "0";
String sensor4 = "0";
//envio de datos por bluetooth
String mensaje_app = "0";
//crear objetos
Adafruit_SSD1306 display(128, 64, &Wire, -1);
BluetoothSerial SerialBT;
MQUnifiedsensor sensorP1("esp32", 3.3, 5.0, 1.47, 12, 33, "mqx1");
MQUnifiedsensor sensorP2("esp32", 3.3, 5.0, 1.47, 12, 32, "mqx2");
MQUnifiedsensor sensorP3("esp32", 3.3, 5.0, 1.47, 12, 35, "mqx3");
MQUnifiedsensor sensorP4("esp32", 3.3, 5.0, 1.47, 12, 34, "mqx4");
//imagenes 76x32
const unsigned char SAT_sat [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6b, 0x81, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x02, 0xff, 0xbb, 0x10, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0x30, 0x00, 0x00, 
	0x00, 0x00, 0x0f, 0xff, 0xfe, 0x60, 0x00, 0x00, 0x00, 0x01, 0x2f, 0xff, 0xff, 0xc0, 0x00, 0x00, 
	0x00, 0x03, 0x7d, 0xf7, 0xff, 0x80, 0x00, 0x00, 0x00, 0x03, 0xfd, 0x27, 0xbf, 0xb8, 0x00, 0x00, 
	0x00, 0x07, 0xf9, 0x26, 0x77, 0xff, 0x80, 0x00, 0x00, 0x2f, 0xd9, 0x24, 0x61, 0xf8, 0x00, 0x00, 
	0x00, 0x3c, 0xcb, 0x26, 0x40, 0xff, 0x00, 0x00, 0x00, 0x3c, 0x4b, 0x27, 0x81, 0xfe, 0x00, 0x00, 
	0x00, 0x74, 0xcd, 0xbc, 0xc7, 0xfc, 0x00, 0x00, 0x00, 0x63, 0x30, 0x40, 0x4c, 0xf8, 0x00, 0x00, 
	0x00, 0x74, 0x00, 0x00, 0x78, 0xfc, 0x20, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x18, 0x7e, 0x60, 0x00, 
	0x01, 0x90, 0x80, 0x0f, 0x88, 0xff, 0xc0, 0x00, 0x01, 0xd3, 0x80, 0x19, 0x87, 0x87, 0xc0, 0x00, 
	0x01, 0x25, 0x80, 0x3b, 0x84, 0x0f, 0x80, 0x00, 0x00, 0xa5, 0x80, 0x3f, 0x83, 0xff, 0xc0, 0x00, 
	0x00, 0x67, 0x80, 0x2f, 0x82, 0x03, 0xf0, 0x00, 0x00, 0x27, 0x80, 0x3f, 0x01, 0xff, 0x00, 0x00, 
	0x00, 0x13, 0x00, 0x00, 0x01, 0x0f, 0x80, 0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0xf7, 0xc0, 0x00, 
	0x01, 0xb0, 0x00, 0x00, 0x00, 0xf7, 0xe0, 0x00, 0x03, 0xf4, 0x00, 0x00, 0x01, 0x9f, 0x80, 0x00, 
	0x03, 0xd8, 0x03, 0x00, 0x00, 0x87, 0xc0, 0x00, 0x1a, 0xd0, 0x00, 0xe0, 0x00, 0xe7, 0xc0, 0x00, 
	0x0e, 0x20, 0x40, 0x1c, 0x01, 0xf3, 0xe0, 0x00, 0x06, 0x3e, 0x86, 0x03, 0xc1, 0x3f, 0x80, 0x00, 
	0x07, 0x3f, 0x01, 0x80, 0x39, 0x1f, 0xc0, 0x00, 0x07, 0x1c, 0x00, 0x70, 0x01, 0x58, 0x60, 0x00, 
	0x07, 0x88, 0xf0, 0x0e, 0x01, 0x4e, 0x00, 0x00, 0x02, 0x87, 0x00, 0x00, 0x01, 0xd7, 0xc0, 0x00, 
	0x02, 0x08, 0x84, 0x00, 0x01, 0xff, 0x80, 0x00, 0x07, 0xe6, 0xff, 0xbf, 0x9f, 0xff, 0x00, 0x00, 
	0x1f, 0x81, 0x24, 0x60, 0xf7, 0x58, 0x00, 0x00, 0x03, 0xfa, 0x48, 0x90, 0xe7, 0x44, 0x00, 0x00, 
	0x00, 0xe3, 0xd1, 0x05, 0xe5, 0x66, 0x00, 0x00, 0x0d, 0xc2, 0x62, 0x45, 0xe5, 0x7f, 0x80, 0x00, 
	0x01, 0xb8, 0x4a, 0xc9, 0xe7, 0xe3, 0x80, 0x00, 0x01, 0xf1, 0x0c, 0xf3, 0x6e, 0xe0, 0x80, 0x00, 
	0x03, 0xe3, 0x25, 0xf2, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0xff, 0x6f, 0xf7, 0x7d, 0xf8, 0x00, 0x00, 
	0x01, 0xff, 0xf7, 0xef, 0xff, 0xdc, 0x00, 0x00, 0x01, 0x3f, 0xf7, 0xcf, 0xdf, 0xc0, 0x00, 0x00, 
	0x00, 0x3f, 0xe7, 0x1f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00
};
//imagenes 20x20
const unsigned char SAT_bt [] PROGMEM = {
	0x01, 0xf0, 0x00, 0x07, 0xfc, 0x00, 0x0f, 0xfe, 0x00, 0x0f, 0xbf, 0x00, 0x1f, 0x9f, 0x00, 0x1f, 
	0x8f, 0x00, 0x1d, 0xa7, 0x00, 0x1c, 0xa7, 0x00, 0x1e, 0x0f, 0x00, 0x1f, 0x1f, 0x00, 0x1f, 0x1f, 
	0x00, 0x1e, 0x0f, 0x00, 0x1c, 0xa7, 0x00, 0x1d, 0xa7, 0x00, 0x1f, 0x8f, 0x00, 0x1f, 0x9f, 0x00, 
	0x0f, 0xbf, 0x00, 0x0f, 0xbe, 0x00, 0x07, 0xfc, 0x00, 0x01, 0xf0, 0x00
};
const unsigned char SAT_bat [] PROGMEM = {
	0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x03, 0xf8, 0x00, 0x0f, 0xfe, 0x00, 0x0f, 0xfe, 0x00, 0x0f, 
	0xfe, 0x00, 0x0f, 0xfe, 0x00, 0x0f, 0x9e, 0x00, 0x0f, 0x9e, 0x00, 0x0f, 0x1e, 0x00, 0x0f, 0x0e, 
	0x00, 0x0e, 0x0e, 0x00, 0x0f, 0x9e, 0x00, 0x0f, 0xbe, 0x00, 0x0f, 0xbe, 0x00, 0x0f, 0xfe, 0x00, 
	0x0f, 0xfe, 0x00, 0x0f, 0xfe, 0x00, 0x0f, 0xfe, 0x00, 0x00, 0x00, 0x00
};
const unsigned char SAT_alrm [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x01, 0xf8, 0x00, 0x06, 
	0x0c, 0x00, 0x44, 0x04, 0x40, 0x4c, 0x06, 0x40, 0x4c, 0x02, 0x60, 0xcc, 0x02, 0x60, 0x48, 0x06, 
	0x40, 0x4c, 0x02, 0x40, 0x4c, 0x02, 0x40, 0x48, 0x06, 0x40, 0x0c, 0x02, 0x00, 0x1f, 0xff, 0x00, 
	0x12, 0xe5, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00
};
const unsigned char SAT_sens [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x03, 0x1c, 0x00, 0x04, 0x93, 0x00, 0x08, 
	0x03, 0x00, 0x0c, 0x22, 0x80, 0x10, 0x20, 0x80, 0x10, 0x61, 0x80, 0x14, 0x00, 0x80, 0x10, 0x00, 
	0x80, 0x00, 0x00, 0x80, 0x09, 0xf9, 0x00, 0x0c, 0x09, 0x00, 0x02, 0x46, 0x00, 0x03, 0xf8, 0x00, 
	0x01, 0xf8, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char SAT_cab [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x01, 0x08, 0x00, 0x01, 0x18, 0x00, 0x00, 
	0x00, 0x00, 0x03, 0xfc, 0x00, 0x03, 0xfc, 0x00, 0x01, 0xb8, 0x00, 0x01, 0x98, 0x00, 0x01, 0xb8, 
	0x00, 0x01, 0xf0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 
	0x00, 0x40, 0x00, 0x01, 0x80, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char SAT_XX [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x00, 0x01, 0x98, 0x00, 0x03, 
	0x9c, 0x00, 0x03, 0x0c, 0x00, 0x06, 0x66, 0x00, 0x06, 0x66, 0x00, 0x0c, 0x63, 0x00, 0x0c, 0x63, 
	0x00, 0x18, 0x01, 0x80, 0x38, 0x01, 0xc0, 0x30, 0x60, 0xc0, 0x60, 0x60, 0x60, 0x60, 0x00, 0x60, 
	0x7f, 0xff, 0xe0, 0x3f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
void setup() {
  //iniciar display e interfaz de encendido
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("software version:");
  display.println("0.42.4 beta");
  display.println("by: jfmf, ncp, jjed");
  display.display();
  delay(3000);
  display.clearDisplay();
  textoCentrado("erizo_SAT", 0);
  display.drawBitmap(35, 3, SAT_sat, 58, 58, WHITE);
  display.display();
  //iniciar sensores
  sensorP1.init();
  sensorP2.init();
  sensorP3.init();
  sensorP4.init();
  sensorP1.setRegressionMethod(1);
  sensorP2.setRegressionMethod(1);
  sensorP3.setRegressionMethod(1);
  sensorP4.setRegressionMethod(1);
  //botones (entradas)
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
  pinMode(boton3, INPUT);
  pinMode(boton4, INPUT);
  pinMode(alimentacion, INPUT);
  //reles y buzzer (salidas)
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  pinMode(rele3, OUTPUT);
  pinMode(rele4, OUTPUT);
  pinMode(buzzer, OUTPUT);
  //serial y bluetooth
  Serial.begin(115200);
  SerialBT.begin("erizo_SAT");
  //d
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  //delay
  delay(3000);
  display.clearDisplay();
}

void loop() {
  //limpieza
  display.clearDisplay();
  //control
  tiempoActual = millis();
  //lectura de botones
  boton1 = digitalRead(16);
  boton2 = digitalRead(17);
  boton3 = digitalRead(18);
  boton4 = digitalRead(19);
  //lectura de alimentacion
  alimentacion = digitalRead(23);
  if(alimentacion ==0 && alimentacion_anterior == 1){
    caidas = caidas+1;
  }
  if(alimentacion ==1){
    alimentacion_estado = "cable";
  }else{
    alimentacion_estado = "bateria";
  }
  alimentacion_anterior = alimentacion;
  //menu boton arriba y abajo
  if(boton1 == 1 && boton1_anterior == 0){
    contador1 = contador1 +1;
  }
  boton1_anterior = boton1;
  if(boton2 == 1 && boton2_anterior == 0){
    contador1 = contador1 -1;
  }
  boton2_anterior = boton2;
  //boton atras
  if(boton4 == 1 && boton4_anterior == 0){
    menu = menu_anterior;
    contador1 = 0;
  }
  boton4_anterior = boton4;
  //menu control
  switch(menu){
    case 0:
    if(boton3 == 1 && boton3_anterior == 0){
      switch(contador1){
        case 0:
        menu = 1;
        break;
        case 1:
        menu = 2;
        break;
        case 2:
        menu = 3;
        break;
        case 3:
        menu = 4;
      }
    }
    boton3_anterior = boton3;
    break;
    case 3:
    if(boton3 == 1 && boton3_anterior == 0){
      switch(contador1){
        case 0:
        menu = 6;
        break;
        case 1:
        menu = 7;
        break;
        case 2:
        menu = 8;
        break;
        case 3:
        menu = 9;
        break;
      }
    }
    boton3_anterior = boton3;
    break;
    case 2:
    if(boton3 == 1 && boton3_anterior == 0){
      switch(contador1){
        case 0:
        menu = 10;
        break;
        case 1:
        menu = 11;
        break;
        case 2:
        menu = 12;
        break;
        case 3:
        menu = 13;
        break;
      }
    }
    boton3_anterior = boton3;
    break;
    //pendiente para hacer tras los sensores
    case 10:
    if(boton3 == 1 && boton3_anterior == 0){
      switch(contador1){
        case 0:
        asignarSensor("mq2", sensor1, lim1);
        break;
        case 1:
        asignarSensor("mq3", sensor1, lim1);
        break;
        case 2:
        asignarSensor("mq4", sensor1, lim1);
        break;
        case 3:
        asignarSensor("mq5", sensor1, lim1);
        break;
        case 4:
        asignarSensor("mq6", sensor1, lim1);
        break;
        case 5:
        asignarSensor("mq7", sensor1, lim1);
        break;
        case 6:
        asignarSensor("mq8", sensor1, lim1);
        break;
        case 7:
        asignarSensor("mq9", sensor1, lim1);
        break;
        case 8:
        asignarSensor("mq131", sensor1, lim1);
        break;
        case 9:
        asignarSensor("mq137", sensor1, lim1);
        break;
      }
    }
    boton3_anterior = boton3;
    break;
    case 11:
    if(boton3 == 1 && boton3_anterior == 0){
      switch(contador1){
        case 0:
        asignarSensor("mq2", sensor2, lim2);
        break;
        case 1:
        asignarSensor("mq3", sensor2, lim2);
        break;
        case 2:
        asignarSensor("mq4", sensor2, lim2);
        break;
        case 3:
        asignarSensor("mq5", sensor2, lim2);
        break;
        case 4:
        asignarSensor("mq6", sensor2, lim2);
        break;
        case 5:
        asignarSensor("mq7", sensor2, lim2);
        break;
        case 6:
        asignarSensor("mq8", sensor2, lim2);
        break;
        case 7:
        asignarSensor("mq9", sensor2, lim2);
        break;
        case 8:
        asignarSensor("mq131", sensor2, lim2);
        break;
        case 9:
        asignarSensor("mq137", sensor2, lim2);
        break;
      }
    }
    boton3_anterior = boton3;
    break;
    case 12:
    if(boton3 == 1 && boton3_anterior == 0){
      switch(contador1){
        case 0:
        asignarSensor("mq2", sensor3, lim3);
        break;
        case 1:
        asignarSensor("mq3", sensor3, lim3);
        break;
        case 2:
        asignarSensor("mq4", sensor3, lim3);
        break;
        case 3:
        asignarSensor("mq5", sensor3, lim3);
        break;
        case 4:
        asignarSensor("mq6", sensor3, lim3);
        break;
        case 5:
        asignarSensor("mq7", sensor3, lim3);
        break;
        case 6:
        asignarSensor("mq8", sensor3, lim3);
        break;
        case 7:
        asignarSensor("mq9", sensor3, lim3);
        break;
        case 8:
        asignarSensor("mq131", sensor3, lim3);
        break;
        case 9:
        asignarSensor("mq137", sensor3, lim3);
        break;
      }
    }
    boton3_anterior = boton3;
    break;
    case 13:
    if(boton3 == 1 && boton3_anterior == 0){
      switch(contador1){
        case 0:
        asignarSensor("mq2", sensor4, lim4);
        break;
        case 1:
        asignarSensor("mq3", sensor4, lim4);
        break;
        case 2:
        asignarSensor("mq4", sensor4, lim4);
        break;
        case 3:
        asignarSensor("mq5", sensor4, lim4);
        break;
        case 4:
        asignarSensor("mq6", sensor4, lim4);
        break;
        case 5:
        asignarSensor("mq7", sensor4, lim4);
        break;
        case 6:
        asignarSensor("mq8", sensor4, lim4);
        break;
        case 7:
        asignarSensor("mq9", sensor4, lim4);
        break;
        case 8:
        asignarSensor("mq131", sensor4, lim4);
        break;
        case 9:
        asignarSensor("mq137", sensor4, lim4);
        break;
      }
    }
    boton3_anterior = boton3;
    break;
  }
  //boton adelante
  if(boton3 == 1){
    contador1 = 0;
  }
  //menu interfaz
  switch(menu){
    //etapa
    case 0:
    max_num = 3;
    display.setCursor(3, 2);
    display.println("lecturas");
    display.setCursor(3, 14);
    display.println("sensores config.");
    display.setCursor(3, 26);
    display.println("reles config.");
    display.setCursor(3, 38);
    display.println("estado");
    break;
    //etapa
    case 1:
    menu_anterior = 0;
    largo1 = map(s1_ppm, 0, lim1, 0, 64);
    largo2 = map(s2_ppm, 0, lim2, 0, 64);
    largo3 = map(s3_ppm, 0, lim3, 0, 64);
    largo4 = map(s4_ppm, 0, lim4, 0, 64);
    display.drawLine(62, 0, 62, 64, WHITE);
    display.drawLine(0, 15, 62, 15, WHITE);
    display.drawLine(0, 31, 62, 31, WHITE);
    display.drawLine(0, 47, 62, 47, WHITE);
    display.drawRect(0, 0, 128, 64, WHITE);
    display.fillRect(64, 2, largo1, 12, WHITE);
    display.fillRect(64, 18, largo2, 12, WHITE);
    display.fillRect(64, 34, largo3, 12, WHITE);
    display.fillRect(64, 50, largo4, 12, WHITE);
    display.setCursor(2, 0);
    display.print("p1 ");
    display.println(sensor1);
    display.print(s1_ppm);
    display.setCursor(2, 16);
    display.print("p2 ");
    display.println(sensor2);
    display.print(s2_ppm);
    display.setCursor(2, 32);
    display.print("p3 ");
    display.println(sensor3);
    display.print(s3_ppm);
    display.setCursor(2, 48);
    display.print("p4 ");
    display.println(sensor4);
    display.print(s4_ppm);
    break;
    case 2:
    menu_anterior = 0;
    max_num= 3;
    display.setCursor(3, 2);
    display.println("puerto 1");
    display.setCursor(3, 14);
    display.println("puerto 2");
    display.setCursor(3, 26);
    display.println("puerto 3");
    display.setCursor(3, 38);
    display.println("puerto 4");
    break;
    case 3:
    menu_anterior = 0;
    max_num = 3;
    display.setCursor(3, 2);
    display.println("rele 1");
    display.setCursor(3, 14);
    display.println("rele 2");
    display.setCursor(3, 26);
    display.println("rele 3");
    display.setCursor(3, 38);
    display.println("rele 4");
    break;
    //etapa 3
    case 4:
    //pendiente insertar el estado
    display.drawLine(48, 0, 48, 64, WHITE);
    display.drawLine(0, 21, 48, 21, WHITE);
    display.drawLine(0, 42, 48, 42, WHITE);
    display.drawRect(0, 0, 128, 64, WHITE);
    display.setCursor(2, 6);
    display.println("sistema");
    display.setCursor(2, 27);
    display.println("sensores");
    display.setCursor(2, 48);
    display.println("reles");
    //aun pendiente
    if(SerialBT.hasClient() == 1){
      display.drawBitmap(49, 1, SAT_bt, 20, 20, WHITE);
    }else{
      display.drawBitmap(49, 1, SAT_XX, 20, 20, WHITE);
    }
    if(alimentacion == 1){
      display.drawBitmap(89, 1, SAT_cab, 20, 20, WHITE);
    }else{
      display.drawBitmap(89, 1, SAT_bat, 20, 20, WHITE);
    }
    estadoSimbolos(s1_ppm, SAT_sens, 49, 22);
    estadoSimbolos(s2_ppm, SAT_sens, 69, 22);
    estadoSimbolos(s3_ppm, SAT_sens, 89, 22);
    estadoSimbolos(s4_ppm, SAT_sens, 109, 22);
    estadoSimbolos(estado_rele1, SAT_alrm, 49, 43);
    estadoSimbolos(estado_rele2, SAT_alrm, 69, 43);
    estadoSimbolos(estado_rele3, SAT_alrm, 89, 43);
    estadoSimbolos(estado_rele4, SAT_alrm, 109, 43);
    break;
    case 5:
    //libre
    break;
    case 10:
    menuSensores(1);
    break;
    case 11:
    menuSensores(2);
    break;
    case 12:
    menuSensores(3);
    break;
    case 13:
    menuSensores(4);
    break;
    //etapa 4
    //reles
    case 6:
    menuRele(ppm_max_1, lim1);
    break;
    case 7:
    menuRele(ppm_max_2, lim2);
    break;
    case 8:
    menuRele(ppm_max_3, lim3);
    break;
    case 9:
    menuRele(ppm_max_4, lim4);
    break;
    case 14:
    menu_anterior = 14;
    contador1 = 0;
    R0 = 0;
    textoCentrado("calibrando el sensor", 18);
    textoCentrado("espere un momento", 27);
    switch(puerto_num){
      case 1:
      textoCentrado(sensor1, 9);
      textoCentrado("puerto 1", 0);
      sensorP1.setRL(rl);
      sensorP1.setA(a);
      sensorP1.setB(b);
      display.display();
      for(int i=0;i<10;i++){
        sensorP1.update();
        R0 += sensorP1.calibrate(calibrador);
        delay(500);
      }
      R0 /= 10.0;
      sensorP1.setR0(R0);
      break;
      case 2:
      textoCentrado(sensor2, 9);
      textoCentrado("puerto 2", 0);
      sensorP2.setRL(rl);
      sensorP2.setA(a);
      sensorP2.setB(b);
      display.display();
      for(int i=0;i<10;i++){
        sensorP2.update();
        R0 += sensorP2.calibrate(calibrador);
        delay(500);
      }
      R0 /= 10.0;
      sensorP2.setR0(R0);
      break;
      case 3:
      textoCentrado(sensor3, 9);
      textoCentrado("puerto 3", 0);
      sensorP3.setRL(rl);
      sensorP3.setA(a);
      sensorP3.setB(b);
      display.display();
      for(int i=0;i<10;i++){
        sensorP3.update();
        R0 += sensorP3.calibrate(calibrador);
        delay(500);
      }
      R0 /= 10.0;
      sensorP3.setR0(R0);
      break;
      case 4:
      textoCentrado(sensor4, 9);
      textoCentrado("puerto 4", 0);
      sensorP4.setRL(rl);
      sensorP4.setA(a);
      sensorP4.setB(b);
      display.display();
      for(int i=0;i<10;i++){
        sensorP4.update();
        R0 += sensorP4.calibrate(calibrador);
        delay(500);
      }
      R0 /= 10.0;
      sensorP4.setR0(R0);
      break;
    }
    textoCentrado("calibrado", 32);
    a= 0;
    b= 0;
    rl= 0;
    R0 = 0;
    calibrador = 0;
    delay(500);
    menu = 2;
    break;
  }
  //dial del menu
  if(contador1 > max_num){
    contador1 = 0;
  }
  if(contador1 < 0){
    contador1 = max_num;
  }
  //pendiente
  y = (12 * contador1)%60;

  if(menu == 1 || menu == 4 || menu >= 6 && menu <= 9 || menu == 14){
    ancho = 0;
    ancho1 = 0;
    alto = 0;
  }else{
    ancho = 110;
    ancho1 = 16;
    alto = 12;
  }
  display.drawRect(0, y, ancho, alto, WHITE);
  display.fillRect(112, y, ancho1, alto, WHITE);
  //calcular ppm
  if (tiempoActual - tiempoAnterior >= intervalo){
    tiempoAnterior = tiempoActual;
    contador2 += 1;
    sensorP1.update();
    s1_pre_ppm += sensorP1.readSensor();
    sensorP2.update();
    s2_pre_ppm += sensorP2.readSensor();
    sensorP3.update();
    s3_pre_ppm += sensorP3.readSensor();
    sensorP4.update();
    s4_pre_ppm += sensorP4.readSensor();
    if(contador2 >= 10){
      s1_ppm = s1_pre_ppm/10;
      s2_ppm = s2_pre_ppm/10;
      s3_ppm = s3_pre_ppm/10;
      s4_ppm = s4_pre_ppm/10;
      s1_pre_ppm = 0;
      s2_pre_ppm = 0;
      s3_pre_ppm = 0;
      s4_pre_ppm = 0;
      contador2 = 0;
    }
  }
  //activar reles
  activacionRele(s1_ppm, ppm_max_1, rele1, estado_rele1);
  activacionRele(s2_ppm, ppm_max_2, rele2, estado_rele2);
  activacionRele(s3_ppm, ppm_max_3, rele3, estado_rele3);
  activacionRele(s4_ppm, ppm_max_4, rele4, estado_rele4);
  if(estado_rele1 == 1||estado_rele2 == 1||estado_rele3 == 1||estado_rele4 == 1){
    if (tiempoActual - tiempoAnterior1 >= intervalo1){
      tiempoAnterior1 = tiempoActual;
      estado_buzzer = !estado_buzzer;
      digitalWrite(buzzer, estado_buzzer);
    }else{
    digitalWrite(buzzer, LOW);
    estado_buzzer = 0;
    }
  }else{
    digitalWrite(buzzer, LOW);
    estado_buzzer = 0;
  }
  //enviar info por bluetooth
  mensaje_app = String(s1_ppm,2) + "," + String(s2_ppm,2) + "," + String(s3_ppm,2) + "," + String(s4_ppm,2) + "," + 
  sensor1 + "," + sensor2 + "," + sensor3 + "," + sensor4 + "," + String(estado_rele1) + "," + String(estado_rele2) + "," +
  String(estado_rele3) + "," + String(estado_rele4) + "," + String(caidas) + "," + alimentacion_estado+ ",";
  //enviar
  SerialBT.print(mensaje_app);
  //printear
  display.display();
  //prueba
  Serial.println(estado_rele1);
}

//funciones personalizadas
//texto centrado
void textoCentrado(String texto, int y) {
  int16_t x1, y1;
  uint16_t w, h;

  display.getTextBounds(texto, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((128 - w) / 2, y);
  display.print(texto);
}
void textoCentrado(int numero, int y) {
  textoCentrado(String(numero), y);
}
//menu reles
void menuRele(int &variable, int variable2){
  menu_anterior = 3;
  contador1 = 0;
  max_num = 0;
  if(boton1 == 1){
    variable = variable + 100;
  }
  if(boton2 == 1){
    variable = variable - 100;
  }
  if(variable> variable2){
    variable = 0;
  }else if(variable<0){
    variable = variable2;
  }
  display.setCursor(0, 0);
  display.println("seleccione el umbral del rele en ppm");
  display.drawRect(0, 26, 128, 12, WHITE);
  display.fillTriangle(60, 23, 68, 23, 64, 19, WHITE);
  display.fillTriangle(60, 40, 68, 40, 64, 44, WHITE);
  textoCentrado(variable, 28);
}
//menu sensores
void menuSensores(int ivan){
  menu_anterior = 2;
  max_num  = 9;
  puerto_num = ivan;
  if(contador1<5){
    display.setCursor(3, 2);
    display.println("mq2 propano");
    display.setCursor(3, 14);
    display.println("mq3 alcohol");
    display.setCursor(3, 26);
    display.println("mq4 metano");
    display.setCursor(3, 38);
    display.println("mq5 gas natural");
    display.setCursor(3, 50);
    display.println("mq6 glp");
  }else{
    display.setCursor(3, 2);
    display.println("mq7 monoxido de C");
    display.setCursor(3, 14);
    display.println("mq8 hidrogeno");
    display.setCursor(3, 26);
    display.println("mq9 CO2");
    display.setCursor(3, 38);
    display.println("mq131 ozono");
    display.setCursor(3, 50);
    display.println("mq137 amoniaco");
  }
}
//ajuste de sensores
void asignarSensor(String variable1, String &variable2, int &variable3){
  variable2 = variable1;
  menu = 14;
  if(variable1 == "mq2"){
    rl = 2;
    a = 658.71;
    b = -2.168;
    calibrador = 9.83;
    variable3 = 10000;
  }else if(variable1 == "mq3"){
    rl = 2;
    a = 0.3934;
    b = -1.504;
    calibrador = 60.0;
    variable3 = 500;
  }else if(variable1 == "mq4"){
    rl = 1;
    a = 1012.7;
    b = -2.786;
    calibrador = 4.4;
    variable3 = 10000;
  }else if(variable1 == "mq5"){
    rl = 2;
    a = 80.897;
    b = -2.431;
    calibrador = 6.5;
    variable3 = 10000;
  }else if(variable1 == "mq6"){
    rl = 2;
    a = 1009.2;
    b = -2.350;
    calibrador = 10.0;
    variable3 = 10000;
  }else if(variable1 == "mq7"){
    rl = 2;
    a = 99.042;
    b = -1.518;
    calibrador = 27.5;
    variable3 = 2000;
  }else if(variable1 == "mq8"){
    rl = 2;
    a = 976.97;
    b = -0.688;
    calibrador = 70.0;
    variable3 = 10000;
  }else if(variable1 == "mq9"){
    rl = 2;
    a = 599.65;
    b = -2.244;
    calibrador = 9.6;
    variable3 = 1000;
  }else if(variable1 == "mq131"){
    rl = 2;
    a = 4269.6;
    b = -1.110;
    calibrador = 15.0;
    variable3 = 1000;
  }else if(variable1 == "mq137"){
    rl = 2;
    a = 102.2;
    b = -2.473;
    calibrador = 3.6;
    variable3 = 500;
  }
}
//encendido reles y buzzer
void activacionRele(float variable1, int variable2, int variable3, bool &variable4){
  if(variable1 > variable2 && variable2 >19){
    digitalWrite(variable3, HIGH);
    variable4 = 1;
  }else{
    digitalWrite(variable3, LOW);
    variable4 = 0;
  }
}
//logos de estado
void estadoSimbolos(int variable1, const unsigned char* variable2, int variable4, int variable5){
  if(isnan(variable1) || isinf(variable1) || variable1 == 0){
    display.drawBitmap(variable4, variable5, SAT_XX, 20, 20, WHITE);
  }else{
    display.drawBitmap(variable4, variable5, variable2, 20, 20, WHITE);
  }
}
