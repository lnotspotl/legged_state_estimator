#ifndef LEGGED_STATE_ESTIMATOR_LEGGED_STATE_ESTIMATOR_SETTINGS_HPP_
#define LEGGED_STATE_ESTIMATOR_LEGGED_STATE_ESTIMATOR_SETTINGS_HPP_

#include <string>
#include <vector>

#include "legged_state_estimator/inekf/noise_params.hpp"
#include "legged_state_estimator/contact_estimator.hpp"


namespace legged_state_estimator {

///
/// @class LeggedStateEstimatorSettings
/// @brief Settings of the legged state estimator.
///
struct LeggedStateEstimatorSettings {
public:
  /// 
  /// @brief Path to the URDF file.
  ///
  std::string urdf_path;

  /// 
  /// @brief Name of the IMU frame specified in the URDF file.
  ///
  std::string imu_frame;

  /// 
  /// @brief Nemes of the contact frames specified in the URDF file.
  ///
  std::vector<std::string> contact_frames;

  /// 
  /// @brief Contact estimator settings. 
  ///
  ContactEstimatorSettings contact_estimator_settings;

  /// 
  /// @brief Noise parameters (covariances) of InEKF. 
  ///
  NoiseParams noise_params;

  /// 
  /// @brief Use dynamics in contact estimation. If false, equilibrium is 
  /// used for contact estimation. Default is false.
  ///
  bool dynamic_contact_estimation = false;

  /// 
  /// @brief Noise (covariance) on contact position. (Possibly is not used in 
  /// InEKF. Contact covariance in noise_params are more important).
  ///
  double contact_position_noise;

  /// 
  /// @brief Noise (covariance) on contact rotation. Only used with surface 
  /// contacts.
  ///
  double contact_rotation_noise;

  /// 
  /// @brief Time step of estimation. 
  ///
  double dt;

  /// 
  /// @brief Cutoff frequency of LPF for gyro sensor. 
  ///
  double lpf_gyro_cutoff;

  /// 
  /// @brief Cutoff frequency of LPF for acceleration of gyro sensor. 
  ///
  double lpf_gyro_accel_cutoff;

  /// 
  /// @brief Cutoff frequency of LPF for linear acceleration measurement from IMU. 
  ///
  double lpf_lin_accel_cutoff;

  /// 
  /// @brief Cutoff frequency of LPF for joint velocities. 
  ///
  double lpf_dqJ_cutoff;

  /// 
  /// @brief Cutoff frequency of LPF for joint accelerations. 
  ///
  double lpf_ddqJ_cutoff;

  /// 
  /// @brief Cutoff frequency of LPF for joint torques. 
  ///
  double lpf_tauJ_cutoff;

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  static LeggedStateEstimatorSettings UnitreeA1(const std::string& urdf_path, 
                                                const double dt);

};

} // namespace legged_state_estimator

#endif // LEGGED_STATE_ESTIMATOR_LEGGED_STATE_ESTIMATOR_SETTINGS_HPP_