#include "HX711.h"
#include <NewPing.h>

// Pinos para os sensores
const int Left = A1;  // Sensor de linha esquerdo no pino A1
const int Right = A2;   // Sensor de linha direito no pino A2

const int in1 = 11;  // Pino de controle IN1 do L298N
const int in2 = 7;  // Pino de controle IN2 do L298N
const int ena = 9;  // Pino de PWM (ENA) para o lado direito
const int in3 = 8;  // Pino de controle IN3 do L298N
const int in4 = 12; // Pino de controle IN4 do L298N
const int enb = 10; // Pino de PWM (ENB) para o lado esquerdo

const int in1_atuador = 5;  // Pino de controle IN1 do 2º L298N
const int in2_atuador = 4;  // Pino de controle IN2 do 2º L298N
const int ena_atuador = 6;  // Pino de PWM (ENA) para o lado direito (2º L298N)

const int LOADCELL_DOUT_PIN = 3;
const int LOADCELL_SCK_PIN = 2;
HX711 scale;
#define CALIBRATION_FACTOR 482.541

const int trigPin = A3; //Pino trig do ultrasónico ligado ao pino A0
const int echoPin = A0; //Pino echo do ultrasónico ligado ao pino A3
NewPing sonar(trigPin, echoPin);

void setup() {
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(Left, INPUT);
  pinMode(Right, INPUT);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  pinMode(ena_atuador, OUTPUT);
  pinMode(in1_atuador, OUTPUT);
  pinMode(in2_atuador, OUTPUT);

  digitalWrite(in1_atuador, LOW);
  digitalWrite(in2_atuador, LOW);
  analogWrite(ena_atuador, 0);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(CALIBRATION_FACTOR);   
  scale.tare();               

  Serial.begin(57600);  // Inicializa a comunicação serial para monitoramento

  delay(5000);  // Aguarda 5 segundos para colocar o peso
  int initialReading = round(scale.get_units());
  Serial.print("Peso inicial: ");
  Serial.println(initialReading);
  delay(1000);  // Adicione um atraso opcional se necessário
}

void loop() {
  int LerLeft = analogRead(Left);
  int LerRight = analogRead(Right);
  int distancia = getDistance();
  if (distancia <= 15){
    MotorParar();
  } else {
  if (LerLeft > 35 && LerRight <= 35) {
    MotorDireita();
  }
  if (LerLeft <= 35 && LerRight > 35) {
    MotorEsquerda();
  }
  if (LerLeft > 35 && LerRight > 35) {
    MotorParar();
    
    // Aguarda 5 segundos
    delay(5000);

    int reading = round(scale.get_units());

    if (reading >= 100) {
      AtivarAtuador();
      delay(11000);
      DesativarAtuador();
      delay(11000);
    }
  }
  if (LerLeft <= 35 && LerRight <= 35) {
    MotorContinuar();
  }
  }
}

void MotorDireita() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(ena, 230);
  analogWrite(enb, 230);
}

void MotorEsquerda() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(ena, 227);
  analogWrite(enb, 227);
}

void MotorParar() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(ena, 0);
  analogWrite(enb, 0);
}

void MotorContinuar() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(ena, 70);
  analogWrite(enb, 70);
}

void AtivarAtuador(){
  digitalWrite(in1_atuador, HIGH);
  digitalWrite(in2_atuador, LOW);
  analogWrite(ena_atuador, 255);
}

void DesativarAtuador(){
  digitalWrite(in1_atuador, LOW);
  digitalWrite(in2_atuador, HIGH);
  analogWrite(ena_atuador, 255);
}

int getDistance(){
  delay(50);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 100;
  }
  return cm;
}