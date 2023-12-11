
#include <VarSpeedServo.h> // inclusão da bibliotecaa
#include <Wire.h>

VarSpeedServo motora; //cria objeto apartir da biblioteca
VarSpeedServo motorb;
VarSpeedServo motorc;
VarSpeedServo motord; //cria objeto apartir da biblioteca
VarSpeedServo motore;
VarSpeedServo motorf;

char td[16];
int i;
void setup()
{
  Wire.begin(0x0A);                
  Wire.onReceive(receiveEvent); 
  Serial.begin(9600);
  
  motora.attach(9); //motorA pino 3
  motorb.attach(10); //motorB pino 5
  motorc.attach(6); //motorC pino 6
  motord.attach(8); //motorA pino 3
  motore.attach(11); //motorB pino 5
  motorf.attach(5); //motorC pino 6        
}

void loop() {}

void receiveEvent(int numBytes)
{
  i = 0;
  while (Wire.available()>1)
  { 
    td[i] = Wire.read();
    i++;
  }
  int valor = Wire.read();
  String topico = td;
  if(topico == "servo1"){
    Serial.println(topico);
    Serial.println(valor);
    motora.slowmove(valor, 10); //posição, velocidade
    delay (3000);// tempo de espera
  }
  else if(topico == "servo2"){
    Serial.println(topico);
    Serial.println(valor);
    motorb.slowmove(valor, 10); //posição, velocidade
    delay (3000);// tempo de espera
  }
  else if(topico == "servo3"){
    Serial.println(topico);
    Serial.println(valor);
    motorc.slowmove(valor, 10); //posição, velocidade
    delay (3000);// tempo de espera
  }
  else if(topico == "servo4"){
    Serial.println(topico);
    Serial.println(valor);
    motord.slowmove(valor, 10); //posição, velocidade
    delay (3000);// tempo de espera
  }
  else if(topico == "servo5"){
    Serial.println(topico);
    Serial.println(valor);
    motorf.slowmove(valor, 10); //posição, velocidade
    delay (3000);// tempo de espera
  }
}
