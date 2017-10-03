#include "ros/ros.h"
#include "cmg_msgs/SpeedList.h"
#include "cmg_msgs/GimbalTarget.h"

#include <iostream>
#include <string>

int main(int argc, char ** argv) {
	float time = 15;
	int fw_min = 0, fw_max = 1;
	ros::init(argc, argv, "cmg_test");
	ros::NodeHandle n("~");

	ros::Publisher fw_pub = n.advertise<cmg_msgs::SpeedList>("fw_speeds",1);
	ros::Publisher gb_pub = n.advertise<cmg_msgs::GimbalTarget>("gb_speeds",1);
		
	cmg_msgs::SpeedList fw_cmd;
	for (int i = fw_min; i <= fw_max; i++) {
		cmg_msgs::Speed spd;
		spd.id = i;
		spd.speed = 205;
		fw_cmd.speeds.push_back(spd);
	}
	fw_pub.publish(fw_cmd);

	cmg_msgs::GimbalTarget gb_cmd;
	for (int i = 0; i <= fw_max-fw_min; i++) {
		gb_cmd.positions.push_back(0.0);
	}
	gb_cmd.mode = 1;
	
	while (ros::ok()) {
		std::string s;
		std::getline(std::cin, s);
		if (s == "done") {
			return 0;
		}
		std::cout << "Starting flywheel" << std::endl;
		for (int i = 0; i <= fw_max-fw_min; i++) {
			fw_cmd.speeds[i].speed = 350;
		}
		fw_pub.publish(fw_cmd);
		ros::Duration(1).sleep();
		std::cout << "Starting gimbal" << std::endl;
		for (int i = 0; i <= fw_max-fw_min; i++) {
			gb_cmd.positions[i] = 1.0;
		}
		gb_pub.publish(gb_cmd);
		std::cout << "Waiting for " << time << " seconds" << std::endl;
		ros::Duration(time).sleep();

		std::cout << "Stopping gimbal" << std::endl;
		for (int i = 0; i <= fw_max-fw_min; i++) {
			gb_cmd.positions[i] = 0.0;
		}
		gb_pub.publish(gb_cmd);
		ros::Duration(1).sleep();
		std::cout << "Stopping flywheel" << std::endl;
		for (int i = 0; i <= fw_max-fw_min; i++) {
			fw_cmd.speeds[i].speed = 205;
		}
		fw_pub.publish(fw_cmd);
	}
}
