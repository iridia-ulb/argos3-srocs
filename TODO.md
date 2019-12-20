# TODO

1. Clean up drone pointmass 3D model, analyse variable use and copying
    * Remove use of CRadians and switch to using std::cos, std::sin etc
2. Clean up drone camera sensor
    * Move sensor configuration to the control interface
    * Fix up constant definitions
    * Update camera sensor to ignore the sensing robot and the entity holding the tag/led (similar to the drone)
3. Make robot implementations consistent
    * In the entity, parse strings for mediums
    * Update BuilderBot hardware initialization sequence to match that of the Drone
4. Use the TConfiguration pattern in CCI_PiPuckRangefindersSensor for all sensors
