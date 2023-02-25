// You can use this only by using Arduino IDE

#include <Adafruit_NeoPixel.h>

#define LED_PIN     6 //what pin i use like comunication port
#define NUM_LEDS    7 //number of LED

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// function declaration
uint32_t wheel(byte wheelPos);

void setup() {
  pixels.begin();
}

void loop() {
  static uint8_t hue = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i != 0) {  // skip first LED
      uint32_t color = wheel((i * 256 / NUM_LEDS + hue) % 256);
      pixels.setPixelColor(i, color); // set color
    }
  }
  pixels.setBrightness(2); //minimum 1 maximum 255

  pixels.show();
  delay(5);
  hue++;
}

// rotating colors
// function definition
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
