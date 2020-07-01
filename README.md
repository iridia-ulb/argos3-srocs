# ARGoS3-SRoCS #
ARGoS3 plug-ins for the Swarm Robotics Construction System and more!

## Compilation and installation ##

This repository is updated with the main ARGoS repository [ilpincy/argos3](https://github.com/ilpincy/argos3). Take note that the package releases for ARGoS may be out of date and may be incompatible with the code in this repository. The CMake scripts expect to find a system-wide installation of ARGoS which will be linked against during the build process. It is currently not possible to build against a local user build of the simulator. The most straightforward way of using these plugins for the moment is to compile and install the latest master branch of both [ilpincy/argos3](https://github.com/ilpincy/argos3) and [allsey87/argos3](https://github.com/allsey87/argos3-srocs).

### Removing old versions of ARGoS ###
Before installation, you must remove any previously installed versions of ARGoS and their shared libraries from your system. For example:
```bash
# On Linux (with package manager on a Debian-based system such as Ubuntu)
sudo dpkg --remove argos3-simulator
# On Linux (without a package manager (be careful!))
sudo rm -vr /usr/local/*/argos3*
# On MacOS
sudo brew remove argos3
```

You should also remove any local configuration that you have for ARGoS:
```bash
# On Linux
rm $HOME/.config/ARGoS/ARGoS.conf
# On MacOS
defaults write info.argos-sim.ARGoS
```

### Compiling and installing ARGoS ###
Once you have uninstalled previous versions of ARGoS, you can compile and install the latest versions of ARGoS from Github. Before compiling ARGoS, you should check whether the [dependencies for compilation](https://github.com/ilpincy/argos3#compiling-argos) are installed.
```bash
git clone https://github.com/ilpincy/argos3.git
cd argos3
mkdir build
cd build
cmake -DARGOS_DOCUMENTATION=OFF -DCMAKE_BUILD_TYPE=Release ../src
make
sudo make install
sudo ldconfig
```

After ARGoS has installed, you can then install the ARGoS-SRoCS plugins:
```bash
git clone https://github.com/allsey87/argos3-srocs.git
cd argos3-srocs
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ../src
make
sudo make install
sudo ldconfig
```

## Running the demos ##
This repository currently contains three robot models: The BuilderBot, the Pi-Puck, and the Drone. These robots (especially the Pi-Puck and the Drone) are currently under development and may change in the near future. The following tests are automatically configured in the ARGoS3-SRoCS build directory:

### Pi-Puck ###
For the Pi-Puck, running the following experiment will load a lua controller that shows how the drive the robot around, how to use the ground sensors, and how to control the LEDs.
```bash
argos3 -c path/to/build/testing/pi-puck/test_pipuck.argos
```
### BuilderBot ###
For the BuilderBot, the following experiment simply adds the robot to the environment alongside the blocks which it is capable of picking up and moving around. A high-level API for this model is in development and will be available soon.
```bash
argos3 -c path/to/build/testing/builderbot/test_builderbot.argos
```

### Drone ###
The following experiment runs a very simple controller which allows the drone to be flown around the arena.
```bash
argos3 -c path/to/build/testing/drone/test_drone.argos
```








