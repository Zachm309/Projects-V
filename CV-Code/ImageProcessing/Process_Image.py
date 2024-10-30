#Process_Image.py

### Processing Package ###

#This package will handle the core image processing tasks like edge detection
#contour finding, and curve fitting

##########################

import cv2
import numpy as np
import time
import sys
sys.stdout.flush()



#preform edge detection using canny
def detect_edges(image, low_threshold=245, high_threshold=150):
    return cv2.Canny(image, low_threshold, high_threshold)

#Find contours on edge detected image
def count_white_pixels(segment):
    # White pixels in a binary image (Canny output) are represented by 255
    white_pixel_count = np.sum(segment == 255)
    return white_pixel_count


# Split image into regions and count white pixels in each
def analyze_image_segments(image):
    height, width = image.shape
    
    
    segments = {
        "Left": image[:, :width // 2],  # Left half
        "Right": image[:, width // 2:],  # Right half
        "Top": image[:height // 2, :]  # Top half
    }
    # Count the number of white pixels (edges) in each segment
    white_pixel_counts = {seg: cv2.countNonZero(segments[seg]) for seg in segments} 

    # Determine which segment has the most white pixels
    max_segment = max(white_pixel_counts, key=white_pixel_counts.get)
    max_count = white_pixel_counts[max_segment]
    
  
  
    # Print the result to the terminal
    #print(f"Direction: {max_segment}")

    return max_segment, max_count, white_pixel_counts
    
def highlight_sections(image, max_segment):
    height, width, = image.shape
    highlighted_image = cv2.cvtColor(image, cv2.COLOR_GRAY2BGR)
    
    
    color = (0, 255, 0)
    thickness = 2
    
    #Define the coordinates for each segment
    if max_segment == "Left":
        top_left = (0, 0)
        bottom_right = (width // 2, height)
    elif max_segment == "Right":
        top_left = (width // 2, 0,)
        bottom_right = (width, height)
    else:
        top_left = (0, 0)
        bottom_right = (width, height // 2)
    
    time.sleep(0.5)
    print("Direction:", max_segment, flush=True)
    
    cv2.rectangle(highlighted_image, top_left, bottom_right, color, thickness)
    
    return highlighted_image




