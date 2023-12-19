import numpy as np
import cv2 as cv

image = cv.imread('Vision/ball.jpg')
image = cv.resize(image, (0,0), fx=0.25, fy=0.25) 
image = cv.flip(image, 1)

# simple color detection for detecting and localizing ball
# returns none if no detection
# returns a normalized center pointer if detection
def detect_ball(image):
    height, width = image.shape[:2]
    image = cv.cvtColor(image, cv.COLOR_BGR2HSV)

    lower = np.array([22, 93, 0], dtype="uint8")
    upper = np.array([45, 255, 255], dtype="uint8")
    mask = cv.inRange(image, lower, upper)

    # discard heirarchy
    cnts, _ = cv.findContours(mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)

    if(len(cnts) != 0):
        cnt = max(cnts, key = cv.contourArea)
        # threshold contour to avoid tiny detections
        if(cv.contourArea(cnt) > 10000):
            x,y,w,h = cv.boundingRect(cnt)

            # get center point, normalized to [-1, 1] for x and y, using cartesian style sign
            offset_x = (2.0*x + w - width)/(width)
            offset_y = (height - 2.0*y - h)/(height)

            return (offset_x, offset_y)

    else:
        return None
    
