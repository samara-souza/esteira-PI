#include <VirtualWire.h>
//#include <LiquidCrystal_I2C.h>

//LiquidCrystal_I2C lcd(0x27, 16, 2);


int ativaMotor = 2;

//int botLiga = 48;
//int botDesliga = 46;


int paradaCor = 3;
// HIGH- 4
// LOW- 5

int paradaVer = 8;
// HIGH- 9
// LOW- 10

int paradaAzul = 11;
// HIGH- 12
// LOW- 13

int sensorCancela = 22;
// HIGH- 24
// LOW- 26

int cancelaPneu = 28;
// HIGH- 30
// LOW- 32

int relePneu = 34;
// HIGH- 36
// LOW- 38

#define pinS0 31
#define pinS1 33
#define pinS2 35
#define pinS3 37
#define pinOut 39
// HIGH- 41
// LOW- 43


//53- CS

//Variáveis

int pot;
int saidaMotor;
int cor = 0;
int qtAzul = 0;
int qtVermelho = 0;
int qtCinza = 0;
int controlePeca = 0;

int valorVermelho = 0;
int valorVerde = 0;
int valorAzul = 0;

int abrirCancela = 0;

int motorParado = 0;


int ligar = 1;
int desligar = 1;

int dadoControle[3];
int dadoControleLen = 3;

int dadoLcd[4];
int dadoLcdLen = 4;


void setup() {

  Serial.begin(9600);

  //pinMode(botLiga, INPUT_PULLUP);
  //pinMode(botDesliga, INPUT_PULLUP);

  pinMode(ativaMotor, OUTPUT);

  pinMode(paradaCor, INPUT);
  pinMode(paradaVer, INPUT);
  pinMode(paradaAzul, INPUT);
  pinMode(sensorCancela, INPUT);

  pinMode(cancelaPneu, OUTPUT);
  pinMode(relePneu, OUTPUT);

  pinMode(pinS0, OUTPUT);
  pinMode(pinS1, OUTPUT);
  pinMode(pinS2, OUTPUT);
  pinMode(pinS3, OUTPUT);
  pinMode(pinOut, INPUT);

  pinMode(53, LOW);

  digitalWrite(cancelaPneu, HIGH);
  digitalWrite(relePneu, HIGH);

  digitalWrite(pinS0, HIGH);
  digitalWrite(pinS1, LOW);


  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);

  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);

  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  
  digitalWrite(24, HIGH);
  digitalWrite(26, LOW);

  digitalWrite(30, HIGH);
  digitalWrite(32, LOW);

  digitalWrite(36, HIGH);
  digitalWrite(38, LOW);

  digitalWrite(41, HIGH);
  digitalWrite(43, LOW);


  
  //lcd.begin(16, 2);
  //lcd.init();
  //lcd.backlight();

  // Receptor
  vw_set_rx_pin(36);
  vw_setup(4000);
  vw_rx_start();

  // Transmissor
  vw_set_tx_pin(38);
  vw_setup(4000);

}

void loop() {
  //lcd.clear();
  //lcd.setCursor(0, 0);
  //lcd.print("Teste");

  dadosControle();

  if (ligar == 0) {
    //cor = 0;
    qtAzul = 0;
    qtVermelho = 0;
    qtCinza = 0;

    motorParado = 0;

    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print("Liga");

    while (true) {

      if (motorParado == 0) {
        ligaSistema();
        if (desligar == 0) {
          paradaEsteira();
          motorParado = 1;
          break;
        }
      }
    }
  }
}

void ligaSistema() {

  dadosControle();

  if (desligar == 0) {
    paradaEsteira();
    motorParado = 1;
  }

  //lcd.clear();
  //lcd.setCursor(0, 0);
  //lcd.print("V:      ");
  //lcd.setCursor(3, 0);
  //lcd.print(qtVermelho);

  //lcd.setCursor(8, 0);
  //lcd.print("A:");
  //lcd.setCursor(11, 0);
  //lcd.print(qtAzul);

  //lcd.setCursor(0, 1);
  //lcd.print("C:");
  //lcd.setCursor(3, 1);
  //lcd.print(qtCinza);

  Serial.println("Clear");
  Serial.println("V");
  Serial.println(qtVermelho);

  Serial.println("A");
  Serial.println(qtAzul);

  Serial.println("C");
  Serial.println(qtCinza);

  ligaEsteira();

  abreCancela();

  //Serial.println(digitalRead(paradaCor));

  if (digitalRead(paradaCor) == LOW) {
    paradaEsteira();
    cor = identificaCor();
    ligaEsteira();

    controlePeca = 0;
    while (true) {
      separaCor(cor);
      if (controlePeca == 1) {
        controlePeca = 0;
        break;
      }
    }
  }
}

void paradaEsteira() {
  digitalWrite(ativaMotor, 0);
}

void ligaEsteira() {
  //pot = analogRead(A0);
  //saidaMotor = pot / 4;

  analogWrite(ativaMotor, saidaMotor);
}

int identificaCor() {

  //Vermelho
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, LOW);
  valorVermelho = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);
  digitalWrite(pinS2, HIGH);
  //Azul
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, HIGH);
  valorAzul = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);
  //Verde
  digitalWrite(pinS2, HIGH);
  valorVerde = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);

  //
  Serial.println(valorVermelho);
  Serial.println(valorAzul);
  Serial.println(valorVerde);
  //

  if ((valorVermelho < 100) && (valorAzul < 80) && (valorVerde < 100) && (valorAzul < valorVermelho) && (valorAzul < valorVerde)) {
    //Serial.println(valorVermelho);
    //Serial.println(valorAzul);
    //Serial.println(valorVerde);

    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print("Cinza");

    Serial.println("Clear");
    Serial.println("Cinza");
    delay(1000);
    return 3;
  }

  if (valorVermelho < valorAzul && valorVermelho < valorVerde) {
    //Serial.println(valorVermelho);
    //Serial.println(valorAzul);
    //Serial.println(valorVerde);

    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print("Vermelho");

    Serial.println("Clear");
    Serial.println("Vermelho");
    delay(1000);
    return 1;
  }

  if (valorAzul < valorVermelho && valorAzul < valorVerde) {
    //Serial.println(valorVermelho);
    //Serial.println(valorAzul);
    //Serial.println(valorVerde);

    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print("Azul");

    Serial.println("Clear");
    Serial.println("Azul");
    delay(1000);
    return 2;
  }

  dadosControle();

  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print(corStr);

  // return 1 para vermelho, 2 para azul e 3 para cinza
}

void separaCor(int cor) {

  dadosControle();

  if (desligar == 0) {
    paradaEsteira();
    motorParado = 1;
  }

  Serial.println(cor);
  if (cor == 1) {
    if (digitalRead(paradaVer) == LOW) {
      paradaEsteira();
      //delay(4000);
      qtVermelho++;
      atuador();
      controlePeca = 1;
      abrirCancela = 1;
    }
  }

  else if (cor == 2) {
    if (digitalRead(paradaAzul) == LOW) {
      paradaEsteira();
      //delay(4000);
      qtAzul++;
      atuador();
      controlePeca = 1;
      abrirCancela = 1;
    }
  }
  else if (cor == 3) {
    if (digitalRead(paradaAzul) == LOW) {
      qtCinza++;
      controlePeca = 1;
      abrirCancela = 1;
    }
  }
}

void atuador() {
  digitalWrite(relePneu, LOW);
  delay(5000);
  digitalWrite(relePneu, HIGH);
}

void abreCancela() {
  if (abrirCancela == 1) {
    digitalWrite(cancelaPneu, LOW);

    if (digitalRead(sensorCancela) == 1) {
      digitalWrite(cancelaPneu, HIGH);
      abrirCancela = 0;
    }
  }
}

void dadosControle() {

  pinMode(53, HIGH);
  if (vw_get_message((uint8_t)dadoControle, dadoControleLen)) {
    saidaMotor = dadoControle[0];

    if (dadoControle[1] == 0) {
      ligar = 0;
      desligar = 1;
    }
    else {
      ligar = 1;
      desligar = 1;
    }

    if (dadoControle[2] == 0) {
      ligar = 1;
      desligar = 0;
    }
    else {
      ligar = 1;
      desligar = 1;
    }
  }
  pinMode(53, LOW);

  dadoLcd[0] = cor;
  dadoLcd[1] = qtAzul;
  dadoLcd[2] = qtVermelho;
  dadoLcd[3] = qtCinza;

  vw_send((uint8_t)dadoLcd, dadoLcdLen);
  vw_wait_tx();

}
