/******************************************************************************
 *  Copyright (C) 2015 by Daniel Iglesias                                     *
 *                                                                            *
 *  This file is part of Nemesis.                                             *
 *                                                                            *
 *  Nemesis is free software: you can redistribute it and/or modify           *
 *  it under the terms of the GNU Lesser General Public License as            *
 *  published by the Free Software Foundation, either version 3 of the        *
 *  License, or (at your option) any later version.                           *
 *                                                                            *
 *  Nemesis is distributed in the hope that it will be useful,                *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU Lesser General Public License for more details.                       *
 *                                                                            *
 *  You should have received a copy of the GNU Lesser General Public          *
 *  License along with Nemesis.  If not, see <http://www.gnu.org/licenses/>.  *
 *****************************************************************************/

#ifndef MKNIXGAUSSPOINT_H
#define MKNIXGAUSSPOINT_H

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

class Material;
class LoadThermalBody;

/**
@author Daniel Iglesias
*/
class GaussPoint : public Point {

public:
    GaussPoint();

    GaussPoint( int dim_in, double alpha_in, double weight_in, double jacobian_in,
                Material* mat_in, int num, double coor_x, double coor_y,
                double dc_in, bool );

    GaussPoint( int dim_in, double alpha_in, double weight_in, double jacobian_in,
                Material* mat_in, int num, double coor_x, double coor_y,
                double coor_z, double dc_in, bool );

    virtual ~GaussPoint();

    virtual void shapeFunSolve( std::string, double ) override;

    virtual void fillFEmatrices( )=0;

    void setMaterial( Material& mat_in )
    {this->mat = &mat_in; }

    void computeCij( );
    std::vector<double> getShapeCij();
    std::vector<double> getCij();
    std::vector<double> getTemps();
    double getCFactor();

    void computeHij( );
    std::vector<double> getShapeHij();
    std::vector<double> getHij();
    double getHFactor();

    void computeQext( LoadThermalBody* );

    virtual void computeFint( )=0;

    virtual void computeFext( )=0;

    virtual void computeMij( )=0;

    virtual void computeKij( )=0;

    virtual void computeStress( )=0;

    virtual void computeNLStress( )=0;

    virtual void computeNLFint( )=0;

    virtual void computeNLKij( )=0;

    void assembleCij( SparseMatrix<data_type> & );
    void assembleCijWithMap(data_type *globalCapacity,
                            uint *matrixMap,
                            int num_nodes);

    void presenceCij(int* presenceMatrix, int num_nodes);
    void presenceHij(int* presenceMatrix, int num_nodes);

    void assembleHij( SparseMatrix<data_type> & );
    void assembleHijWithMap(data_type *globalConductivity,
                            uint *matrixMap,
                            int num_nodes);

    void assembleQext( VectorX<data_type> & );

    double  getNodePhi(int deriv, int node);

    double  getWeight();

    double  getJacobian();
    int  getSupportSize();

    virtual void assembleMij( SparseMatrix<data_type> & )=0;

    virtual void assembleKij( SparseMatrix<data_type> & )=0;

    virtual void assembleRi( VectorX<data_type> &, int )=0;

    virtual void assembleFint( VectorX<data_type> & )=0;

    virtual void assembleFext( VectorX<data_type> & )=0;

    virtual double calcPotentialE( const VectorX<data_type> & )=0;

    virtual double calcKineticE( const VectorX<data_type> & )=0;

    virtual double calcElasticE( )=0;

    void mapThermalNumbers(int* thermalMap, int my_position);

    void mapNodeNumbers(uint* thermalMap,
                        int my_position,
                        int total_nodes);

    void gnuplotOutStress( std::ofstream& );

protected:
    int num;
    double weight;
    Material* mat;
    bool stressPoint;
    double avgTemp;

    lmx::DenseMatrix<data_type> B;
    lmx::DenseMatrix<data_type> C;
    lmx::DenseMatrix<data_type> H;
    lmx::DenseMatrix<data_type> M;
    lmx::DenseMatrix<data_type> K;

    VectorX<data_type> tension;
    VectorX<data_type> r; // = integral( Phi^T * tension )dA

    VectorX<data_type> Qext;
    VectorX<data_type> fint;
    VectorX<data_type> fext;
};

} //Namespace mknix

#endif
