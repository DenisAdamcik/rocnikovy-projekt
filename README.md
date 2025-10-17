# 🚗 ESP32-C6 RC Autíčko (Wi-Fi Remote Car)

Tento projekt ukazuje, jak postavit jednoduché **RC autíčko ovládané přes Wi-Fi** pomocí **dvou ESP32-C6** mikrokontrolérů.  
Jeden ESP32-C6 slouží jako **ovladač**, druhý jako **přijímač** připojený k **H-můstku** pro řízení dvou DC motorků
---

## 🧩 Hardware

### 🔹 Přijímač (autíčko)
- **ESP32-C6**
- **H-můstek** – L9110S
- **2× DC motorek** 
- **Zdroj napájení** 


### 🔹 Ovladač
- **ESP32-C6**
- **Joystick a dvě tlačítka** 
- **Napájení 3.3–5V (baterie, USB)**

---

## 📡 Komunikace

ESP32-C6 používají **Wi-Fi (ESP-NOW)**:
- Ovladač posílá hodnoty směru a rychlosti.
- Přijímač převádí příkazy na PWM signály pro H-můstek.
