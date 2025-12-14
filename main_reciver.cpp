#include <ESP8266WiFi.h>
#include <espnow.h>

// ===== STRUKTURA DAT =====
typedef struct struct_message {
  bool button1; // vlevo
  bool button2; // vpřed
  bool button3; // vpravo
  bool button4; // vzad
} struct_message;

struct_message myData;

// ===== PINY MOTORŮ =====
// PRAVÝ motor
#define A1_A 14  // vpřed
#define A1_B 12  // vzad
// LEVÝ motor (prohozeno)
#define B1_A 4   // vzad
#define B1_B 5   // vpřed

unsigned long lastRecvTime = 0;
const unsigned long timeout = 1000;
bool dataReceived = false;

// ===== PWM =====
const int speed = 600;
const int turnSpeed = 200;

void setMotorsPWM(int a1a, int a1b, int b1a, int b1b) {
  analogWrite(A1_A, a1a);
  analogWrite(A1_B, a1b);
  analogWrite(B1_A, b1a);
  analogWrite(B1_B, b1b);
}

// ===== CALLBACK =====
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  lastRecvTime = millis();
  dataReceived = true;

  // vypis prijatych dat (stejný jako původní)
  Serial.print("Buttons: ");
  Serial.print(myData.button1 == 0 ? "1 " : "_ ");
  Serial.print(myData.button2 == 0 ? "2 " : "_ ");
  Serial.print(myData.button3 == 0 ? "3 " : "_ ");
  Serial.print(myData.button4 == 0 ? "4 " : "_ ");
  Serial.print(" | ");

  // počet stisknutých tlačítek
  int pressed =
    (myData.button1 == 0) +
    (myData.button2 == 0) +
    (myData.button3 == 0) +
    (myData.button4 == 0);

  // =====================
  //  3+ tlačítek = STOP
  // =====================
  if (pressed >= 3) {
    setMotorsPWM(0, 0, 0, 0);
    Serial.println("ACTION: STOP (3+ buttons)");
    return;
  }

  // =====================
  // VPŘED + VLEVO (Oblouk)
  // Pravý rychlý vpřed, Levý zpomalený vpřed
  // =====================
  if (myData.button2 == 0 && myData.button1 == 0) {
    setMotorsPWM(speed, 0, 0, turnSpeed);
    Serial.println("ACTION: VPRED + VLEVO (oblouk)");
    return;
  }

  // =====================
  // VPŘED + VPRAVO (Oblouk)
  // Pravý zpomalený vpřed, Levý rychlý vpřed
  // =====================
  if (myData.button2 == 0 && myData.button3 == 0) {
    setMotorsPWM(turnSpeed, 0, 0, speed);
    Serial.println("ACTION: VPRED + VPRAVO (oblouk)");
    return;
  }

  // =====================
  // VZAD + VLEVO (Oblouk)
  // Pravý rychlý vzad, Levý zpomalený vzad
  // =====================
  if (myData.button4 == 0 && myData.button1 == 0) {
    setMotorsPWM(0, speed, turnSpeed, 0);
    Serial.println("ACTION: VZAD + VLEVO (oblouk)");
    return;
  }

  // =====================
  // VZAD + VPRAVO (Oblouk)
  // Pravý zpomalený vzad, Levý rychlý vzad
  // =====================
  if (myData.button4 == 0 && myData.button3 == 0) {
    setMotorsPWM(0, turnSpeed, speed, 0);
    Serial.println("ACTION: VZAD + VPRAVO (oblouk)");
    return;
  }

  // =====================
  // VPŘED (oba plná vpřed)
  // =====================
  if (myData.button2 == 0) {
    setMotorsPWM(speed, 0, 0, speed);
    Serial.println("ACTION: VPRED");
    return;
  }

  // =====================
  // VZAD (oba plná vzad)
  // =====================
  if (myData.button4 == 0) {
    setMotorsPWM(0, speed, speed, 0);
    Serial.println("ACTION: VZAD");
    return;
  }

  // =====================
  // VLEVO (na místě)
  // Pravý vpřed, Levý vzad
  // =====================
  if (myData.button1 == 0) {
    // Pravý vpřed (speed, 0), Levý vzad (turnSpeed, 0)
    setMotorsPWM(speed, 0, turnSpeed, 0);
    Serial.println("ACTION: VLEVO (na místě)");
    return;
  }

  // =====================
  // VPRAVO (na místě)
  // Pravý vzad, Levý vpřed
  // =====================
  if (myData.button3 == 0) {
    // Pravý vzad (0, speed), Levý vpřed (0, turnSpeed)
    setMotorsPWM(0, speed, 0, turnSpeed);
    Serial.println("ACTION: VPRAVO (na místě)");
    return;
  }

  // =====================
  // STOP
  // =====================
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
  // dokud nepřišla data → STOP
  if (!dataReceived) {
    setMotorsPWM(0, 0, 0, 0);
    // Vhodné přidat malou prodlevu, aby se nezahlcovala sériová komunikace
    // delay(10); 
    return;
  }

  // ztráta signálu → STOP
  if (millis() - lastRecvTime > timeout) {
    setMotorsPWM(0, 0, 0, 0);
    Serial.println("ACTION: STOP (timeout)");
  }
}
