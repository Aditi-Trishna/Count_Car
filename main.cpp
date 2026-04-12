#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

class CarCounter {
private:
    string videoPath;
    VideoCapture cap;
    Ptr<BackgroundSubtractor> bgSubtractor;
    
    int carCount;
    int line_y;
    int offset;
    int minContArea;

    //helper method
    Point getCentroid(Rect rect) {
        return Point(rect.x + rect.width / 2, rect.y + rect.height / 2);
    }

public:
    CarCounter(string path, int linePosition = 350, int errOffset = 6, int minArea = 500) 
        : videoPath(path), line_y(linePosition), offset(errOffset), minContArea(minArea), carCount(0) {
        
        // Initialize Background Subtractor here
        bgSubtractor = createBackgroundSubtractorMOG2(500, 50.0, true);
    }

    //Main processing method
    bool runProcessing() {
        cap.open(videoPath);
        
        if (!cap.isOpened()) {
            cerr << "Error: Could not open the video file." << endl;
            return false;
        }

        Mat frame, fgMask;

        while (true) {
            cap >> frame;
            if (frame.empty()) break; // End of video

            // Resize frame for faster processing
            resize(frame, frame, Size(800, 600));

            // Draw the predefined counting line
            line(frame, Point(0, line_y), Point(frame.cols, line_y), Scalar(0, 255, 0), 2);

            // Apply Background Subtraction
            bgSubtractor->apply(frame, fgMask);

            // Optimization & Noise Clean up
            GaussianBlur(fgMask, fgMask, Size(3, 3), 5);
            threshold(fgMask, fgMask, 200, 255, THRESH_BINARY);
            
            Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
            morphologyEx(fgMask, fgMask, MORPH_CLOSE, kernel);
            morphologyEx(fgMask, fgMask, MORPH_OPEN, kernel);

            // Find Contours
            vector<vector<Point>> contours;
            findContours(fgMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

            for (size_t i = 0; i < contours.size(); i++) {
                // Filter out small contours
                if (contourArea(contours[i]) < minContArea) continue;

                Rect boundingRect = cv::boundingRect(contours[i]);
                Point centroid = getCentroid(boundingRect);

                // Draw bounding box and centroid
                rectangle(frame, boundingRect, Scalar(255, 0, 0), 2);
                circle(frame, centroid, 4, Scalar(0, 0, 255), -1);

                // The Counting Logic
                if (centroid.y > (line_y - offset) && centroid.y < (line_y + offset)) {
                    carCount++;
                    line(frame, Point(0, line_y), Point(frame.cols, line_y), Scalar(0, 165, 255), 3); 
                }
            }

            // Display the live count on the screen
            putText(frame, "Cars: " + to_string(carCount), Point(50, 50), 
                    FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 255), 3);
            imshow("Original Video", frame);
            imshow("Foreground Mask (Optimized)", fgMask);

            // Press 'q' to exit early
            if (waitKey(30) == 'q') break;
        }

        cap.release();
        destroyAllWindows();
        cout << "Video processing complete." << endl;
        cout << "Total Cars Detected: " << carCount << endl;

        return true;
    }

    //Getter method to retrieve the count securely
    int getFinalCount() const {
        return carCount;
    }
};

int main() {
    // Instantiate the CarCounter object
    CarCounter highwayCounter("highway.mp4", 450, 25, 150);

    // Execute the counting process
    highwayCounter.runProcessing();

    return 0;
}