#include <ESP8266WiFi.h>
#include <espnow.h>

typedef struct struct_message {
  int a0Value;
  bool button1;
  bool button2;
  bool button3;
} struct_message;

struct_message myData;

#define A1_A 14  // D5 – pravý motor 1
#define A1_B 12  // D6 – pravý motor 2
#define B1_A 4   // D7 – levý motor 1
#define B1_B 5   // D8 – levý motor 2

// === Konfigurace ===// Středová hodnota joysticku
const int deadzone = 40;    

// === Proměnné pro bezpečnostní stop ===
unsigned long lastRecvTime = 0;
const unsigned long timeout = 1000; // 1 sekunda bez dat → stop

// === Funkce řízení motorů ===
void motorStop() {
  digitalWrite(A1_A, LOW);
  digitalWrite(A1_B, LOW);
  digitalWrite(B1_A, LOW);
  digitalWrite(B1_B, LOW);
}

void motorForward() {
  digitalWrite(A1_A, HIGH);
  digitalWrite(A1_B, LOW);
  digitalWrite(B1_A, LOW);
  digitalWrite(B1_B, HIGH);
}

void motorBackward() {
  digitalWrite(A1_A, LOW);
  digitalWrite(A1_B, HIGH);
  digitalWrite(B1_A, HIGH);
  digitalWrite(B1_B, LOW);
}

// === Callback při přijetí dat ===
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  lastRecvTime = millis();

  int joy = myData.a0Value;

  // === Řízení motoru ===
  if (abs(joy - joyCenter) < deadzone) {
        if (myData.button1 == 0) {
      // ZPĚT - VLEVO
      digitalWrite(A1_A, LOW);
      digitalWrite(A1_B, HIGH);
      digitalWrite(B1_A, LOW);
      digitalWrite(B1_B, LOW);
      Serial.println("Motor: ZPĚT - VLEVO");
    } 
    else if (myData.button3 == 0) {
      // ZPĚT - VPRAVO
      digitalWrite(A1_A, LOW);
      digitalWrite(A1_B, LOW);
      digitalWrite(B1_A, HIGH);
      digitalWrite(B1_B, LOW);
      Serial.println("Motor: ZPĚT - VPRAVO");
    } 
    else{
    motorStop();
    Serial.println("Motor: STOP");
    }
  } 
  else if (joy < joyCenter) {
    // Pohyb vpřed
    if (myData.button1 == 0) {
      // VLEVO
      digitalWrite(A1_A, HIGH);
      digitalWrite(A1_B, LOW);
      digitalWrite(B1_A, LOW);
      digitalWrite(B1_B, LOW);
      Serial.println("Motor: VPŘED - VLEVO");
    } 
    else if (myData.button3 == 0) {
      // VPRAVO
      digitalWrite(A1_A, LOW);
      digitalWrite(A1_B, LOW);
      digitalWrite(B1_A, LOW);
      digitalWrite(B1_B, HIGH);
      Serial.println("Motor: VPŘED - VPRAVO");
    } 
    else {
      // ROVNĚ
      motorForward();
      Serial.println("Motor: VPŘED");
    }
  } 
  else if (joy > joyCenter) {
    // Pohyb zpět
    if (myData.button1 == 0) {
      // ZPĚT - VLEVO
      digitalWrite(A1_A, LOW);
      digitalWrite(A1_B, HIGH);
      digitalWrite(B1_A, LOW);
      digitalWrite(B1_B, LOW);
      Serial.println("Motor: ZPĚT - VLEVO");
    } 
    else if (myData.button3 == 0) {
      // ZPĚT - VPRAVO
      digitalWrite(A1_A, LOW);
      digitalWrite(A1_B, LOW);
      digitalWrite(B1_A, HIGH);
      digitalWrite(B1_B, LOW);
      Serial.println("Motor: ZPĚT - VPRAVO");
    } 
    else {
      // ROVNĚ
      motorBackward();
      Serial.println("Motor: ZPĚT");
    }
  }
}

// === Nastavení ===
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  pinMode(A1_A, OUTPUT);
  pinMode(A1_B, OUTPUT);
  pinMode(B1_A, OUTPUT);
  pinMode(B1_B, OUTPUT);

  motorStop();

  if (esp_now_init() != 0) {
    Serial.println("Chyba inicializace ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Receiver připraven – čeká na data...");
}

void loop() {
  if (millis() - lastRecvTime > timeout) {
    motorStop();
  }
}
