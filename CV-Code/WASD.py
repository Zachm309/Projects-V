import cv2
import mediapipe as mp
import numpy as np

# Initialize MediaPipe and OpenCV
mp_drawing = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands
hands = mp_hands.Hands()

# Initialize camera feed
cap = cv2.VideoCapture(0)

# Define gestures by checking the position of landmarks
def detect_gesture(landmarks, width, height):
    # Get landmark coordinates (normalized coordinates scaled to image size)
    thumb_tip = np.array([landmarks.landmark[mp_hands.HandLandmark.THUMB_TIP].x * width,
                          landmarks.landmark[mp_hands.HandLandmark.THUMB_TIP].y * height])
    index_tip = np.array([landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP].x * width,
                          landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP].y * height])
    middle_tip = np.array([landmarks.landmark[mp_hands.HandLandmark.MIDDLE_FINGER_TIP].x * width,
                           landmarks.landmark[mp_hands.HandLandmark.MIDDLE_FINGER_TIP].y * height])
    ring_tip = np.array([landmarks.landmark[mp_hands.HandLandmark.RING_FINGER_TIP].x * width,
                         landmarks.landmark[mp_hands.HandLandmark.RING_FINGER_TIP].y * height])
    pinky_tip = np.array([landmarks.landmark[mp_hands.HandLandmark.PINKY_TIP].x * width,
                          landmarks.landmark[mp_hands.HandLandmark.PINKY_TIP].y * height])

    # Gesture logic for WASD
    if thumb_tip[1] < index_tip[1] < middle_tip[1]:  # Gesture for "W" (thumb raised above others)
        return 'W'
    elif index_tip[1] < middle_tip[1] and middle_tip[1] < ring_tip[1]:  # Gesture for "A" (index finger extended)
        return 'A'
    elif middle_tip[1] < index_tip[1] and ring_tip[1] < pinky_tip[1]:  # Gesture for "S" (middle finger extended)
        return 'S'
    elif (np.linalg.norm(thumb_tip - index_tip) < 30 and  # Closed fist: all fingertips close to each other
          np.linalg.norm(index_tip - middle_tip) < 30 and
          np.linalg.norm(middle_tip - ring_tip) < 30 and
          np.linalg.norm(ring_tip - pinky_tip) < 30):
        return 'D'
    return None

# Main loop
while True:
    ret, frame = cap.read()
    if not ret:
        break
    
    # Flip and convert the frame from BGR to RGB for MediaPipe
    frame_rgb = cv2.cvtColor(cv2.flip(frame, 1), cv2.COLOR_BGR2RGB)
    height, width, _ = frame.shape  # Get dimensions of the frame

    # Process the frame to detect hand landmarks
    results = hands.process(frame_rgb)

    # Convert the image back from RGB to BGR for displaying with OpenCV
    frame_bgr = cv2.cvtColor(frame_rgb, cv2.COLOR_RGB2BGR)

    # If hand landmarks are detected, attempt to detect gestures
    if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
            # Draw the hand landmarks on the frame
            mp_drawing.draw_landmarks(frame_bgr, hand_landmarks, mp_hands.HAND_CONNECTIONS)

            # Detect gesture
            gesture = detect_gesture(hand_landmarks, width, height)
            if gesture:
                print(gesture)  # Print the detected gesture to the terminal

    # Display the frame
    cv2.imshow('Hand Tracker for WASD Gestures', frame_bgr)

    # Press 'q' to exit the loop
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release resources
cap.release()
cv2.destroyAllWindows()
