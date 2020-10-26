#include "RGBstrip.h"

RGBstrip::RGBstrip(unsigned char _pinR, unsigned char _pinG, unsigned char _pinB)
{
  init(_pinR, _pinG, _pinB, false);
}

RGBstrip::RGBstrip(unsigned char _pinR, unsigned char _pinG, unsigned char _pinB, bool _active_low)
{
  init(_pinR, _pinG, _pinB, _active_low);
}

void RGBstrip::init(unsigned char _pinR, unsigned char _pinG, unsigned char _pinB, bool _active_low)
{
  pinR = _pinR;
  pinG = _pinG;
  pinB = _pinB;
  active_low = _active_low;
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
  hue = 0;
  saturation = 0;
  brightness = 100;
  on = 0;
  hsv2rgb(hue, saturation, brightness, &red, &green, &blue);
  Serial.printf("red=%d, green=%d, blue=%d\n", red, green, blue);
  check();
}

RGBstrip::~RGBstrip()
{

}

void RGBstrip::hsv2rgb(unsigned int h, unsigned int s, unsigned int v, unsigned int* r, unsigned int* g, unsigned int* b)
{
  float H = h;  // 0..360
  float S = s / 100.0;  // 0..100
  float V = v / 100.0;  // 0..100
  float C = V * S;
  // Serial.printf("h=%d, H=%f, S=%f, V=%f, C=%f\n", h, H, S, V, C);

  float X = C * (1-fabs(fmod(H/60,2)-1));
  float m = V - C;
  // Serial.printf("fabs(fmod(H/60,2)-1)=%f, X=%f, m=%f\n", fabs(fmod(H/60,2)-1), X, m);
  float Rn, Gn, Bn;
  if (H>=0 && H<60)
  {
    Rn = C;
    Gn = X;
    Bn = 0;
  }
  else if (H>=60 && H<120)
  {
    Rn = X;
    Gn = C;
    Bn = 0;
  }
  else if (H>=120 && H<180)
  {
    Rn = 0;
    Gn = C;
    Bn = X;
  }
  else if (H>=180 && H<240)
  {
    Rn = 0;
    Gn = X;
    Bn = C;
  }
  else if (H>=240 && H<300)
  {
    Rn = X;
    Gn = 0;
    Bn = C;
  }
  else if (H>=300 && H<360)
  {
    Rn = C;
    Gn = 0;
    Bn = X;
  }
  // Serial.printf("Rn: %f  Gn: %f  Bn: %f\n", Rn, Gn, Bn);
  *r = round((Rn + m) * 255);
  *g = round((Gn + m) * 255);
  *b = round((Bn + m) * 255);
}

void RGBstrip::rgb2hsv(unsigned int r, unsigned int g, unsigned int b, unsigned int* h, unsigned int* s, unsigned int* v)
{
  float Rn = r/255.0;   // 0..255
  float Gn = g/255.0;   // 0..255
  float Bn = b/255.0;   // 0..255

  float Cmax = (Rn>Gn)?(Rn):(Gn);
  Cmax = (Bn>Cmax)?(Bn):(Cmax);
  float Cmin = (Rn<Gn)?(Rn):(Gn);
  Cmin = (Bn<Cmax)?(Bn):(Cmin);

  float D = Cmax-Cmin;

  float H;
  if (D==0)
    H = 0;
  else if (Cmax == Rn)
    H = 60 * (fmod((Gn-Bn)/D,6));
  else if (Cmax == Gn)
    H = 60 * (((Bn-Rn)/D)+2);
  else if (Cmax == Bn)
    H = 60 * (((Rn-Gn)/D)+4);

  float S;
  if (Cmax==0)
    S = 0;
  else
    S = D / Cmax;

  float V = Cmax;

  *h = round(H);
  *s = round(S*100);
  *v = round(V*100);
}

unsigned int RGBstrip::scale8to10bit(unsigned int in)
{
  return round(in*1023.0/255.0);
}

void RGBstrip::set_on(bool _on)
{
  if (_on)
    on = 1;
  else
    on=0;
}

void RGBstrip::set_hue(unsigned int _hue)
{
  if (_hue>=0 && _hue<360)
  {
    hue = _hue;
    hsv2rgb(hue, saturation, brightness, &red, &green, &blue);
  }
}

void RGBstrip::set_saturation(unsigned int _saturation)
{
  if (_saturation>=0 && _saturation<=100)
  {
    saturation = _saturation;
    hsv2rgb(hue, saturation, brightness, &red, &green, &blue);
  }
}

void RGBstrip::set_brightness(unsigned int _brightness)
{
  if (_brightness>=0 && _brightness<=100)
  {
    brightness = _brightness;
    hsv2rgb(hue, saturation, brightness, &red, &green, &blue);
  }
}

void RGBstrip::set_red(unsigned int _red)
{
  if (_red>=0 && _red<=255)
    red = _red;
    rgb2hsv(red, green, blue, &hue, &saturation, &brightness);
}

void RGBstrip::set_green(unsigned int _green)
{
  if (_green>=0 && _green<=255)
    green = _green;
    rgb2hsv(red, green, blue, &hue, &saturation, &brightness);
}

void RGBstrip::set_blue(unsigned int _blue)
{
  if (_blue>=0 && _blue<=255)
    blue = _blue;
    rgb2hsv(red, green, blue, &hue, &saturation, &brightness);
}

void RGBstrip::set_rgb(unsigned int _red, unsigned int _green, unsigned int _blue)
{
  set_red(_red);
  set_green(_green);
  set_blue(_blue);
}

void RGBstrip::set_hsv(unsigned int _hue, unsigned int _saturation, unsigned int _brightness)
{
  set_hue(_hue);
  set_saturation(_saturation);
  set_brightness(_brightness);
}

void RGBstrip::check()
{
  Serial.printf("R: %d  G: %d  B: %d  (H: %d  S: %d  V: %d)\n", red, green, blue, hue, saturation, brightness);
  if (active_low)
  {
    analogWrite(pinR, 1023-on*scale8to10bit(red));
    analogWrite(pinR, 1023-on*scale8to10bit(green));
    analogWrite(pinR, 1023-on*scale8to10bit(blue));
  }
  else
  {
    // Serial.printf("on*R: %d  on*G: %d  on*B: %d\n", on*scale8to10bit(red), on*scale8to10bit(green), on*scale8to10bit(blue));
    analogWrite(pinR, on*scale8to10bit(red));
    analogWrite(pinG, on*scale8to10bit(green));
    analogWrite(pinB, on*scale8to10bit(blue));
  }
}
