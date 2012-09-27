/*
 *  Gazebo - Outdoor Multi-Robot Simulator
 *  Copyright (C) 2003  
 *     Nate Koenig & Andrew Howard
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
/*
 * Desc: 3D position interface.
 * Author: Sachin Chitta and John Hsu
 * Date: 10 June 2008
 * SVN: $Id$
 */
#ifndef GAZEBO_DRC_ROBOT_PLUGIN_HH
#define GAZEBO_DRC_ROBOT_PLUGIN_HH

#include <boost/thread.hpp>

#include "physics/physics.h"
#include "transport/TransportTypes.hh"
#include "common/Time.hh"
#include "common/Plugin.hh"
#include "common/Events.hh"

#include "boost/thread/mutex.hpp"

namespace gazebo
{
  class DRCRobotPlugin : public ModelPlugin
  {
    /// \brief Constructor
    public: DRCRobotPlugin();

    /// \brief Destructor
    public: virtual ~DRCRobotPlugin();

    /// \brief Load the controller
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf);

    /// \brief Update the controller
    private: void UpdateStates();

    private: physics::WorldPtr world_;
    private: physics::ModelPtr model_;

    private: boost::mutex update_mutex;

    /// Pointer to the update event connection
    private: event::ConnectionPtr update_connection_;

    /// Sets DRC Robot feet placement
    /// No reachability checking here.
    public: void SetFeetPose(math::Pose _l_pose, math::Pose _r_pose);

    void FixLink(physics::LinkPtr link);
    void UnfixLink();
    private: physics::JointPtr joint_;
  };
/** \} */
/// @}
}
#endif
