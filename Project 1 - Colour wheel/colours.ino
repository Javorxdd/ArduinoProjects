// You can use this only by using Arduino IDE

#include <Adafruit_NeoPixel.h>

#define LED_PIN     6
#define NUM_LEDS    7

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Deklarace funkce
uint32_t wheel(byte wheelPos);

void setup() {
  pixels.begin();
}

void loop() {
  static uint8_t hue = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i != 0) {  // přeskakuje první LED diodu
      uint32_t color = wheel((i * 256 / NUM_LEDS + hue) % 256);
      pixels.setPixelColor(i, color); // nastavení barvy
    }
  }
  pixels.setBrightness(2);

  pixels.show();
  delay(5);
  hue++;
}

// Definice funkce
uint32_t wheel(byte wheelPos) {
  if (wheelPos < 85) {
    return pixels.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
  } else if (wheelPos < 170) {
    wheelPos -= 85;
    return pixels.Color(255 - wheelPos * 3, 0, wheelPos * 3);
  } else {
    wheelPos -= 170;
    return pixels.Color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
}
