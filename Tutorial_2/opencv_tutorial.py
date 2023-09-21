from inspect import CO_VARARGS
import PyQt5
import os
from pathlib import Path
from PyQt5.QtWidgets import QWidget # others imports

import matplotlib.pyplot as plt
import cv2 as cv

# PyQt with OpenCV fix
# Source: https://stackoverflow.com/questions/68417682/qt-and-opencv-app-not-working-in-virtual-environment
os.environ["QT_QPA_PLATFORM_PLUGIN_PATH"] = os.fspath(
    Path(PyQt5.__file__).resolve().parent / "Qt5" / "plugins"
)

# part 1
img = cv.imread('mateo.jpg')
img = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
plt.figure()
plt.imshow(img, cmap='gray', vmin=0, vmax=255)
plt.show(block=False)

# part 2
edges = cv.Canny(img,100,200)
plt.figure()
plt.imshow(edges, cmap='gray', vmin=0, vmax=255)
plt.show(block=False) 

# part 3
face_detector=cv.CascadeClassifier(cv.data.haarcascades + 'haarcascade_frontalface_default.xml')
results = face_detector.detectMultiScale(img, scaleFactor=1.01,minNeighbors=5,minSize=(32, 32), flags=cv.CASCADE_SCALE_IMAGE)
faces = img.copy()
for (x,y,w,h) in results:
    cv.rectangle(faces,(x,y),(x+w,y+h),(0,255,0),2)
  
plt.figure()
plt.imshow(faces, cmap='gray', vmin=0, vmax=255)
plt.show(block=False)


# try another image?
img2 = cv.imread('mckenna.jpg')
img2 = cv.cvtColor(img2, cv.COLOR_BGR2GRAY)
faces2 = img2.copy()
results = face_detector.detectMultiScale(faces2, scaleFactor=1.5,minNeighbors=5,minSize=(32, 32), flags=cv.CASCADE_SCALE_IMAGE)
for (x,y,w,h) in results:
    cv.rectangle(faces2,(x,y),(x+w,y+h),(0,255,0),2)
  
plt.figure()
plt.imshow(faces2, cmap='gray', vmin=0, vmax=255)
plt.show(block=True) # last needs to block so the program doesn't immediately exit

# Note: interesting that McKenna's face was much easier to detect than Mateo's? Maybe it is the background?
#   Maybe the helmet Mateo is wearing?
#   Maybe it is an algorithmic bias and the trained haar-cascade struggles with dark skin?