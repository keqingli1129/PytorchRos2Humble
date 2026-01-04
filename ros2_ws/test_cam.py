import cv2

# Try to open device 2
cap = cv2.VideoCapture(2) 

if not cap.isOpened():
    print("ERROR: Could not open /dev/video2")
    # Check if backend issue
    print("Backend:", cap.getBackendName())
else:
    print("SUCCESS: /dev/video2 is open!")
    ret, frame = cap.read()
    if ret:
        print(f"Read frame: {frame.shape}")
    else:
        print("Opened, but failed to read frame.")
    cap.release()