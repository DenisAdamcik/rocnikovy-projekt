#include <ESP8266WiFi.h>
#include <espnow.h>

// ===== STRUKTURA DAT =====
typedef struct struct_message {
  bool button1; // VZAD
  bool button2; // VPRAVO
  bool button3; // VPŘED
  bool button4; // VLEVO
} struct_message;

struct_message myData;

// ===== PINY MOTORŮ =====
// PRAVÝ motor
#define A1_A 14  // vpřed
#define A1_B 12  // vzad
// LEVÝ motor
#define B1_A 4   // vzad
#define B1_B 5   // vpřed

// ===== LED PINY =====
#define LED_GREEN 13  // ESP-NOW spojeno
#define LED_RED   15  // ESP-NOW odpojeno

unsigned long lastRecvTime = 0;
const unsigned long timeout = 1000;
bool dataReceived = false;

// ===== PWM =====
const int speed = 600;
const int turnSpeed = 300;
const int dif = 15;

void setMotorsPWM(int a1a, int a1b, int b1a, int b1b) {
  analogWrite(A1_A, a1a);
  analogWrite(A1_B, a1b);
  analogWrite(B1_A, b1a + dif);
  analogWrite(B1_B, b1b + dif);
}

// ===== CALLBACK =====
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  lastRecvTime = millis();
  dataReceived = true;

  // ESP-NOW spojení OK
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);

  Serial.print("Buttons: ");
  Serial.print(myData.button1 == 0 ? "VZAD "   : "_ ");
  Serial.print(myData.button3 == 0 ? "VPRED "  : "_ ");
  Serial.print(myData.button2 == 0 ? "VPRAVO " : "_ ");
  Serial.print(myData.button4 == 0 ? "VLEVO "  : "_ ");
  Serial.print(" | ");

  int pressed =
    (myData.button1 == 0) +
    (myData.button2 == 0) +
    (myData.button3 == 0) +
    (myData.button4 == 0);

  // 3 nebo 4 tlačítka = STOP (bezpečnost)
  if (pressed >= 3) {
    setMotorsPWM(0, 0, 0, 0);
    Serial.println("ACTION: STOP (3+ buttons)");
    return;
  }

  // ===== KOMBINACE =====

  // VPŘED + VLEVO
  if (myData.button3 == 0 && myData.button4 == 0) {
    setMotorsPWM(speed, 0, 0, 200);
    Serial.println("ACTION: VPRED + VLEVO");
    return;
  }

  // VPŘED + VPRAVO
  if (myData.button3 == 0 && myData.button2 == 0) {
    setMotorsPWM(200, 0, 0, speed);
    Serial.println("ACTION: VPRED + VPRAVO");
    return;
  }

  // VZAD + VLEVO
  if (myData.button1 == 0 && myData.button4 == 0) {
    setMotorsPWM(0, speed, 200, 0);
    Serial.println("ACTION: VZAD + VLEVO");
    return;
  }

  // VZAD + VPRAVO
  if (myData.button1 == 0 && myData.button2 == 0) {
    setMotorsPWM(0, 200, speed, 0);
    Serial.println("ACTION: VZAD + VPRAVO");
    return;
  }

  // ===== JEDNOTLIVÉ SMĚRY =====

  // VPŘED
  if (myData.button3 == 0) {
    setMotorsPWM(speed, 0, 0, speed);
    Serial.println("ACTION: VPRED");
    return;
  }

  // VZAD
  if (myData.button1 == 0) {
    setMotorsPWM(0, speed, speed, 0);
    Serial.println("ACTION: VZAD");
    return;
  }

  // VLEVO (otáčení)
  if (myData.button4 == 0) {
    setMotorsPWM(speed, 0, turnSpeed, 0);
    Serial.println("ACTION: VLEVO");
    return;
  }

  // VPRAVO (otáčení)
  if (myData.button2 == 0) {
    setMotorsPWM(0, speed, 0, turnSpeed);
    Serial.println("ACTION: VPRAVO");
    return;
  }

  // žádné tlačítko
  setMotorsPWM(0, 0, 0, 0);
  Serial.println("ACTION: STOP");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  pinMode(A1_A, OUTPUT);
  pinMode(A1_B, OUTPUT);
  pinMode(B1_A, OUTPUT);
  pinMode(B1_B, OUTPUT);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // výchozí stav LED
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);

  setMotorsPWM(0, 0, 0, 0);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("ESP-NOW Slave Initialized. Waiting for data...");
}

void loop() {
  if (!dataReceived) {
    setMotorsPWM(0, 0, 0, 0);
    return;
  }

  if (millis() - lastRecvTime > timeout) {
    setMotorsPWM(0, 0, 0, 0);

    // ztráta spojení
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);

    Serial.println("ACTION: STOP (timeout)");
  }
}
