#Process_Image.py

### Processing Package ###

#This package will handle the core image processing tasks like edge detection
#contour finding, and curve fitting

##########################

import cv2
import numpy as np

#preform edge detection using canny
def detect_edges(image, low_threshold=50, high_threshold=150):
    return cv2.Canny(image, low_threshold, high_threshold)

#Find contours on edge detected image
def find_contours(image):
    countours, _ = cv2.findContours(image, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    return countours

#Draw cubic splines along contour points
def draw_curves(image, contour):
    if len(contour) < 5:
        return #Not enough points for meaniful curves
    
    if len(contour) >= 5:
        ellipse = cv2.fitEllipse(contour)
        cv2,ellipse(image, ellipse, (0, 255, 0), 2) #Draw ellipse in green

    return image
