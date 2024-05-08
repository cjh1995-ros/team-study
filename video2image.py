import cv2
import os
import argparse

def main():
    # Argument parsing setup
    parser = argparse.ArgumentParser(description='Convert video to images.')
    parser.add_argument('--input-video', type=str, required=True, help='Path to the input video file.')
    parser.add_argument('--output-path', type=str, required=True, help='Path to output the images.')
    parser.add_argument('--rescale', type=float, default=1.0, help='Factor to rescale the output images (1.0 for no rescale).')
    
    args = parser.parse_args()

    # Check if the output directory exists, and create if it does not
    if not os.path.exists(args.output_path):
        os.makedirs(args.output_path)
        print(f"Created directory {args.output_path}")

    # Open the video file
    cap = cv2.VideoCapture(args.input_video)
    if not cap.isOpened():
        print("Error: Could not open video.")
        return

    frame_count = 0
    ret, frame = cap.read()

    print(f"Frame shape: {frame.shape}")

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        # Rescale the frame if necessary
        if args.rescale != 1.0:
            width = int(frame.shape[1] * args.rescale)
            height = int(frame.shape[0] * args.rescale)
            frame = cv2.resize(frame, (width, height), interpolation=cv2.INTER_AREA)

        # Save the frame as an image file
        output_filename = os.path.join(args.output_path, f"frame_{frame_count:04d}.jpg")
        cv2.imwrite(output_filename, frame)
        frame_count += 1

    cap.release()
    print("Done extracting frames.")

if __name__ == "__main__":
    main()
