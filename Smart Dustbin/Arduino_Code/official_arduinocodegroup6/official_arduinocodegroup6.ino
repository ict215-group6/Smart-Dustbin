#include <Servo.h>

// Pins

const uint8_t TRIG_LEVEL = 7;
const uint8_t ECHO_LEVEL = 6;
const uint8_t TRIG_HAND  = 5;
const uint8_t ECHO_HAND  = 4;
const uint8_t SERVO_LID  = 9;
const uint8_t SERVO_AUX  = 10;
const uint8_t BUZZER_PIN = 11;
const uint8_t LED_PIN    = 12;

// Config (adjust)

const unsigned int binHeight = 38;
const unsigned int FULL_THRESHOLD = 85;
const unsigned int HAND_THRESHOLD = 15;
const unsigned long LEVEL_INTERVAL = 2000;
const unsigned long OPEN_DURATION = 2000;
const unsigned long HAND_COOLDOWN = 3000;

const uint8_t LID_CLOSED = 0;
const uint8_t LID_OPEN   = 85;
const uint8_t AUX_DOWN   = 0;
const uint8_t AUX_UP     = 90;

const unsigned long BUZZ_ON_MS  = 300;
const unsigned long BUZZ_OFF_MS = 700;
const uint8_t LEVEL_SAMPLES = 4;

// LED blink timing

const unsigned long LED_BLINK_MS = 500;

// State

Servo lidServo, auxServo;
unsigned long lastLevelRead = 0;
unsigned long lastHandOpen = 0;
unsigned long lastBuzzToggle = 0;
unsigned long lastLedToggle = 0;
bool buzzerOn = false;
bool ledState = false;
bool ledBlinkEnabled = false;
int lastFillPercent = -1;
bool forceFull = false;
bool prevIsFull = false;

long readUltrasonicCM(uint8_t trigPin, uint8_t echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  unsigned long dur = pulseIn(echoPin, HIGH, 30000UL);
  if (dur == 0) return -1;
  return (long)(dur * 0.034 / 2.0);
}

int readLevelPercent() {
  long sum = 0;
  int valid = 0;
  for (uint8_t i = 0; i < LEVEL_SAMPLES; ++i) {
    long d = readUltrasonicCM(TRIG_LEVEL, ECHO_LEVEL);
    if (d > 0) { sum += d; valid++; }
    delay(40);
  }
  if (valid == 0) return -1;
  long avg = sum / valid;
  long pct = map((int)avg, (int)binHeight, 0, 0, 100);
  if (pct < 0) pct = 0;
  if (pct > 100) pct = 100;
  return (int)pct;
}

void openLid() {
  lidServo.write(LID_OPEN);
  lastHandOpen = millis();
}

void closeLid() {
  lidServo.write(LID_CLOSED);
}

void setFullOutputs(bool full) {
  if (full) {
    auxServo.write(AUX_UP);
    // LED handled separately (blink)
  } else {
    auxServo.write(AUX_DOWN);
    noTone(BUZZER_PIN);
    buzzerOn = false;
    ledBlinkEnabled = false;
    ledState = false;
    digitalWrite(LED_PIN, LOW);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_LEVEL, OUTPUT); pinMode(ECHO_LEVEL, INPUT);
  pinMode(TRIG_HAND, OUTPUT);  pinMode(ECHO_HAND, INPUT);
  pinMode(BUZZER_PIN, OUTPUT); pinMode(LED_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); digitalWrite(LED_PIN, LOW);

  lidServo.attach(SERVO_LID);
  auxServo.attach(SERVO_AUX);
  lidServo.write(LID_CLOSED);
  auxServo.write(AUX_DOWN);

  digitalWrite(LED_PIN, HIGH);
  tone(BUZZER_PIN, 1000, 80);
  delay(150);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  unsigned long now = millis();

  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 'f' || c == 'F') {
      forceFull = !forceFull;
      Serial.print("Force full: ");
      Serial.println(forceFull ? "ON" : "OFF");
    }
  }


  if (now - lastLevelRead >= LEVEL_INTERVAL) {
    lastLevelRead = now;
    int fill = readLevelPercent();
    if (fill >= 0) {
      lastFillPercent = fill;
      Serial.print("Fill %: "); Serial.println(fill);
    } else {
      Serial.println("Level sensor no echo");
    }
  }


  bool isFull = forceFull || (lastFillPercent >= 0 && lastFillPercent >= FULL_THRESHOLD);

  if (isFull != prevIsFull) {
    prevIsFull = isFull;
    if (isFull) {
      ledBlinkEnabled = true; // enable LED blink when entering full
    }
    setFullOutputs(isFull);
  }

  if (!isFull) {
    long handDist = readUltrasonicCM(TRIG_HAND, ECHO_HAND);
    if (handDist > 0 && handDist <= HAND_THRESHOLD && (now - lastHandOpen) > HAND_COOLDOWN) {
      openLid();
    }
  }


  if (lastHandOpen != 0 && (now - lastHandOpen) >= OPEN_DURATION) {
    closeLid();
    lastHandOpen = 0;
  }


  if (isFull) {
    if (!buzzerOn && (now - lastBuzzToggle >= BUZZ_OFF_MS)) {
      tone(BUZZER_PIN, 1000);
      buzzerOn = true;
      lastBuzzToggle = now;
    } else if (buzzerOn && (now - lastBuzzToggle >= BUZZ_ON_MS)) {
      noTone(BUZZER_PIN);
      buzzerOn = false;
      lastBuzzToggle = now;
    }
  }

  
  if (ledBlinkEnabled) {
    if (now - lastLedToggle >= LED_BLINK_MS) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
      lastLedToggle = now;
    }
  }

  delay(80);
}