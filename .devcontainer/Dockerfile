# 1. BASE LAYER: Ubuntu 22.04 + CUDA 11.8 (Supports ROS 2 Humble)
# Switched from pytorch/pytorch (Ubuntu 20.04) to NVIDIA base to allow ROS 2 Humble installation.
FROM nvidia/cuda:11.8.0-cudnn8-devel-ubuntu22.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# 2. SYSTEM PREREQUISITES & ROS 2 SETUP
# ROS 2 requires locales to be set up correctly first
RUN apt-get update && apt-get install -y \
    locales \
    curl \
    gnupg2 \
    lsb-release \
    software-properties-common \
    && locale-gen en_US en_US.UTF-8 \
    && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8 \
    && add-apt-repository universe

ENV LANG=en_US.UTF-8

# Add ROS 2 Humble Repository
RUN curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg && \
    echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(lsb_release -cs) main" | tee /etc/apt/sources.list.d/ros2.list > /dev/null

# 3. INSTALL PYTHON & PYTORCH 2.0.1
# Installing Python 3.10 (native to 22.04) and PyTorch manually since we aren't using the PyTorch base image.
RUN apt-get update && apt-get install -y python3-pip python3-dev && \
    pip3 install --no-cache-dir \
    "numpy<2.0" \
    torch==2.0.1+cu118 \
    torchvision==0.15.2+cu118 \
    torchaudio==2.0.2+cu118 \
    --index-url https://download.pytorch.org/whl/cu118

# 4. INSTALL SYSTEM DEPENDENCIES (Your original list + ROS 2)
# Added 'ros-humble-desktop'
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    unzip \
    pkg-config \
    libgtk2.0-dev \
    libopenblas-dev \
    liblapack-dev \
    libx11-dev \
    libboost-all-dev \
    libeigen3-dev \
    libarmadillo-dev \
    libdlib-dev \
    libmlpack-dev \
    libhdf5-dev \
    ros-humble-desktop \
    python3-colcon-common-extensions \
    && rm -rf /var/lib/apt/lists/*

# 5. OPENCV (Build from Source)
# Built against the system CUDA and new ROS 2 environment
# WORKDIR /tmp
# RUN git clone https://github.com/opencv/opencv.git && \
#     git clone https://github.com/opencv/opencv_contrib.git && \
#     cd opencv && mkdir build && cd build && \
#     cmake -D CMAKE_BUILD_TYPE=RELEASE \
#           -D CMAKE_INSTALL_PREFIX=/usr/local \
#           -D OPENCV_EXTRA_MODULES_PATH=/tmp/opencv_contrib/modules \
#           -D WITH_CUDA=ON \
#           -D WITH_CUDNN=ON \
#           -D OPENCV_DNN_CUDA=ON \
#           -D WITH_EIGEN=ON \
#           -D WITH_V4L=ON \
#           -D BUILD_opencv_python3=ON \
#           .. && \
#     make -j$(nproc) && make install && \
#     cd / && rm -rf /tmp/opencv*

# 6. SHARK LIBRARY (Your original config)
RUN git clone --recursive https://github.com/Shark-ML/Shark.git /tmp/Shark && \
    cd /tmp/Shark && \
    mkdir build && cd build && \
    cmake -D BUILD_EXAMPLES=OFF \
          -D BUILD_TESTING=OFF \
          -D ENABLE_HDF5=ON \
          .. && \
    make -j$(nproc) && \
    make install && \
    ldconfig && \
    rm -rf /tmp/Shark
# Add this to your Dockerfile (apt-get install section)
RUN apt-get update && apt-get install -y \
    libgstreamer1.0-0 \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    gstreamer1.0-libav \
    gstreamer1.0-tools \
    gstreamer1.0-x \
    gstreamer1.0-alsa \
    gstreamer1.0-gl \
    gstreamer1.0-gtk3 \
    gstreamer1.0-qt5 \
    gstreamer1.0-pulseaudio
    
# Configure Environment for C++ LibTorch
# Set the Torch path for CMake so find_package(Torch) works automatically
RUN python3 -c "import torch; import os; print(os.path.join(torch.utils.cmake_prefix_path, 'Torch'))" > /tmp/torch_dir && \
    export TORCH_PATH=$(cat /tmp/torch_dir) && \
    echo "Using Torch at: $TORCH_PATH"

# We must use ENV to make it persistent for the user session
# Note: The path below is the standard location for pip installs on Ubuntu.
# If the dynamic check above gives a different path, use that instead.
ENV Torch_DIR=/usr/local/lib/python3.10/dist-packages/torch/share/cmake/Torch
ENV LD_LIBRARY_PATH=/usr/local/lib/python3.10/dist-packages/torch/lib:$LD_LIBRARY_PATH
# 7. USER SETUP & ENVIRONMENT
ARG USER_ID=1000
ARG GROUP_ID=1000

RUN groupadd -g ${GROUP_ID} ethanli && \
    useradd -m -u ${USER_ID} -g ethanli -s /bin/bash ethanli && \
    usermod -aG sudo ethanli && \
    echo "ethanli ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers && \
    usermod -aG video ethanli  # Grant access to video devices (cameras)

USER ethanli
WORKDIR /workspace

# Source ROS 2 in the user's bashrc so it loads automatically
RUN echo "source /opt/ros/humble/setup.bash" >> /home/ethanli/.bashrc
RUN echo "alias build='cd /workspaces/PytorchRos2Humble/ros2_ws && colcon build --symlink-install && source install/setup.bash '" >> /home/ethanli/.bashrc
CMD ["/bin/bash"]
