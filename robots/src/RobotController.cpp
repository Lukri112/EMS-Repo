#include "ros/ros.h"
#include <fstream>
#include <unistd.h>

const int NUM_MOTORS_MEC = 8;
const int NUM_MOTORS_K_A = 4;


const bool IS_MECANUM = false;
const bool IS_KETTEN = false;
const bool IS_ACKERMANN = false;

// GPIO Pins on Raspberry Pi for Mecanum, Ketten and Ackermann
const int GPIO_PINS_MEC[NUM_MOTORS_MEC] = {19, 13, 10, 9, 16, 12, 21, 20};
const int GPIO_PINS_K_A[NUM_MOTORS_K_A] = {16, 12, 21, 20};

const int MOTOR1_DIR1_PIN = 21; // Motor front left (Wheel 1) , Motor left Ketten/Ackermann (Wheel 1)
const int MOTOR1_DIR2_PIN = 20; // Motor front left (Wheel 1) , Motor left Ketten/Ackermann (Wheel 1)
const int MOTOR2_DIR1_PIN = 16; // Motor front right (Wheel 2) , Motor right Ketten/Ackermann (Wheel 2)
const int MOTOR2_DIR2_PIN = 12; // Motor front right (Wheel 2) , Motor right Ketten/Ackermann (Wheel 2)
const int MOTOR3_DIR1_PIN = 10; // Motor rear right(Wheel 3)
const int MOTOR3_DIR2_PIN = 19; // Motor rear right(Wheel 3)
const int MOTOR4_DIR1_PIN = 13;  // Motor rear left(Wheel 4)
const int MOTOR4_DIR2_PIN = 9; // Motor rear left(Wheel 4)



void exportGPIO(int pin) {
    std::ofstream exportFile("/sys/class/gpio/export");
    exportFile << pin;
    exportFile.close();
}



void setDirection(int pin, bool isOutput) {
    std::string direction = isOutput ? "out" : "in";
    std::ofstream directionFile("/sys/class/gpio/gpio" + std::to_string(pin) + "/direction");
    directionFile << direction;
    directionFile.close();
}



void writeValue(int pin, bool value) {
    std::ofstream valueFile("/sys/class/gpio/gpio" + std::to_string(pin) + "/value");
    valueFile << value;
    valueFile.close();
}



class RobotBase {
public:
    virtual void initializeMotorPins() = 0;
    virtual void controlMotors(char input) = 0;
};



class MecanumRobot : public RobotBase {
public:
    void initializeMotorPins() override {
        // Export and set default direction for all GPIO pins
        for (int i = 0; i < NUM_MOTORS_MEC; ++i) {
            exportGPIO(GPIO_PINS_MEC[i]);
            setDirection(GPIO_PINS_MEC[i], true);           //Sets the GPIO Pins to Outputs VERY IMPORTANT!!!
        }
    }

    void controlMotors(char input) override {

        writeValue(MOTOR1_DIR1_PIN, false);
        writeValue(MOTOR1_DIR2_PIN, false);
        writeValue(MOTOR2_DIR1_PIN, false);
        writeValue(MOTOR2_DIR2_PIN, false);
        writeValue(MOTOR3_DIR1_PIN, false);
        writeValue(MOTOR3_DIR2_PIN, false);
        writeValue(MOTOR4_DIR1_PIN, false);
        writeValue(MOTOR4_DIR2_PIN, false);

        ros::Duration(0.05).sleep();//50 milliseconds delay necessary because sudden direction changes result in voltage drop and pi reboots

        switch (input) {
            case 'w':

                // Motor 1 forward, Motor 2 forward, Motor 3 forward, Motor 4 forward
                writeValue(MOTOR1_DIR1_PIN, true);
                writeValue(MOTOR1_DIR2_PIN, false);
                writeValue(MOTOR2_DIR1_PIN, true);
                writeValue(MOTOR2_DIR2_PIN, false);
                writeValue(MOTOR3_DIR1_PIN, true);
                writeValue(MOTOR3_DIR2_PIN, false);
                writeValue(MOTOR4_DIR1_PIN, true);
                writeValue(MOTOR4_DIR2_PIN, false);
                ROS_INFO("Motors are ON (Forward)");

                break;

            case 's':

                // Motor 1 backward, Motor 2 backward, Motor 3 backward, Motor 4 backward
                writeValue(MOTOR1_DIR1_PIN, false);
                writeValue(MOTOR1_DIR2_PIN, true);
                writeValue(MOTOR2_DIR1_PIN, false);
                writeValue(MOTOR2_DIR2_PIN, true);
                writeValue(MOTOR3_DIR1_PIN, false);
                writeValue(MOTOR3_DIR2_PIN, true);
                writeValue(MOTOR4_DIR1_PIN, false);
                writeValue(MOTOR4_DIR2_PIN, true);
                ROS_INFO("Motors are ON (Backward)");

                break;

            case 'a':

                // Move left: Motor 1 backward, Motor 2 forward, Motor 3 backward, Motor 4 forward
                writeValue(MOTOR1_DIR1_PIN, false);
                writeValue(MOTOR1_DIR2_PIN, true);
                writeValue(MOTOR2_DIR1_PIN, true);
                writeValue(MOTOR2_DIR2_PIN, false);
                writeValue(MOTOR3_DIR1_PIN, false);
                writeValue(MOTOR3_DIR2_PIN, true);
                writeValue(MOTOR4_DIR1_PIN, true);
                writeValue(MOTOR4_DIR2_PIN, false);
                ROS_INFO("Moving left");

                break;

            case 'd':

                // Move right: Motor 1 forward, Motor 2 backward, Motor 3 forward, Motor 4 backward

                writeValue(MOTOR1_DIR1_PIN, true);
                writeValue(MOTOR1_DIR2_PIN, false);
                writeValue(MOTOR2_DIR1_PIN, false);
                writeValue(MOTOR2_DIR2_PIN, true);
                writeValue(MOTOR3_DIR1_PIN, true);
                writeValue(MOTOR3_DIR2_PIN, false);
                writeValue(MOTOR4_DIR1_PIN, false);
                writeValue(MOTOR4_DIR2_PIN, true);
                ROS_INFO("Moving right");

                break;

            case 'q':

                // Move right: Motor 1 nothing, Motor 2 forward, Motor 3 nothing, Motor 4 forward
                writeValue(MOTOR1_DIR1_PIN, false);
                writeValue(MOTOR1_DIR2_PIN, false);
                writeValue(MOTOR2_DIR1_PIN, true);
                writeValue(MOTOR2_DIR2_PIN, false);
                writeValue(MOTOR3_DIR1_PIN, false);
                writeValue(MOTOR3_DIR2_PIN, false);
                writeValue(MOTOR4_DIR1_PIN, true);
                writeValue(MOTOR4_DIR2_PIN, false);
                ROS_INFO("Moving diagonal forward left");

                break;

            case 'e':

                // Move right: Motor 1 forward, Motor 2 nothing, Motor 3 forward, Motor 4 nothing
                writeValue(MOTOR1_DIR1_PIN, true);
                writeValue(MOTOR1_DIR2_PIN, false);
                writeValue(MOTOR2_DIR1_PIN, false);
                writeValue(MOTOR2_DIR2_PIN, false);
                writeValue(MOTOR3_DIR1_PIN, true);
                writeValue(MOTOR3_DIR2_PIN, false);
                writeValue(MOTOR4_DIR1_PIN, false);
                writeValue(MOTOR4_DIR2_PIN, false);
                ROS_INFO("Moving diagonal forward right");

                break;

            case 'y':

                // Move right: Motor 1 backward, Motor 2 nothing, Motor 3 backward, Motor 4 nothing
                writeValue(MOTOR1_DIR1_PIN, false);
                writeValue(MOTOR1_DIR2_PIN, true);
                writeValue(MOTOR2_DIR1_PIN, false);
                writeValue(MOTOR2_DIR2_PIN, false);
                writeValue(MOTOR3_DIR1_PIN, false);
                writeValue(MOTOR3_DIR2_PIN, true);
                writeValue(MOTOR4_DIR1_PIN, false);
                writeValue(MOTOR4_DIR2_PIN, false);
                ROS_INFO("Moving diagonal backward left");

                break;

            case 'c':

                // Move right: Motor 1 nothing, Motor 2 backward, Motor 3 nothing, Motor 4 backward
                writeValue(MOTOR1_DIR1_PIN, false);
                writeValue(MOTOR1_DIR2_PIN, false);
                writeValue(MOTOR2_DIR1_PIN, false);
                writeValue(MOTOR2_DIR2_PIN, true);
                writeValue(MOTOR3_DIR1_PIN, false);
                writeValue(MOTOR3_DIR2_PIN, false);
                writeValue(MOTOR4_DIR1_PIN, false);
                writeValue(MOTOR4_DIR2_PIN, true);
                ROS_INFO("Moving diagonal backward right");

                break;

            case 'l':

                // Turn left: Motor 1 forward, Motor 2 backward, Motor 3 forward, Motor 4 backward
                writeValue(MOTOR1_DIR1_PIN, false);
                writeValue(MOTOR1_DIR2_PIN, true);
                writeValue(MOTOR2_DIR1_PIN, true);
                writeValue(MOTOR2_DIR2_PIN, false);
                writeValue(MOTOR3_DIR1_PIN, true);
                writeValue(MOTOR3_DIR2_PIN, false);
                writeValue(MOTOR4_DIR1_PIN, false);
                writeValue(MOTOR4_DIR2_PIN, true);
                ROS_INFO("Turning left");

                break;

            case 'r':

                // Turn right: Motor 1 backward, Motor 2 forward, Motor 3 backward, Motor 4 forward
                writeValue(MOTOR1_DIR1_PIN, true);
                writeValue(MOTOR1_DIR2_PIN, false);
                writeValue(MOTOR2_DIR1_PIN, false);
                writeValue(MOTOR2_DIR2_PIN, true);
                writeValue(MOTOR3_DIR1_PIN, false);
                writeValue(MOTOR3_DIR2_PIN, true);
                writeValue(MOTOR4_DIR1_PIN, true);
                writeValue(MOTOR4_DIR2_PIN, false);
                ROS_INFO("Turning right");

                break;

            default:

                // Stop all motors if the input is not recognized
                writeValue(MOTOR1_DIR1_PIN, false);
                writeValue(MOTOR1_DIR2_PIN, false);
                writeValue(MOTOR2_DIR1_PIN, false);
                writeValue(MOTOR2_DIR2_PIN, false);
                writeValue(MOTOR3_DIR1_PIN, false);
                writeValue(MOTOR3_DIR2_PIN, false);
                writeValue(MOTOR4_DIR1_PIN, false);
                writeValue(MOTOR4_DIR2_PIN, false);
                ROS_INFO("Motors are OFF");

        }

    }

};

class KettenRobot : public RobotBase {
public:

    void initializeMotorPins() override {

            // Export and set default direction for all GPIO pins
        for (int i = 0; i < NUM_MOTORS_K_A; ++i) {
            exportGPIO(GPIO_PINS_K_A[i]);
            setDirection(GPIO_PINS_K_A[i], true);           //Sets the GPIO Pins to Outputs VERY IMPORTANT!!!
        }
    }

    void controlMotors(char input) override {
            switch (input) {
            case 's':

                // Motor 1 backward, Motor 2 backward
                writeValue(MOTOR1_DIR1_PIN, true); //m1
                writeValue(MOTOR1_DIR2_PIN, false);
                writeValue(MOTOR2_DIR1_PIN, true);
                writeValue(MOTOR2_DIR2_PIN, false); //m1
                ROS_INFO("Motors are ON (Forward)");

                break;

            case 'w':

                // Motor 1 forward, Motor 2 forward
                writeValue(MOTOR1_DIR1_PIN, false);
                writeValue(MOTOR1_DIR2_PIN, true);
                writeValue(MOTOR2_DIR1_PIN, false);
                writeValue(MOTOR2_DIR2_PIN, true);
                ROS_INFO("Motors are ON (Backward)");

                break;

            case 'd':

                // Turn right: Motor 1 backward, Motor 2 forward
                writeValue(MOTOR1_DIR1_PIN, false);
                writeValue(MOTOR1_DIR2_PIN, false);
                writeValue(MOTOR2_DIR1_PIN, true);
                writeValue(MOTOR2_DIR2_PIN, true);
                ROS_INFO("Turning right");

                break;

            case 'a':

                // Turn left: Motor 1 forward, Motor 2 backward
                writeValue(MOTOR1_DIR1_PIN, true);
                writeValue(MOTOR1_DIR2_PIN, true);
                writeValue(MOTOR2_DIR1_PIN, false);
                writeValue(MOTOR2_DIR2_PIN, false);
                ROS_INFO("Turning left");

                break;

            default:

                // Stop all motors if the input is not recognized
                writeValue(MOTOR1_DIR1_PIN, false);
                writeValue(MOTOR1_DIR2_PIN, false);
                writeValue(MOTOR2_DIR1_PIN, false);
                writeValue(MOTOR2_DIR2_PIN, false);
                ROS_INFO("Motors are OFF");

        }
    }
};


class AckermannRobot : public RobotBase {
public:

    void initializeMotorPins() override {
            // Export and set default direction for all GPIO pins
        for (int i = 0; i < NUM_MOTORS_K_A; ++i) {
            exportGPIO(GPIO_PINS_K_A[i]);
            setDirection(GPIO_PINS_K_A[i], true);           //Sets the GPIO Pins to Outputs VERY IMPORTANT!!!

        }
    }


    void controlMotors(char input) override {

            switch (input) {

            case 'w':

                // Motor 1 forward, Motor 2 backward
                writeValue(MOTOR1_DIR1_PIN, true);
                writeValue(MOTOR1_DIR2_PIN, false);
                writeValue(MOTOR2_DIR1_PIN, false);
                writeValue(MOTOR2_DIR2_PIN, true);
                ROS_INFO("Motors are ON (Forward)");

                break;

            case 's':

                // Motor 1 backward, Motor 2 forward
                writeValue(MOTOR1_DIR1_PIN, false);
                writeValue(MOTOR1_DIR2_PIN, true);
                writeValue(MOTOR2_DIR1_PIN, true);
                writeValue(MOTOR2_DIR2_PIN, false);
                ROS_INFO("Motors are ON (Backward)");

                break;

            default:

                // Stop all motors if the input is not recognized
                writeValue(MOTOR1_DIR1_PIN, false);
                writeValue(MOTOR1_DIR2_PIN, false);
                writeValue(MOTOR2_DIR1_PIN, false);
                writeValue(MOTOR2_DIR2_PIN, false);
                ROS_INFO("Motors are OFF");

        }
    }
};



int main(int argc, char **argv) {

    // Initialize ROS
    ros::init(argc, argv, "RobotController");
    ros::NodeHandle nh;

    char robotType;
    char input;

    RobotBase* robot = nullptr;

    while (true) {

        std::cout << "Choose robot type (m for Mecanum, k for Ketten, a for Ackermann): ";
        std::cin >> robotType;


        switch (robotType) {

            case 'm':
                robot = new MecanumRobot();
		        std::cerr << "\nYour robot type = Mecanum.\n\nEnter command (wasd = normal, qryc = diagonal, l = left turn, r = right turn, x = Stop)" << std::endl;
                break;

            case 'k':
                robot = new KettenRobot();
		        std::cerr << "\nYour robot type = Ketten.\n\nEnter command (w = forward, s = backwards, a = left turn, d = right turn, x = Stop)" << std::endl;
                break;

            case 'a':
                robot = new AckermannRobot();
		        std::cerr << "\nYour robot type = Ackermann.\n\nEnter command (w = forward, s = backwards, x = Stop)" << std::endl;
                break;

            default:
                std::cerr << "\nInvalid robot type. Please enter a valid option." << std::endl;
                continue;  // Continue the loop to ask for input again

        }

        // Initialize motor pins for the selected robot
        robot->initializeMotorPins();
        break;
    }

    ros::Rate rate(10); // Frequency 10Hz

    while (ros::ok()) {

        std::cin >> input;
        // Control motors for the selected robot
        robot->controlMotors(input);

        ros::spinOnce();
        rate.sleep();

    }

    return 0;
}
