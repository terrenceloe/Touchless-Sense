# Touchless Sense ✋💡

**Touchless Sense** ist ein Embedded-ML-Projekt, bei dem mithilfe des APDS9960-Sensors und TinyML Gesten sowie die Umgebungshelligkeit erkannt werden – komplett ohne Berührung. Ziel war es, einfache Handbewegungen zur Steuerung von LEDs zu verwenden, um einen berührungslosen, intuitiven Prototypen zu entwickeln.

## 🔧 Verwendete Hardware

- **Arduino Nano 33 BLE Sense**
- **APDS9960 Sensor** (für Proximity und Licht)
- **Eingebaute LEDs**:
  - **Grüne LED (RGB oben)**: Reaktion auf Handgesten (wave/hover)
  - **Orange LED (LED_BUILTIN)**: Reaktion auf Helligkeit (bright/dark)

## 🧠 Modelle & Funktionsweise

Es wurden **zwei Modelle** trainiert und verwendet:

1. **Proximity-Modell (Gestensteuerung)**  
   → Erkennt `wave`, `hover` und `idle` anhand von Proximity-Daten.

2. **Brightness-Modell (Helligkeitserkennung)**  
   → Erkennt `bright` und `dark` anhand der Umgebungshelligkeit.

Jedes Modell wurde separat in Edge Impulse trainiert, getestet und anschließend als `.zip` exportiert und in Arduino implementiert.

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
## ⚠️ Herausforderungen

- Kombinierte Modelle (Proximity + Brightness) in einem Sketch zu vereinen war problematisch → Konflikte im `model_metadata.h`
- Links-/Rechts-Gesten konnten im Proximity-Modell nicht zuverlässig erkannt werden → daher Fokus auf `wave` und `hover`
- Die Gestenerkennung wurde teilweise durch veränderte Lichtverhältnisse beeinflusst

## ✅ Fazit

Durch getrennte Modelle und gezielte Feature-Auswahl konnte ein stabiler Prototyp entwickelt werden. Die Anwendung zeigt, wie TinyML auch mit einfacher Hardware eine intuitive, berührungslose Steuerung ermöglicht.
