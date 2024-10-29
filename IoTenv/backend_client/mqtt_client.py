import paho.mqtt.client as mqtt
import ssl
import json
import requests
import config
import logging

logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

latest_measurement = None

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        logging.info(f"Successfully connected to broker")
        client.subscribe(config.SENSOR_READING_TOPIC)
        client.subscribe(config.ADD_SENSOR_TOPIC)
    else:
        logging.warning(f"Failed to connect, return code {rc}")

def validate_measurement(data):
    if "temperature" in data and "humidity" in data:
        return True
    return False

def on_message(client, userdata, msg):
    global latest_measurement
    data = json.loads(msg.payload.decode("utf-8"))

    if msg.topic == config.SENSOR_READING_TOPIC:
        if validate_measurement(data):
            latest_measurement = data
            logging.info(f"Updated latest_measurement in on_message: {latest_measurement}")
        else:
            logging.warning("Invalid data format received")

    elif msg.topic == config.ADD_SENSOR_TOPIC:
        payload = ({
            "macAddress": (data.get("macAddress")),
            "name": (data.get("name")),
        })
        response = requests.post(config.ADD_SENSOR_URL, json=payload, 
                                    cert=(config.CLIENT_CERT, config.CLIENT_KEY), verify=config.CA_CERT)
        logging.info("Sensor added!")

def create_mqtt_client():
    client = mqtt.Client()
    try:
        client.tls_set(ca_certs=config.CA_CERT, certfile=config.CLIENT_CERT, keyfile=config.CLIENT_KEY)
    except ssl.SSLError as e:
        logging.warning(f"SSL Error: {e}")
        exit(1)

    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(config.MQTT_BROKER, config.MQTT_PORT)
    client.loop_start()

    return client
