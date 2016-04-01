#include "ros/ros.h"
#include "icare/sonar.h"
#include "std_msgs/Float32.h"
#include <algorithm>
#define move 15
#define obstacle 150

float sonar[3] = {0,0,0};
float yaw = 0;
float handle_angle = 0;
std_msgs::Float32 msg;
ros::Subscriber dist_sub;
ros::Publisher Handle_angle_pub;
void settings(void);
void Heading(void);
void changeDIR(int);
void Update(float);
void Brake(void);

void Callback_sonar(const icare::sonar::ConstPtr& msg)
   {
     	sonar[0] = msg->sonarLeft;
     	sonar[1] = msg->sonarFront;
     	sonar[2] = msg->sonarRight;
   }

/*
void Callback_yaw(const std_msgs::Float32::ConstPtr& msg4)
   {
//     ROS_INFO("I heard: [%f]", msg->data.c_str());
	yaw = msg4->data;
   }
*/

//ros::Subscriber<std_msgs::Float32> yaw_sub = n.subscribe("yaw", &Callback_yaw);

void Heading()
{

	if(sonar[0] < obstacle && sonar[1] < obstacle && sonar[2] < obstacle)
		{Brake();}
	if(std::min(sonar[0],sonar[1],sonar[2])==sonar[0])
		{changeDIR(1);}
	else if(std::min(sonar[0],sonar[1],sonar[2])==sonar[1])
		{changeDIR(2);}
	else if(std::min(sonar[0],sonar[1],sonar[2])==sonar[2])
		{changeDIR(3);}
}
void changeDir(int Obstructed_sonar)
{
 float angle = 0;
	switch(Obstructed_sonar)
		{
			case 1:
				if(sonar[1] > sonar[2])
					angle-=move;
				else
					angle-=(2*move);
			case 2:
				if(sonar[0] > sonar[2])
					angle+=move;
				else
					angle-=move;
			case 3:
				if(sonar[1] > sonar[0])
					angle+=move;
				else
					angle+=(2*move);
		}
	Update(angle);
}
void Update(float angle)
{
	handle_angle = angle;
	msg.data = handle_angle;
	Handle_angle_pub.publish(msg);
	ros::spinOnce();
	while(sonar[0] < obstacle && sonar[1] < obstacle && sonar[2] < obstacle);
	handle_angle = 0;
	msg.data = handle_angle;
	Handle_angle_pub.publish(msg);	
	ros::spinOnce();
}
void Brake()
{
	//braking 
	msg.data = -999;
	Handle_angle_pub.publish(msg);

}
int main(int argc, char **argv)
{
	ros::init(argc, argv, "listener");
	ros::init(argc, argv, "talker");
	ros::NodeHandle n;
	ros::Subscriber dist_sub = n.subscribe("sonar", Callback_sonar);
	ros::spin();
	ros::Publisher Handle_angle_pub = n.advertise<std_msgs::Float32>("Handle_angle", 1000, &msg);
	while(1)
		Heading();
	
}















