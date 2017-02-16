#include "ur_modern_driver/ros/rt_publisher.h"

bool RTPublisher::publish_joints(RTShared &packet, ros::Time &t) {
	sensor_msgs::JointState joint_msg;
    joint_msg.header.stamp = t;
    joint_msg.name = _joint_names;

    for(auto const& q : packet.q_actual) {
        joint_msg.position.push_back(q);
    }
    for(auto const& qd : packet.qd_actual) {
        joint_msg.velocity.push_back(qd);
    }
    for(auto const& i : packet.i_actual) {
        joint_msg.effort.push_back(i);
    }

    _joint_pub.publish(joint_msg);

    return true;
}

bool RTPublisher::publish_wrench(RTShared &packet, ros::Time &t) {
    geometry_msgs::WrenchStamped wrench_msg;
    wrench_msg.header.stamp = t;

    wrench_msg.wrench.force.x = packet.tcp_force[0];
    wrench_msg.wrench.force.y = packet.tcp_force[1];
    wrench_msg.wrench.force.z = packet.tcp_force[2];
    wrench_msg.wrench.torque.x = packet.tcp_force[3];
    wrench_msg.wrench.torque.y = packet.tcp_force[4];
    wrench_msg.wrench.torque.z = packet.tcp_force[5];

    _wrench_pub.publish(wrench_msg);

    return true;
}

bool RTPublisher::publish_tool(RTShared &packet, ros::Time &t) {
    geometry_msgs::TwistStamped tool_twist;

    tool_twist.header.stamp = t;

    tool_twist.header.frame_id = _base_frame;

    tool_twist.twist.linear.x = packet.tcp_speed_actual.position.x;
    tool_twist.twist.linear.y = packet.tcp_speed_actual.position.y;
    tool_twist.twist.linear.z = packet.tcp_speed_actual.position.z;
    tool_twist.twist.angular.x = packet.tcp_speed_actual.rotation.x;
    tool_twist.twist.angular.y = packet.tcp_speed_actual.rotation.y;
    tool_twist.twist.angular.z = packet.tcp_speed_actual.rotation.z;

    _tool_vel_pub.publish(tool_twist);

    return true;
}

bool RTPublisher::publish(RTShared &packet) {
    ros::Time time = ros::Time::now();
    return publish_joints(packet, time) && publish_wrench(packet, time) && publish_tool(packet, time);
}

bool RTPublisher::consume(RTState_V1_6__7 &state) {
    return publish(state);
}
bool RTPublisher::consume(RTState_V1_8 &state) {
    return publish(state);
}
bool RTPublisher::consume(RTState_V3_0__1 &state) {
    return publish(state);
}
bool RTPublisher::consume(RTState_V3_2__3 &state) {
    return publish(state);
}