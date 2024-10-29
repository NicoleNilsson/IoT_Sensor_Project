from flask import Flask, jsonify
import time
import mqtt_client
from config import REQUEST_TOPIC
import logging

logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

app = Flask(__name__)
client = mqtt_client.create_mqtt_client()

@app.route('/request_measurement/<mac_address>', methods=['GET'])
def request_measurement(mac_address):
    mqtt_client.latest_measurement = None
    logging.info(f"Published request for MAC address: {mac_address}")

    client.publish(REQUEST_TOPIC, mac_address)

    start_time = time.time()
    timeout = start_time + 10

    while mqtt_client.latest_measurement is None and time.time() < timeout:
        time.sleep(0.1)

    if mqtt_client.latest_measurement is None:
        logging.warning("Timeout waiting for measurement.")
        return jsonify({"error": "Timeout waiting for measurement"}), 504

    logging.info(f"Returning measurement: {mqtt_client.latest_measurement}")
    return jsonify(mqtt_client.latest_measurement)
