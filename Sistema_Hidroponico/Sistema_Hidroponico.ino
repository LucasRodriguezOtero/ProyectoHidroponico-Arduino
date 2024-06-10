#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TimeLib.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int relayBombaPin = 13;
const int relayLuzPin = 12;
const int ledPin = 11;
const int buttonPin = 10;
const int buzzerPin = 9;
const int ldrPin = A0;
const int waterSensorPin = A1;


bool bombaActiva = false;

void setup() {
  Serial.begin(9600);
  pinMode(relayBombaPin, OUTPUT);
  pinMode(relayLuzPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  lcd.init();
  lcd.begin(16, 2);
 
  lcd.backlight();

  
  setTime(17, 10, 30, 30, 11, 2023);

}

void loop() {
 
  int horaActual = hour();
  int minutoActual = minute();

  
  adjustTime(1);

  
  if (digitalRead(waterSensorPin) == LOW) {
    // Sin agua, apagar la bomba y activar la chicharra
    apagarBomba();
    alertaAguaAgotada();
  } else {
    if (!bombaActiva) {
      activarBomba();
    }
  }

 
  if (horaActual >= 8 && horaActual < 18) {
    if (analogRead(ldrPin) < 200) {
      digitalWrite(relayLuzPin, LOW);
    } else {
      digitalWrite(relayLuzPin, HIGH);   
    }
  } else {
    digitalWrite(relayLuzPin, LOW); 
  }


  
  mostrarInformacion(horaActual, minutoActual);

  delay(1000);
}

void activarBomba() {
  digitalWrite(relayBombaPin, LOW);
  bombaActiva = true;
}

void apagarBomba() {
  digitalWrite(relayBombaPin, HIGH);
  bombaActiva = false;
}

void alertaAguaAgotada() {
  
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    delay(2000);
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    delay(2000);
  
  digitalWrite(ledPin, LOW);
}



void mostrarInformacion(int hora, int minuto) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hora: "); 
  lcd.print(hora);
  lcd.print(":");
  lcd.print(minuto);
  

  //serial
  
  Serial.print("Hora Actual: ");
  Serial.print(hora);
  Serial.print(":");
  Serial.print(minuto);

  
  Serial.print("; Proximo bombeo: ");
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.println();
}
