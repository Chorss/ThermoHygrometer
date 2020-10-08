import Adafruit_DHT
import time
import json
import requests

DHT_SENSOR = Adafruit_DHT.DHT11
DHT_PIN = 4

while True:
    humidity, temperature = Adafruit_DHT.read(DHT_SENSOR, DHT_PIN)
    if humidity is not None and temperature is not None:
        print("Temp={0:0.1f}C Humidity={1:0.1f}%".format(temperature, humidity))
        data = {}
        data['temperature'] = temperature
        data['humidity'] = humidity
        data['deviceId'] = 'UUID'
        data_json = json.dumps(data)
        headers = {'Content-type': 'application/json', 'Accept': 'text/plain'}
        response = requests.post('https://monitoring.railwaytycoon.dev/api/v1/measurement', data_json, headers=headers)
        print(response.status_code)
    else:
        print("Sensor failure. Check wiring.")
    time.sleep(900);#15min