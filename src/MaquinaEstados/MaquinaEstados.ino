/**************************** Dependencias externas *************************/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <LiquidCrystal.h>

/************************************ Pines *********************************/
// LCD 16x2
#define LCD_RS 8
#define LCD_E  9
#define LCD_D4 2
#define LCD_D5 3
#define LCD_D6 4
#define LCD_D7 6

// NeoPixel PTH
#define RGB_LED 5

// Arreglo de botones y último estado del botón
// Nota: Los siguientes "DEFINE" son únicamente para
// mejorar la lectura del código al momento de codificar.
#define BTN_MENU  0
#define BTN_EXIT  1
#define BTN_UP    2
#define BTN_DOWN  3

// Este arreglo contiene los pines utilizados para los botones
uint8_t button[4] = {
  7,
  10,
  12,
  A0
};

// Este arreglo contiene el último estado conocido de cada línea
uint8_t button_state[4];

/********************************* Globales *********************************/
// Estado de nuestro autómata
#define S_HOME  0
#define S_SET_R 1
#define S_SET_G 2
#define S_SET_B 3
uint8_t estado = S_HOME;

// Información de color RGB
uint8_t r = 0;
uint8_t g = 0;
uint8_t b = 0;

// Helpers
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, RGB_LED, NEO_RGB + NEO_KHZ800);

void setup() {
  // Configurar como PULL-UP para ahorrar resistencias
  pinMode(button[BTN_MENU], INPUT_PULLUP);
  pinMode(button[BTN_EXIT], INPUT_PULLUP);
  pinMode(button[BTN_UP], INPUT_PULLUP);
  pinMode(button[BTN_DOWN], INPUT_PULLUP);

  // Se asume que el estado inicial es HIGH
  button_state[0] = HIGH;
  button_state[1] = HIGH;
  button_state[2] = HIGH;
  button_state[3] = HIGH;

  // Iniciar helpers
  pixels.begin();
  lcd.begin(16, 2); // User interface

  // Desplegar la info por defecto
  showColor();
  printHome();
}

void showColor() {
  pixels.setPixelColor(0, pixels.Color(r,g,b));
  pixels.show();
}

void printHome() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("[MENU] Set RGB");
  printColor();
}

void printRed() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set RED");
  printColor();
}

void printGreen() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set GREEN");
  printColor();
}

void printBlue() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set BLUE");
  printColor();
}

uint8_t flancoSubida(int btn) {
  uint8_t valor_nuevo = digitalRead(button[btn]);
  uint8_t result = button_state[btn]!=valor_nuevo && valor_nuevo == 1;
  button_state[btn] = valor_nuevo;
  return result;
}

void printColor() {
  lcd.setCursor(0,1);
  lcd.print("RGB: ");
  lcd.print(r, DEC);
  lcd.print(" ");
  lcd.print(g, DEC);
  lcd.print(" ");
  lcd.print(b, DEC);
}

void loop() {
  switch(estado) {
    case S_HOME:
      if(flancoSubida(BTN_MENU)) {
        estado = S_SET_R;
        printRed();
        break;
      }
      break;
    case S_SET_R:
      if(flancoSubida(BTN_MENU)) {
        estado = S_SET_G;
        printGreen();
        break;
      }
      if(flancoSubida(BTN_EXIT)) {
        estado = S_HOME;
        printHome();
        break;
      }
      if(flancoSubida(BTN_UP)) {
        if(r<255) {
          r++;
        } else {
          r = 0;
        }
        showColor();
        printRed();
        break;
      }
      if(flancoSubida(BTN_DOWN)) {
        if(r>0) {
          r--;
        } else {
          r = 255;
        }
        showColor();
        printRed();
        break;
      }
      break;
    case S_SET_G:
      if(flancoSubida(BTN_MENU)) {
        estado = S_SET_B;
        printBlue();
        break;
      }
      if(flancoSubida(BTN_EXIT)) {
        estado = S_HOME;
        printHome();
        break;
      }
      if(flancoSubida(BTN_UP)) {
        if(g<255) {
          g++;
        } else {
          g = 0;
        }
        showColor();
        printGreen();
        break;
      }
      if(flancoSubida(BTN_DOWN)) {
        if(g>0) {
          g--;
        } else {
          g = 255;
        }
        showColor();
        printGreen();
        break;
      }
      break;
    case S_SET_B:
      if(flancoSubida(BTN_MENU)) {
        estado = S_SET_R;
        printRed();
        break;
      }
      if(flancoSubida(BTN_EXIT)) {
        estado = S_HOME;
        printHome();
        break;
      }
      if(flancoSubida(BTN_UP)) {
        if(b<255) {
          b++;
        } else {
          b = 0;
        }
        showColor();
        printBlue();
        break;
      }
      if(flancoSubida(BTN_DOWN)) {
        if(b>0) {
          b--;
        } else {
          b = 255;
        }
        showColor();
        printBlue();
        break;
      }
      break;
  };
}
