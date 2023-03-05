#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUM_LEDS 7
#define BRIGHTNESS 2

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
}

void loop() {
  // nastavení LED diod 1, 3, 5 a 7 na modrou barvu
  strip.setPixelColor(1, 255, 0, 255); // modrá LED 1
  strip.setPixelColor(3, 255, 0, 255); // modrá LED 3
  strip.setPixelColor(5, 255, 0, 255); // modrá LED 5
  strip.setPixelColor(7, 255, 0, 255); // modrá LED 7

  // nastavení LED diod 0, 2, 4 a 6 na červenou barvu
  strip.setPixelColor(0, 255, 0, 255); // červená LED 0
  strip.setPixelColor(2, 255, 0, 255); // červená LED 2
  strip.setPixelColor(4, 255, 0, 255); // červená LED 4
  strip.setPixelColor(6, 255, 0, 255); // červená LED 6

  // aktualizace LED modulu s nastavenými barvami a jasem
  strip.show();

}
