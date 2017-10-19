#include "ev3dev.h"
#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>

using namespace std;
using namespace ev3dev; 

sensor get_connected_sensor(string &sensor_type)
{
  sensor arrSensors[4] {
    { INPUT_1 },
    { INPUT_2 },
    { INPUT_3 },
    { INPUT_4 }
  };
  for (unsigned i=0; i<4; ++i)
  {
    sensor &s = arrSensors[i];
    if (s.connected() && s.driver_name() == sensor_type)
    {
      return s;
    }
  }      
  //ros::ROS_INFO("no sensor connected of type: s%", sensor_type.c_str());
}

int main(int argc, char **argv)
{
  sound::speak("setting up e v 3  i r node", true);

  ros::init(argc, argv, "ev3_ir_talker_node");

  ros::NodeHandle n;

  ros::Publisher ev3_ir_pub = n.advertise<std_msgs::String>("ev3_ir", 1000);

  ros::Rate loop_rate(10);

  ros::Time last_time;
  ros::Time cur_time;

  string sensor_driver_name = "lego-ev3-ir";

  sensor ir_sensor = get_connected_sensor(sensor_driver_name);

  ir_sensor.set_mode("IR-PROX");

  sound::speak("sending i r data", true);

  while (ros::ok())
  {    
    cur_time = ros::Time::now();

    ros::Duration time_diff = cur_time - last_time;

    std_msgs::String msg;
    
    std::stringstream ss;
    ss << ir_sensor.value();
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());

    ev3_ir_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();

    last_time = cur_time;
  }

  return 0;
}
