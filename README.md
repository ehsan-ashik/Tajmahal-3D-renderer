# Taj Mahal 3D Rendering with OpenGL

This Windows application renders a 3D model of the Taj Mahal, the awe-inspiring architectural monument built by Shah Jahan for his beloved wife Mumtaz. Learn more about the Taj Mahal [here](https://en.wikipedia.org/wiki/Taj_Mahal).

**Rendered Images**: Explore the `rendered_images` folder to see examples of the rendered Taj Mahal model. Example:
![Rendered image of the model from the app](https://github.com/ehsan-ashik/Tajmahal-3D-renderer/blob/main/rendered%20images/white-light-1.PNG)

## Features

* Generates a textured 3D model of the Taj Mahal.
* Applies ambient lighting with optional white and blue point lights.
* Rotates light sources 360 degrees, creating dynamic lighting and shadows.
* Offers user control over camera movement for exploration of the 3D space.

## Keyboard Controls

* **Up Arrow:** Zoom In
* **Down Arrow:** Zoom Out
* **1:** Camera Pitch Up
* **2:** Camera Pitch Down
* **3:** Camera Yaw Right
* **4:** Camera Yaw Left
* **5:** Camera Roll Up
* **6:** Camera Roll Down
* **7:** Toggle White Point Light
* **8:** Toggle Blue Point Light
* **9:** Start Light Source Rotation
* **0:** Stop Light Source Rotation

## Dependencies

* **GNU Compiler Collection (C, C++, OpenMP):** Install using MSYS2 package `mingw-w64-x86_64-gcc`
* **OpenGL Glut library:** Install using MSYS2 package `mingw-w64-x86_64-freeglut`

## Building and Running

**1. Install MSYS2:**

Download and install MSYS2 from [here](https://www.msys2.org/).

**2. Install Dependencies:**

Use MSYS2 to install the required packages: `pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-freeglut`

**3. Compile and Run:**

Open a terminal in the project directory and run these commands:

**Compile:**
`g++ -Wall -o main main.cpp -lfreeglut -lglu32 -lopengl32 -lm`

**Run:**
`./main`  # or  `main.exe` (Windows)
