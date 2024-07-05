#include <VirtualWire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// SCL A5
// SDA A4

int botLiga = 2;
int botDesliga = 3;

int pot = 0 ;
int saidaMotor = 0;

int cor = 0;
int qtAzul = 0;
int qtVermelho = 0;
int qtCinza = 0;


int dadoControle[3];
int dadoControleLen = 3;

int dadoLcd[4];
int dadoLcdLen = 4;


void setup() {
  pinMode(botLiga, INPUT_PULLUP);
  pinMode(botDesliga, INPUT_PULLUP);

  pinMode(4, LOW);

  // Receptor
  vw_set_rx_pin(6);
  vw_setup(4000);
  vw_rx_start();

  // Transmissor
  vw_set_tx_pin(7);
  vw_setup(4000);
}

void loop() {
  
  pinMode(4, HIGH);
  
  if (vw_get_message((uint8_t)dadoLcd, dadoLcdLen)) {
    if (dadoLcd[0] != 0) {
      if (dadoLcd[0] == 1) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Vermelho");
      }
      if (dadoLcd[0] == 2) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Azul");
      }
      if (dadoLcd[0] == 3) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Cinza");
      }
    }
    else {

      qtAzul = dadoLcd[1];
      qtVermelho = dadoLcd[2];
      qtCinza = dadoLcd[3];

      lcd.setCursor(0, 0);
      lcd.print("V:      ");
      lcd.setCursor(3, 0);
      lcd.print(qtVermelho);

      lcd.setCursor(8, 0);
      lcd.print("A:");
      lcd.setCursor(11, 0);
      lcd.print(qtAzul);

      lcd.setCursor(0, 1);
      lcd.print("C:");
      lcd.setCursor(3, 1);
      lcd.print(qtCinza);
    }
  }

    pinMode(4, LOW);

  pot = analogRead(A0);
  saidaMotor = pot / 4;
  dadoControle[0] = saidaMotor;

  if (digitalRead(botLiga) == 0) {
    dadoControle[1] = 0;
  }
  else {
    dadoControle[1] = 1;
  }

  if (digitalRead(botDesliga) == 0) {
    dadoControle[2] = 0;
  }
  else {
    dadoControle[2] = 1;
  }


  vw_send((uint8_t)dadoControle, dadoControleLen);
  vw_wait_tx();
}
