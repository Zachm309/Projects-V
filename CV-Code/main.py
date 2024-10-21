#main.py

### Main Processing file ###

#This file takes the info from the other steps
#and displays the image

############################

import cv2
import time
from PreProcessing.pre_Process import convert_to_grayscale, apply_gaussian_blur
from ImageProcessing.Process_Image import detect_edges, analyze_image_segments, highlight_sections
from postProcessing.post_Process import display_image
from flask import Flask,render_template,Response
import threading




def main():
    
    
    cap = cv2.VideoCapture(0)
    
    if not cap.isOpened():
        print("Failed to access camera")
        return
    
    processing_delay= 0.05
    print_interval = 2.0
    
    
    while True:
        ret, img = cap.read()
        if not ret:
            print("Failed to grab frame")
            break

        # PRE-PROCESSING
        gray = convert_to_grayscale(img)
        blurred = apply_gaussian_blur(gray)

        #IMAGE PROCESSING
        edges = detect_edges(blurred)
        
        max_segment, max_count, counts = analyze_image_segments(edges)
        
        highlighted_frame = highlight_sections(edges.copy(), max_segment)
        
        display_image(highlighted_frame, "Highlighted Frame")


        if cv2.waitKey(1) == 13:
            break

    cap.release()
    cv2.destroyAllWindows()



if __name__ == "__main__":
    
    
    main()
