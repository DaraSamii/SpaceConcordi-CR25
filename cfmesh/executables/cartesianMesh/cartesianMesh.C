/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | cfMesh: A library for mesh generation
   \\    /   O peration     |
    \\  /    A nd           | Author: Franjo Juretic (franjo.juretic@c-fields.com)
     \\/     M anipulation  | Copyright (C) Creative Fields, Ltd.
-------------------------------------------------------------------------------
License
    This file is part of cfMesh.

    cfMesh is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    cfMesh is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with cfMesh.  If not, see <http://www.gnu.org/licenses/>.

Application
    Generates cartesian mesh

Description
    - takes a triangulated surface and generates a cartesian mesh

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "cartesianMeshGenerator.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Main program:

int main(int argc, char *argv[])
{
    argList::addBoolOption("version", "Print version information");

#   include "setRootCase.H"

    #if OPENFOAM >= 1906
    if (args.found("version"))
    #else
    if (args.optionFound("version"))
    #endif
    {
        Info<<
#       include "../../versionInfo.H"
            << endl;
        return 0;
    }

#   include "createTime.H"

    cartesianMeshGenerator cmg(runTime);

    Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s\n"
        << "ClockTime = " << runTime.elapsedClockTime() << " s" << endl;

    cmg.writeMesh();

    Info << "End\n" << endl;
    return 0;
}

// ************************************************************************* //
