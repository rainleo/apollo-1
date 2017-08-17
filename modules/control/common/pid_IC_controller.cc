/******************************************************************************
 * Copyright 2017 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include "modules/control/common/pid_IC_controller.h"

#include <cmath>

#include "modules/common/log.h"

namespace apollo {
namespace control {

double PIDICController::Control(const double error, const double dt) {
  if (dt <= 0) {
    AWARN << "dt <= 0, will use the last output";
    return previous_output_;
  }
  double diff = 0;
  double output = 0;

  if (first_hit_) {
    first_hit_ = false;
  } else {
    diff = (error - previous_error_) / dt;
  }
  // integral clamping
  if (!integrator_enabled_) {
    integral_ = 0;
  } else {
    double u = error * kp_ + integral_ + error * dt * ki_ + diff * kd_;
    if (((error * u) > 0) &&
        ((u > saturation_high_) || (u < saturation_low_))) {
      // Do not update integral_
    } else {
      integral_ += error * dt * ki_;
    }
  }

  previous_error_ = error;
  output = error * kp_ + integral_ + diff * kd_;  // Ki already applied
  previous_output_ = output;
  return output;
}

}  // namespace control
}  // namespace apollo
