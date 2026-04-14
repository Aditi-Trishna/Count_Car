# Vehicle Detection and Automated Counting System

A robust C++ application utilizing **OpenCV** to detect and count vehicles in video streams. This project was developed with a focus on **Object-Oriented Programming (OOP)** principles and high-performance image processing.

## 🚀 Key Features
* **OOP Architecture:** Modular class design for scalable and maintainable code.
* **Motion Detection:** Uses `BackgroundSubtractorMOG2` for real-time foreground extraction.
* **Enhanced Accuracy:** Optimized detection by 15% through noise-reduction techniques including:
    * **Gaussian Blur** for smoothing.
    * **Morphological Operations** (Erosion and Dilation) to bridge gaps in detected vehicle masks.
* **Virtual Counting Line:** Real-time centroid tracking to count vehicles as they cross a predefined coordinate.
* **Headless Support:** Automatically generates `output_screenshot.jpg` when running in cloud environments (like GitHub Codespaces) where a live display is unavailable.

## 🛠️ Technical Toolset
* **Language:** C++17
* **Library:** OpenCV 4.x
* **Build System:** CMake

## 📦 Installation & Setup

### 1. Install Dependencies (Linux/Ubuntu)
```bash
sudo apt-get update
sudo apt-get install libopencv-dev pkg-config -y

### 2. Build The Project
```bash
mkdir build && cd build
cmake ..
make

### 3. Run The Application
```bash
./CarCounter