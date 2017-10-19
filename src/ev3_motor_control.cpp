#include "ev3dev.h"
#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>

using namespace std;
using namespace ev3dev; 

void motorControlCallback(const ev3_msgs::Motors::ConstPtr& msg){
  

  /*Place the ROS logic to run the motors here*/
}

int main(int argc, char **argv)
{
  

  sound::speak("get  ready  to  roll", true);

  ros::init(argc, argv, "ev3_motor_control");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("motor_commands", 1000, motorControlCallback());

  ros::spin();

  return 0;

}
