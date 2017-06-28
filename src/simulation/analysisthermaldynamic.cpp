/***************************************************************************
 *   Copyright (C) 2013 by Daniel Iglesias                                 *
 *   http://code.google.com/p/mknix                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "analysisthermaldynamic.h"
#include "simulation.h"

namespace mknix {

AnalysisThermalDynamic::AnalysisThermalDynamic()
    : Analysis()
{
}


AnalysisThermalDynamic::AnalysisThermalDynamic( Simulation* simulation_in,
        double to_in,
        double tf_in,
        double At_in,
        char * integrator_in)
    :   Analysis(simulation_in)
    , integratorType(integrator_in)
    , to(to_in)
    , tf(tf_in)
    , At(At_in)
{
    theProblem.setDiffSystem( *theSimulation );
    theProblem.setIntegrator( integrator_in );
    theProblem.setTimeParameters( to_in, tf_in, At_in );

//  theProblem.setOutputFile("dis.dat", 0);
//     theProblem.setOutputFile("flux.dat", 1);
    if (theProblem.isIntegratorExplicit() ) {
      if(OLD_CODE) theProblem.setEvaluation( static_cast<void (*)(const lmx::Vector<data_type>&,
                                                                  lmx::Vector<data_type>&,
                                                                  double)>(&Simulation::explicitThermalEvaluation) );
      else theProblem.setEvaluation( static_cast<void (*)(const VectorX<data_type>&,
                                                          VectorX<data_type>&,
                                                          double)>(&Simulation::explicitThermalEvaluation) );
    }
    else {

    if(OLD_CODE)theProblem.setEvaluation( static_cast<void (*)(lmx::Vector<data_type>&,
                                                              lmx::Vector<data_type>&,
                                                              double)>(&Simulation::dynamicThermalEvaluation) );
    else theProblem.setEvaluation( static_cast<void (*)(VectorX<data_type>&,
                                                        VectorX<data_type>&,
                                                        double)>(&Simulation::dynamicThermalEvaluation) );

      if(OLD_CODE)theProblem.setResidue( static_cast<void (*)(lmx::Vector<data_type>&,
                                                              const lmx::Vector<data_type>&,
                                                              const lmx::Vector<data_type>&,
                                                              double)>(&Simulation::dynamicThermalResidue) );
      else theProblem.setResidue( static_cast<void (*)(VectorX<data_type>&,
                                                       const VectorX<data_type>&,
                                                       const VectorX<data_type>&,
                                                       double)>(&Simulation::dynamicThermalResidue) );

      if(OLD_CODE)theProblem.setJacobian( static_cast<void (*)(lmx::Matrix<data_type>&,
                                                              const lmx::Vector<data_type>&,
                                                              double,
                                                              double)>(&Simulation::dynamicThermalTangent) );
      else theProblem.setJacobian( static_cast<void (*)(SparseMatrix<data_type>&,
                                                       const VectorX<data_type>&,
                                                       double,
                                                       double)>(&Simulation::dynamicThermalTangent) );
      if (epsilon == 0.0)
          if(OLD_CODE)theProblem.setConvergence( static_cast<bool (*)(const lmx::Vector<data_type>&,
                                                                      const lmx::Vector<data_type>&,
                                                                      double)>(&Simulation::dynamicThermalConvergence) );
          else theProblem.setConvergence( static_cast<bool (*)(const VectorX<data_type>&,
                                                                const VectorX<data_type>&,
                                                                double)>(&Simulation::dynamicThermalConvergence) );
      else
          theProblem.setConvergence( epsilon );
    }
    theProblem.setStepTriggered( &Simulation::stepTriggered );
}


AnalysisThermalDynamic::~AnalysisThermalDynamic()
{
}

void AnalysisThermalDynamic::init(lmx::Vector< data_type > * qt_in, int vervosity)
{
  std::cout << "AnalysisThermalDynamic::init with LMX" <<std::cout;
//     if( lmx::getMatrixType() == 1 )
//    theProblem.setSparsePatternJacobian( theSimulation->getSparsePattern() ); // TBD for 1-DOF
//         cout << *qt_in << endl;
    theProblem.setVervosity(vervosity);
    theProblem.setInitialConfiguration( *qt_in );
    theProblem.initialize();
}

void AnalysisThermalDynamic::init(VectorX< data_type > * qt_in, int vervosity)
{
  std::cout << "AnalysisThermalDynamic::init with Eigen" <<std::endl;

    theProblem.setVervosity(vervosity);
    theProblem.setInitialConfiguration( *qt_in );
    theProblem.initialize();
}

void AnalysisThermalDynamic::nextStep()
{
    theProblem.stepSolve();
}


void AnalysisThermalDynamic::solve( lmx::Vector< data_type > * qt_in,
                                    lmx::Vector< data_type >* qdot_in = 0,
                                    lmx::Vector< data_type >* not_used = 0)
{
    if( lmx::getMatrixType() == 1 )
//    theProblem.setSparsePatternJacobian( theSimulation->getSparsePattern() ); // TBD for 1-DOF
        cout << *qt_in << endl;
    theProblem.setInitialConfiguration( *qt_in );
    theProblem.solve();
}

void AnalysisThermalDynamic::solve( VectorX< data_type > * qt_in,
                                    VectorX< data_type >* qdot_in = 0,
                                    VectorX< data_type >* not_used = 0)
{
    if( lmx::getMatrixType() == 1 )
//    theProblem.setSparsePatternJacobian( theSimulation->getSparsePattern() ); // TBD for 1-DOF
        cout << *qt_in << endl;
    theProblem.setInitialConfiguration( *qt_in );
    theProblem.solve();
}


}
