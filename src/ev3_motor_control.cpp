#include "ev3dev.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "ev3dev_ros/MotorCommands.h"

#include <errno.h>
#include <sstream>

using namespace std;
using namespace ev3dev; 
using ev3dev_ros::MotorCommands;

class Motors{
  public:
    Motors(string right_motor_port, string left_motor_port, int max_speed): right_motor_port(right_motor_port), left_motor_port(left_motor_port), max_speed(max_speed) {
      connect_motors();
      set_command("run-direct");
    }

    void motorCommandCallback(MotorCommands mc){
      update_command(mc.right_speed, mc.left_speed);
  
      //ROS_INFO("speed: %f - direction: %f", mc.speed, mc.direction);
    }
    void irCallback(const std_msgs::String::ConstPtr& msg){
      ir = stoi(msg->data);
  
      //ROS_INFO("speed: %f - direction: %f", mc.speed, mc.direction);
    }
  private:
    int max_speed;
    int ir = 100;
		int count = 0;
    string right_motor_port;
    string left_motor_port;
    //vector<motor> motors_;
    motor rm = motor(INPUT_AUTO);
    motor lm = motor(INPUT_AUTO);
    float last_right_speed;
    float last_left_speed;
    float right_speed = 0;
    float left_speed = 0;


    void connect_motors()
    {
      ROS_INFO("connecting motors");
      motor arrMotors[4] {
        { OUTPUT_A },
        { OUTPUT_B },
        { OUTPUT_C },
        { OUTPUT_D }
      };
      for (unsigned i=0; i<4; ++i)
      {
        ROS_INFO("finding motors");
        motor &m = arrMotors[i];
        if (m.connected() && m.driver_name() == "lego-ev3-l-motor" && 
            m.address() == right_motor_port)
        {
          ROS_INFO("pushing right motor tp motors_ motors");
          rm=m;
        }else if (m.connected() && m.driver_name() == "lego-ev3-l-motor" && 
            m.address() == left_motor_port)
        {
          ROS_INFO("pushing left motor tp motors_ motors");
          lm=m;
        }else{
          ROS_INFO("No motor connected to: %s", arrMotors[i]);
        }
      }      
    }
    void set_command(string command){
      ROS_INFO("setting command: %s", command);
      rm.set_command(command);
      lm.set_command(command);
    }

    void update_command(float right_speed, float left_speed){
      right_speed = right_speed*-100;
      left_speed = left_speed*-100;
			try{
        if(ir < 30){
          rm.set_duty_cycle_sp(-100);
          lm.set_duty_cycle_sp(-100);
        }else if(last_right_speed != right_speed && last_left_speed != left_speed){
          if(count%10==0)ROS_INFO("right motor speed: %f", right_speed);
          rm.set_duty_cycle_sp((int)(right_speed));
          if(count%10==0)ROS_INFO("left motor speed: %f", left_speed);
          lm.set_duty_cycle_sp((int)(left_speed));
				}
      }catch(...) { cout << "[" << strerror(errno) << "]" << endl; }
			last_right_speed = right_speed;
			last_left_speed = left_speed;
			count++;
    }
};

int main(int argc, char **argv)
{
	char str[256];

  cout << "What is my topic?: ";
	cin.get(str, 256);

	string topicName(str);

  sound::speak(topicName + " is  ready  to  roll", true);

  ros::init(argc, argv, "ev3_motor_control");

  ros::NodeHandle n;

  Motors ev3_motors(OUTPUT_A, OUTPUT_B, 100);

  ros::Subscriber ir_sub = n.subscribe("ev3_ir", 5, &Motors::irCallback, &ev3_motors);

  ros::Subscriber motor_sub = n.subscribe(topicName, 5,  &Motors::motorCommandCallback, &ev3_motors);

  ros::spin();

  return 0;

}
