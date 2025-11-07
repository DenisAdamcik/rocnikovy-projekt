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

const int deadzone = 40;
unsigned long lastRecvTime = 0;
const unsigned long timeout = 1000;
const int joyCenter = 800; 

// === Univerzální funkce pro ovládání motorů ===
void setMotors(bool a1a, bool a1b, bool b1a, bool b1b) {
  digitalWrite(A1_A, a1a ? HIGH : LOW);
  digitalWrite(A1_B, a1b ? HIGH : LOW);
  digitalWrite(B1_A, b1a ? HIGH : LOW);
  digitalWrite(B1_B, b1b ? HIGH : LOW);
}

// === Callback při přijetí dat ===
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  lastRecvTime = millis();

  int joy = myData.a0Value;

  // === Řízení motoru ===
  if (abs(joy - joyCenter) < deadzone) {
    // Střed / stop
    if (myData.button1 == 0) {
      setMotors(0, 1, 0, 0); //VLEVO
      Serial.println("Motor: ZPĚT - VLEVO");
    } 
    else if (myData.button3 == 0) {
      setMotors(0, 0, 1, 0); //VPRAVO
      Serial.println("Motor: ZPĚT - VPRAVO");
    } 
    else {
      setMotors(0, 0, 0, 0); // STOP
      Serial.println("Motor: STOP");
    }
  }  
  else if (joy < joyCenter) {
    // Pohyb vpřed
    if (myData.button1 == 0) {
      setMotors(1, 0, 0, 0); // VPŘED - VLEVO
      Serial.println("Motor: VPŘED - VLEVO");
    } 
    else if (myData.button3 == 0) {
      setMotors(0, 0, 0, 1); // VPŘED - VPRAVO
      Serial.println("Motor: VPŘED - VPRAVO");
    } 
    else {
      setMotors(1, 0, 0, 1); // VPŘED
      Serial.println("Motor: VPŘED");
    }
  } 
  else {
    // Pohyb zpět
    if (myData.button1 == 0) {
      setMotors(0, 1, 0, 0); // ZPĚT - VLEVO
      Serial.println("Motor: ZPĚT - VLEVO");
    } 
    else if (myData.button3 == 0) {
      setMotors(0, 0, 1, 0); // ZPĚT - VPRAVO
      Serial.println("Motor: ZPĚT - VPRAVO");
    } 
    else {
      setMotors(0, 1, 1, 0); // ZPĚT
      Serial.println("Motor: ZPĚT");
    }
  }
}

// === Setup ===
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  pinMode(A1_A, OUTPUT);
  pinMode(A1_B, OUTPUT);
  pinMode(B1_A, OUTPUT);
  pinMode(B1_B, OUTPUT);

  setMotors(0,0,0,0); // STOP

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
    setMotors(0,0,0,0); // STOP při timeoutu
  }
}
