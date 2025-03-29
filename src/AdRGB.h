#ifndef ADRGB_h
#define ADRGB_h
#include <Arduino.h>
#include <ESP8266WiFi.h>

class ADRGB{
public:
    ADRGB(uint8_t R_pin, uint8_t G_pin, uint8_t B_pin);
    ADRGB(uint8_t R_pin, uint8_t G_pin, uint8_t B_pin, uint8_t W_pin);

    void setMove(uint8_t r_fin, uint8_t g_fin, uint8_t b_fin, uint8_t w_fin, uint32_t time);
    void setMove(uint8_t r_fin, uint8_t g_fin, uint8_t b_fin, uint32_t time);
    void setMoveW(uint8_t w_fin, uint32_t time);
    void setMoveCol(uint8_t color_id, uint8_t clr_fin, uint32_t time);
    void setMoveHSV(uint8_t h, uint8_t s, uint8_t v, uint32_t time);
    void setMoveHEX(uint32_t color, uint32_t time);
    void setMoveHEX(const String& hex, uint32_t time);
    void setAccel(uint8_t r_fin, uint8_t g_fin, uint8_t b_fin, uint8_t w_fin);
    void setAccel(uint8_t r_fin, uint8_t g_fin, uint8_t b_fin);
    void setAccelCol(uint8_t color_id, uint8_t clr_fin);
    void setAccelHEX(uint32_t color);
    void setAccelHEX(const String& hex);
    void setBright(uint8_t _br);
    void setBrightW(uint8_t _br);
    void setBrightSmooth(uint8_t _br, uint32_t time);
    void setBrightWSmooth(uint8_t _br, uint32_t time);

    void tick();
    void HighFrequency(bool freq);
    void setMaxBright(uint8_t percent);

    uint8_t getRed();
    uint8_t getGreen();
    uint8_t getBlue();
    uint8_t getWhite();

    uint8_t getBright();
    uint8_t getBrightW();

private:
    void MainDriver();

    uint8_t r_pin = 0, g_pin = 0, b_pin = 0, w_pin = 0; //led pin
    uint8_t _r = 0, _g = 0, _b = 0, _w = 0;             //current brightness
    float userBright = 100.0, userBrightW = 100.0;                           //visible brightness

    float clr_stp[4]; //0-Red, 1-Green, 2-Blue, 3-White;
    float clr_cur[4];
    float clr_to_fin[4];

    float br_to_fin, br_stp;

    uint8_t maxBrightNum = 255;

    uint32_t MainTimer;

    uint8_t FrameStep = 10;

    uint32_t HEX_decoder(const String& hex);

    bool freqFlag = false;
    bool newBr;
};

#endif