# 🚗 ESP2866 RC Autíčko (Wi-Fi Remote Car)

Tento projekt ukazuje, jak postavit jednoduché **RC autíčko ovládané přes Wi-Fi** pomocí **dvou ESP32-C6** mikrokontrolérů.  
Jeden ESP32-C6 slouží jako **ovladač**, druhý jako **přijímač** připojený k **H-můstku** pro řízení dvou DC motorků
---

## 🧩 Hardware

### 🔹 Přijímač (autíčko)
- **ES2866**
- **H-můstek** – L9110S
- **2× DC motorek** 


### 🔹 Ovladač
- **ESP2866**
- **Joystick a dvě tlačítka** 

---

## 📡 Komunikace

ESP2866 používají **Wi-Fi (ESP-NOW)**:
- Ovladač posílá hodnoty směru a rychlosti.
- Přijímač převádí příkazy na PWM signály pro H-můstek.
