#include "ros/ros.h"
#include "cmg_msgs/SpeedList.h"
#include "cmg_msgs/GimbalTarget.h"

#include <iostream>
#include <string>

int main(int argc, char ** argv) {
	float time = 2;
	int fw_id = 0;
	ros::init(argc, argv, "cmg_test");
	ros::NodeHandle n("~");

	ros::Publisher fw_pub = n.advertise<cmg_msgs::SpeedList>("fw_speeds",1);
	ros::Publisher gb_pub = n.advertise<cmg_msgs::GimbalTarget>("gb_speeds",1);
	
	while (ros::ok()) {
		std::string s;
		std::getline(std::cin, s);
		if (s == "done") {
			return 0;
		}
		std::cout << "Starting flywheel" << std::endl;
		cmg_msgs::SpeedList fw_cmd;
		cmg_msgs::Speed spd;
		spd.id = fw_id;
		spd.speed = 315;
		fw_cmd.speeds.push_back(spd);
		fw_pub.publish(fw_cmd);
		std::cout << "Starting gimbal" << std::endl;
		cmg_msgs::GimbalTarget gb_cmd;
		gb_cmd.positions.push_back(1.0);
		gb_cmd.mode = 1;
		gb_pub.publish(gb_cmd);
		std::cout << "Waiting for " << time << " seconds" << std::endl;
		ros::Duration(time).sleep();

		std::cout << "Stopping gimbal" << std::endl;
		gb_cmd.positions[0] = 0.0;
		gb_pub.publish(gb_cmd);
		std::cout << "Stopping flywheel" << std::endl;
		fw_cmd.speeds[0].speed = 0;
		fw_pub.publish(fw_cmd);
	}
}
