import cv2
from stitching import Stitcher
import os



# Video path
video_path = "/Users/krisko/Downloads/IMG_9184.MOV"
#folder for frames
output_folder = "frames"

# Check if the folder exists and remove all files in it
if os.path.exists(output_folder):
    for f in os.listdir(output_folder):
        file_path = os.path.join(output_folder, f)
        if os.path.isfile(file_path):
            os.remove(file_path)
else:
    # Create the folder if it doesn't exist
    os.makedirs(output_folder)

cap = cv2.VideoCapture(video_path)
fps = cap.get(cv2.CAP_PROP_FPS)
frame_interval = int(fps)
print(f"Exatracting every {frame_interval} frames")

total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
print(f"Total frames: {total_frames}")

frame_count = 0
saved_frame_count = 0
frame_files = []

#extract frames
for i in range(0, total_frames, frame_interval):
    cap.set(cv2.CAP_PROP_POS_FRAMES, i)
    
    ret, frame = cap.read()
    if not ret:
        break
    
    frame_name = f"{output_folder}/frame_{saved_frame_count:04d}.jpg"
    cv2.imwrite(frame_name, frame)
    frame_files.append(frame_name)  
    saved_frame_count += 1

cap.release()
print(f"Extracted {saved_frame_count} frames")

# Stitch images
if saved_frame_count > 1:
    images = [] 
    for f in frame_files:
        # resized = cv2.resize(cv2.imread(f), (800, 600))
        images.append(cv2.imread(f))
        
    
    # stitcher = cv2.Stitcher_create()  
    sticher = Stitcher(detector="brisk", confidence_threshold=0.5)
    '''
    For Speed and Simplicity: ORB is the best choice. It’s fast, open-source, and works well in most real-time scenarios.
	For High Accuracy and Robustness: SIFT is the best if accuracy is the top priority, and you can afford the slower performance.
	For a Balance Between Speed and Robustness: BRISK is a good middle ground, faster than SIFT but more robust than ORB.
	For Better Performance in Noisy or Distorted Images: AKAZE is a good choice, offering better robustness than ORB and often faster than SIFT.

    confidence_threshold: the confidence level when stiching images 
    '''
    pano = sticher.stitch(images)
    cv2.imwrite("panoramic_image.jpg", pano)
    # cv2.imshow("panoramic_image.jpg", pano)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()

else:
    print("Not enough frames extracted for stitching.")


