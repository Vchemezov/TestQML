import os
import sys
import subprocess
import shutil
import platform
from pathlib import Path

# Configuration
PROJECT_NAME = "TestQML"
BUILD_DIR = "cmake_build"
INSTALL_DIR = "TestQML_install"
DEPLOY_DIR = "TestQML_deploy"
NSIS_SCRIPT_COPY = "build/installer/TestQML.nsi"  # NSIS script for Windows installer
NSIS_SCRIPT = "TestQML.nsi"  # NSIS script for Windows installer

def run_command(command, cwd=None):
    """Run a shell command."""
    print(f"Running: {' '.join(command)}")
    result = subprocess.run(command, cwd=cwd)
    if result.returncode != 0:
        print(f"Command failed with return code {result.returncode}")
        sys.exit(1)

def build_project():
    """Build the project using CMake."""
    if not os.path.exists(BUILD_DIR):
        os.makedirs(BUILD_DIR)
    
    # Configure and build the project
    run_command(["cmake", "-S", ".", "-B", BUILD_DIR])
    run_command(["cmake", "--build", BUILD_DIR, "--config", "Release"])

def install_project():
    """Install the project to the INSTALL_DIR."""
    if not os.path.exists(INSTALL_DIR):
        os.makedirs(INSTALL_DIR)
    
    # Install the project
    run_command(["cmake", "--install", BUILD_DIR, "--prefix", INSTALL_DIR])

def create_windows_installer():
    """Create an NSIS installer for Windows."""
    if not os.path.exists(DEPLOY_DIR):
        os.makedirs(DEPLOY_DIR)
    
    # Copy the NSIS script to the install directory
    shutil.copy(NSIS_SCRIPT_COPY, INSTALL_DIR)
    
    # Run NSIS to create the installer
    nsis_path = "C:/Program Files (x86)/NSIS/makensis.exe"  # Default NSIS path on Windows
    if not os.path.exists(nsis_path):
        print(f"NSIS not found at {nsis_path}. Please install NSIS.")
        sys.exit(1)
    
    run_command([nsis_path, NSIS_SCRIPT], cwd=INSTALL_DIR)
    
    # Move the installer to the deploy directory
    installer_name = f"{PROJECT_NAME}_Installer.exe"
    shutil.move(os.path.join(INSTALL_DIR, installer_name), os.path.join(DEPLOY_DIR, installer_name))
    os.remove(os.path.join(INSTALL_DIR, NSIS_SCRIPT))

def create_linux_tarball():
    """Create a .tar.gz archive for Linux."""
    if not os.path.exists(DEPLOY_DIR):
        os.makedirs(DEPLOY_DIR)
    
    # Create the tarball
    tarball_name = f"{PROJECT_NAME}.tar.gz"
    run_command(["tar", "-czf", tarball_name, "-C", INSTALL_DIR, "."])
    
    # Move the tarball to the deploy directory
    shutil.move(tarball_name, os.path.join(DEPLOY_DIR, tarball_name))

def main():
    os.chdir('..')
    # Detect the operating system
    system = platform.system()
    
    # Build and install the project
    build_project()
    install_project()
    
    # Create the deployment package based on the OS
    if system == "Windows":
        create_windows_installer()
    elif system == "Linux":
        create_linux_tarball()
    else:
        print(f"Unsupported operating system: {system}")
        sys.exit(1)
    
    print("Deployment package created successfully!")

if __name__ == "__main__":
    main()