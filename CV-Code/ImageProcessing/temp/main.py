from flask import Flask, render_template,Response
import cv2
cap = cv2.VideoCapture(0)
# Create an instance of the Flask class

app = Flask(__name__)

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

# Define the route for the root URL
@app.route('/')
def main():
    return render_template('index.html')  # Render the HTML template
    
@app.route('/video')
def video():
    return Response(generate_frames(),mimetype='multipart/x-mixed-replace: boundry=frame')

# Run the Flask application
if __name__ == '__main__':
    app.run(debug=False)  # Run in debug mode to allow live reloading
