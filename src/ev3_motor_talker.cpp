#include "ev3dev.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "ev3dev_ros/MotorCommands.h"

#include <errno.h>
#include <sstream>

using namespace std;
using namespace ev3dev; 
using ev3dev_ros::MotorCommands;

motor motor_menu()
{
  motor arrMotors[4] = {
    { OUTPUT_A },
    { OUTPUT_B },
    { OUTPUT_C },
    { OUTPUT_D }
  };

  char c = 0;
  do
  {
    cout << endl
         << "*** pick motor ***" << endl
         << endl;

    for (unsigned i=0; i<4; ++i)
    {
      motor &m = arrMotors[i];
      if (m.connected())
      {
        cout << "(" << 1+i << ") " << m.driver_name() << " motor on port " << m.address() << endl;
      }
    }
    cout << endl;
    cout << "(q)uit"  << endl;
    cout << endl
         << "Choice: ";
    cin >> c;

    switch (c)
    {
    case '1':
    case '2':
    case '3':
    case '4':
      if (arrMotors[c-'1'].connected())
        return arrMotors[c-'1'];
     break;
    }
  }
  while (c != 'q');
}


int main(int argc, char **argv)
{
	char str[256];

  cout << "What is my topic?: ";
	cin.get(str, 256);

	string topicName(str);

  sound::speak(topicName + " is  looking  for  motors", true);

  cout << "What motor is left? " << endl;
  motor left_motor = motor_menu();

  cout << "What motor is right? " << endl;
  motor right_motor = motor_menu();

  ros::init(argc, argv, "ev3_motor_talker");

  ros::NodeHandle n;

  ros::Publisher ev3_motor_right_pub = n.advertise<std_msgs::String>(topicName + "_right_motor_odom", 5);
  ros::Publisher ev3_motor_left_pub = n.advertise<std_msgs::String>(topicName + "_left_motor_odom", 5);

  ros::Rate loop_rate(2);

  ros::Time last_time;
  ros::Time cur_time;

  sound::speak("sending motor position", true);

  while (ros::ok())
  {    
    cur_time = ros::Time::now();

    ros::Duration time_diff = cur_time - last_time;

    std_msgs::String msgl;
    std_msgs::String msgr;
    
    std::stringstream ssl;
    ssl << left_motor.position();
    msgl.data = ssl.str();

    std::stringstream ssr;
    ssr << right_motor.position();
    msgr.data = ssr.str();

    ROS_INFO("left: %s, right: %s", msgl.data.c_str(), msgr.data.c_str());

    ev3_motor_right_pub.publish(msgr);
    ev3_motor_left_pub.publish(msgl);

    ros::spinOnce();

    loop_rate.sleep();

    last_time = cur_time;
  }  

  ros::spin();

  return 0;

}
