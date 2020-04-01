#include <FastLED.h>
//==================================================
// ОБЩИЕ ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
//==================================================
unsigned long timer =    0;           // общий таймер системы
byte stage =             0;           // содержит текущий режим работы
#define normalMode       1            // значение stage при нормальном режиме работы
#define alarmMode        2            // значение stage при аварийном режиме работы
#define demoMode         3            // значение stage при демо режиме работы

//==================================================
// КНОПКИ
//==================================================
#define NM_BUTTON           13           // кнопка нормального режима
#define AM_BUTTON           12           // кнопка аварийного режима
#define DM_BUTTON           8            // кнопка демо режима
boolean normalButtonFlag =  0;           // флаг нормального режима
boolean alarmButtonFlag =   0;           // флаг аварийного режима
boolean demoButtonFlag =    0;           // флаг демо режима

//==================================================
// ПЕРВАЯ ЛИНИЯ СВЕТОДИОДОВ
//==================================================
byte reactorBrightness = 167;         // яркость светимости реактора
#define NUM_LEDS_1       100          // кол-во светодиодов в первой линии
#define DATA_PIN_1       11           // ~ контакт данных первой линии
CRGB leds1[NUM_LEDS_1];               // массив светодиодов для первой линии
byte led1Counter =       0;           // счетчик для первой линии

//==================================================
// ВТОРАЯ ЛИНИЯ СВЕТОДИОДОВ
//==================================================
#define NUM_LEDS_2       100          // кол-во светодиодов во второй линии
#define DATA_PIN_2       10           // ~ контакт данных второй линии
CRGB leds2[NUM_LEDS_2];               // массив светодиодов для второй линии

//==================================================
// ТРЕТЬЯ ЛИНИЯ СВЕТОДИОДОВ
//==================================================
#define NUM_LEDS_3       100          // кол-во светодиодов в третьей линии
#define DATA_PIN_3       9            // ~ контакт данных третьей линии
CRGB leds3[NUM_LEDS_3];               // массив светодиодов для третьей линии

//==================================================
// ЧЕТВЕРТАЯ ЛИНИЯ СВЕТОДИОДОВ
//==================================================
#define NUM_LEDS_4       100          // кол-во светодиодов в четвертой линии
#define DATA_PIN_4       9           // ~ контакт данных четвертой линии
CRGB leds4[NUM_LEDS_4];               // массив светодиодов для четвертой линии

//==================================================
// ДЛЯ ВСЕХ ЛИНИЙ СВЕТОДИОДОВ
//==================================================
#define NUM_STRIPS        4              // количество линий
CLEDController *controllers[NUM_STRIPS]; // массив светодиодных линий
#define BRIGHTNESS        25             // яркость всех линий, кроме реактора

//==================================================
//ФУНКЦИИ ОТРИСОВКИ
//==================================================
// функция рисует пунктир
void flowColor(byte stepLeds, byte beginFlow, byte numberLeds, CRGB color1, CRGB color2, byte numberColor, CRGB leds[]) {

  for (int i = beginFlow - 1; i <= beginFlow + numberLeds - stepLeds; i += stepLeds) {
    if (numberColor == 2) {
      leds[i] = color1;
      leds[i + 1] = color2;
    } else {
      leds[i] = color1;
    }
  }
}

// функция заполняет определенный участок черным цветом
void fillBlack(byte beginFill, byte endFill, CRGB leds[]) {
  fill_solid(&(leds[beginFill]), endFill, CRGB::Black); // заполнение линии
}

//==================================================
//ФУНКЦИИ ЭТАПОВ ПРОГРАММЫ
//==================================================
// функция считывания значения кнопок, возвращает режим работы
void readButtonData() {
  boolean readNormalButton = digitalRead(NM_BUTTON);          // значение кнопки нормального режима
  boolean readAlarmButton = digitalRead(AM_BUTTON);           // значение кнопки аварийного режима
  boolean readDemoButton = digitalRead(DM_BUTTON);            // значение кнопки демо режима

  //проверка нажатия кнопки нормального режима
  if (readNormalButton == true && normalButtonFlag == false) {
    normalButtonFlag = true;
    alarmButtonFlag = false;
    demoButtonFlag = false;
    timer = 0; // обнуление таймера
    stage = 1; // изменение стадии работы
  }

  //проверка нажатия кнопки аварийного режима
  if (readAlarmButton == true && alarmButtonFlag == false) {
    alarmButtonFlag = true;
    normalButtonFlag = false;
    demoButtonFlag = false;
    timer = 0;
    stage = 2;
  }

  //проверка нажатия кнопки демо режима
  if (readDemoButton == true && demoButtonFlag == false) {
    demoButtonFlag = true;
    normalButtonFlag = false;
    alarmButtonFlag = false;
    timer = 0;
    stage = 3;
  }
}

// функция работы нормального режима
void normalFunc() {
  // каждый   x   такт
  if (timer % 4 == 0) {

    fillBlack(0, 9, leds1); // заполнение линии черным цветом
    flowColor(3, 0 + led1Counter, 9, CRGB(255, 0, 0), CRGB(255, 255, 255), 1, leds1); // заполнение ленты пунктиром (размер стопы, начало, конец, цвет1, цвет2, кол-во цветов, какая лента)
    controllers[0]->showLeds(BRIGHTNESS); // отображение конкретной светодиодной линии

    if (led1Counter >= 3) led1Counter = 0; // если сдвиг больше 3 то 0
    led1Counter++; // увеличиваем сдвиг на 1

  }

  fill_solid(&(leds1[36]), 4, CHSV(0, 255, reactorBrightness)); //включение рекатора (начальный элемент, кол-во, цвет)
}

// функция работы аварийного режима
void alarmFunc() {
}

// функция работы демо режима
void demoFunc() {
}

// функция работы начального режима
void defaultFunc() {
}

void setup() {
  controllers[0] = &FastLED.addLeds<WS2812B, DATA_PIN_1, GRB>(leds1, NUM_LEDS_1); // инициализация первой светодиодной линии
  //controllers[1] = &FastLED.addLeds<WS2812B, DATA_PIN_1, GRB>(ledStrip1, NUM_LEDS_1); // инициализация второй светодиодной линии
  //controllers[2] = &FastLED.addLeds<WS2812B, DATA_PIN_1, GRB>(ledStrip1, NUM_LEDS_1); // инициализация третьей светодиодной линии
  //controllers[3] = &FastLED.addLeds<WS2812B, DATA_PIN_1, GRB>(ledStrip1, NUM_LEDS_1); // инициализация четвертой светодиодной линии

  pinMode(NM_BUTTON, INPUT); // инициализация кнопки нормального режима
  pinMode(AM_BUTTON, INPUT); // инициализация кнопки аварийного режима
  pinMode(DM_BUTTON, INPUT); // инициализация кнопки демо режима

  Serial.begin(9600);
}


void loop() {

  readButtonData(); // считывание положения кнопок

  //проверка текущего режима и выполнение соответствующих инструкций
  switch (stage) {
    case normalMode:
      normalFunc(); // выполнение нормального режима
      Serial.println("normal mode");
      Serial.println(timer);
      break;
    case alarmMode:
      alarmFunc(); // выполнение аварийного режима
      Serial.println("alarm mode");
      Serial.println(timer);
      break;
    case demoMode:
      demoFunc(); // выполнение демо режима
      Serial.println("demo mode");
      Serial.println(timer);
      break;

    default:
      defaultFunc(); // выполнение начального режима
      Serial.println("default");
      Serial.println(timer);
      break;
  }

  timer++; // увеличение общего системного таймера на 1
  delay(100); // 1 системный такт
}
