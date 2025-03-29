# Touchless Sense ✋💡

**Touchless Sense** ist ein Embedded-ML-Projekt, bei dem mithilfe des APDS9960-Sensors und TinyML Gesten sowie die Umgebungshelligkeit erkannt werden – komplett ohne Berührung. Ziel war es, einfache Handbewegungen zur Steuerung von LEDs zu verwenden, um einen berührungslosen, intuitiven Prototypen zu entwickeln.

## 🔧 Verwendete Hardware

- **Arduino Nano 33 BLE Sense**
- **APDS9960 Sensor** (für Proximity und Licht)
- **Eingebaute LEDs**:
  - **Grüne LED (RGB oben)**: Reaktion auf Handgesten
  - **Orange LED (LED_BUILTIN)**: Reaktion auf Helligkeit (bright/dark)

## 🧠 Modelle & Funktionsweise

Es wurden **zwei Modelle** trainiert und verwendet:

1. **Proximity-Modell (Gestensteuerung)**  
   → Erkennt `wave`, `hover` und `idle` anhand von Proximity-Daten.

2. **Brightness-Modell (Lichterkennung)**  
   → Erkennt `bright` und `dark` anhand der Umgebungshelligkeit (Clear-Wert).

Jedes Modell wurde separat in Edge Impulse trainiert, getestet und anschließend als `.zip` exportiert und in Arduino implementiert.

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
├── brightness/
│   └── sketch_mar25b/   → Arduino-Sketch für Brightness-Modell
│
├── proximity/
│   └── sketch_mar22a/   → Arduino-Sketch für Proximity-Modell
```

## 🛠️ Setup

1. Projekt in Edge Impulse trainieren
2. Modelle als Arduino Library exportieren
3. `.ino`-Sketch hochladen
4. Serial Monitor aktivieren → Vorhersagen & LED-Reaktionen live verfolgen

## ⚠️ Herausforderungen

- Kombinierte Modelle (Proximity + Brightness) in einem Sketch zu vereinen war problematisch → Konflikte im `model_metadata.h`
- Links-/Rechts-Gesten konnten im Proximity-Modell nicht zuverlässig erkannt werden → daher Fokus auf `wave` und `hover`
- Die Gestenerkennung wurde teilweise durch veränderte Lichtverhältnisse beeinflusst

## ✅ Fazit

Durch getrennte Modelle und gezielte Feature-Auswahl konnte ein stabiler Prototyp entwickelt werden. Die Anwendung zeigt, wie TinyML auch mit einfacher Hardware eine intuitive, berührungslose Steuerung ermöglicht.
