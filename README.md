# 🚗 ESP2866 RC Autíčko (Wi-Fi Remote Car)

Tento projekt ukazuje, jak postavit jednoduché **RC autíčko ovládané přes Wi-Fi** pomocí **dvou ESP8266** mikrokontrolérů.  
Jeden ESP8266 slouží jako **ovladač**, druhý jako **přijímač** připojený k **H-můstku** pro řízení dvou DC motorků
---

## 🧩 Hardware

### 🔹 Přijímač (autíčko)
- **ES2866**
- **H-můstek** – L9110S
- **2× DC motorek** 


### 🔹 Ovladač
- **ESP2866**
- **Čtyři Pull up rezistory(Tlačítka)**
- **Čtyři rezistory 10k** 

---

## 📡 Komunikace

ESP2866 používají **Wi-Fi (ESP-NOW)**:
- Ovladač posílá informace z tlacitek
- Přijímač převádí příkazy na PWM signály pro H-můstek.


# 🎥 YouTube Video

▶️ Odkaz na video:  
https://www.youtube.com/watch?v=MKybWMqmsl4
