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
from flask import Flask,render_template,Response
from werkzeug.serving import run_simple
import threading



app=Flask(__name__)
cap = cv2.VideoCapture(0)

def generate_frames():
    while True:

        ## read the camera frame
        success,frame=cap.read()
        if not success:
            break
        else:
            ret,buffer=cv2.imencode('.jpg',frame)
            frame=buffer.tobytes()
        
        yield(b'--frame\r\n'
                    b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')


@app.route('/')
def index():
    return render_template('index.html')

@app.route('/video')
def video():
    return Response(generate_frames(),mimetype='multipart/x-mixed-replace: boundry=frame')

def main():
    
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

def run_flask():
    app.run(ssl_context=('cert.pem', 'key.pem'), debug=True, use_reloader=False)


if __name__ == "__main__":
    
    flask_thread = threading.Thread(target=run_flask)
    flask_thread.start()
    
    
    main()