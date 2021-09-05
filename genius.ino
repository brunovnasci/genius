#define GAME_TOGGLE 1
#define LED_COUNT 4
#define RED 3
#define GREEN 4
#define YELLOW 5
#define BLUE 6
#define BUZZER 7
#define RED_PIN 8
#define GREEN_PIN 9
#define YELLOW_PIN 10
#define BLUE_PIN 11

// TODO: Buzzer!

int lastButtonState;
int currentButtonState;
int leds[LED_COUNT] = {
  RED,
  GREEN,
  YELLOW,
  BLUE
};

int ledPins[LED_COUNT] = {
  RED_PIN,
  GREEN_PIN,
  YELLOW_PIN,
  BLUE_PIN
};

int ledTone[LED_COUNT] = {
  262,
  330,
  494,
  740
};

int combination[200];
int playerInput[200];
bool isGameOver = false;
int actualRound = 0;

void setup() {
  Serial.begin(9600);
  currentButtonState = digitalRead(GAME_TOGGLE);

  pinMode(GAME_TOGGLE, INPUT);
  pinMode(RED_PIN, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN_PIN, INPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW_PIN, INPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(BLUE_PIN, INPUT);
  pinMode(BLUE, OUTPUT);
}

void resetGameContext() {
  combination[200] = {};
  playerInput[200] = {};
  isGameOver = false;
  actualRound = 0;
}

bool gameToggle() {
  lastButtonState    = currentButtonState;
  currentButtonState = digitalRead(GAME_TOGGLE);

  return lastButtonState == LOW && currentButtonState == HIGH;
}

void loop() {
  if (gameToggle()) {
    while (!isGameOver) {
      insertSequence();
      showSequence();
      waitPlayer();
      actualRound++;
      delay(750);
    }
    resetGameContext();
  }
}

void insertSequence() {
  int nextSequence = random(3, 7);
  combination[actualRound] = nextSequence;
}

void showSequence() {
  for (int sequenceIndex = 0; sequenceIndex <= actualRound; sequenceIndex++) {
    flashLed(combination[sequenceIndex], 500, 500);
  }
}

void waitPlayer() {
  for (int sequenceIndex = 0; sequenceIndex <= actualRound; sequenceIndex++) {
    receivePlayerInput(sequenceIndex);
    validatePlayerInput(sequenceIndex);

    if (isGameOver) {
      break;
    }
  }
}

void receivePlayerInput(int sequenceIndex) {
  bool inputted = false;
  while (!inputted) {
    for (int ledIndex = 0; ledIndex <= LED_COUNT - 1; ledIndex++) {
      if (digitalRead(ledPins[ledIndex]) == HIGH) {
        int inputtedLed = leds[ledIndex];
        playerInput[sequenceIndex] = inputtedLed;
        flashLed(inputtedLed, 300, 0);
        inputted = true;
      }
    }
  }
}

void validatePlayerInput(int sequenceIndex) {
  if (playerInput[sequenceIndex] != combination[sequenceIndex]) {
    isGameOver = true;
  }
}

void flashLed(int ledPin, int timeActive, int afterDelay) {
  digitalWrite(ledPin, HIGH);
  delay(timeActive);
  digitalWrite(ledPin, LOW);
  delay(afterDelay);
}
