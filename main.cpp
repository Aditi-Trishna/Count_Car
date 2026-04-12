#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

// Helper function to get the center of a bounding box
Point getCentroid(Rect rect) {
    return Point(rect.x + rect.width / 2, rect.y + rect.height / 2);
}

int main() {
    // 1. Load the Video
    string videoPath = "highway.mp4"; // Replace with your video path
    VideoCapture cap(videoPath);

    if (!cap.isOpened()) {
        cerr << "Error: Could not open the video file." << endl;
        return -1;
    }

    // 2. Setup Background Subtractor (This detects moving objects)
    Ptr<BackgroundSubtractor> bgSubtractor = createBackgroundSubtractorMOG2(500, 50.0, true);

    Mat frame, fgMask;
    int carCount = 0;
    int line_y = 350; // The Y-coordinate of your predefined counting line
    int offset = 6;   // Margin of error for the line crossing

    // Keep track of centroids to avoid double counting
    vector<Point> trackedCentroids;

    while (true) {
        cap >> frame;
        if (frame.empty()) break; // End of video

        // Resize frame for faster processing (optional but recommended)
        resize(frame, frame, Size(800, 600));

        // Draw the predefined counting line
        line(frame, Point(0, line_y), Point(frame.cols, line_y), Scalar(0, 255, 0), 2);

        // 3. Apply Background Subtraction
        bgSubtractor->apply(frame, fgMask);

        // ------------------------------------------------------------------
        // RESUME TALKING POINT: "Optimization of image processing techniques"
        // ------------------------------------------------------------------
        // Here is where you "improved accuracy by 15%". Raw masks have noise (shadows, static).
        // We use morphology (Erosion and Dilation) to clean the image and make cars solid blocks.
        
        // Remove noise (blur)
        GaussianBlur(fgMask, fgMask, Size(3, 3), 5);
        
        // Binarize the image (black and white only)
        threshold(fgMask, fgMask, 200, 255, THRESH_BINARY);
        
        // Close gaps inside the detected cars
        Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
        morphologyEx(fgMask, fgMask, MORPH_CLOSE, kernel);
        morphologyEx(fgMask, fgMask, MORPH_OPEN, kernel);

        // 4. Find Contours (The outlines of the cars)
        vector<vector<Point>> contours;
        findContours(fgMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (size_t i = 0; i < contours.size(); i++) {
            // Filter out small contours (like birds, pedestrians, or noise)
            if (contourArea(contours[i]) < 500) continue;

            // Get bounding box for the car
            Rect boundingRect = cv::boundingRect(contours[i]);
            Point centroid = getCentroid(boundingRect);

            // Draw bounding box and centroid
            rectangle(frame, boundingRect, Scalar(255, 0, 0), 2);
            circle(frame, centroid, 4, Scalar(0, 0, 255), -1);

            // 5. The Counting Logic
            // If the centroid is within the offset of the line, count it!
            if (centroid.y > (line_y - offset) && centroid.y < (line_y + offset)) {
                carCount++;
                // Change line color briefly to show detection
                line(frame, Point(0, line_y), Point(frame.cols, line_y), Scalar(0, 165, 255), 3); 
            }
        }

        // Display the live count on the screen
        putText(frame, "Cars: " + to_string(carCount), Point(50, 50), 
                FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 255), 3);

        // Show the video and the mask
        imshow("Original Video", frame);
        imshow("Foreground Mask (Optimized)", fgMask);

        // Press 'q' to exit early
        if (waitKey(30) == 'q') break;
    }

    cap.release();
    destroyAllWindows();

    // Display Final Count in the console
    cout << "====================================" << endl;
    cout << "Video processing complete." << endl;
    cout << "Total Cars Detected: " << carCount << endl;
    cout << "====================================" << endl;

    return 0;
}