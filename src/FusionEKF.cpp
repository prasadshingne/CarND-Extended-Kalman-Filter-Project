#include "FusionEKF.h"
#include <iostream>
#include "Eigen/Dense"
#include "tools.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::cout;
using std::endl;
using std::vector;

/**
 * Constructor.
 */
FusionEKF::FusionEKF()
{


    is_initialized_ = false;

    previous_timestamp_ = 0;

    // initializing matrices
    R_laser_ = MatrixXd(2, 2);
    R_radar_ = MatrixXd(3, 3);
    H_laser_ = MatrixXd(2, 4);
    // Hj_      = MatrixXd(3, 4);

    //measurement covariance matrix - laser
    R_laser_ << 0.0225, 0.0,
                0.0,    0.0225;

    //measurement covariance matrix - radar
    R_radar_ << 0.09, 0.0,    0.0,
                0.0,  0.0009, 0.0,
                0.0,  0.0,    0.09;


    /**
     * TODO: Finish initializing the FusionEKF.
     * TODO: Set the process and measurement noises
     */
    H_laser_ << 1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0;

    // Hj_      << 1, 1, 0, 0,
    //            1, 1, 0, 0,
    //            1, 1, 1, 1;


    // state covariance matrix P
    /**
    ekf_.P_ = MatrixXd(4, 4);
    ekf_.P_ << 1, 0, 0, 0,
               1, 1, 0, 0,
               0, 0, 1000.0, 0,
               0, 0, 0, 1000.0;

    // the initial transition matrix F_
    ekf_.F_ = MatrixXd(4, 4);
    ekf_.F_ << 1, 0, 1, 0,
              0, 1, 0, 1,
              0, 0, 1, 0,
              0, 0, 0, 1;
    */
    // measurement noises
    noise_ax_ = 9.0;
    noise_ay_ = 9.0;

}

/**
 * Destructor.
 */
FusionEKF::~FusionEKF() {}

void FusionEKF::ProcessMeasurement(const MeasurementPackage &measurement_pack)
{
    /**
     * Initialization
     */
    if (!is_initialized_)
    {
        /**
         * TODO: Initialize the state ekf_.x_ with the first measurement.
         * TODO: Create the covariance matrix.
         * You'll need to convert radar from polar to cartesian coordinates.
         */

        // first measurement
        cout << "the first measurement is received; "
             << "measurement is performed by the "
             << ((measurement_pack.sensor_type_ == MeasurementPackage::RADAR) ? "radar " : "lidar ")
             << "sensor"
             << endl;

        VectorXd x = VectorXd(4);
        if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR)
        {
            // TODO: Convert radar from polar to cartesian coordinates
            //         and initialize state.
            // convert polar from polar to cartesian coordinate system
            double meas_rho     = measurement_pack.raw_measurements_[0];
            double meas_phi     = measurement_pack.raw_measurements_[1];
            double meas_rho_dot = measurement_pack.raw_measurements_[2];

            double meas_px      = meas_rho     * cos(meas_phi);
            double meas_py      = meas_rho     * sin(meas_phi);
            double meas_vx      = meas_rho_dot * cos(meas_phi);
            double meas_vy      = meas_rho_dot * sin(meas_phi);

            // initial state in case the first measurement comes from radar sensor
            x << meas_px,
                 meas_py,
                 meas_vx, // although radar gives velocity data in the form of the range rate rho dot​, a radar measurement
                 meas_vy; // does not contain enough information to determine the state variable velocities vx and vy

        }
        else if (measurement_pack.sensor_type_ == MeasurementPackage::LASER)
        {
            // TODO: Initialize state.
            // initial state in case the first measurement comes from lidar sensor
            x << measurement_pack.raw_measurements_[0],
                 measurement_pack.raw_measurements_[1],
                                                   0.0,    // we have no info about the velocity for lidar measurement
                                                   0.0;

        } else {
            cout << "unknown sensor type of measurement; skipping initialization" << endl;
        }

        // state covariance matrix (initial velocity is unknown, hence the level of uncertainty is high)
        MatrixXd P(4, 4);
        P << 1, 0,    0,    0,
             0, 1,    0,    0,
             0, 0, 1000,    0,
             0, 0,    0, 1000;

    // state transition matrix (initially Δt is 0)
        MatrixXd F(4, 4);
        F << 1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1;

    // process covariance matrix (initially Δt is 0, hence Q consists of 0's;
    //                            Eigen initializes matrices with 0's by default)
        MatrixXd Q(4, 4);

        ekf_.Init(x, P, F, H_laser_, R_laser_, R_radar_, Q);

        previous_timestamp_ = measurement_pack.timestamp_;

        // done initializing, no need to predict or update
        is_initialized_ = true;
        return;
    }

    /**
     * Prediction
     */

    /**
     * TODO: Update the state transition matrix F according to the new elapsed time.
     * Time is measured in seconds.
     * TODO: Update the process noise covariance matrix.
     * Use noise_ax = 9 and noise_ay = 9 for your Q matrix.
     */

    //compute the time elapsed between current and previous measurement

    double dt = (measurement_pack.timestamp_ - previous_timestamp_) / 1000000.0;	//dt - expressed in seconds
    previous_timestamp_ = measurement_pack.timestamp_;

    double dt2 = dt  * dt;
    double dt3 = dt2 * dt;
    double dt4 = dt3 * dt;

    // 1. Modify the F matrix so that the time is integrated
    ekf_.F_(0,2) = dt;
    ekf_.F_(1,3) = dt;

    // 2. Set the process covariance matrix Q
    ekf_.Q_      = MatrixXd(4, 4);
    ekf_.Q_     << dt4*noise_ax_/4,               0.0, dt3*noise_ax_/2,            0.0,
                               0.0,   dt4*noise_ay_/4,            0.0, dt3*noise_ax_/2,
                   dt3*noise_ax_/2,               0.0,  dt2*noise_ax_,             0.0,
                               0.0,   dt3*noise_ay_/2,            0.0,   dt2*noise_ax_;


    ekf_.Predict();

    /**
     * Update
     */

    /**
     * TODO:
     * - Use the sensor type to perform the update step.
     * - Update the state and covariance matrices.
     */

    if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
        // Radar updates
        ekf_.UpdateEKF(measurement_pack.raw_measurements_);
    } else {
        // Lidar updates
        ekf_.Update(measurement_pack.raw_measurements_);
    }

    // print the output
    std::cout << "==========" << std::endl
              << "x_ = "      << std::endl << ekf_.x_ << std::endl
              << "-----"      << std::endl
              << "P_ = "      << std::endl << ekf_.P_ << std::endl
              << "==========" << std::endl            << std::endl;
}
