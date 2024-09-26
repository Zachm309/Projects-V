import cv2
cap = cv2.VideoCapture(0)

while True:
    ret, img = cap.read()

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray, (103, 103), 0)
    canny = cv2.Canny(blur, 10, 70)
    ret, mask = cv2.threshold(canny, 150, 150, cv2.THRESH_BINARY)

    cv2.imshow('Video Feed', mask)
    if cv2.waitKey(1) == 13:
        break

cap.release()
cv2.destroyAllWindows()