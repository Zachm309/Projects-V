#preprocess.py


### Processing Package ###

#This package will handle the core image processing tasks like edge detection
#contour finding, and curve fitting

#Does operation such as:
# -Grayscale converison
# -Noise Reduction(I.e gaussian blur)

##########################
import cv2

def convert_to_grayscale(image):
    #convert image to grayscale
    return cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

def apply_gaussian_blur(image, kernal_size=(9, 9), sigma=2):
    #Apply gaussian blur to reduce noise
    return cv2.GaussianBlur(image, kernal_size, sigma)
