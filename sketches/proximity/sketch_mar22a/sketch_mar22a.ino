#include <Arduino.h>
#include <Arduino_APDS9960.h>
#include <daemon-test_inferencing.h>  // Your Edge Impulse gesture model

#define GREEN_LED_PIN   LEDG  // Top green RGB LED

float raw_buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

// Green LED blinking control
bool green_blink_enabled = false;
bool green_led_on = false;
unsigned long last_green_blink_time = 0;
const unsigned long GREEN_BLINK_INTERVAL = 100;  // 0.1s

int get_feature_data(size_t offset, size_t length, float *out_ptr) {
  memcpy(out_ptr, raw_buffer + offset, length * sizeof(float));
  return 0;
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(GREEN_LED_PIN, HIGH);  // HIGH = OFF for RGB LED

  if (!APDS.begin()) {
    Serial.println("Failed to initialize APDS9960!");
    while (1);
  }

  Serial.println("Gesture model ready.");
}

void loop() {
  // === Blink handler (runs outside inference) ===
  if (green_blink_enabled && millis() - last_green_blink_time >= GREEN_BLINK_INTERVAL) {
    last_green_blink_time = millis();
    green_led_on = !green_led_on;
    digitalWrite(GREEN_LED_PIN, green_led_on ? LOW : HIGH);  // LOW = ON
  }

  // === Collect proximity data ===
  size_t idx = 0;
  const unsigned long interval = 1000 / EI_CLASSIFIER_FREQUENCY;

  while (idx < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
    unsigned long start = millis();

    if (APDS.proximityAvailable()) {
      raw_buffer[idx++] = (float)APDS.readProximity();
    }

    // Keep blinking during data collection
    while ((millis() - start) < interval) {
      if (green_blink_enabled && millis() - last_green_blink_time >= GREEN_BLINK_INTERVAL) {
        last_green_blink_time = millis();
        green_led_on = !green_led_on;
        digitalWrite(GREEN_LED_PIN, green_led_on ? LOW : HIGH);
      }
    }
  }

  // === Run classification ===
  signal_t signal;
  signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
  signal.get_data = &get_feature_data;

  ei_impulse_result_t result;
  if (run_classifier(&signal, &result, false) == EI_IMPULSE_OK) {
    const char* label = result.classification[0].label;
    float score = result.classification[0].value;
    for (size_t i = 1; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
      if (result.classification[i].value > score) {
        score = result.classification[i].value;
        label = result.classification[i].label;
      }
    }

    Serial.print("Gesture: ");
    Serial.print(label);
    Serial.print(" (");
    Serial.print(score, 3);
    Serial.println(")");

    // === LED control based on gesture ===
    if (strcmp(label, "wave") == 0) {
      green_blink_enabled = true;
    }
    else if (strcmp(label, "hover") == 0) {
      green_blink_enabled = false;
      digitalWrite(GREEN_LED_PIN, HIGH);  // OFF
      green_led_on = false;
    }
  }

  delay(100);  // Optional small delay before restarting loop
}
