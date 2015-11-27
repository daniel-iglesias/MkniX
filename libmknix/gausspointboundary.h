//-- Licencia --
#ifndef MKNIXGAUSSPOINTBOUNDARY_H
#define MKNIXGAUSSPOINTBOUNDARY_H

#include "LMX/lmx.h"
#include "point.h"

//////////////////////////////////////////// Doxygen file documentation entry:
/*!
  \file gausspoint.h

  \brief Point for numerical integration.

  \author Daniel Iglesias

 */
//////////////////////////////////////////// Doxygen file documentation (end)

namespace mknix {

class LoadThermalBoundary1D;

/**
@author Daniel Iglesias
*/
class GaussPointBoundary : public Point {

public:
    GaussPointBoundary();

    GaussPointBoundary( int dim_in,
                        double alpha_in,
                        double weight_in,
                        double jacobian_in,
                        int num_in,
                        double coor_x,
                        double dc_in );

    GaussPointBoundary( int dim_in,
                        double alpha_in,
                        double weight_in,
                        double jacobian_in,
                        int num_in,
                        double coor_x,
                        double coor_y,
                        double dc_in );

    ~GaussPointBoundary();

    virtual void shapeFunSolve( std::string, double ) override;

    void computeQext( LoadThermalBoundary1D* );
    void assembleQext( lmx::Vector<data_type> & );

//     void gnuplotOutStress( std::ofstream& );

protected:
    void initializeMatVecs();

protected:
    int num;
    double weight;

    lmx::Vector<data_type> Qext;
};

} //Namespace mknix

#endif
