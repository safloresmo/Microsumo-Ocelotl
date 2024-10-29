#include <FastLED.h>
#define NUM_LEDS 2
#define DATA_PIN 10
CRGB leds[NUM_LEDS];
int i = 0;
byte sensores;
/////////////// SENSORES
#define S5 14 //A0 -- SENSOR DE LINEA       
#define S4 15 //A1 15
#define S3 16 //A2 16     
#define S2 A3 //A3 17--- FD   
#define S1 A4 //A4  ---- FI
////////////// MOTORES
#define MD1  4
#define PWMD 3
#define MD2  2
#define MI1  5
#define PWMI 6
#define MI2  7
/////////////// BOTONES
#define SGY 8
#define SPD 9
////////////// ACTIVADOR
#define ACTIVADOR 11 //MOSI
#define ACTIVADOR2 12 //MISO
#define X 35
#define E 50
int estrategia = 0;
int FD = 0;
int FI = 0;
int linea = 0;
int atk = 0;
int centro = 0;
int espera  = 0;
int espera2 = 0; 
int espera3 = 0;
byte FD_1 = 0, FI_1 = 0, LINEA_1 = 0;
int GI = 0, RI = 0, BI = 0;
int GD = 0, RD = 0, BD = 0;
bool DER = true, IZQ = false;

//leds[0].setRGB( 0, 0, 0);   //[1](R,G,B) --- led 0 es DER   COLORES RGB
//leds[1].setRGB( 0, 0, 0);   //[0](R,G,B) --- led 1 es IZQ   COLORES RGB

void setup() {
    /////////////////////// inicializamos variables 
    estrategia = 0;
    /////////////////////// SENSORES,BOTONES Y ACTIVADOR COMO ENTRADA
    pinMode(S3, INPUT);
    pinMode(S4, INPUT);
    pinMode(S5, INPUT);
    pinMode(SGY, INPUT);
    pinMode(SPD, INPUT);
    pinMode(ACTIVADOR, INPUT);
    ///////////////////// PINES DE MOTOR COMO SALIDAS
    pinMode(MD1, OUTPUT);
    pinMode(MD2, OUTPUT);
    pinMode(MI1, OUTPUT);
    pinMode(MI2, OUTPUT);
    pinMode(PWMD, OUTPUT);
    pinMode(PWMI, OUTPUT);
    ////////////////////////  APGAMOS TODO INICIALMENTE
    digitalWrite(MD1, LOW);
    digitalWrite(MD2, LOW);
    digitalWrite(MI1, LOW);
    digitalWrite(MI2, LOW);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // configuracion de leds RGB
    intro(); // secuencia de inicio de LEDS
    leds[0].setRGB(0, 0, 0); // APAGAMOS LOS LEDS
    leds[1].setRGB(0, 0, 0);
    //test();
    delay(100);
}

void loop() {

    FD = analogRead(S1);
    FI = analogRead(S2);
    linea = digitalRead(S3);

    if (linea == 0) { LINEA_1 = 0B00000100; }
    else { LINEA_1 = 0B00000000; }
    if (FD > 350) { FD_1 = 0B00000001; }
    else { FD_1 = 0B00000000; }
    if (FI > 350) { FI_1 = 0B00000010; }
    else { FI_1 = 0B00000000; }

    sensores = LINEA_1 + FD_1 + FI_1;

    switch (estrategia)
    {
    case 0: ///////////////////////////////////////////////////////////////////////// TEST

        switch (sensores)
        {
        case B00000001: /// sensor DER
            GI = 0; RI = 0; BI = 0;
            GD = 30; RD = 0; BD = 0;
            break;
        case B00000010: /// sensor IZQ
            GI = 30; RI = 0; BI = 0;
            GD = 0; RD = 0; BD = 0;
            break;

        case B00000011:/// AMBOS FRONTALES
            GI = 30; RI = 0; BI = 0;
            GD = 30; RD = 0; BD = 0;
            break;

        case B00000111:///AMBOS FRONTALES Y DE LINEA
            GI = 30; RI = 30; BI = 40;
            GD = 30; RD = 30; BD = 40;
            break;

        case B00000100:///LINEA
            GI = 0; RI = 0; BI = 40;
            GD = 0; RD = 0; BD = 40;
            break;

        case B00000000:///ninguno
            GI = 0; RI = 0; BI = 0;
            GD = 0; RD = 0; BD = 0;
            break;
        }
        leds[1].setRGB(RI, GI, BI); //IZQ
        leds[0].setRGB(RD, GD, BD); //DER

        break;
    case 1: ///////////////////////////////////////////////////////////////////////// ESTRATEGIA 1
        leds[1].setRGB(0, 0, 0); //IZQ
        leds[0].setRGB(E, 0, 0); //DER
        break;
    case 2: ///////////////////////////////////////////////////////////////////////// ESTRATEGIA 2
        leds[1].setRGB(E, 0, 0); //IZQ
        leds[0].setRGB(0, 0, 0); //DER
        break;
    case 3: ///////////////////////////////////////////////////////////////////////// ESTRATEGIA 3
        leds[1].setRGB(E, 0, 0); //IZQ
        leds[0].setRGB(E, 0, 0); //DER
        break;
    case 4: ///////////////////////////////////////////////////////////////////////// ESTRATEGIA 4
        leds[1].setRGB(0, E, 0); //IZQ
        leds[0].setRGB(0, 0, 0); //DER
        break;
    case 5: ///////////////////////////////////////////////////////////////////////// ESTRATEGIA 5
        leds[1].setRGB(0, 0, 0); //IZQ
        leds[0].setRGB(0, E, 0); //DER
        break;
    case 6: ///////////////////////////////////////////////////////////////////////// ESTRATEGIA 6
        leds[1].setRGB(0, E, 0); //IZQ
        leds[0].setRGB(0, E, 0); //DER
        break;
    case 7: ///////////////////////////////////////////////////////////////////////// ESTRATEGIA 7
        leds[1].setRGB(0, 0, E); //IZQ
        leds[0].setRGB(0, 0, 0); //DER
        break;
    default:
        leds[0].setRGB(0, 0, 0); // APAGAMOS LOS LEDS
        leds[1].setRGB(0, 0, 0);
    }

    if (digitalRead(SGY) == HIGH) { // aumento de rutina
        estrategia++;
        delay(300);
    }
    
    if (estrategia > 7) { estrategia = 0; }
    
    if (digitalRead(SPD) == HIGH) { // libre
       retardo5seg();
       rutina();
    }
    
    if ((digitalRead(ACTIVADOR) == HIGH)) { // ARRANQUE
       rutina();
    }
  
    FastLED.show();
    delayMicroseconds(100);

}

void rutina() {
    switch (estrategia)
    {
    case 0: //sin rutina
    DER = true; IZQ = false;
        batalla();
        break;
    case 1: //adelante
    DER = true; IZQ = false;
        adelante(253, 255, 500);
        batalla();
        break;
    case 2:// giro der
    DER = true; IZQ = false;
        derecha(253, 255, 100);
        batalla();
        break;
    case 3:// giro izq
    DER = false; IZQ = true;
        izquierda(253, 255, 100);
        batalla();
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    }
}

void batalla() {
while(1) {
    FD = analogRead(S1);
    FI = analogRead(S2);
    linea = digitalRead(S3);

    if (linea == 0) { LINEA_1 = 0B00000100; }
    else { LINEA_1 = 0B00000000; }
    if (FD > 350) { FD_1 = 0B00000001; }
    else { FD_1 = 0B00000000; }
    if (FI > 350) { FI_1 = 0B00000010; }
    else { FI_1 = 0B00000000; }

    sensores = LINEA_1 + FD_1 + FI_1;

        switch (sensores)
        {
        case B00000001: /// sensor derecho
        DER = true; IZQ = false;
            derecha(170, 150, 4);
            paro(20);
            break;
        case B00000010: /// sensor izquierdo
        DER = false; IZQ = true;
            izquierda(150, 170, 4);
            paro(20);
            break;
        case B00000011:/// ambos frontales
        DER = true; IZQ = false;
if(espera2 == 0){
           paro(1);
           espera++;
           if(espera>500){adelante(120, 122, 60);espera=0;espera3++;}
           if(espera3>2){espera2=1;}
  }
else{
           adelante(atk, atk+2, 1);
           centro++;
           delay(1);
           if(centro>2){atk++;centro=0;}
           if(atk>252){atk=253;}
  }
            break;
        case B00000100:/// linea
        DER = true; IZQ = false;
           atras(240, 240, 200);
            derecha(240, 240, 180);
            break;
        case B00000000:///ninguno

if(espera2==0){
if(DER == true){
derecha(150, 150, 1);
  }
if(IZQ == true){
izquierda(150, 150, 1);
  }  
}
  
else{
  adelante(140, 144, 1);
  }   
   
            break;
        }
}
}

void test() {
    while (1) {
        adelante(255, 255, 20);
paro(1000);
        atras(255, 255, 20);
paro(1000);
        izquierda(255, 255, 20);
paro(1000);
        derecha(255, 255, 20);
paro(1000);
        }

}

void adelante(byte PWM2, byte PWM1, int T) {
    digitalWrite(MD2, HIGH);
    digitalWrite(MD1, LOW);
    analogWrite(PWMD, PWM1);
    digitalWrite(MI1, HIGH);
    digitalWrite(MI2, LOW);
    analogWrite(PWMI, PWM2);
    delay(T);
}

void atras(byte PWM2, byte PWM1, int T) {
    digitalWrite(MD2, LOW);
    digitalWrite(MD1, HIGH);
    analogWrite(PWMD, PWM1);
    digitalWrite(MI1, LOW);
    digitalWrite(MI2, HIGH);
    analogWrite(PWMI, PWM2);
    delay(T);
}

void izquierda(byte PWM2, byte PWM1, int T) {
    digitalWrite(MD2, HIGH);
    digitalWrite(MD1, LOW);
    analogWrite(PWMD, PWM1);
    digitalWrite(MI1, LOW);
    digitalWrite(MI2, HIGH);
    analogWrite(PWMI, PWM2);
    delay(T);
}

void derecha(byte PWM2, byte PWM1, int T) {
    digitalWrite(MD2, LOW);
    digitalWrite(MD1, HIGH);
    analogWrite(PWMD, PWM1);
    digitalWrite(MI1, HIGH);
    digitalWrite(MI2, LOW);
    analogWrite(PWMI, PWM2);
    delay(T);
}

void paro(int T) {
    digitalWrite(MD1, LOW);
    digitalWrite(MD2, LOW);
    analogWrite(PWMD, 0);
    digitalWrite(MI1, LOW);
    digitalWrite(MI2, LOW);
    analogWrite(PWMI, 0);
    delay(T);
}

void intro() {
    while (i < 256) {
        leds[0] = CHSV(i, 255, 80);
        leds[1] = CHSV(i, 255, 80);
        FastLED.show();
        delay(1);
        i++;
    }
    i = 0;
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    FastLED.show();
    delay(20);
}

void retardo5seg() {
    while (i < 246) {
        leds[0] = CHSV(i, 255, 80);
        leds[1] = CHSV(i, 255, 80);
        FastLED.show();
        delay(20);
        i++;
    }
    i = 0;
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    FastLED.show();
    delay(20);
}


