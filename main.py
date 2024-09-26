#main.py

### Main Processing file ###

#This file takes the info from the other steps
#and displays the image

############################

import cv2

from PreProcessing.pre_process import convert_to_grayscale, apply_gaussian_blur
from ImageProcessing.Process_Image import detect_edges, find_contours, draw_curves
from PostProcessing.PreProcessing import display_image, log_detection_info

def main():
    cap = cv2.VideoCapture(0)

    while True:
        ret, img = cap.read()
        if not ret:
            break

        # PRE-PROCESSING
        gray = convert_to_grayscale(img)
        blurred = apply_gaussian_blur(gray)

        #IMAGE PROCESSING
        edges = detect_edges(blurred)
        contours = find_contours(edges)

        #POST-PROCESSING
        output_image = cv2.cvtColor(edges, cv2.COLOR_GRAY2BGR)
        for contour in contours:
            output_image = draw_curves(output_image, contour)
    
        display_image(output_image, "Edge Detection with curved")
        log_detection_info(contours)

        if cv2.waitkey(1) == 13:
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()