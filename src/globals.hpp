#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "main.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include "lemlib/api.hpp"
#include "sensor_loc.cpp"

/*----------------------Defines----------------------*/
inline std::string autonomousPath = "none"; // Default autonomous path
inline std::string teamColor = "none"; // Default team color

// Constants
constexpr double PI = 3.141592;

/*---Controller---*/
inline pros::Controller master(pros::E_CONTROLLER_MASTER);
inline pros::Controller slave(pros::E_CONTROLLER_PARTNER);

/*------------------Global Variables------------------*/
// provides a cleaner way to access controller buttons
namespace buttons {
    constexpr auto R1 = pros::E_CONTROLLER_DIGITAL_R1;
    constexpr auto R2 = pros::E_CONTROLLER_DIGITAL_R2;
    constexpr auto L1 = pros::E_CONTROLLER_DIGITAL_L1;
    constexpr auto L2 = pros::E_CONTROLLER_DIGITAL_L2;
    constexpr auto A = pros::E_CONTROLLER_DIGITAL_A;
    constexpr auto B = pros::E_CONTROLLER_DIGITAL_B;
    constexpr auto Y = pros::E_CONTROLLER_DIGITAL_Y;
    constexpr auto X = pros::E_CONTROLLER_DIGITAL_X;
    constexpr auto UP = pros::E_CONTROLLER_DIGITAL_UP;
    constexpr auto DOWN = pros::E_CONTROLLER_DIGITAL_DOWN;
    constexpr auto LEFT = pros::E_CONTROLLER_DIGITAL_LEFT;
    constexpr auto RIGHT = pros::E_CONTROLLER_DIGITAL_RIGHT;
}

// provides a cleaner way to access motor brake modes
const pros::motor_brake_mode_e_t brake = pros::E_MOTOR_BRAKE_BRAKE; // quickly stops the motor but does not hold position
const pros::motor_brake_mode_e_t coast = pros::E_MOTOR_BRAKE_COAST; // spins freely when stopped
const pros::motor_brake_mode_e_t hold = pros::E_MOTOR_BRAKE_HOLD;   // holds the motor in place when stopped





//==============================================================================================================
//====================================== Start of LemLibs Configuration ========================================
//==============================================================================================================

/*-------------Define all configurations-------------*/
// Standard motor definitions, include the port and gearset
/*
Red Gearset: 100 RPM
Green Gearset: 200 RPM
Blue Gearset: 600 RPM
*/
inline pros::MotorGroup leftDT({1}, pros::v5::MotorGears::green); 
inline pros::MotorGroup rightDT({-5}, pros::v5::MotorGears::green); 
inline pros::Motor arm({4}, pros::v5::MotorGears::green); 
inline pros::Motor claw({7}, pros::v5::MotorGears::green); 
inline pros::Imu IMU(9);
inline pros::Distance distance (2);


// ================== Chassis Configuration ==================

// Drivetrain Settings
lemlib::Drivetrain drivetrain(&leftDT,                      // left motor group
                              &rightDT,                     // right motor group
                              11.75,                        // Track Width
                              lemlib::Omniwheel::NEW_4,     // Wheel Diameter
                              200,                          // Drivetrain rpm 
                              2                             // Horizontal Drift (default 2)
);


// ================== Odometry Configuration ==================

// Odometry Settings
lemlib::OdomSensors sensors(nullptr,    // vertical tracking wheel 1
                            nullptr,    // vertical tracking wheel 2
                            nullptr,    // horizontal tracking wheel 1
                            nullptr,    // horizontal tracking wheel 2,
                            &IMU        // inertial sensor
);



// ================== PID Configuration ==================

// lateral PID controller
lemlib::ControllerSettings angular_controller(3,    // proportional gain (kP)                       * IMPORTANT
                                              0,    // integral gain (kI)                           * If needed
                                              16,   // derivative gain (kD)                         * IMPORTANT
                                              4,    // anti windup                                  * Optional       
                                              1,    // small error range, in degrees                
                                              100,  // small error range timeout, in milliseconds   
                                              4,    // large error range, in degrees                
                                              500,  // large error range timeout, in milliseconds
                                              0     // maximum acceleration (slew)                  * Safe to ignore
);

lemlib::ControllerSettings lateral_controller(16,   // proportional gain (kP)                       * IMPORTANT
                                              0,    // integral gain (kI)                           * If Needed
                                              4,    // derivative gain (kD)                         * IMPORTANT         
                                              3,    // anti windup                                  * Optional
                                              1,    // small error range, in inches
                                              100,  // small error range timeout, in milliseconds
                                              3,    // large error range, in inches
                                              500,  // large error range timeout, in milliseconds
                                              70    // maximum acceleration (slew)                  * If robot slips or tips
);


/*============================ NEW: Drive Curve ============================ 
    - Previously, all driver input has been linear
        - Moving the joystick 10% gives 10% the output and moving 100% gives 100% the output
    - This makes fine control difficult at low speed
    - But how do we make low speeds less sensitive without losing max speed?
        - Solution: Drive Curve
        - These REDUCE sensitivity at low speeds, and INCREASES sensitivity at high speeds
    - Further Reading: https://www.vexforum.com/t/expo-drive-lemlibs-implementation/123337
    - Interactive Graph: https://www.desmos.com/calculator/umicbymbnl

Task:
    1) Run the code below with the improved drive curve, what do you notice?
    2) Read the header file for driveCurve to understand what each parameters does
        - What are the range of values for each?
    Parameters:
        - Deadband:
        - Minimum Output:
        - Curve:
    3) Play around with the different values and visualize them on the interactive graph
*/

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// Finally, create the hassis
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors // odometry sensors
                        //&throttle_curve,              // NEW: throttle curve 
                        //&steer_curve                  // NEW: steer curve
);


#endif