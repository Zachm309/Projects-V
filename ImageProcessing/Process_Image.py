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
def count_white_pixels(segment):
    # White pixels in a binary image (Canny output) are represented by 255
    white_pixel_count = np.sum(segment == 255)
    return white_pixel_count


# Split image into regions and count white pixels in each
def analyze_image_segments(image):
    height, width = image.shape
    left_segment = image[:, :width // 2]  # Left half
    right_segment = image[:, width // 2:]  # Right half
    top_segment = image[:height // 2, :]  # Top half

    # Count the number of white pixels (edges) in each segment
    left_count = cv2.countNonZero(left_segment)
    right_count = cv2.countNonZero(right_segment)
    top_count = cv2.countNonZero(top_segment)

    # Determine which segment has the most white pixels
    max_count = max(left_count, right_count, top_count)
    
    if max_count == left_count:
        direction = "Left"
    elif max_count == right_count:
        direction = "Right"
    else:
        direction = "Straight"

    # Print the result to the terminal
    print(f"Direction: {direction} | Left: {left_count}, Right: {right_count}, Top: {top_count}")

    return direction, max_count