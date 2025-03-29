# Touchless Sense ✋💡

**Touchless Sense** ist ein Embedded-ML-Projekt, bei dem mithilfe des APDS9960-Sensors und TinyML Gesten sowie die Umgebungshelligkeit erkannt werden – komplett ohne Berührung. Ziel war es, einfache Handbewegungen zur Steuerung von LEDs zu verwenden, um einen berührungslosen, intuitiven Prototypen zu entwickeln.

## 🔧 Verwendete Hardware

- **Arduino Nano 33 BLE Sense**
- **APDS9960 Sensor** (für Proximity und Licht)
- **Eingebaute LEDs**:
  - **Blaue LED (RGB oben)**: Reaktion auf Handgesten (wave/hover)
  - **Orange LED (LED_BUILTIN)**: Reaktion auf Helligkeit (bright/dark)

## 🧠 Modelle & Funktionsweise

Es wurden **zwei Modelle** trainiert und verwendet:

1. **Proximity-Modell (Gestensteuerung)**  
   → Erkennt `wave`, `hover` und `idle` anhand von Proximity-Daten  
   → Trainingsdaten: `@60 x 3 Sekunden`  
   → Window Size: `2 Sekunden`, Frequency: `50 Hz`  
   → Processing Block: `Spectral Analysis (proximity)`  
   → Learning Block: `Classification & Anomaly Detection (K-Means)`  
   → Genauigkeit: `97,5%`
![image](https://github.com/user-attachments/assets/dfaa0600-4470-420b-a702-d0275d74b21d)
![image](https://github.com/user-attachments/assets/3a502d4e-5a81-46ad-b6b3-ae4b98575537)
![image](https://github.com/user-attachments/assets/141ad6c1-8bfd-4707-b750-ff7bd801e810)
![image](https://github.com/user-attachments/assets/8ba4660f-91e1-4c86-9696-1b043d9113e7)
![image](https://github.com/user-attachments/assets/d688f196-c86d-43ea-ba0d-d70bf060ab34)


 
3. **Brightness-Modell (Helligkeitserkennung)**  
   → Erkennt `bright` und `dark` anhand der Umgebungshelligkeit  
   → Trainingsdaten: `@30 x 10 Sekunden`  
   → Window Size: `1 Sekunde`, Frequency: `50 Hz`  
   → Processing Block: `Spectral Analysis (brightness)`  
   → Learning Block: `Classification & Anomaly Detection (K-Means)`  
   → Genauigkeit: `99,9%`
![image](https://github.com/user-attachments/assets/e729d505-8bed-4fbd-9d11-d76e2efdc7d8)
![image](https://github.com/user-attachments/assets/6f53a67c-9a02-4628-a960-69738e3b88b2)
![image](https://github.com/user-attachments/assets/6872fa6f-b470-4a04-9a5e-09e91d9f18de)
![image](https://github.com/user-attachments/assets/3e9d5cd2-937a-40e6-8089-41a81ae18f08)
![image](https://github.com/user-attachments/assets/d25e56f5-d11a-4014-ba8a-058b4d0ed84c)

<br>
<br>
<br>
<br>
<br>
<br>

Jedes Modell wurde separat in Edge Impulse trainiert, getestet und anschließend als `.zip` exportiert und in Arduino implementiert.
![image](https://github.com/user-attachments/assets/98ba84d2-b132-4369-927f-075cb22a9c3c)

Zum Clonen:
- Proximity-Modell: https://studio.edgeimpulse.com/public/652063/live
- Brightness-Modell: https://studio.edgeimpulse.com/public/654614/live

## 🎯 Funktionen

| Aktion           | Beschreibung                                    |
|------------------|--------------------------------------------------|
| **Wave**         | Grüne LED beginnt zu blinken                     |
| **Hover**        | Grüne LED hört auf zu blinken                    |
| **Dark**         | Orange LED blinkt (z. B. bei verdecktem Sensor)  |
| **Bright**       | Orange LED bleibt aus                           |

## 💻 Projektstruktur
```
Touchless-Sense/
│
├── sketches/
│   ├── proximity_sketch_mar22a/    → Arduino-Sketch für Proximity-Modell
│   └── brightness_sketch_mar25b/   → Arduino-Sketch für Brightness-Modell
│
├── arduino-library-zip/
│   ├── ei-daemon-test-arduino-1.x.x.zip → Edge-Impulse-Modell für Brightness
│   └── ei-brightness-test-arduino-1.x.x.zip → Edge-Impulse-Modell für Proximity

```

## 🛠️ Setup

1. Arduino Nano 33 BLE Sense per USB anschließen
2. Edge-Impulse-Modell einbinden:
In der Arduino IDE → Sketch → Include Library → Add .ZIP Library
→ Je nach Anwendung ei-brightness-test-arduino.zip oder ei-daemon-test-arduino.zip aus dem Ordner arduino-library-zip/ auswählen
3. Sketch öffnen: z. B. brightness_sketch_mar25b.ino oder proximity_sketch_mar22a.ino im Ordner sketches/
4. Sketch hochladen
5. Serial Monitor öffnen (115200 Baud)
6. Aktion durchführen (wave/hover und bright/dark)


