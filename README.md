# OpenDrono

This project contains all of the CAD and Code required to build 'OpenDrono'


![Cad Screenshot](https://user-images.githubusercontent.com/19732253/102841461-fd7db800-43d2-11eb-9e32-441be6bba056.PNG)
https://cad.onshape.com/documents/5a39f1ee6e81977bc9495c99/w/998eaf910537d694b83bb4dd/e/3f6d457c204f112068c093ae


This project also contains gerber files for making the OpenDrono Printed Circuit Board.  This helps ensure wiring is cleaner and disconnections occur less often.  
![Cad Screenshot](https://user-images.githubusercontent.com/19732253/102846319-a6311500-43dd-11eb-8b7d-f3f5aee82713.PNG)


### Project Progress
- [x] Initial CAD prototype completed 
- [x] Code to test motor controllers completed 
- [ ] Implement DShot Support
- [x] Read BNO055 IMU
- [x] Implement motor mixing 
- [x] Implement PID Control 
- [ ] Implement distance sensor for elevation control  
- [ ] Brainstorm localization methods 
- [ ] Brainstorm perception methods (CV, distance sensors, etc) 
- [ ] Implement Model Predictive Controller 




# Disclaimer 
This project is in its infancy and nearly everything is subject to massive overhauls


While testing, make sure your OpenDrono's propellers are off to ensure your safety.  Then adjust the + and - signs in the motor mixing section of the code (such as the following).  This makes sure that the feedback from each fo the individual controllers moves the drone in the correct direction.  Failure to do this will cause great instability and will likely damage your OpenDrono or yourself (True Story). 
```c
    frontRightSpeed = heightPower - rollPower - pitchPower - yawPower;
    frontLeftSpeed = heightPower + rollPower - pitchPower + yawPower;
    backRightSpeed = heightPower - rollPower + pitchPower + yawPower;
    backLeftSpeed = heightPower + rollPower + pitchPower - yawPower; 
```



