from inspect import CO_VARARGS
import PyQt5
import os
from pathlib import Path
from PyQt5.QtWidgets import QWidget # others imports

import matplotlib.pyplot as plt
import matplotlib.patches as patches

import cv2 as cv

from pupil_apriltags import Detector

os.environ["QT_QPA_PLATFORM_PLUGIN_PATH"] = os.fspath(
    Path(PyQt5.__file__).resolve().parent / "Qt5" / "plugins"
)

# Cannot figure out how to acces my webcam in WSL, taking a picture in windows and then moving it over

# cv.namedWindow("preview")
# vc = cv.VideoCapture(0)

# if vc.isOpened(): # try to get the first frame
#     rval, frame = vc.read()
# else:
#     rval = False

# while rval:
#     cv.imshow("preview", frame)
#     rval, frame = vc.read()
#     key = cv2.waitKey(20)
#     if key == 27: # exit on ESC
#         break

# vc.release()
# cv.destroyWindow("preview")

# PyQt with OpenCV fix
# Source: https://stackoverflow.com/questions/68417682/qt-and-opencv-app-not-working-in-virtual-environment

img = cv.imread('WIN_20231027_23_03_04_Pro.jpg', cv.IMREAD_GRAYSCALE)
print(type(img[0][0]))
detector = Detector()
result = detector.detect(img)
print(result)
fig, ax = plt.subplots()
ax.imshow(img, cmap='gray')
rect = patches.Polygon(result[0].corners, closed=True, linewidth=1, edgecolor='r', facecolor='none')
ax.add_patch(rect)
plt.show()