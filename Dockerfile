# School Management System Dockerfile
FROM ubuntu:22.04

# Avoid prompts from apt
ENV DEBIAN_FRONTEND=noninteractive

# Install required packages
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    git \
    vim \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy project files
COPY . .

# Setup project structure
RUN make setup

# Build the application
RUN make all

# Create a non-root user
RUN useradd -m -s /bin/bash schooluser && \
    chown -R schooluser:schooluser /app

# Switch to non-root user
USER schooluser

# Expose any ports if needed (for future web interface)
EXPOSE 8080

# Default command
CMD ["make", "run"]
