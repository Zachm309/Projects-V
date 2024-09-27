#main.py

### Main Processing file ###

#This file takes the info from the other steps
#and displays the image

############################

import cv2
import time
from PreProcessing.pre_Process import convert_to_grayscale, apply_gaussian_blur
from ImageProcessing.Process_Image import detect_edges, analyze_image_segments, count_white_pixels
from postProcessing.post_Process import display_image


def main():
    cap = cv2.VideoCapture(0)
    last_print_time = time.time()

    while True:
        ret, img = cap.read()
        if not ret:
            break

        # PRE-PROCESSING
        gray = convert_to_grayscale(img)
        blurred = apply_gaussian_blur(gray)

        #IMAGE PROCESSING
        edges = detect_edges(blurred)

        if time.time() - last_print_time >= 1:
            analyze_image_segments(edges)
            last_print_time = time.time()

        #POST-PROCESSING
        output_image = cv2.cvtColor(edges, cv2.COLOR_GRAY2BGR)
    
        display_image(output_image, "Edge Detection with curved")

        if cv2.waitKey(1) == 13:
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()