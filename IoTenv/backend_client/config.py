import os

MQTT_BROKER = os.getenv("MQTT_BROKER", "192.168.1.128")  # använd 'mosquitto' för nätverksnamn
MQTT_PORT = int(os.getenv("MQTT_PORT", 8884))
CA_CERT = os.getenv("CA_CERT_PATH", "/app/certs/myCA.crt")
CLIENT_CERT = os.getenv("CLIENT_CERT_PATH", "/app/certs/backend_client.crt")
CLIENT_KEY = os.getenv("CLIENT_KEY_PATH", "/app/certs/backend_client.key")

REQUEST_TOPIC = "request_measurement"
SENSOR_READING_TOPIC = "sensor_reading"
ADD_SENSOR_TOPIC = "add_sensor"

ADD_SENSOR_URL = "http://host.docker.internal:8080/sensor/add"