[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/prdUCId4)
[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-2972f46106e565e64193e422d61a12cf1da4916b45550586e14ef0a7c637dd04.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=21146115)
# CECS-347 Project 2 Pat 3 
Master Repo for CECS 347

Implement Mode 1: Object follower.

Development steps:

Start with your part 1 project. Rename your project to Following_Robot. Add the following module to the project: ADC0SS3.c/h. You can find all this modules in project ADCOneSample.
Use Follow_Me.c as your main test file to test forward/backward following with one sensor.
Remove Follow_Me.c and add Following_Robot.c as main program. Replace ADC0SS3 module with ADC1SS2 module to take care of three sensor outputs. You can find an example on how to sample three analog signals in reference project WallFollower: ADCSWTrigger.c. Add two more sensors to your robot car and implement function object_follower() in Following_Robot.c. You can set mode = OBJECT_FOLLOWER for part 2 development and use function follow_me() in Follow_me.c as your start code for function object_follower().
One problem found:

When testing ADC together with PWM, we observe the following initialization order:

ADC0 needs to be initialized before PWM. 

I haven't figured out the reason. If any one find the reason, please come see me.

 
