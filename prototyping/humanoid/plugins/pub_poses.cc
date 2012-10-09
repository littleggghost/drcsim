/*
 * Copyright 2011 Nate Koenig & Andrew Howard
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#include "gazebo/gazebo.hh"
#include "physics/physics.h"
#include "transport/transport.hh"
#include "msgs/MessageTypes.hh"
#include "common/Time.hh"
#include "common/Events.hh"

gazebo::common::Time sim_time(-1);
void OnStats( const boost::shared_ptr<gazebo::msgs::WorldStatistics const> &_msg)
{
  sim_time = gazebo::common::Time(_msg->sim_time().sec(),_msg->sim_time().nsec());
  gzerr << "got stat [" << sim_time << "]\n";
}


int main(int argc, char** argv)
{
  // std::list<std::string> worldNames;
  // gazebo::transport::get_topic_namespaces(worldNames);
  // gzdbg << "world name [" << *(worldNames.begin()) << "]\n";

  gazebo::load();
  gazebo::init();

  gazebo::transport::NodePtr node;
  node = gazebo::transport::NodePtr(new gazebo::transport::Node());
  // node->Init(*(worldNames.begin()));
  node->Init("default");

  gazebo::transport::SubscriberPtr sub = node->Subscribe("/gazebo/default/world_stats", &OnStats);

  gazebo::run();
  while(sim_time < 0)
  {
    gzwarn << sim_time << "\n";
  }

  gazebo::transport::PublisherPtr pubPoses;
  pubPoses = node->Advertise<gazebo::msgs::PoseTrajectory>("/gazebo/model_poses",10);
  pubPoses->WaitForConnection();

  gazebo::msgs::PoseTrajectory pose_trajectory;
  pose_trajectory.set_name("test_trajecotry");
  pose_trajectory.set_id(100);

  for (int k = 0; k < 1000; k++)
  {
    gazebo::msgs::PoseStamped pose_stamped;
    gazebo::common::Time t(0.01*k);
    pose_stamped.mutable_time()->set_sec((sim_time+t).sec);
    pose_stamped.mutable_time()->set_nsec((sim_time+t).nsec);
    pose_stamped.mutable_pose()->set_name("test_pose_stamped");
    pose_stamped.mutable_pose()->mutable_position()->set_x(0.001*k);
    pose_stamped.mutable_pose()->mutable_position()->set_y(0);
    pose_stamped.mutable_pose()->mutable_position()->set_z(0.051);
    pose_stamped.mutable_pose()->mutable_orientation()->set_w(1);
    pose_stamped.mutable_pose()->mutable_orientation()->set_x(0);
    pose_stamped.mutable_pose()->mutable_orientation()->set_y(0);
    pose_stamped.mutable_pose()->mutable_orientation()->set_z(0);

    pose_trajectory.add_pose_stamped()->CopyFrom(pose_stamped);
  }
  pubPoses->Publish(pose_trajectory);


  gazebo::transport::PublisherPtr pubConfiguration;
  pubConfiguration = node->Advertise<gazebo::msgs::ModelConfiguration>("/gazebo/model_configuration",10);
  pubConfiguration->WaitForConnection();

  gazebo::msgs::ModelConfiguration model_configuration;
  model_configuration.mutable_time()->set_sec((sim_time.sec)); // delay 10 seconds
  model_configuration.mutable_time()->set_nsec((sim_time.nsec));

  model_configuration.add_joint_names("l_ankle_joint");
  model_configuration.add_joint_names("r_ankle_joint");
  model_configuration.add_joint_names("r_foot_joint");
  model_configuration.add_joint_names("l_foot_joint");

  model_configuration.add_joint_positions(0.0);
  model_configuration.add_joint_positions(0.0);
  model_configuration.add_joint_positions(0.0);
  model_configuration.add_joint_positions(0.0);

  model_configuration.add_joint_names("l_thigh_pan_joint");
  model_configuration.add_joint_names("l_thigh_lift_joint");
  model_configuration.add_joint_names("l_calf_joint");

  model_configuration.add_joint_positions(0.0);
  model_configuration.add_joint_positions(0.0);
  model_configuration.add_joint_positions(0.0);

  model_configuration.add_joint_names("r_thigh_pan_joint");
  model_configuration.add_joint_names("r_thigh_lift_joint");
  model_configuration.add_joint_names("r_calf_joint");

  model_configuration.add_joint_positions(0.0);
  model_configuration.add_joint_positions(-M_PI/2.0);
  model_configuration.add_joint_positions(0.0);

  model_configuration.add_joint_names("torso_joint");

  model_configuration.add_joint_positions(0.0);

  model_configuration.add_joint_names("l_shoulder_joint");
  model_configuration.add_joint_names("l_upper_arm_joint");
  model_configuration.add_joint_names("l_forearm_joint");
  model_configuration.add_joint_names("l_wrist_joint");
  model_configuration.add_joint_names("l_hand_joint");

  model_configuration.add_joint_positions(M_PI/2.0);
  model_configuration.add_joint_positions(-M_PI/2.0);
  model_configuration.add_joint_positions(0.0);
  model_configuration.add_joint_positions(0.0);
  model_configuration.add_joint_positions(0.0);

  model_configuration.add_joint_names("r_shoulder_joint");
  model_configuration.add_joint_names("r_upper_arm_joint");
  model_configuration.add_joint_names("r_forearm_joint");
  model_configuration.add_joint_names("r_wrist_joint");
  model_configuration.add_joint_names("r_hand_joint");

  model_configuration.add_joint_positions(-M_PI/2.0);
  model_configuration.add_joint_positions(-M_PI/2.0);
  model_configuration.add_joint_positions(-M_PI/2.0);
  model_configuration.add_joint_positions(-M_PI/2.0);
  model_configuration.add_joint_positions(0.0);

  model_configuration.add_joint_names("neck_joint");
  model_configuration.add_joint_names("head_joint");

  model_configuration.add_joint_positions(0.0);
  model_configuration.add_joint_positions(0.0);



  pubConfiguration->Publish(model_configuration);


  gazebo::fini();
  return 0;
}
