// header files
#include "globals.hpp" //contains motor definitions, other libraries, and global variables
#include "AutonSelector.hpp" // contains how to select a specific path
#include "Autonomous_Paths.hpp" // Contains the different autonomous paths

// creates autonSelector object
AutonSelector autonSelector;

void on_center_button() {}	

void initialize() {
    chassis.calibrate(); // calibrate sensors
    pros::screen::erase();
    pros::delay(1000);
    /*
    Sets up the autons and passes in the all the routes as a vector (similar to an array)
    Each autonomousRoute contains the team color, name, description, and the acutal path
    */
    autonSelector.setAutons(std::vector<autonomousRoute>{
        //autonomousRoute{"color", "name", "description", path from Autonomous_Paths.hpp},
        autonomousRoute{"red", "Red Auton 1", "1st Red Auton", redAuton1},
        autonomousRoute{"red", "Red Auton 2", "2nd Red Auton", redAuton2},
        autonomousRoute{"blue", "Blue Auton 1", "1st Blue Auton", blueAuton1},
        autonomousRoute{"blue", "Blue Auton 2", "2nd Blue Auton", blueAuton2}
    });
    autonSelector.setSkillsAuton(autonomousRoute{"red", "Skills", "Skills Auton", exampleAuton});
    // Starts the auton selector screen (NOT the actual auton yet)
    autonSelector.start();

    // ===================================================================
    // =========== Put standard initialization code below here ===========
    // ===================================================================
}

void disabled() {}
   
void competition_initialize() {}

void autonomous() {
    // Code to run during the autonomous period
    autonSelector.runAuton();
}
 

/* ================================== Improving Driver Control ==================================

- Controllers have 2 joysticks, each with an x (horizontal) and y (vertical) component
- Below we will go over some coding examples of different driver control options 

1) Option 1: Tank 
    - One simple way is to bind one joystick to one wheel and the other joystick to the second drivetrain.
    - Look at the following example to see how to implement it.

    chassis.tank(
        int left,                   -> Power for the left side (-127, 127)
        int right,                  -> Power for the right side (-127, 127)
        bool disableDriveCurve,     -> True = NO curve, Default = false
    )

2) Option 2: Single Stick Arcade 
    - One stick is used to control the robot completely  
    - Both the X and Y readings from the joystick are used
3) Option 3: Double Stick Arcade 
    - Similar to the single stick, but one stick controls the turning and one controls the drive
    - Usually, the left stick controls the drive and the right stick controls turning

    chassis.arcade(
        int throttle,               -> Power for the forward/backward movement (-127, 127)
        int turn,                   -> Power for the turning (-127, 127)
        bool disableDriveCurve,     -> True = NO curve, Default = false
        float desaturateBias        -> How much to favor angular motion over lateral motion (0-1)
                                        0 = favor lateral, 1 = favor angular, 0.5 = equal
    )
    - Why do we have desaturateBias?

4) Option 4: Single/Double Curvature
    - Similar to arcade but is better for turning
    - Turns the robot based on a curve rather than on the spot
    - Useful for high speed turning to be more controlled
    - More used in FRC style robots

    chassis.curvature(
        int throttle,               -> Power for the forward/backward movement (-127, 127)
        int turn,                   -> Power for the turning (-127, 127)
        bool disableDriveCurve      -> True = NO curve, Default = false
    )


Task:
    - Implement each type of drivetrain above
    - Pay special attention to the desaturateBias parameter for arcade
    - Now make a macro that can switch between the different types of controls
*/
void opcontrol() {
    arm.set_brake_mode(hold);
    claw.set_brake_mode(hold);

    bool isTank = true; // Start with tank control
    bool isArcade = false;
    bool isCurve = false;
	while (true) {
        // Option 1: Tank Control
        int throttle = master.get_analog(ANALOG_LEFT_Y);
        int tur = master.get_analog(ANALOG_RIGHT_X);
        //chassis.tank(throttle, turn);

        // Option 2: Single Stick Arcade
        // Option 3: Double Stick Arcade


        // Option 4: Single/Double Curvature

        
        chassis.arcade(throttle, tur, false, 0.3);


        
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) { arm.move_velocity(100); }
        else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) { arm.move_velocity(-100); } 
        else { arm.move_velocity(0); }

        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { claw.move_velocity(100);}
        else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { claw.move_velocity(-100); } 
        else { claw.move_velocity(0); }
		
        pros::delay(20);
	}
}

//================================= Macros =====================================================================
/*
- Macros are a simple way to run a set of commands with one button press
- They are useful for complex actions that would be difficult to do manually
    - Additionally, they ensure consistency and accuracy especially in high pressure situations
- However, unlike macros != auton
    - Auton code requires no driver input and generally a longer sequence of actions
    - Macros are generally short sequences of actions that require some driver input
- Think of some examples of macros in a VEX game


Practice:
    1. Macros for raising the arm for low, middle, and top goals
    2. Macro to have the claw automatically close when it senses a ball
    3. Now have the macro wait a couple of seconds and then automatically drop the ball. So all you have to do is align the robot!
    4. Try coding this macro both with and without a task. Which is more correct?
*/