import cv2
import numpy as np
from keras.models import load_model
from Adafruit_IO import Client, Feed, RequestError
import time
ADAFRUIT_IO_USERNAME = "HyperPalBuddy"
ADAFRUIT_IO_KEY = "aio_WUPk552u7PEwT02cpuLUS752zRCi"

model = load_model("mymodel.h5")
results = {0: 'without mask', 1: 'mask'}
GR_dict = {0: (0, 0, 255), 1: (0, 255, 0)}
rect_size = 4
cap = cv2.VideoCapture(1)
haarcascade = cv2.CascadeClassifier(
    'haarcascade_frontalface_default.xml')
while True:
    (rval, im) = cap.read()
    im = cv2.flip(im, 1, 1)

    rerect_size = cv2.resize(im, (im.shape[1] // rect_size, im.shape[0] // rect_size))
    faces = haarcascade.detectMultiScale(rerect_size)
    for f in faces:
        (x, y, w, h) = [v * rect_size for v in f]

        face_img = im[y:y + h, x:x + w]
        rerect_sized = cv2.resize(face_img, (150, 150))
        normalized = rerect_sized / 255.0
        reshaped = np.reshape(normalized, (1, 150, 150, 3))
        reshaped = np.vstack([reshaped])
        result = model.predict(reshaped)

        label = np.argmax(result, axis=1)[0]

        cv2.rectangle(im, (x, y), (x + w, y + h), GR_dict[label], 2)
        cv2.rectangle(im, (x, y - 40), (x + w, y), GR_dict[label], -1)
        cv2.putText(im, results[label], (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 255, 255), 2)
        """
        Changes Enter below
        """
        if results[label]=='mask':
            aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
            digital = aio.feeds('smartlock')
            aio.send(digital.key, "ON")
            time.sleep(10)
            aio.send(digital.key, "OFF")
            


    cv2.imshow('LIVE', im)
    key = cv2.waitKey(10)

    if key == 27:
        break
cap.release()
cv2.destroyAllWindows()