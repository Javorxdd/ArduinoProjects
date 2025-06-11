#include <Arduino.h>


// FROM HERE

// --- PIN settings  ---
const int BUTTON_PINS[] = {13, 2, 3}; // Button: 0=RED, 1=YELLOW 2=GREEN
const int RED_LIGHT_PINS[] = {4, 5, 6};
const int YELLOW_LIGHT_PINS[] = {7, 8, 9};
const int GREEN_LIGHT_PINS[] = {10, 11, 12};
const int PIEZO_PIN = A0;

const int* LIGHT_PINS[] = {RED_LIGHT_PINS, YELLOW_LIGHT_PINS, GREEN_LIGHT_PINS};

// --- Game constants ---
const int WINNING_SCORE = 60;
const int NUM_PLAYERS = 3;
const int FIRST_LIGHT = 20;
const int SECOND_LIGHT = 40;
const int THIRD_LIGHT = 60;

// TO HERE YOU CAN CHANGE IT!

// --- Game variable ---
int scores[NUM_PLAYERS] = {0, 0, 0};
int lastButtonStates[NUM_PLAYERS] = {LOW, LOW, LOW};
unsigned long lastDebounceTime[NUM_PLAYERS] = {0, 0, 0};
const unsigned long DEBOUNCE_DELAY = 50;

bool gameOver = false;
int winner = -1;

// --- Function prototypes ---
void updatePlayerLeds(int playerIndex); // Light up LED panels
void playVictoryFanfare(int winningPlayerIndex); // Plays fanfare to winner
void allLedsOff();  // Turn off all LED's
void resetGame(); // Resets game

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUM_PLAYERS; i++) {
    pinMode(BUTTON_PINS[i], INPUT);
  }

  for (int i = 0; i < NUM_PLAYERS; i++) {
    const int* pins = LIGHT_PINS[i];
    for (int j = 0; j < 3; j++) {
      pinMode(pins[j], OUTPUT);
    }
  }

  pinMode(PIEZO_PIN, OUTPUT);

  allLedsOff();

  Serial.println("Game is ready to start! Final score: " + String(WINNING_SCORE) + " points.");
}

void loop() {
  if (!gameOver) {
    for (int i = 0; i < NUM_PLAYERS; i++) {

      int currentReading = digitalRead(BUTTON_PINS[i]);

      if ((millis() - lastDebounceTime[i]) > DEBOUNCE_DELAY) {

        if (currentReading != lastButtonStates[i]) {
          lastDebounceTime[i] = millis();
          lastButtonStates[i] = currentReading;
  
          if (lastButtonStates[i] == LOW) {
            
            scores[i]++;
            Serial.print("Player ");
            Serial.print(i + 1);
            Serial.print(" has score: ");
            Serial.println(scores[i]);
    
            updatePlayerLeds(i);
    
            if (scores[i] >= WINNING_SCORE) {
              gameOver = true;
              winner = i;
              break;
            }
          }
        }
      }
    }
  }
  else {
    Serial.print("!!! PLAYER ");
    Serial.print(winner + 1);
    Serial.println(" WON!!!");

    // Turn off all enabled LED's
    allLedsOff();

    // Turns on fanfare
    playVictoryFanfare(winner);

    // Now we will wait and resets game after 3 seconds
    Serial.println("Game will restart after 3 seconds...");
    delay(3000); // 3 seconds of pause
    resetGame(); // Resets game
  }
}

/**
 * @brief Resets game to default
 */
void resetGame() {
  Serial.println("--------------------");
  Serial.println("NEW GAME BEGINS!");
  
  // Erasing all score
  for (int i = 0; i < NUM_PLAYERS; i++) {
    scores[i] = 0;
    lastButtonStates[i] = HIGH;
  }
  
  // Turn off all LED's
  allLedsOff();
  
  // We will set game variables to default
  gameOver = false;
  winner = -1;
}

void updatePlayerLeds(int playerIndex) {
  if (playerIndex < 0 || playerIndex >= NUM_PLAYERS) return;

  int score = scores[playerIndex];
  const int* pins = LIGHT_PINS[playerIndex];

  // We are using game variables
  digitalWrite(pins[0], (score >= FIRST_LIGHT) ? HIGH : LOW);
  digitalWrite(pins[1], (score >= SECOND_LIGHT) ? HIGH : LOW);
  digitalWrite(pins[2], (score >= THIRD_LIGHT) ? HIGH : LOW);
}

void allLedsOff() {
  for (int pin = 4; pin <= 12; pin++) {
    digitalWrite(pin, LOW);
  }
}

void playVictoryFanfare(int winningPlayerIndex) {
  unsigned long startTime = millis();
  unsigned long fanfareDuration = 10000;

  int melody[] = {262, 330, 392, 523, 392, 330};
  int noteDuration = 150;

  const int* winning_pins = LIGHT_PINS[winningPlayerIndex];

  while (millis() - startTime < fanfareDuration) {
    for (int i = 0; i < 6 && (millis() - startTime < fanfareDuration); i++) {
      digitalWrite(winning_pins[0], HIGH);
      digitalWrite(winning_pins[1], HIGH);
      digitalWrite(winning_pins[2], HIGH);

      tone(PIEZO_PIN, melody[i], noteDuration);
      delay(noteDuration + 50);

      digitalWrite(winning_pins[0], LOW);
      digitalWrite(winning_pins[1], LOW);
      digitalWrite(winning_pins[2], LOW);
      delay(50);
    }
  }

  noTone(PIEZO_PIN);

  // After the fanfare, LED's of the winner will permanently shine for 3 seconds
  digitalWrite(winning_pins[0], HIGH);
  digitalWrite(winning_pins[1], HIGH);
  digitalWrite(winning_pins[2], HIGH);
}
