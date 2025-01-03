#include <WiFi.h>
#include <PubSubClient.h>

// --- Wi-Fi and MQTT Configuration ---
const char* ssid = "Von";
const char* password = "12345678";
const char* mqttBroker = "broker.emqx.io";
const char* mqttTopic = "traffic_light/status";
const char* clientId = "ESP32_Traffic_Light";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

// --- LED Configuration ---
const int ledPins[] = {15, 2, 0, 17, 5, 18};
const int ledCount = sizeof(ledPins) / sizeof(ledPins[0]);

// --- Traffic Light States ---
bool sequenceOn = false;
String currentMode = "";
int manualStateIndex = 0;
bool manualTransitioning = false;
const int manualStates[][6] = {
    {0, 0, 1, 1, 0, 0}, // Direction 1 Green, Direction 2 Red
    {1, 0, 0, 0, 0, 1}  // Direction 2 Green, Direction 1 Red
};
const int manualTransitionStates[][6] = {
    {0, 1, 0, 1, 0, 0}, // Direction 1 Yellow, Direction 2 Red
    {1, 0, 0, 0, 1, 0}  // Direction 2 Yellow, Direction 1 Red
};

// --- Helper Functions ---
void setLights(int states[]) {
    for (int i = 0; i < ledCount; i++) {
        digitalWrite(ledPins[i], states[i]);
    }
}

void blinkLights(int states[], int duration = 500) {
  setLights(states);
  delay(duration);
  int offState[ledCount] = {0, 0, 0, 0, 0, 0};
  setLights(offState);
  delay(duration);
}

void manualControl(bool transitioning = false) {
    if (transitioning) {
        setLights((int*)manualTransitionStates[manualStateIndex]);
    } else {
        setLights((int*)manualStates[manualStateIndex]);
    }
}

void handleMQTTMessage(char* topic, byte* payload, unsigned int length) {
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    message.toLowerCase();

    Serial.printf("Received: %s on %s\n", message.c_str(), topic);

    if (message == "lights off") {
        sequenceOn = false;
        currentMode = "";
        int offState[ledCount] = {0, 0, 0, 0, 0, 0};
        setLights(offState);
    } else if (message == "lights on") {
        sequenceOn = false;
        currentMode = "";
        int allOnState[ledCount] = {1, 1, 1, 1, 1, 1};
        setLights(allOnState);
    } else if (message == "all green") {
        sequenceOn = false;
        currentMode = "";
        int allGreenState[] = {0, 0, 1, 0, 0, 1};
        setLights(allGreenState);
    } else if (message == "all yellow") {
        sequenceOn = false;
        currentMode = "";
        int allYellowState[] = {0, 1, 0, 0, 1, 0};
        setLights(allYellowState);
    } else if (message == "all red") {
        sequenceOn = false;
        currentMode = "";
        int allRedState[] = {1, 0, 0, 1, 0, 0};
        setLights(allRedState);
    } else if (message == "blink green") {
      sequenceOn = true;
      currentMode = "blink_green";
      int blinkGreenState[] = {0, 0, 1, 0, 0, 1};
      while (sequenceOn && currentMode == "blink_green") {
        blinkLights(blinkGreenState);
        client.loop();  // Check for MQTT messages to interrupt
      }
    } else if (message == "blink yellow") {
      sequenceOn = true;
      currentMode = "blink_yellow";
      int blinkYellowState[] = {0, 1, 0, 0, 1, 0};
      while (sequenceOn && currentMode == "blink_yellow") {
        blinkLights(blinkYellowState);
        client.loop();  // Check for MQTT messages to interrupt
      }
    } else if (message == "blink red") {
      sequenceOn = true;
      currentMode = "blink_red";
      int blinkRedState[] = {1, 0, 0, 1, 0, 0};
      while (sequenceOn && currentMode == "blink_red") {
        blinkLights(blinkRedState);
        client.loop();  // Check for MQTT messages to interrupt
      }
    } else if (message == "blink red yellow") {
      sequenceOn = true;
      currentMode = "blink_red_yellow";
      int blinkRedYellowState[] = {1, 0, 0, 0, 1, 0};
      while (sequenceOn && currentMode == "blink_red_yellow") {
        blinkLights(blinkRedYellowState);
        client.loop();  // Check for MQTT messages to interrupt
      }
    } else if (message == "blink yellow red") {
      sequenceOn = true;
      currentMode = "blink_yellow_red";
      int blinkYellowRedState[] = {0, 1, 0, 1, 0, 0};
      while (sequenceOn && currentMode == "blink_yellow_red") {
        blinkLights(blinkYellowRedState);
        client.loop();  // Check for MQTT messages to interrupt
      }
    } else if (message == "manual mode") {
        sequenceOn = false;
        currentMode = "manual_mode";
        manualStateIndex = 0;
        manualControl();
    } else if (message == "change direction") {
        if (currentMode == "manual_mode" && !manualTransitioning) {
            manualTransitioning = true;
            manualControl(true);
            delay(1000);

            int allRedState[ledCount] = {1, 0, 0, 1, 0, 0};
            setLights(allRedState);
            delay(500);

            manualStateIndex = (manualStateIndex + 1) % 2;
            manualControl();

            manualTransitioning = false;
        }
    } else if (message == "control traffic") {
    sequenceOn = true;
    currentMode = "manage_traffic";
    // States array: {Red1, Yellow1, Green1, Red2, Yellow2, Green2, Duration(ms)}
    int states[][7] = {
        {0, 0, 1, 1, 0, 0, 3000},  // Direction 1 Green, Direction 2 Red
        {0, 1, 0, 1, 0, 0, 1000},  // Direction 1 Yellow, Direction 2 Red
        {1, 0, 0, 1, 0, 0, 500},   // Both directions Red
        {1, 0, 0, 0, 0, 1, 3000},  // Direction 2 Green, Direction 1 Red
        {1, 0, 0, 0, 1, 0, 1000},  // Direction 2 Yellow, Direction 1 Red
        {1, 0, 0, 1, 0, 0, 500}    // Both directions Red
    };
      while (sequenceOn && currentMode == "manage_traffic") {
        for (int i = 0; i < 6; i++) {
            if (!sequenceOn || currentMode != "manage_traffic") break;
            int currentState[] = {states[i][0], states[i][1], states[i][2], 
                                states[i][3], states[i][4], states[i][5]};
            setLights(currentState);
            delay(states[i][6]);
            client.loop();
        }
    }
    } else if (message == "christmas sequence") {
    sequenceOn = true;
    currentMode = "christmas";
    int currentColor = 0;
    int currentLight = 0;
    
    while (sequenceOn && currentMode == "christmas") {
        int lightState[] = {0, 0, 0, 0, 0, 0};
        
        if (currentColor == 0 && currentLight == 0) {
            lightState[0] = lightState[3] = 1;  // Red
        } else if (currentColor == 1) {
            lightState[1] = lightState[4] = 1;  // Yellow
        } else if (currentColor == 2) {
            lightState[2] = lightState[5] = 1;  // Green
        }
        
        setLights(lightState);
        currentColor = (currentColor + 1) % 3;
        if (currentColor == 0) currentLight = (currentLight + 1) % 2;
        
        delay(500);
        client.loop();
    }
}
}

void connectWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("Connecting to Wi-Fi...");
        delay(1000);
    }
    Serial.println("Connected to Wi-Fi");
}

void connectMQTT() {
    while (!client.connected()) {
        Serial.print("Connecting to MQTT...");
        if (client.connect(clientId)) {
            Serial.println("Connected");
            client.subscribe(mqttTopic);
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" Try again in 5 seconds");
            delay(5000);
        }
    }
}

// --- Setup and Loop ---
void setup() {
    Serial.begin(115200);

    for (int i = 0; i < ledCount; i++) {
        pinMode(ledPins[i], OUTPUT);
        digitalWrite(ledPins[i], LOW);
    }

    connectWiFi();
    client.setServer(mqttBroker, 1883);
    client.setCallback(handleMQTTMessage);
}

void loop() {
    if (!client.connected()) {
        connectMQTT();
    }
    if (sequenceOn) { // Check if a blinking sequence is active
    client.loop(); // Handle incoming MQTT messages to interrupt sequence
    }
    client.loop();
    delay(10);
}
