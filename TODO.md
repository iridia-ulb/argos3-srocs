# TODO

1. Clean up drone pointmass 3D model, analyse variable use and copying
    * Remove use of CRadians and switch to using std::cos, std::sin etc
2. Clean up drone camera sensor
    * Move sensor configuration to the control interface
    * Fix up constant definitions
3. Make robot implementations consistent
    * In the entity, parse strings for mediums
    * Update BuilderBot hardware initialization sequence to match that of the Drone