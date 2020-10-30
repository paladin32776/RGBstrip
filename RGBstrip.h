#include "Arduino.h"
#include <math.h>

#ifndef RGBSTRIP_H
#define RGBSTRIP_H

class RGBstrip
{
  private:
    unsigned char pinR, pinG, pinB;
    bool active_low;
    unsigned int red, green, blue, hue, saturation, brightness, on;
    void init(unsigned char _pinR, unsigned char _pinG, unsigned char _pinB, bool _active_low);
    void hsv2rgb(unsigned int h, unsigned int s, unsigned int v, unsigned int* r, unsigned int* g, unsigned int* b);
    void rgb2hsv(unsigned int r, unsigned int g, unsigned int b, unsigned int* h, unsigned int* s, unsigned int* v);
    unsigned int scale8to10bit(unsigned int in);
  public:
    RGBstrip(unsigned char _pinR, unsigned char _pinG, unsigned char _pinB);
    RGBstrip(unsigned char _pinR, unsigned char _pinG, unsigned char _pinB, bool _active_low);
    ~RGBstrip();
    void set_on(unsigned int _on);
    void set_hue(unsigned int _hue);
    void set_saturation(unsigned int _saturation);
    void set_brightness(unsigned int _brightness);
    void set_red(unsigned int _red);
    void set_green(unsigned int _green);
    void set_blue(unsigned int _blue);
    void set_rgb(unsigned int _red, unsigned int _green, unsigned int _blue);
    void set_hsv(unsigned int _red, unsigned int _green, unsigned int _blue);

    unsigned int get_on();
    unsigned int invert_on();
    unsigned int get_hue();
    unsigned int get_saturation();
    unsigned int get_brightness();

    void check();
};

#endif // RGBSTRIP_H
