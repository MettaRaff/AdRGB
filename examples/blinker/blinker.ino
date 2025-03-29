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
