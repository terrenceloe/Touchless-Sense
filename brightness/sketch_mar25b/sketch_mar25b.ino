#include <Arduino.h>
#include <Arduino_APDS9960.h>
#include <brightness-test_inferencing.h>  // Your Edge Impulse brightness model

#define ORANGE_LED_PIN LED_BUILTIN  // Built-in LED on many boards

float brightness_buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

// LED blink state
bool blink_enabled = false;
bool led_on = false;
unsigned long last_blink_time = 0;
const unsigned long BLINK_INTERVAL = 100;  // 0.1s

int get_brightness_data(size_t offset, size_t length, float *out_ptr) {
  memcpy(out_ptr, brightness_buffer + offset, length * sizeof(float));
  return 0;
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(ORANGE_LED_PIN, OUTPUT);
  digitalWrite(ORANGE_LED_PIN, LOW);

  if (!APDS.begin()) {
    Serial.println("Failed to initialize APDS9960!");
    while (1);
  }

  Serial.println("Brightness model ready.");
}

void loop() {
  // === Handle blinking LED ===
  if (blink_enabled && millis() - last_blink_time >= BLINK_INTERVAL) {
    last_blink_time = millis();
    led_on = !led_on;
    digitalWrite(ORANGE_LED_PIN, led_on ? HIGH : LOW);
  }

  size_t idx = 0;
  const unsigned long interval = 1000 / EI_CLASSIFIER_FREQUENCY;

  while (idx < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
    unsigned long start = millis();

    if (APDS.colorAvailable()) {
      int r, g, b, c;
      APDS.readColor(r, g, b, c);
      brightness_buffer[idx++] = (float)c;
    }

    // Small delay to maintain sample rate
    while ((millis() - start) < interval) {
      // Still blink while waiting
      if (blink_enabled && millis() - last_blink_time >= BLINK_INTERVAL) {
        last_blink_time = millis();
        led_on = !led_on;
        digitalWrite(ORANGE_LED_PIN, led_on ? HIGH : LOW);
      }
    }
  }

  // === Run inference ===
  signal_t signal;
  signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
  signal.get_data = get_brightness_data;

  ei_impulse_result_t result;
  if (run_classifier(&signal, &result, false) == EI_IMPULSE_OK) {
    const char* label = result.classification[0].label;
    float score = result.classification[0].value;

    for (size_t i = 1; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
      if (result.classification[i].value > score) {
        label = result.classification[i].label;
        score = result.classification[i].value;
      }
    }

    Serial.print("Brightness: ");
    Serial.print(label);
    Serial.print(" (");
    Serial.print(score, 3);
    Serial.println(")");

    // === Control LED based on label ===
    if (strcmp(label, "dark") == 0) {
      blink_enabled = true;
    } else if (strcmp(label, "bright") == 0) {
      blink_enabled = false;
      digitalWrite(ORANGE_LED_PIN, LOW);
      led_on = false;
    }
  } else {
    Serial.println("Inference failed!");
  }

  delay(100);  // Brief pause before next classification round
}
