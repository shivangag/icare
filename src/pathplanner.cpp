#include <stdlib.h>
#include "ros/ros.h"
#include "geometry_msgs/Point32.h"
#include "icare/Steer.h"
#define MAX_DISTANCE 200
#define SAFE_DISTANCE 50
#define MOVE 15
#define SPEED 85
#define STOP 0

float sonar[3] = {0,0,0};
float yaw = 0;



void sonarCallback(const geometry_msgs::Point32::ConstPtr& msg){
	sonar[0] = msg->x;
	sonar[1] = msg->y;
	sonar[2] = msg->z;
	ROS_INFO("%f %f %f",sonar[0], sonar[1], sonar[2]);
}

int main(int argc,char** argv){
	ros::init(argc,argv,"pathplanner");
	icare::Steer steer_msg;
	ros::NodeHandle n;

	ros::Publisher steerPub = n.advertise<icare::Steer>("Steer",200);
	ros::Rate loop_rate(10);
	

	ros::Subscriber sonarSub = n.subscribe("sonar",200,sonarCallback);

	while(ros::ok()){
		steer_msg.angle=0;
		if(sonar[1]<SAFE_DISTANCE){
			if(sonar[0]>SAFE_DISTANCE||sonar[2]>SAFE_DISTANCE){
				steer_msg.speed=SPEED;
				if(sonar[0]>sonar[1])
					//directionMsg.data = "left";
					steer_msg.angle=MOVE;
				else
					steer_msg.angle=-1*MOVE;
			}
			else
				steer_msg.speed=STOP;
		}
		else
			steer_msg.speed=SPEED;
		steerPub.publish( steer_msg );
		loop_rate.sleep();
		ros::spinOnce();
	}
	
}