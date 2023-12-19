import numpy as np
import cv2 as cv

image = cv.imread('ball.jpg')
image = cv.resize(image, (0,0), fx=0.25, fy=0.25) 
height, width = image.shape[:2]


original = image.copy()

image = cv.cvtColor(image, cv.COLOR_BGR2HSV)
lower = np.array([22, 93, 0], dtype="uint8")
upper = np.array([45, 255, 255], dtype="uint8")
mask = cv.inRange(image, lower, upper)

cnts, _ = cv.findContours(mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)

if(len(cnts) != 0):
    cnt = max(cnts, key = cv.contourArea)
    
    if(cv.contourArea(cnt) > 10000):
        x,y,w,h = cv.boundingRect(cnt)
        cv.rectangle(original, (x, y), (x + w, y + h), (36,255,12), 2)
        
        center_x = (x+w/2)
        center_y = (y+h/2)

        # get center point, normalized to [-1, 1] for x and y, using cartesian style sign
        offset_x = (2.0*x + w - width)/(width)
        offset_y = (height - 2.0*y - h)/(height)
        print(offset_x)
        print(offset_y)

        cv.circle(original, (int(center_x), int(center_y)), radius=10, color = (12, 36, 255), thickness=-1)



cv.imshow('original', original)
cv.waitKey()