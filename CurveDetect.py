import cv2
import numpy as np
import scipy.interpolate as spi

def draw_curve(image, points, color=(0, 255, 0)):
    # Fit a cubic spline curve to the points
    if len(points) < 3:
        return
    
    
    points = np.array(points, dtype=np.float32)
    x = points[:, 0]
    y = points[:, 1]
    
    # Parameter t for interpolation
    t = np.arange(len(points))
    
    # Create cubic splines for x and y
    spline_x = spi.CubicSpline(t, x)
    spline_y = spi.CubicSpline(t, y)
    
    # Create fine interpolation values (smoothing)
    t_fine = np.linspace(0, len(points)-1, 100)
    x_fine = spline_x(t_fine)
    y_fine = spline_y(t_fine)
    
    # Combine x and y for drawing the curve
    curve_points = np.vstack((x_fine, y_fine)).astype(np.int32).T
    
    # Draw the curve on the image
    cv2.polylines(image, [curve_points], isClosed=False, color=color, thickness=2)

# Initialize video capture
cap = cv2.VideoCapture(0)

while True:
    ret, img = cap.read()
    if not ret:
        break

    # Convert image to grayscale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    
    #apply gaussian blur
    blur = cv2.GaussianBlur(gray, (9, 9), 2)

    # Edge detection
    edges = cv2.Canny(blur, 50, 150)

    # Convery the grayscale image back to RGB
    edges_bgr = cv2.cvtColor(edges, cv2.COLOR_GRAY2BGR)
    
    # Detect lines using HoughLinesP
    lines = cv2.HoughLinesP(edges, 1, np.pi / 180, 100, minLineLength=50, maxLineGap=10)
    
    # Draw detected lines on the edge-detected image
    line_points =[]
    if lines is not None:
        for line in lines:
            x1, y1, x2, y2 = line[0]
            line_points.append([x1, y1])
            line_points.append([x2, y2])
            #cv2.line(edges, (x1, y1), (x2, y2), (0, 0, 255), 2)

    if len(line_points) > 2:
        # Sort line points to fit a smooth curve through them
        line_points = sorted(line_points, key=lambda x: x[0])
        
        #Convert edge detected image to BGR to overlay colored lines and curves
        #edges_bgr = cv2.cvtColor(edges, cv2.COLOR_GRAY2BGR)

        # Draw a curve passing through line points
        draw_curve(edges, line_points, color=(0, 255, 0))

        # Display the result
        cv2.imshow("Blended Lines and Curves", edges_bgr)
    

    # Break on pressing Enter
    if cv2.waitKey(1) == 13:
        break

cap.release()
cv2.destroyAllWindows()

#This is me changing the file
