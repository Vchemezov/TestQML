
# TestQML Build Instructions

This document provides step-by-step instructions for setting up the development environment and building the project on **Windows** and **Linux**.

---

## Prerequisites

### 1. **Python**
   - **Windows**:
     1. Download the latest version of Python from [python.org](https://www.python.org/downloads/windows/).
     2. During installation, ensure you check the box to **Add Python to PATH**.
     3. Verify the installation by running:
        ```bash
        python --version
        ```
   - **Linux**:
     Python is usually pre-installed. If not, install it using your package manager:
     ```bash
     sudo apt update
     sudo apt install python3
     ```

### 2. **CMake**
   - **Windows**:
     1. Download the CMake 3.15 or later from [cmake.org](https://cmake.org/download/).
     2. Install CMake and ensure you select the option to **Add CMake to the system PATH**.
     3. Verify the installation by running:
        ```bash
        cmake --version
        ```
   - **Linux**:
     Install CMake using your package manager:
     ```bash
     sudo apt update
     sudo apt install cmake
     ```

### 3. **Qt5 (Optional, if your project uses Qt)**
   - **Windows**:
     1. Download the Qt installer from [qt.io](https://www.qt.io/download).
     2. Run the installer and select the components you need (e.g., Qt 5.x.x, Qt Creator, etc.).
     3. Ensure you add Qt's `bin` directory to your system `PATH`.
   - **Linux**:
     Install Qt5 using your package manager:
     ```bash
     sudo apt update
     sudo apt install qt5-default
     ```

### 4. **NSIS (Windows Only, for creating installers)**
   - Download NSIS from [nsis.sourceforge.io](https://nsis.sourceforge.io/Download).
   - Install NSIS and ensure it is added to your system `PATH`.
   - Verify the installation by running:
     ```bash
     makensis --version
     ```

---

## Building the Project

### 1. Clone the Repository
   ```bash
   git clone https://github.com/vchemezov/TestQML.git
   cd TestQML
   ```
   
### 2. Build the Project
   - Run the provided Python script to build the project and create the deployment package:
     ```bash
     python build/build.py
     ```

   - The script will:
     1. Configure and build the project using CMake.
     2. Install the built files to the `install` directory.
     3. Create a deployment package:
        - **Windows**: An NSIS installer in the `deploy` directory.
        - **Linux**: A `.tar.gz` archive in the `deploy` directory.
        
## Deployment Packages

### Windows
   - The NSIS installer (`TestQML_Installer.exe`) will be created in the `deploy` directory.
   - Run the installer to install the application on your system.

### Linux
   - The `.tar.gz` archive (`TestQML.tar.gz`) will be created in the `deploy` directory.
   - Extract the archive and run the application:
     ```bash
     tar -xzf TestQML.tar.gz
     cd TestQML
     ./TestQML
     ```

---

## Troubleshooting

### CMake Errors
   - Ensure CMake is installed and added to your `PATH`.
   - If you encounter missing dependencies, install them using your package manager or download them manually.

### NSIS Errors (Windows)
   - Ensure NSIS is installed and added to your `PATH`.
   - Verify that the `installer.nsi` script exists in your project directory.

### Python Errors
   - Ensure Python is installed and added to your `PATH`.
   - Verify the Python version using `python --version`.

---

## Contact
For questions or support, please contact:
- **Chemezov Vitaliy**: vpointc90@gmail.com
- **Project Repository**: [GitHub](https://github.com/vchemezov/TestQML)
