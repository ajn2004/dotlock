# dotlock

An experiment in reinforcement learning to simulate a dot affected by a double-drift function and build a toggle-able localization stabilization solution.

## Quick-Start

To get started download the project
```bash
git clone https://codeberg.org/andrewnelsonnyc/dotlock.git
cd dotlock
```

Install imgui into `./external/`
```bash
mkidr external
cd external
git clone https://github.com/ocorunt/imgui.git
```

This project uses CMake to compile, ensure it and the sdl dev librar is installed
```bash
sudo apt install cmake libsdl2-dev
```

Starting from the root folder, build the project and run
```bash
mkdir build && cd build
cmake ..
make
./dotlock
```
