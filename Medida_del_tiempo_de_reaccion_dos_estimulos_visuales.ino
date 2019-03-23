
/////////////////////////////////////////////////////////////////////////
//
//  Medida del tiempo de reacción doble visual
//
//  Autor: Adán Casares Ruiz
//  Última modificación: 23/03/19
//  Plataforma: Arduino
//
//  Funcionalidad: Medir el tiempo de reacción visual de una persona,
//  realizar una sería de operaciones estadísticas básicas,para luego
//  mostrarlas por pantalla.
//
/////////////////////////////////////////////////////////////////////////

#include <rgb_lcd.h>
#include <string.h>

const int touch = 2;
const int botton1 = 8;    //  botón led rojo
const int botton2 = 7;    //  botón led verde
const int led1 = 4;       //  led rojo
const int led2 = 13;      //  led verde
rgb_lcd lcd;
int myArray[5];
int tSuma = 0;

void setup() {
  pinMode(touch, INPUT);
  pinMode(botton1, INPUT);
  pinMode(botton2, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  lcd.begin(16, 2);
  randomSeed(millis());
}

/////////////////////////////////////////////////////////////////////////
//  Función que calcula la desviación típica
/////////////////////////////////////////////////////////////////////////

int desviacionTipica(int tSuma, int myArray[]) {

  int sum = 0;
  for (int i = 0; i < 5; i++) {

    sum = pow((myArray[i] - tSuma / 5), 2) + sum;

  }
  sum = sum / 5;
  sum = sqrt(sum);
  return sum;
}


void loop() {

  while (1) {

    if (digitalRead(touch) == HIGH) {
      //  Iniciamos el programa cuando tocamos el sensor táctil
      int error = 0;        //  Inicializamos variables
      tSuma = 0;
      int i = 0;
      int tMax = 0;
      while (i < 5) {
        //  Encendemos los led
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        randomSeed(millis());
        int second = random(5000, 7000);
        //  Esperamos entre 5 y 7 segundos
        delay(second);
        //  Apagamos uno de los dos led
        if (second > 6000) {
          digitalWrite(led1, LOW);
        }
        else {
          digitalWrite(led2, LOW);
        }

        int t0 = millis();

        while ((digitalRead(botton1) == LOW) && (digitalRead(botton2) == LOW)) {
        //  Esperamos el tiempo de reacción del usuario
        }

        int t1 = millis();    //  Medimos el tiempo de reacion
        int t = t1 - t0;

        if ((!digitalRead(led1) && digitalRead(botton1)) || (!digitalRead(led2) && digitalRead(botton2))) {

          if (t >= 70) {
            myArray[i] = t;
            tSuma = tSuma + t;
          }
          if (t > tMax) {
            tMax = t;               //  Comprobamos el tiempo máximo
          }

          lcd.display();
          //  Comprobamos si el usuario hace trampas
          if (t < 70) {
            lcd.print("Error");     //  Mostramos un mensaje de ERROR
            delay(5000);
            lcd.clear();
          }
          else {
            lcd.print(t);           //  Mostramos el tiempo de reacción
            delay(5000);
            lcd.clear();
            i++;
          }
        }
        else {
          error = error + 1;
          lcd.print("Error");
          delay(5000);
          lcd.clear();
        }
      }

      int desTip = desviacionTipica(tSuma, myArray);

      lcd.print("Media: ");         //  Mostramos la media por pantalla
      lcd.print(tSuma / 5);
      lcd.setCursor(0, 1);
      lcd.print("Maximo: ");        //  Mostramos el máximo por pantalla
      lcd.print(tMax);
      delay(5000);
      lcd.clear();
      lcd.print("Desv. Tip.: ");    //  Mostramos la desviación típica
      lcd.print(desTip);
      delay(5000);
      lcd.clear();
    }
  }
}
