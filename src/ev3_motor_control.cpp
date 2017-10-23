#include "ev3dev.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "ev3dev_ros/MotorCommands.h"

#include <sstream>

using namespace std;
using namespace ev3dev; 
using ev3dev_ros::MotorCommands;

void motorCommandCallback(MotorCommands mc){
  int speed = mc.speed;
  int direction = mc.direction;
  ROS_INFO("speed: %i - direction: %i", speed, direction);
  /*Place the ROS logic to run the motors here*/
}


int main(int argc, char **argv)
{
  

  sound::speak("get  ready  to  roll", true);

  ros::init(argc, argv, "ev3_motor_control");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("motor_command", 1000, motorCommandCallback);

  ros::spin();

  return 0;

}
