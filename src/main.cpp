#include "main.h"
#include "pros/misc.hpp"

pros::Controller controller(pros::E_CONTROLLER_MASTER);

inline pros::Motor intake(2);
inline pros::Motor conveyor(13);
inline pros::Optical optical(9);

bool racism = false;

int red = 30;
int blue = 210;
int color = red;

enum status {
  FORWARD = 0,
  BACKWARD,
  STOP
};

enum status conveyorStatus;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

void task1(void *params)
{
	while (1){
		if (conveyorStatus == FORWARD) {
			conveyor.move(127);
			intake.move(127);

			if (optical.get_hue() > color - 30 && 
				optical.get_hue() < color + 30 &&
				optical.get_proximity() <= 100 &&
				racism) {
				pros::delay(200);
				conveyor.move_velocity(0);
				intake.move_velocity(0);
				pros::delay(200);
			}
		}
		else if (conveyorStatus == BACKWARD) {
			conveyor.move(-127);
			intake.move(-127);
		}
		else {
			conveyor.move(0);
			intake.move(0);
		}

		pros::delay(1);
	}
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Task taskA (
		task1, 
		(void*)"1", 
		TASK_PRIORITY_DEFAULT,
		TASK_STACK_DEPTH_DEFAULT, 
		"task 1"
	);

	optical.set_led_pwm(100);

	while (true) {
		if (controller.get_digital(DIGITAL_R2)){ // itake + conveyor forward
			conveyorStatus = FORWARD;
		} 
		else if (controller.get_digital(DIGITAL_R1)){ // itake + conveyor forward
			conveyorStatus = BACKWARD;
		} 
		else {
			conveyorStatus = STOP;
		}

		if (controller.get_digital_new_press(DIGITAL_LEFT)){ // itake + conveyor forward
			racism = !racism;
		}

		pros::delay(30); 
	}
}