# Cross Spherical gear actuation

<p align="center">
  <a href="https://www.youtube.com/watch?v=hhDdfiRCQS4">
    <img src="https://i.ytimg.com/an_webp/hhDdfiRCQS4/mqdefault_6s.webp?du=3000&sqp=COzZvKUG&rs=AOn4CLA3J-dkEpdE3YvnwDObh-YkiyPLpw" alt="ABENICS: Active Ball Joint Mechanism With Three-DoF Based on Spherical Gear Meshings" width="240" height="180"><br>
    Watch the original work on youtube.
  </a>
</p>

# Our implementation 
This project involves the use of 3D printing to create various components. The project's centerpiece is the cross-spherical gear, driven by two actuators, each equipped with two motors. While the original design utilized Brushless DC motors from Maxon Motor Inc, this project opts for NEMA17 stepper motors to reduce cost and complexity. The stepper motors are controlled using TMC2208 stepper motor drivers, which are interfaced with Teensy4.0.

<p align="center">
  <img src="images/DIY-abenics.png" alt="ABENICS, The original implementation" width="600" height="325"><br>
</p>

The gear system is driven by actuators, each equipped with two NEMA17 stepper motors. These motors provide the necessary torque and precision for the gear mechanism. The TMC2208 stepper motor drivers, controlled by an overclocked Teensy4.0 microcontroller, ensure smooth and accurate motor operation. The Teensy4.0 is overclocked to 916 MHz to handle all four motors, inverse kinematics, UI, and sensor feedback.

All components are 3D printed on FDM printers.
