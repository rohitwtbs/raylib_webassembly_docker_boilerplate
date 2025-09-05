# Boilerplate Dockerfile for compiling Raylib C++ games to WebAssembly
# Uses emscripten SDK

FROM emscripten/emsdk:latest

# Install git and cmake (for building raylib and dependencies)
RUN apt-get update && apt-get install -y git python3-pip && pip3 install --upgrade cmake

# Set up working directory
WORKDIR /src

# Clone raylib (latest stable)
RUN git clone --depth 1 https://github.com/raysan5/raylib.git

# Build raylib for WebAssembly
RUN mkdir -p raylib/build && \
    cd raylib/build && \
    emcmake cmake .. -DBUILD_EXAMPLES=OFF -DBUILD_GAMES=OFF -DBUILD_SHARED_LIBS=OFF && \
    emmake make

# Copy your game source code into /src/game
# (You should mount your game source to /src/game when running the container)

# Example build command (edit as needed):
# em++ /src/game/main.cpp -I/src/raylib/src -L/src/raylib/build/raylib -lraylib -o /src/game/dist/index.html \
#     -s USE_GLFW=3 -s ASYNCIFY -s WASM=1 -O2 --shell-file /src/raylib/examples/web/shell.html

# Default command: print help
CMD echo "Mount your game source to /src/game and run em++ as shown in the Dockerfile comments."
