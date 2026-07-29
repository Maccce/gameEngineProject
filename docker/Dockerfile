FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies as root
RUN apt-get update && apt-get install -y --no-install-recommends \
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
    autoconf \
    autoconf-archive \
    automake \
    libtool \
    openssh-client \
    ca-certificates \
    bash-completion \
    && rm -rf /var/lib/apt/lists/*

# Ubuntu 24.04 image already contains ubuntu user,
# but keep this portable for future base images
RUN id ubuntu >/dev/null 2>&1 || \
    useradd \
        --create-home \
        --shell /bin/bash \
        --uid 1000 \
        ubuntu

# Ensure home directory ownership
RUN mkdir -p /home/ubuntu \
    && chown -R ubuntu:ubuntu /home/ubuntu

# Configure bash completion
RUN echo "source /usr/share/bash-completion/bash_completion" >> /home/ubuntu/.bashrc \
    && chown ubuntu:ubuntu /home/ubuntu/.bashrc

# Switch to non-root user
USER ubuntu

WORKDIR /home/ubuntu

# Install vcpkg as the developer user
RUN git clone https://github.com/microsoft/vcpkg.git \
    && ./vcpkg/bootstrap-vcpkg.sh

ENV VCPKG_ROOT=/home/ubuntu/vcpkg

WORKDIR /workspace
