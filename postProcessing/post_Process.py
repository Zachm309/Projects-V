#postProcess.py

### Post Processing Package ###

#This package will handle the post process package like displaying the
#window, and eventually moving motors

###############################

import cv2

#Display image in window
def display_image(image, window_name="Image"):
    cv2.imshow(window_name,image)

#log information about detected lines or curved
def log_detection_info(contours):
    for contour in contours:
        if len(contour) >= 5:
            print(f"Curve Detected with {len(contour)} points")
        else:
            print(f"Straight line detected with {len(contour)} points")
