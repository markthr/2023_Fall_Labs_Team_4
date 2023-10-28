import cv2
import matplotlib.pyplot as plt


# Part 1 (couldn't get relative path to work)
img = cv2.imread(r"C:\Users\alien\Documents\CollegeStuff\2023_Fall_Labs_Team_4\Tutorial_2\Drip_Goku.jpg", cv2.IMREAD_GRAYSCALE)
cv2.imshow('image', img)
cv2.waitKey(0)

# Part 2
#img_blur = cv2.GaussianBlur(img_gray, (3,3), 0) 
edges = cv2.Canny(image=img, threshold1=100, threshold2=200)
cv2.imshow('Canny Edge Detection', edges)
cv2.waitKey(0)

# Part 3
img2 = cv2.imread(r"C:\Users\alien\Documents\CollegeStuff\2023_Fall_Labs_Team_4\Tutorial_2\Desean_Strickland.jpeg", cv2.IMREAD_GRAYSCALE)
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
cv2.imshow('image', img2)
cv2.waitKey(0)
faces = face_cascade.detectMultiScale(img2)
for (x, y, w, h) in faces:
    cv2.rectangle(img2, (x, y), (x + w, y + h), (0, 255, 0), 4)
img_rgb = cv2.cvtColor(img2, cv2.COLOR_BGR2RGB)
cv2.imshow('image',img_rgb)
cv2.waitKey(0)





cv2.destroyAllWindows()