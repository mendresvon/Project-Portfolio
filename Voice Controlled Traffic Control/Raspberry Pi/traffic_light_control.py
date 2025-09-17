from umqtt.simple import MQTTClient
import network
import time
import ubinascii
from machine import Pin

# --- Wi-Fi and MQTT Configuration ---
ssid = "Von"
password = "12345678"
mqtt_broker = "broker.emqx.io"
mqtt_topic = b"traffic_light/status"
client_id = ubinascii.hexlify(machine.unique_id())

# --- LED Configuration ---
led_pins = [2, 3, 4, 6, 7, 8]  # GPIO pins connected to LEDs (adjust as needed)
leds = [Pin(pin, Pin.OUT) for pin in led_pins] # List comprehension for more compact LED setup


def connect_wifi():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(ssid, password)
    while not wlan.isconnected():
        print("Connecting to Wi-Fi...")
        time.sleep(1)
    print("Connected to Wi-Fi:", wlan.ifconfig())
    return wlan


# --- LED Control Functions ---
def set_lights(red1, yellow1, green1, red2, yellow2, green2):  # More flexible light setting
    leds[0].value(red1)
    leds[1].value(yellow1)
    leds[2].value(green1)
    leds[3].value(red2)
    leds[4].value(yellow2)
    leds[5].value(green2)

def blink_light(light_index, duration=0.5):  # Helper function to blink individual lights
    leds[light_index].value(1)
    time.sleep(duration)
    leds[light_index].value(0)
    time.sleep(duration)

sequence_on = False
manual_state_index = 0
manual_transitioning = False  # Flag to indicate a transition is in progress
manual_states = [
    (0, 0, 1, 1, 0, 0),  # Direction 1 Green, Direction 2 Red
    (1, 0, 0, 0, 0, 1),  # Direction 2 Green, Direction 1 Red
]
manual_transition_states = [
     (0, 1, 0, 1, 0, 0),  # Direction 1 Yellow, Direction 2 Red
     (1, 0, 0, 0, 1, 0) # Direction 2 Yellow, Direction 1 Red
]

def manual_control(transitioning=False):
    global manual_state_index
    if transitioning:
        current_state = manual_transition_states[manual_state_index]
    else:
        current_state = manual_states[manual_state_index]
    set_lights(current_state[0], current_state[1], current_state[2], current_state[3], current_state[4], current_state[5])

def mqtt_callback(topic, msg):
    global sequence_on, current_mode, manual_state_index, manual_transitioning  # Use global variables to track state
    msg = msg.decode().lower()
    print(f"Received: {msg} on {topic.decode()}")

    # 0 lights off
    if msg == "lights off":
        sequence_on = False
        current_mode = None  # Stop any active mode
        set_lights(0, 0, 0, 0, 0, 0)
    
    # 1 lights on
    elif msg == "lights on":
        sequence_on = False
        current_mode = None
        set_lights(1, 1, 1, 1, 1, 1)

    # 2 all green
    elif msg == "all green":
        sequence_on = False
        current_mode = None
        set_lights(0, 0, 1, 0, 0, 1)

    # 3 all yellow
    elif msg == "all yellow":
        sequence_on = False
        current_mode = None
        set_lights(0, 1, 0, 0, 1, 0)

    # 4 all red
    elif msg == "all red":
        sequence_on = False
        current_mode = None
        set_lights(1, 0, 0, 1, 0, 0)
    
    # 5 blink green
    elif msg == "blink green":
        sequence_on = True
        current_mode = "blinking_green"
        while sequence_on and current_mode == "blinking_green":
            set_lights(0, 0, 1, 0, 0, 1)
            time.sleep(0.5)
            set_lights(0, 0, 0, 0, 0, 0)
            time.sleep(0.5)
            client.check_msg()

    # 6 blink yellow
    elif msg == "blink yellow":
        sequence_on = True
        current_mode = "blinking_yellow"
        while sequence_on and current_mode == "blinking_yellow":
            set_lights(0, 1, 0, 0, 1, 0)
            time.sleep(0.5)
            set_lights(0, 0, 0, 0, 0, 0)
            time.sleep(0.5)
            client.check_msg()

    # 7 blink red
    elif msg == "blink red":
        sequence_on = True
        current_mode = "blinking_red"
        while sequence_on and current_mode == "blinking_red":
            set_lights(1, 0, 0, 1, 0, 0)
            time.sleep(0.5)
            set_lights(0, 0, 0, 0, 0, 0)
            time.sleep(0.5)
            client.check_msg()
    
    # 8 blink red yellow
    elif msg == "blink red yellow":
        sequence_on = True
        current_mode = "blinking_red_yellow"
        while sequence_on and current_mode == "blinking_red_yellow":
            set_lights(1, 0, 0, 0, 1, 0)
            time.sleep(0.5)
            set_lights(0, 0, 0, 0, 0, 0)
            time.sleep(0.5)
            client.check_msg()
    
    # 9 blink yellow red
    elif msg == "blink yellow red":
        sequence_on = True
        current_mode = "blinking_yellow_red"
        while sequence_on and current_mode == "blinking_yellow_red":
            set_lights(0, 1, 0, 1, 0, 0)
            time.sleep(0.5)
            set_lights(0, 0, 0, 0, 0, 0)
            time.sleep(0.5)
            client.check_msg()

    # 10 control traffic
    elif msg == "control traffic":
        sequence_on = True
        current_mode = "manage_traffic"
        # Traffic light states: (Red1, Yellow1, Green1, Red2, Yellow2, Green2, Duration)
        states = [
            (0, 0, 1, 1, 0, 0, 3),  # Direction 1 Green, Direction 2 Red
            (0, 1, 0, 1, 0, 0, 1),  # Direction 1 Yellow, Direction 2 Red
            (1, 0, 0, 1, 0, 0, 0.5),  # Both directions Red (all stop briefly)
            (1, 0, 0, 0, 0, 1, 3),  # Direction 2 Green, Direction 1 Red
            (1, 0, 0, 0, 1, 0, 1),  # Direction 2 Yellow, Direction 1 Red
            (1, 0, 0, 1, 0, 0, 0.5),  # Both directions Red (all stop briefly)
        ]
        while sequence_on and current_mode == "manage_traffic":
            for red1, yellow1, green1, red2, yellow2, green2, duration in states:
                if not sequence_on or current_mode != "manage_traffic":
                    break
                set_lights(red1, yellow1, green1, red2, yellow2, green2)
                time.sleep(duration)
                client.check_msg()
    
    # 11 manual mode
    elif msg == "manual mode":
        sequence_on = False
        current_mode = "manual_mode"
        manual_state_index = 0
        manual_control()
    
    # 11.5 change direction
    elif msg == "change direction":
         if current_mode == "manual_mode" and not manual_transitioning:
            manual_transitioning = True
            
            # Transition to yellow
            manual_control(transitioning=True)
            time.sleep(1)
            set_lights(1, 0, 0, 1, 0, 0)
            time.sleep(0.5)

            # Transition to red and next state after yellow transition
            manual_state_index = (manual_state_index + 1) % len(manual_states)
            manual_control()
            
            manual_transitioning = False
    # 12 christmas sequence
    elif msg == "christmas sequence":
        sequence_on = True
        current_mode = "christmas"
        current_light = 0
        current_color = 0
        while sequence_on and current_mode == "christmas":
            set_lights(0, 0, 0, 0, 0, 0)
            if current_color == 0:
                if current_light == 0:
                    set_lights(1, 0, 0, 1, 0, 0)
                else:
                    set_lights(0, 0, 0, 0, 0, 0)
            elif current_color == 1:
                set_lights(0, 1, 0, 0, 1, 0)
            elif current_color == 2:
                set_lights(0, 0, 1, 0, 0, 1)

            current_color = (current_color + 1) % 3
            if current_color == 0:
                current_light = (current_light + 1) % 2

            client.check_msg()
            time.sleep(0.5)

def subscribe_to_mqtt():
    try:
        client = MQTTClient(client_id, mqtt_broker)
        client.set_callback(mqtt_callback)  # Set the callback function
        client.connect()
        client.subscribe(mqtt_topic)
        print(f"Subscribed to topic: {mqtt_topic.decode()}")
        return client
    except Exception as e:
        print(f"Error connecting to MQTT: {e}")
        return None


# --- Main Execution ---
connect_wifi()
client = subscribe_to_mqtt()

if client:
    while True:
        try:
            client.check_msg()
            time.sleep(1)
        except KeyboardInterrupt:
            print("Exiting...")
            client.disconnect()
            break
        except Exception as e:
            print(f"MQTT Error: {e}")
            client.disconnect()
            print("Attempting to reconnect...")
            time.sleep(5)
            connect_wifi()
            client = subscribe_to_mqtt()
