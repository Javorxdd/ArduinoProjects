#include <DHT.h>

#define DHT_PIN 9
const int rainSensorPin = 7;  // Připojte výstupní pin komparátoru LM393 ke digitálnímu pinu 7 na Arduinu
int previousRainValue = LOW;  // Předchozí hodnota detekce deště
DHT dht(DHT_PIN, DHT11);

void setup() {
  pinMode(rainSensorPin, INPUT);  // Nastavte pin jako vstupní
  dht.begin();
  Serial.begin(9600);  // Inicializace sériové komunikace pro výstup
}

void loop() {
  int rainValue = digitalRead(rainSensorPin);  // Čtení hodnoty z digitálního pinu

  if (rainValue != previousRainValue) {
    if (rainValue == HIGH) {
      sendRainStatusToESP("Ne, vem si žabky.");  // Odešlete informaci o dešti na ESP8266
    } else {
      sendRainStatusToESP("Prší, vem si holinky");  // Odešlete informaci o dešti na ESP8266
    }
    previousRainValue = rainValue;  // Aktualizace předchozí hodnoty detekce deště
  }

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Chyba při čtení z DHT senzoru!");
    return;
  }

  sendTemperatureAndHumidity(temperature, humidity);  // Odešlete teplotu a vlhkost na ESP8266

  delay(1000);  // Zpoždění mezi čteními
}

void sendRainStatusToESP(String status) {
  Serial.print("RainStatus:");
  Serial.println(status);
}

void sendTemperatureAndHumidity(float temperature, float humidity) {
  Serial.print("Temperature:");
  Serial.println(temperature);
  Serial.print("Humidity:");
  Serial.println(humidity);
}
