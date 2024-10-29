#include <FastLED.h>
#define NUM_LEDS 2
#define DATA_PIN 10
CRGB leds[NUM_LEDS];
int i = 0;
byte sensores;
/////////////// SENSORES
#define S5 A0 //A0 14 -- SDER      
#define S4 A1 //A1 15-- SFRONTAL
#define S3 A2 //A2 16-- SIZQ
#define S2 17 //A3 17--- LINEA IZQ  
#define S1 18 //A4  18---- LINEA DER
////////////// MOTORES
#define MI1  2
#define PWMI 3
#define MI2  4
#define MD1  5
#define PWMD 6
#define MD2  7
/////////////// BOTONES
#define SGY 8
#define SPD 9
////////////// ACTIVADOR
#define ACTIVADOR 11 //MOSI
#define ACTIVADOR2 12 //MISO
#define X 10
#define E 50
int estrategia = 0;
int FD = 0;
int FI = 0;
int FC = 0;
int linea = 0;
int lineaizq = 0;
int atk = 0;
int atk2 = 40;
int esperacentro = 0;
byte FD_1 = 0, FI_1 = 0, FC_1 = 0, LINEA_1 = 0, LINEAIZQ_1 = 0;
int GI = 0, RI = 0, BI = 0;
int GD = 0, RD = 0, BD = 0;
bool START = false;
bool DER = true, IZQ = false, FRONT = true;

//leds[0].setRGB( 0, 0, 0);   //[1](R,G,B) --- led 0 es DER   COLORES RGB
//leds[1].setRGB( 0, 0, 0);   //[0](R,G,B) --- led 1 es IZQ   COLORES RGB

void setup() {
    estrategia = 0;
    /////////////////////// SENSORES,BOTONES Y ACTIVADOR COMO ENTRADA
    pinMode(S1, INPUT);
    pinMode(S2, INPUT);
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
    test();
    delay(50);
}

void loop() {

    lecturas();

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

        case B00000011:///ambos
            GI = 30; RI = 0; BI = 0;
            GD = 30; RD = 0; BD = 0;
            break;

        case B00000000:///ninguno

            GI = 0; RI = 0; BI = 0;
            GD = 0; RD = 0; BD = 0;
            break;
        }
        if (linea == 0) {
            GI = 10; RI = 10; BI = 10;
            GD = 10; RD = 10; BD = 10;
        }

        leds[1].setRGB(RI, GI, BI); //IZQ
        leds[0].setRGB(RD, GD, BD); //DER

        break;
    case 1: ///////////////////////////////////////////////////////////////////////// ESTRATEGIA 0
        leds[1].setRGB(0, 0, 0); //IZQ
        leds[0].setRGB(0, 0, 0); //DER
        break;
    case 2: ///////////////////////////////////////////////////////////////////////// ESTRATEGIA 1
        leds[1].setRGB(0, 0, 0); //IZQ
        leds[0].setRGB(E, 0, 0); //DER
        break;
    case 3: ///////////////////////////////////////////////////////////////////////// ESTRATEGIA 2
        leds[1].setRGB(E, 0, 0); //IZQ
        leds[0].setRGB(0, 0, 0); //DER
        break;
    case 4: ///////////////////////////////////////////////////////////////////////// ESTRATEGIA 3
        leds[1].setRGB(0, 0, 0); //IZQ
        leds[0].setRGB(0, E, 0); //DER
        break;
    case 5: ///////////////////////////////////////////////////////////////////////// ESTRATEGIA 4
        leds[1].setRGB(0, E, 0); //IZQ
        leds[0].setRGB(0, 0, 0); //DER
        break;
    case 6: ///////////////////////////////////////////////////////////////////////// ESTRATEGIA 5
        leds[1].setRGB(0, 0, 0); //IZQ
        leds[0].setRGB(0, 0, E); //DER
        break;
    case 7: ///////////////////////////////////////////////////////////////////////// ESTRATEGIA 6
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
        START = true;
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
        search();
        break;
    case 1: //adelante
        DER = true; IZQ = false; 
        adelante(40, 40, 10);
        adelante(70, 70, 10);
        adelante(80, 80, 20);
        adelante(100, 100, 10);
        adelante(120, 120, 10);
        adelante(150, 150, 10);
        adelante(180, 180, 10);
        paro(10);
        search();
        break;
    case 2:

        break;
    case 3:

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

void lecturas() {
    FD = digitalRead(S5);
    FI = digitalRead(S3);
    linea = digitalRead(S4);


    if (linea == 0) { LINEA_1 = 0B00000100; }
    else { LINEA_1 = 0B00000000; }

    if (FD == 1) { FD_1 = 0B00000001; }
    else { FD_1 = 0B00000000; }

    if (FI == 1) { FI_1 = 0B00000010; }
    else { FI_1 = 0B00000000; }

    sensores = FD_1 + FI_1 ;

}

void search() {
    atk2 = 40;
    atk = 0;
    do {
        lecturas();
        switch (sensores)
        {
        case B00000001: /// sensor DER  
            DER = true; IZQ = false;
            GI = 0; RI = 0; BI = 0;
            GD = X; RD = 0; BD = 0;
            adelante(20,100,1);
            break;

        case B00000010: /// sensor IZQ
            DER = false; IZQ = true; 
            GI = X; RI = 0; BI = 0;
            GD = 0; RD = 0; BD = 0;
            adelante(100, 20,1);
            break;

        case B0000011:///  TODOS LOS SENSORES 
            DER = false; IZQ = true; 
            atk++;
            if (atk > 40) { atk2++; atk = 0; }
            if (atk2 > 240) { atk2 = 50; }
            adelante(atk2,atk2+8,2);

            break;

    
        default:///ninguno
            GI = 0; RI = 0; BI = 0;
            GD = 0; RD = 0; BD = 0;
            FastLED.show();


            if ((DER == false) && (IZQ == true)) {
                izquierda(40, 40, 1);
            }
            else if ((DER == true) && (IZQ == false)) {
                derecha(40, 40, 1);
            }

        }

        if (linea == 0) {
            GI = 10; RI = 10; BI = 10;
            GD = 10; RD = 10; BD = 10;
            leds[1].setRGB(RI, GI, BI); //IZQ
            leds[0].setRGB(RD, GD, BD); //DER
            FastLED.show();

            do {
                atras(80, 80, 1);
            } while (digitalRead(S4) == 0);
            atras(80, 80, 10);

            if (digitalRead(S3) == 0) {
                if (DER == 1) { derecha(80, 80, 105); }
                else { izquierda(80, 80, 110); }
                paro(100);
            }

            GI = 0; RI = 0; BI = 0;
            GD = 0; RD = 0; BD = 0;
            leds[1].setRGB(RI, GI, BI); //IZQ
            leds[0].setRGB(RD, GD, BD); //DER
            FastLED.show();
        }

        FastLED.show();
        delayMicroseconds(100);

    } while ((digitalRead(ACTIVADOR) == HIGH) || (START == true));
    paro(1);
    intro();
}

void test() {

    adelante(0, 100, 50);
    paro(1000);
    adelante(100, 0, 50);
    paro(1000);
    atras(100, 0, 50);
    paro(1000);
    atras(0, 100, 50);
    paro(1000);
    derecha(100, 100, 50);
    paro(1000);
    izquierda(100, 100, 50);
    paro(1000);

}

void adelante(int PWM2, int PWM1, int T) {

    digitalWrite(MD2, HIGH);
    digitalWrite(MD1, LOW);
    analogWrite(PWMD, PWM1 );
    digitalWrite(MI1, HIGH);
    digitalWrite(MI2, LOW);
    analogWrite(PWMI, PWM2);
    delay(T);
}

void atras(int PWM2, int PWM1, int T) {

    digitalWrite(MD2, LOW);
    digitalWrite(MD1, HIGH);
    analogWrite(PWMD, PWM1);
    digitalWrite(MI1, LOW);
    digitalWrite(MI2, HIGH);
    analogWrite(PWMI, PWM2);
    delay(T);
}

void izquierda(int PWM2, int PWM1, int T) {

    digitalWrite(MD2, 0);
    digitalWrite(MD1, 1);
    analogWrite(PWMD, PWM1);
    digitalWrite(MI1, 1);
    digitalWrite(MI2, 0);
    analogWrite(PWMI, PWM2);
    delay(T);
}

void derecha(int PWM2, int PWM1, int T) {

    digitalWrite(MD2, 1);
    digitalWrite(MD1, 0);
    analogWrite(PWMD, PWM1);
    digitalWrite(MI1, 0);
    digitalWrite(MI2, 1);
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

void freno(int T) {
    digitalWrite(MD1, 1);
    digitalWrite(MD2, 1);
    analogWrite(PWMD, 150);
    digitalWrite(MI1, 1);
    digitalWrite(MI2, 1);
    analogWrite(PWMI, 150);
    delay(T);
}

void intro() {
    while (i < 200) {
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




