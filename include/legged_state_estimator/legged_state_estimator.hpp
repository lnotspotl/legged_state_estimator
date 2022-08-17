#ifndef LEGGED_STATE_ESTIMATOR_LEGGED_STATE_ESTIMATOR_HPP_
#define LEGGED_STATE_ESTIMATOR_LEGGED_STATE_ESTIMATOR_HPP_

#include <string>
#include <vector>
#include <cstdio>
#include <limits>

#include "Eigen/Core"
#include "Eigen/StdVector"
#include "Eigen/Geometry"

#include "legged_state_estimator/macros.hpp"
#include "legged_state_estimator/inekf/inekf.hpp"
#include "legged_state_estimator/inekf/inekf_state.hpp"
#include "legged_state_estimator/inekf/noise_params.hpp"
#include "legged_state_estimator/inekf/observations.hpp"
#include "legged_state_estimator/robot_model.hpp"
#include "legged_state_estimator/contact_estimator.hpp"
#include "legged_state_estimator/low_pass_filter.hpp"
#include "legged_state_estimator/legged_state_estimator_settings.hpp"


namespace legged_state_estimator {

class LeggedStateEstimator {
public:
  using Vector3d = Eigen::Matrix<double, 3, 1>;
  using Vector4d = Eigen::Matrix<double, 4, 1>;
  using Vector6d = Eigen::Matrix<double, 6, 1>;
  using Matrix3d = Eigen::Matrix<double, 3, 3>;
  using Matrix6d = Eigen::Matrix<double, 6, 6>;

  LeggedStateEstimator(const LeggedStateEstimatorSettings& settings);

  LeggedStateEstimator();

  ~LeggedStateEstimator();

  LEGGED_STATE_ESTIMATOR_USE_DEFAULT_COPY_CONSTRUCTOR(LeggedStateEstimator);
  LEGGED_STATE_ESTIMATOR_USE_DEFAULT_COPY_ASSIGN_OPERATOR(LeggedStateEstimator);
  LEGGED_STATE_ESTIMATOR_USE_DEFAULT_MOVE_CONSTRUCTOR(LeggedStateEstimator);
  LEGGED_STATE_ESTIMATOR_USE_DEFAULT_MOVE_ASSIGN_OPERATOR(LeggedStateEstimator);

  ///
  /// @brief Initializes the state estimator.
  /// @param[in] base_pos Base position. 
  /// @param[in] base_quat Base orientation expressed by quaternion (x, y, z, w). 
  /// @param[in] base_lin_vel_world Base linear velocity expressed in the world
  /// coordinate. Default is Eigen::Vector3d::Zero().
  /// @param[in] imu_gyro_bias Initial guess of the IMU gyro bias. Default is 
  /// Eigen::Vector3d::Zero().
  /// @param[in] imu_lin_accel_bias Initial guess of the IMU linear acceleration 
  /// bias. Default is Eigen::Vector3d::Zero().
  ///
  void init(const Eigen::Vector3d& base_pos, const Eigen::Vector4d& base_quat,
            const Eigen::Vector3d& base_lin_vel_world=Eigen::Vector3d::Zero(),
            const Eigen::Vector3d& imu_gyro_bias=Eigen::Vector3d::Zero(),
            const Eigen::Vector3d& imu_lin_accel_bias=Eigen::Vector3d::Zero());

  ///
  /// @brief Initializes the state estimator.
  /// @param[in] base_pos Base position. 
  /// @param[in] base_quat Base orientation expressed by quaternion (x, y, z, w). 
  /// @param[in] qJ Raw measurement of the joint positions. 
  /// @param[in] ground_height Ground height. 
  /// @param[in] base_lin_vel_world Base linear velocity expressed in the world
  /// coordinate. Default is Eigen::Vector3d::Zero().
  /// @param[in] imu_gyro_bias Initial guess of the IMU gyro bias. Default is 
  /// Eigen::Vector3d::Zero().
  /// @param[in] imu_lin_accel_bias Initial guess of the IMU linear acceleration 
  /// bias. Default is Eigen::Vector3d::Zero().
  ///
  void init(const Eigen::Vector3d& base_pos, const Eigen::Vector4d& base_quat,
            const Eigen::VectorXd& qJ, 
            const std::vector<double>& ground_height={0., 0., 0., 0.},
            const Eigen::Vector3d& base_lin_vel_world=Eigen::Vector3d::Zero(),
            const Eigen::Vector3d& imu_gyro_bias=Eigen::Vector3d::Zero(),
            const Eigen::Vector3d& imu_lin_accel_bias=Eigen::Vector3d::Zero());

  ///
  /// @brief Updates the state estimation.
  /// @param[in] imu_gyro_raw Raw measurement of the base angular velocity 
  /// expressed in the body local coordinate from IMU gyro sensor.
  /// @param[in] imu_lin_accel_raw Raw measurement of the base linear 
  /// acceleration expressed in the body local coordinate from IMU accelerometer. 
  /// @param[in] qJ Raw measurement of the joint positions. 
  /// @param[in] dqJ Raw measurement of the joint velocities. 
  /// @param[in] tauJ Raw measurement of the joint torques. 
  /// @param[in] f_raw Raw measurement of the foot force sensor. 
  ///
  void update(const Eigen::Vector3d& imu_gyro_raw, 
              const Eigen::Vector3d& imu_lin_accel_raw, 
              const Eigen::VectorXd& qJ, const Eigen::VectorXd& dqJ, 
              const Eigen::VectorXd& tauJ, const std::vector<double>& f_raw={});

  ///
  /// @return const reference to the base position estimate.
  ///
  const Eigen::Block<const Eigen::MatrixXd, 3, 1> getBasePositionEstimate() const;

  ///
  /// @return const reference to the base orientation estimate expressed by a 
  /// rotation matrix.
  ///
  const Eigen::Block<const Eigen::MatrixXd, 3, 3> getBaseRotationEstimate() const;

  ///
  /// @return const reference to the base orientation estimate expressed by 
  /// quaternion.
  ///
  const Eigen::Vector4d& getBaseQuaternionEstimate() const;

  ///
  /// @return const reference to the base linear velocity estimate expressed in 
  /// the world frame.
  ///
  const Eigen::Block<const Eigen::MatrixXd, 3, 1> getBaseLinearVelocityEstimateWorld() const;

  ///
  /// @return const reference to the base linear velocity estimate expressed in 
  /// the body local coordinate.
  ///
  const Eigen::Vector3d getBaseLinearVelocityEstimateLocal() const;

  ///
  /// @return const reference to the base angular velocity estimate expressed in 
  /// the world frame.
  ///
  const Eigen::Vector3d& getBaseAngularVelocityEstimateWorld() const;

  ///
  /// @return const reference to the base angular velocity estimate expressed in 
  /// the local frame.
  ///
  Eigen::Vector3d getBaseAngularVelocityEstimateLocal() const;

  ///
  /// @return const reference to the IMU gyro bias estimate. 
  ///
  const Eigen::VectorBlock<const Eigen::VectorXd, 3> getIMUGyroBiasEstimate() const;

  ///
  /// @return const reference to the IMU linear acceleration bias estimate. 
  ///
  const Eigen::VectorBlock<const Eigen::VectorXd, 3> getIMULinearAccelerationBiasEstimate() const;

  ///
  /// @return const reference to the joint velocity estimates. 
  ///
  const Eigen::VectorXd& getJointVelocityEstimate() const;

  ///
  /// @return const reference to the joint acceleration estimates. 
  ///
  const Eigen::VectorXd& getJointAccelerationEstimate() const;

  ///
  /// @return const reference to the joint torque estimates. 
  ///
  const Eigen::VectorXd& getJointTorqueEstimate() const;

  ///
  /// @return const reference to the conatct estimator. 
  ///
  const ContactEstimator& getContactEstimator() const;

  ///
  /// @return const reference to the state estimator settings. 
  ///
  const LeggedStateEstimatorSettings& getSettings() const;

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

private:
  LeggedStateEstimatorSettings settings_;
  InEKF inekf_;
  vectorKinematics leg_kinematics_;
  RobotModel robot_model_;
  ContactEstimator contact_estimator_;
  LowPassFilter<double, 3> lpf_gyro_accel_world_, lpf_lin_accel_world_;
  LowPassFilter<double, Eigen::Dynamic> lpf_dqJ_, lpf_ddqJ_, lpf_tauJ_;
  Vector3d imu_gyro_raw_world_, imu_gyro_raw_world_prev_, imu_gyro_accel_world_, 
           imu_gyro_accel_local_, imu_lin_accel_raw_world_, imu_lin_accel_local_;
  Vector6d imu_raw_;
  Vector4d quat_;
};

} // namespace legged_state_estimator

#endif // LEGGED_STATE_ESTIMATOR_LEGGED_STATE_ESTIMATOR_HPP_