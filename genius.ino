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

int lastGameToggleState;
int currentGameToggleState;
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

bool isGameOver = false;
int sequence[200];
int playerSequenceInput[200];
int actualRound = 0;

void setup() {
  Serial.begin(9600);
  currentGameToggleState = digitalRead(GAME_TOGGLE);

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
  isGameOver = false;
  sequence[200] = {};
  playerSequenceInput[200] = {};
  actualRound = 0;
}

bool gameToggle() {
  lastGameToggleState    = currentGameToggleState;
  currentGameToggleState = digitalRead(GAME_TOGGLE);

  return lastGameToggleState == LOW && currentGameToggleState == HIGH;
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
  sequence[actualRound] = nextSequence;
}

void showSequence() {
  for (int sequenceIndex = 0; sequenceIndex <= actualRound; sequenceIndex++) {
    flashLed(sequence[sequenceIndex], 500, 500);
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
        playerSequenceInput[sequenceIndex] = inputtedLed;
        flashLed(inputtedLed, 300, 0);
        inputted = true;
      }
    }
  }
}

void validatePlayerInput(int sequenceIndex) {
  if (playerSequenceInput[sequenceIndex] != sequence[sequenceIndex]) {
    isGameOver = true;
  }
}

void flashLed(int ledPin, int timeActive, int afterDelay) {
  digitalWrite(ledPin, HIGH);
  delay(timeActive);
  digitalWrite(ledPin, LOW);
  delay(afterDelay);
}
