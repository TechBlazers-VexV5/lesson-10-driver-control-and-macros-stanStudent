#ifndef SENSORLOC_CPP
#define SENSORLOC_CPP
#include "main.h"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include <math.h>
/**
 * @brief A struct to hold the distance sensor and its offset
 */
struct dist_sensor {
    pros::Distance *sensor;
    lemlib::Pose offset;
};
/**
 * @brief A function to correct the position of the robot using a distance sensor
 * @param sensor The distance sensor to use
 * @param chassis The chassis to correct
 * @param x If true, correct the x position, otherwise correct the y position
 * @param correct_rate The rate at which to correct the position
 * @param forced If true, force the correction even if the distance is not valid
 */
inline void correct_position(dist_sensor sensor, lemlib::Chassis *chassis, bool x, bool forced = true, double correct_rate = 5) {
    double wall_dist = 70.5;
    lemlib::Pose currentPos = chassis->getPose(true);
    double distanceValue = sensor.sensor->get_distance();

    if (distanceValue == 9999) {
        return;
    } else {
        distanceValue = distanceValue * 0.0393701;
        pros::lcd::print(3, "Distance: %f", distanceValue);

    }
    /*double offset_x = offset.x * cos(theta_rad) - offset.y * sin(theta_rad);
        double offset_y = offset.x * sin(theta_rad) + offset.y * cos(theta_rad);*/
    double offset_y = sensor.offset.x * sin(currentPos.theta) + sensor.offset.y * cos(currentPos.theta);
    double offset_x = sensor.offset.x * cos(currentPos.theta) - sensor.offset.y * sin(currentPos.theta);
    pros::lcd::print(4, "offset_y: %f", offset_y);
    pros::lcd::print(5, "offset_x: %f", offset_x);
    double x_value = distanceValue * sin(currentPos.theta + sensor.offset.theta*M_PI/180) + offset_x;
    double y_value = distanceValue * cos(currentPos.theta + sensor.offset.theta*M_PI/180) - offset_y;
    pros::lcd::print(6, "x_value: %f", x_value);
    pros::lcd::print(7, "y_value: %f", y_value);
    if (x) {
        x_value = wall_dist*x_value/abs(x_value) - x_value;
        if (abs(x_value-currentPos.x) < correct_rate || forced) {
            chassis->setPose(x_value, currentPos.y, currentPos.theta, true);
        }
    } else {
        y_value = wall_dist*y_value/abs(y_value) - y_value;
        if (abs(y_value-currentPos.y) < correct_rate || forced) {
            chassis->setPose(currentPos.x, y_value, currentPos.theta, true);
        }
    }



}

#endif
