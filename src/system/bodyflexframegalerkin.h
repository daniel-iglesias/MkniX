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

#ifndef MKNIXFLEXFRAMEGALERKIN_H
#define MKNIXFLEXFRAMEGALERKIN_H

#include "bodyflex.h"

namespace mknix {

/**
  @author AUTHORS <MAILS>
*/
class FlexFrameGalerkin : public FlexBody
{
public:
    FlexFrameGalerkin();

    FlexFrameGalerkin( std::string );

    ~FlexFrameGalerkin();

    std::string getType() {
        return bodyType;
    }

    void setType( std::string type_in ) {
        bodyType = type_in;
    }

//     void initialize( );

    void calcMassMatrix( );

    void calcInternalForces( );

    void calcExternalForces( );

    void calcTangentMatrix( );

    void assembleMassMatrix( SparseMatrix<data_type> & );

    void assembleInternalForces( VectorX<data_type> & );

    void assembleExternalForces( VectorX<data_type> & );

    void assembleTangentMatrix( SparseMatrix<data_type> & );

    void outputStep( const VectorX<data_type>&, const VectorX<data_type>& );

    void outputStep( const VectorX<data_type>& );

private:
    void recoverStressField( int );

private:
    std::string bodyType;
    std::vector<VectorX<data_type> *> stress;
    std::vector<VectorX<data_type> *> energy;
};

}

#endif
