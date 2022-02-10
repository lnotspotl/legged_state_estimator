/* ----------------------------------------------------------------------------
 * Copyright 2018, Ross Hartley
 * All Rights Reserved
 * See LICENSE for the license information
 * -------------------------------------------------------------------------- */

/**
 *  @file   NoiseParams.h
 *  @author Ross Hartley
 *  @brief  Header file for Invariant EKF noise parameter class
 *  @date   September 25, 2018
 **/
#ifndef INEKF_NOISEPARAMS_H
#define INEKF_NOISEPARAMS_H 
#include <Eigen/Dense>
#include <iostream>

namespace inekf {

class NoiseParams {

    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        NoiseParams();

        void setGyroscopeNoise(const double std);
        void setGyroscopeNoise(const Eigen::Vector3d& std);
        void setGyroscopeNoise(const Eigen::Matrix3d& cov);

        void setAccelerometerNoise(const double std);
        void setAccelerometerNoise(const Eigen::Vector3d& std);
        void setAccelerometerNoise(const Eigen::Matrix3d& cov);  

        void setGyroscopeBiasNoise(const double std);
        void setGyroscopeBiasNoise(const Eigen::Vector3d& std);
        void setGyroscopeBiasNoise(const Eigen::Matrix3d& cov);

        void setAccelerometerBiasNoise(const double std);
        void setAccelerometerBiasNoise(const Eigen::Vector3d& std);
        void setAccelerometerBiasNoise(const Eigen::Matrix3d& cov);  

        void setContactNoise(const double std);
        void setContactNoise(const Eigen::Vector3d& std);
        void setContactNoise(const Eigen::Matrix3d& cov);

        const Eigen::Matrix3d& getGyroscopeCov() const;
        const Eigen::Matrix3d& getAccelerometerCov() const;
        const Eigen::Matrix3d& getGyroscopeBiasCov() const;
        const Eigen::Matrix3d& getAccelerometerBiasCov() const;
        const Eigen::Matrix3d& getContactCov() const;

        friend std::ostream& operator<<(std::ostream& os, const NoiseParams& p);  

    private:
        Eigen::Matrix3d Qg_;
        Eigen::Matrix3d Qa_;
        Eigen::Matrix3d Qbg_;
        Eigen::Matrix3d Qba_;
        Eigen::Matrix3d Ql_;
        Eigen::Matrix3d Qc_;
};

} // end inekf namespace
#endif 