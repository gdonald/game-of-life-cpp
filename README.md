## game-of-life-cpp

My take on Conway's Game of Life, in C++ using SDL2

[https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)

![Game of Life](https://raw.githubusercontent.com/gdonald/game-of-life-cpp/master/game-of-life.png)

### Install Dependencies

#### Mac OS

Install Xcode from the app store, and homebrew, then:

    brew install cmake sdl2 sdl2_gfx sdl2_ttf sdl2_image
    
#### Debian Linux

    apt install build-essential cmake clang \
                libsdl2-dev libsdl2-gfx-dev \
                libsdl2-ttf-dev libsdl2-image-dev

### Build
    cmake .
    make

### Run
    ./game-of-life
