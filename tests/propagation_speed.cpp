/* ----------------------------------------------------------------------------
 * Copyright 2018, Ross Hartley <m.ross.hartley@gmail.com>
 * All Rights Reserved
 * See LICENSE for the license information
 * -------------------------------------------------------------------------- */

/**
 *  @file   propagation_speed.cpp
 *  @author Ross Hartley
 *  @brief  Test to determine average propagation speed
 *  @date   September 25, 2018
 **/
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <Eigen/Dense>
#include<Eigen/StdVector>
#include <boost/algorithm/string.hpp>
#include "legged_state_estimator/inekf/inekf.hpp"

#define DT_MIN 1e-6
#define DT_MAX 1

using namespace std;
using namespace legged_state_estimator;
using namespace boost::posix_time;

typedef vector<pair<double,Eigen::Matrix<double,6,1> > > vectorPairIntVector6d;
typedef vector<pair<double,Eigen::Matrix<double,6,1> > >::const_iterator vectorPairIntVector6dIterator;

double stod98(const std::string &s) {
    return atof(s.c_str());
}

int stoi98(const std::string &s) {
    return atoi(s.c_str());
}


int main() 
{
    // Initialize filter
    Eigen::MatrixXd X = Eigen::MatrixXd::Identity(10,10);
    InEKFState state(X);
    InEKF filter(state);

    cout << "Robot's state is initialized to: \n";
    cout << filter.getState() << endl;

    ifstream infile("../data/propagation_speed_test_data.txt");
    string line;
    Eigen::Matrix<double,6,1> m, m_last; 
    double t, t_last;
    m_last << 0,0,0,0,0,0;
    t_last = 0;
    vectorPairIntVector6d measurements_vec;

    // Loop through data file and read in measurements line by line
    while (getline(infile, line)){
        vector<string> measurement;
        boost::split(measurement,line,boost::is_any_of(" "));
        // Handle measurements
        if (measurement[0].compare("IMU")==0){
            t = stod98(measurement[1]); 
            m << stod98(measurement[2]), 
                 stod98(measurement[3]), 
                 stod98(measurement[4]),
                 stod98(measurement[5]),
                 stod98(measurement[6]),
                 stod98(measurement[7]);
            measurements_vec.push_back(pair<double,Eigen::Matrix<double,6,1> > (t, m)); 
        }
    }

    // Propagate all IMU data
    cout << "Propagating " << measurements_vec.size() << " IMU measurements...\n";
    int64_t max_duration = 0;
    int64_t sum_duration = 0;
    for (vectorPairIntVector6dIterator it=measurements_vec.begin(); it!=measurements_vec.end(); ++it) {
        // Propagate using IMU data
        t = it->first;
        m = it->second;
        double dt = t - t_last;
        if (dt > DT_MIN && dt < DT_MAX) {
            ptime start_time = second_clock::local_time();
            filter.Propagate(m_last, dt);
            ptime end_time = second_clock::local_time();
            int64_t duration = (end_time - start_time).total_nanoseconds();
            //cout << "duration: " <<  duration << endl;
            sum_duration += duration;
            if (duration > max_duration)
                max_duration = duration;

            //cout << filter.getState() << endl;
        }
        // Store previous timestamp
        t_last = t;
        m_last = m;
    }

    cout << "max duration: " <<  max_duration << endl;
    cout << "average duration: " <<  double(sum_duration)/measurements_vec.size() << endl;

    return 0;
}
