#include "ev3dev.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "ev3dev_ros/MotorCommands.h"

#include <sstream>

using namespace std;
using namespace ev3dev; 
using ev3dev_ros::MotorCommands;

class Motors{
  public:
    Motors(string right_motor_port, string left_motor_port): right_motor_port(right_motor_port), left_motor_port(left_motor_port), motors_() {
      connect_motors();
      set_command("stop");
    }

    void motorCommandCallback(MotorCommands mc){
      update_command(mc.speed, mc.direction);
  
      ROS_INFO("speed: %i - direction: %i", speed, direction);
      /*Place the ROS logic to run the motors here*/
    }
  private:
    string right_motor_port;
    string left_motor_port;
    vector<motor> motors_;

    void connect_motors()
    {
      motor arrMotors[4] {
        { OUTPUT_A },
        { OUTPUT_B },
        { OUTPUT_C },
        { OUTPUT_D }
      };
      for (unsigned i=0; i<4; ++i)
      {
        motor &m = arrMotors[i];
        if (m.connected() && m.driver_name() == "lego-ev3-l-motor" && 
            m.address() == right_motor_port)
        {
          motors_.push_back(m);
        }else if (m.connected() && m.driver_name() == "lego-ev3-l-motor" && 
            m.address() == left_motor_port)
        {
          motors_.push_back(m);
        }else{
          ROS_INFO("No motor connected to: %s", arrMotors[i]);
        }
      }      
      //ROS_INFO("no motors connected of type: %s", motor_type.c_str());
    }
    void set_command(string command){
      for (unsigned j=0; j<2; j++){
        motor &mo = motors_[j];
        mo.set_command(command);
      }
    }

    void update_command(int &speed, int &direction){
      for (unsigned j=0; j<2; ++j){
        motor &mo = motors_[j];
        if(mo.address() == right_motor_port){
          mo.set_speed_sp(speed-direction);
          mo.set_command("run-forever");
        }else if(mo.address() == left_motor_port){
          mo.set_speed_sp(speed+direction);
          mo.set_command("run-forever");
        }
      }
    }
};

int main(int argc, char **argv)
{
  sound::speak("get  ready  to  roll", true);

  ros::init(argc, argv, "ev3_motor_control");

  ros::NodeHandle n;

  Motors ev3_motors(OUTPUT_A, OUTPUT_B);

  ros::Subscriber sub = n.subscribe("motor_command", 1000,
                                    &Motors::motorCommandCallback, &ev3_motors);

  ros::spin();

  return 0;

}
