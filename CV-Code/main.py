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
from flask import Flask, render_template, Response

app = Flask(__name__)

cap = cv2.VideoCapture(0)

show_regular_feed = True

def generate_frames_edges():
    while True:
        
        success, img = cap.read()
        if not success:
            break
        
        gray = convert_to_grayscale(img)
        blurred = apply_gaussian_blur(gray)
        edges = detect_edges(blurred)
        max_segment, max_count, counts = analyze_image_segments(edges)
        highlighted_frame = highlight_sections(edges.copy(), max_segment)
        ret, buffer = cv2.imencode('.jpg', highlighted_frame)
            
        frame = buffer.tobytes()
        yield(b'--frame\r\n'
                b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')
                
def generate_frames_regular():
    while True:
        
        success, img = cap.read()
        if not success:
            break
        ret, buffer = cv2.imencode('.jpg', img)
            
        frame = buffer.tobytes()
        yield(b'--frame\r\n'
                b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')
        

@app.route('/')
def index():
    return render_template('index.html')
    
@app.route('/video')
def video():
    global show_regular_feed
    if show_regular_feed:
        return Response(generate_frames_edges(),mimetype='multipart/x-mixed-replace; boundary=frame')
    else:
        return Response(generate_frames_regular(),mimetype='multipart/x-mixed-replace; boundary=frame')
    
@app.route('/toggle')
def toggle():
    global show_regular_feed
    show_regular_feed = not show_regular_feed
    return "Toggle stream mode!", 200

def main():
    app.run(host='0.0.0.0', port=5000, debug=False)
    
def cleanup(exception):
    if cap.isOpened():
        cap.release


if __name__ == "__main__":
    main()
