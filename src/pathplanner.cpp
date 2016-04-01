#include <stdlib.h>
#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"
#include "icare/sonar.h"
#define MAX_DISTANCE 200
#define SAFE_DISTANCE 50
#define move 15
#define obstacle 30

//volatile float leftDistance,rightDistance,straightDistance;
float sonar[3] = {0,0,0};
float yaw = 0;
float handle_angle = 0;
std_msgs::Float32 angle_msg;
ros::Subscriber sonarSub;
ros::Publisher handleAnglePub;
void settings(void);
void Heading(void);
void changeDIR(int);
void Update(float);
void Brake(void);

void sonarCallback(const icare::sonar::ConstPtr& msg){
	sonar[0] = msg->sonarLeft;
	sonar[1] = msg->sonarFront;
	sonar[2] = msg->sonarRight;
	ROS_INFO("%f %f %f",sonar[0], sonar[1], sonar[2]);
}

int main(int argc,char** argv){
	std_msgs::Float count=0;
	ros::init(argc,argv,"pathplanner");
	ros::NodeHandle n;
	handleAnglePub = n.advertise<std_msgs::Float32>("Handle_angle",500);
	ros::Rate loop_rate(10);
	ros::spinOnce();

	sonarSub = n.subscribe("sonar",1000,sonarCallback);
	ros::spin();
	while(ros::ok()){
		/*if(straightDistance<SAFE_DISTANCE){
			if(leftDistance>SAFE_DISTANCE||rightDistance>SAFE_DISTANCE){
				if(leftDistance>rightDistance)
					directionMsg.data = "left";
				else
					directionMsg.data = "right";
			}
			else
				directionMsg.data = "stop";
		}
		else
			directionMsg.data = "forward";
		motorPub.publish( directionMsg );
		loop_rate.sleep();
		ros::spinOnce();*/
		//Heading();
		handleAnglePub.publish(++count);
		ros::spinOnce();
		loop_rate.sleep();
	}
}


void Heading()
{

	if(sonar[0] < obstacle && sonar[1] < obstacle && sonar[2] < obstacle)
		{Brake();}
	if(sonar[0]<sonar[1]&&sonar[0]<sonar[2])
		{changeDIR(1);}
	else if(sonar[1]<sonar[0]&&sonar[1]<sonar[2])
		{changeDIR(2);}
	else if(sonar[2]<sonar[1]&&sonar[2]<sonar[0])
		{changeDIR(3);}
}
void changeDIR(int Obstructed_sonar)
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
	angle_msg.data = handle_angle;
	handleAnglePub.publish(angle_msg);
	ros::spinOnce();
	while(sonar[0] < obstacle && sonar[1] < obstacle && sonar[2] < obstacle);
	handle_angle = 0;
	angle_msg.data = handle_angle;
	handleAnglePub.publish(angle_msg);	
	ros::spinOnce();
}
void Brake()
{
	//braking 
	angle_msg.data = -999;
	handleAnglePub.publish(angle_msg);
	ros::spinOnce();

}