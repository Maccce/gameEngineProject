FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y \
    git \
    cmake \
    ninja-build \
    build-essential \
    python3 \
    python3-venv \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    sudo \
    autoconf \
    autoconf-archive \
    automake \
    libtool \
    && rm -rf /var/lib/apt/lists/*

# Install vcpkg
WORKDIR /opt

RUN git clone https://github.com/microsoft/vcpkg.git

RUN chown -R ubuntu:ubuntu /opt/vcpkg

USER ubuntu

WORKDIR /opt/vcpkg

RUN ./bootstrap-vcpkg.sh

ENV VCPKG_ROOT=/opt/vcpkg

WORKDIR /workspace
