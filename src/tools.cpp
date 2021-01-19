#include "tools.h"
#include <iostream>

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth)
{
    /**
     * TODO: Calculate the RMSE here.
     */

    VectorXd rmse(4);
    rmse << 0,0,0,0;

    if (estimations.size() != ground_truth.size() || estimations.size() == 0)
    {
        std::cout << "Invalid estimation or ground_truth data" << std::endl;
        return rmse;
    }

    // accumulate squared residuals
    for (unsigned int i = 0; i < estimations.size(); ++i)
    {

        VectorXd residual = estimations[i] - ground_truth[i];

        // coefficient-wise multiplication
        residual = residual.array()*residual.array();
        rmse += residual;
    }

    // calculate the mean
    rmse = rmse/estimations.size();

    // calculate the squared root
    rmse = rmse.array().sqrt();

    // return rmse
    return rmse;
}

MatrixXd Tools::CalculateJacobian(const VectorXd& x_state)
{
    /**
     * TODO:
     * Calculate a Jacobian here.
     */
    MatrixXd Hj(3,4);

    if ( x_state.size() != 4 )
    {
        std::cout << "ERROR - CalculateJacobian () - The state vector must have size 4." << std::endl;
        return Hj;
    }

    const double & px = x_state(0);
    const double & py = x_state(1);
    const double & vx = x_state(2);
    const double & vy = x_state(3);
    
    double c1 = px*px + py*py;
    double c2 = sqrt(c1);
    double c3 = (c1*c2);

    if (fabs(c1) < 0.0001)
    {
        std::cout << "CalcuateJacobian() - Error: Division by Zero" << std::endl;
    	return Hj;
    }

    Hj << (px/c2), (py/c2), 0, 0,
         -(py/c1), (px/c1), 0, 0,
          py*(vx*py-vy*px)/c3, px*(vy*px-vx*py)/c3, px/c2, py/c2;

    return Hj;
}

VectorXd Tools::ConvertFromCartesianToPolarCoords(const VectorXd &v_cart)
{
    const double & px = v_cart(0);
    const double & py = v_cart(1);
    const double & vx = v_cart(2);
    const double & vy = v_cart(3);

    double rho, phi, rho_dot;
    rho = sqrt(px*px + py*py);
    phi = atan2(py, px);

    // protection from division by zero
    if (rho < 0.000001) {
        rho = 0.000001;
    }

    rho_dot = (px * vx + py * vy) / rho;

    VectorXd v_polar = VectorXd(3);
    v_polar << rho, phi, rho_dot;

    return v_polar;

}