# AdRGB
Библиотека продвинутого управления RGB+W светодиодами для ESP8266 
- Поддержка плавного изменения яркости (перехода цвета)
- Управление через HEX int и HEX String

### Совместимость
Совместима с платформой ESP

## Содержание
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)

<a id="init"></a>
## Инициализация
```cpp
ADRGB mystrip(REDOUT, GREENOUT, BLUEOUT);	         // указать пины светодиодов
ADRGB mystrip(REDOUT, GREENOUT, BLUEOUT, WHITEOUT);  //или так

void HighFrequency(bool freq);                       //true/false включить повышенную частоту ШИМ
void setMaxBright(uint8_t percent);                  //максимальная яркость (на 100% бывают артефакты)
```

<a id="usage"></a>
## Использование
```cpp
void tick();                                                                // тикер. Вызывать как можно чаще

void setMove(uint8_t R, uint8_t G, uint8_t B, uint32_t time);               //перейти в цвет RGB за время time (время в мс)
void setMove(uint8_t R, uint8_t G, uint8_t B, uint8_t W, uint32_t time);    //то же с каналом W
void setMoveW(uint8_t W, uint32_t time);                                    //отдельно на канал W
void setMoveCol(uint8_t color_id, uint8_t val, uint32_t time);              //перейти в цвет по id 0..2 = R..B за время time (время в мс)
void setMoveHSV(uint8_t h, uint8_t s, uint8_t v, uint32_t time);            //перейти в цвет HSV за время time (время в мс)
void setMoveHEX(uint32_t color, uint32_t time);                             //перейти в цвет HEX int за время time (время в мс)
void setMoveHEX(const String& hex, uint32_t time);                          //то же самое для HEX String
void setAccel(uint8_t R, uint8_t G, uint8_t B);                             //быстро перейти в цвет RGB
void setAccel(uint8_t R, uint8_t G, uint8_t B, uint8_t W);                  //быстро перейти в цвет RGB+W
void setAccelCol(uint8_t color_id, uint8_t clr_fin);                        //быстро перейти в цвет по id 0..2 = R..B
void setAccelHEX(uint32_t color);                                           //быстро перейти в цвет HEX int
void setAccelHEX(const String& hex);                                        //быстро перейти в цвет HEX String
void setBright(uint8_t val);                                                //Установить яркость RGB светодиодов
void setBrightW(uint8_t val);                                               //Установить яркость белого светодиода
void setBrightSmooth(uint8_t val, uint32_t time);                           //Плавно сменить яркость RGB светодиодов
void setBrightWSmooth(uint8_t val, uint32_t time);                          //Плавно сменить яркость белого светодиода
```

<a id="example"></a>
## Пример
```cpp
#include <AdRGB.h>

#define REDOUT 12       // красный пин ленты
#define GREENOUT 13     // зеленый пин ленты
#define BLUEOUT 14      // синий пин ленты
#define WHITEOUT 15     // белый пин ленты

ADRGB mystrip(REDOUT, GREENOUT, BLUEOUT, WHITEOUT);

uint32_t mytimer;
uint8_t counter;

void setup(){
    mystrip.HighFrequency(true);
    mystrip.setMaxBright(99);
}

void loop(){
    mystrip.tick();

    if(millis() - mytimer > 1000){
        mytimer = millis();
        blinker();
        counter++;
        if(counter > 2) counter = 0;
    }
}

void blinker(){
    switch(counter){
        case 0:
            mystrip.setMove(255, 0, 0, 1000);
            break;
        case 1:
            mystrip.setMove(0, 255, 0, 1000);
            break;
        case 2:
            mystrip.setMove(0, 0, 255, 1000);
            break;
    }
}
```

<a id="versions"></a>
## Версии
- v1.0