# Use the official Ubuntu image as a base
FROM ubuntu:22.04

# Set environment variables to non-interactive (this prevents some prompts)
ENV DEBIAN_FRONTEND=non-interactive

# Update and install dependencies
RUN apt-get -q update && \
    apt-get -qy install \
        curl \
        build-essential \
        make \
        cmake \
        libc6-dev \
        gcc \
        g++ \
        libeigen3-dev \
        freeglut3-dev \
        python3 \
        python3-pip \
        python3-pytest \
        python3-numpy \
        python3-scipy \
        python3-pandas \
        python3-matplotlib \
        jupyter

RUN  rm -rf /var/lib/apt/lists/*

RUN  pip install pybind11 PySide2 numpy scipy pytest

# Set the working directory in the container
WORKDIR /app

# Command to run when starting the container
CMD [ "bash" ]
