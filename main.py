from Adafruit_IO import Client, Feed, RequestError
import time


ADAFRUIT_IO_USERNAME = "HyperPalBuddy"
ADAFRUIT_IO_KEY = "aio_WUPk552u7PEwT02cpuLUS752zRCi"



aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

try:
    digital = aio.feeds('smartlock')
except RequestError:
    print('Feed Not Found')
while True:
    aio.send(digital.key, "ON")
    time.sleep(1)
    aio.send(digital.key, "OFF")
