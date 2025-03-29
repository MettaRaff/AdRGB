#include "AdRGB.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

#define DEBUG_LED false

ADRGB::ADRGB(uint8_t R_pin, uint8_t G_pin, uint8_t B_pin)
{
    r_pin = R_pin;
    g_pin = G_pin;
    b_pin = B_pin;
    pinMode(r_pin, OUTPUT);
    pinMode(g_pin, OUTPUT);
    pinMode(b_pin, OUTPUT);
}

ADRGB::ADRGB(uint8_t R_pin, uint8_t G_pin, uint8_t B_pin, uint8_t W_pin)
{
    r_pin = R_pin;
    g_pin = G_pin;
    b_pin = B_pin;
    w_pin = W_pin;
    pinMode(r_pin, OUTPUT);
    pinMode(g_pin, OUTPUT);
    pinMode(b_pin, OUTPUT);
    pinMode(w_pin, OUTPUT);
}

void ADRGB::HighFrequency(bool freq)
{
    freqFlag = freq;
    if (freqFlag)
        analogWriteFreq(30000);
}

void ADRGB::setMaxBright(uint8_t percent)
{
    if (percent > 0 && percent <= 100)
        maxBrightNum = 255 / 100 * percent;
}

void ADRGB::setBright(uint8_t _br)
{
    userBright = constrain(_br, 0, 100);
}

void ADRGB::setBrightSmooth(uint8_t _br, uint32_t time)
{
    if (userBright != _br)
    {
        newBr = true;
        br_to_fin = _br - userBright;
        br_stp = br_to_fin / time * FrameStep;
    }
}

void ADRGB::setBrightW(uint8_t _br)
{
    userBrightW = _br;
    setAccelCol(3, _br);
}

void ADRGB::setBrightWSmooth(uint8_t _br, uint32_t time)
{
    userBrightW = _br * 2.55;
    setMoveCol(3, userBrightW, time);
}

uint8_t ADRGB::getBright()
{
    return userBright;
}

uint8_t ADRGB::getBrightW()
{
    return userBrightW;
}

void ADRGB::setMoveHSV(uint8_t h, uint8_t s, uint8_t v, uint32_t time)
{
    float r, g, b;

    float H = (float)h / 255;
    float S = (float)s / 255;
    float V = (float)v / 255;

    int i = int(H * 6);
    float f = H * 6 - i;
    float p = V * (1 - S);
    float q = V * (1 - f * S);
    float t = V * (1 - (1 - f) * S);

    switch (i % 6)
    {
    case 0:
        r = V, g = t, b = p;
        break;
    case 1:
        r = q, g = V, b = p;
        break;
    case 2:
        r = p, g = V, b = t;
        break;
    case 3:
        r = p, g = q, b = V;
        break;
    case 4:
        r = t, g = p, b = V;
        break;
    case 5:
        r = V, g = p, b = q;
        break;
    }

    setMove(r * 255, g * 255, b * 255, time);
}

void ADRGB::setMoveHEX(uint32_t color, uint32_t time)
{
    int _r, _g, _b;
    _r = (color >> 16) & 0xff;
	_g = (color >> 8) & 0xff;
	_b = color & 0xff;
    setMove(_r, _g, _b, time);
}

void ADRGB::setMoveHEX(const String& hex, uint32_t time)
{
    setMoveHEX(HEX_decoder(hex), time);
}

void ADRGB::setAccelHEX(uint32_t color)
{
    int _r, _g, _b;
    _r = (color >> 16) & 0xff;
	_g = (color >> 8) & 0xff;
	_b = color & 0xff;
    setAccel(_r, _g, _b);
}

void ADRGB::setAccelHEX(const String& hex)
{
    setAccelHEX(HEX_decoder(hex));
}

void ADRGB::setMove(uint8_t r_fin, uint8_t g_fin, uint8_t b_fin, uint8_t w_fin, uint32_t time)
{
    setMoveCol(0, r_fin, time);
    setMoveCol(1, g_fin, time);
    setMoveCol(2, b_fin, time);
    setMoveCol(3, w_fin, time);
}

void ADRGB::setMove(uint8_t r_fin, uint8_t g_fin, uint8_t b_fin, uint32_t time)
{
    setMoveCol(0, r_fin, time);
    setMoveCol(1, g_fin, time);
    setMoveCol(2, b_fin, time);
}

void ADRGB::setMoveW(uint8_t w_fin, uint32_t time)
{
    setMoveCol(3, w_fin, time);
}

void ADRGB::setMoveCol(uint8_t color_id, uint8_t clr_fin, uint32_t time)
{
    clr_to_fin[color_id] = clr_fin - clr_cur[color_id];
    clr_stp[color_id] = clr_to_fin[color_id] / time * FrameStep;

    /*Serial.print(color_id);
    Serial.print(' ');
    Serial.print(clr_to_fin[color_id]);
    Serial.print(' ');
    Serial.println(clr_stp[color_id]);*/
}

void ADRGB::setAccel(uint8_t r_fin, uint8_t g_fin, uint8_t b_fin, uint8_t w_fin)
{
    setAccelCol(0, r_fin);
    setAccelCol(1, g_fin);
    setAccelCol(2, b_fin);
    setAccelCol(3, w_fin);
}

void ADRGB::setAccel(uint8_t r_fin, uint8_t g_fin, uint8_t b_fin)
{
    setAccelCol(0, r_fin);
    setAccelCol(1, g_fin);
    setAccelCol(2, b_fin);
}

void ADRGB::setAccelCol(uint8_t color_id, uint8_t clr_fin)
{
    clr_to_fin[color_id] = clr_fin - clr_cur[color_id];
    clr_stp[color_id] = clr_to_fin[color_id];
}

uint32_t ADRGB::HEX_decoder(const String& hex){
    if (hex.length() < 6)
        return 0x000000;
    uint32_t val = 0;
    uint8_t i = (hex[0] == '#') ? 1 : 0;
    for (; i < hex.length(); i++)
    {
        val <<= 4;
        uint8_t d = hex[i];
        d -= (d <= '9') ? 48 : ((d <= 'F') ? 55 : 87);
        val |= d;
    }
    return val;
}

void ADRGB::tick()
{
    if (millis() - MainTimer > FrameStep)
    {
        MainTimer = millis();
        MainDriver();
    }
}

void ADRGB::MainDriver()
{
    for (uint8_t i = 0; i < 4; i++)
    {
        if (abs(clr_to_fin[i]) >= abs(clr_stp[i]))
        {
            clr_cur[i] += clr_stp[i];
            clr_to_fin[i] -= clr_stp[i];
        }
        if (abs(clr_to_fin[i]) < abs(clr_stp[i]) && clr_to_fin[i] != 0)
        {
            clr_cur[i] += clr_to_fin[i];
            clr_to_fin[i] = 0;
        }
    }

    if (newBr)
    {
        if (abs(br_to_fin) >= abs(br_stp))
        {
            userBright += br_stp;
            br_to_fin -= br_stp;
        }
        if (abs(br_to_fin) < abs(br_stp) && br_to_fin != 0)
        {
            userBright += br_to_fin;
            br_to_fin = 0;
            newBr = false;
        }
    }

    _r = constrain(map(abs(clr_cur[0]), 0, 255, 0, maxBrightNum), 0, 255);
    _g = constrain(map(abs(clr_cur[1]), 0, 255, 0, maxBrightNum), 0, 255);
    _b = constrain(map(abs(clr_cur[2]), 0, 255, 0, maxBrightNum), 0, 255);
    _w = constrain(map(abs(clr_cur[3]), 0, 255, 0, maxBrightNum), 0, 255);

    _r = float(_r * userBright / 100);
    _g = float(_g * userBright / 100);
    _b = float(_b * userBright / 100);
    //_w = float(_w * userBrightW / 100);

    analogWrite(r_pin, _r);
    analogWrite(g_pin, _g);
    analogWrite(b_pin, _b);
    analogWrite(w_pin, _w);
}

uint8_t ADRGB::getRed()
{
    return _r;
}

uint8_t ADRGB::getGreen()
{
    return _g;
}

uint8_t ADRGB::getBlue()
{
    return _b;
}

uint8_t ADRGB::getWhite()
{
    return _w;
}