/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | cfMesh: A library for mesh generation
   \\    /   O peration     | Author: Franjo Juretic (franjo.juretic@c-fields.com)
    \\  /    A nd           | Copyright (C) Creative Fields, Ltd.
     \\/     M anipulation  | Copyright (C) 2017 Oliver Oxtoby
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

Description
    Write out the cellLevel and pointLevel arrays as geometric fields, for
    easier postprocessing

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "fvMesh.H"
#ifdef OPENFOAM
#include "columnFvMesh.H"
#endif
#include "volFields.H"
#include "dictionary.H"
#include "Time.H"
#include "pointMesh.H"
#include "pointFields.H"
#include "zeroGradientFvPatchFields.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
#   include "setRootCase.H"
#   include "createTime.H"
#   include "createMesh.H"

    // Retrieve or populate the cell refinement level
    if (!mesh.foundObject<volScalarField>("cellLevel"))
    {
        autoPtr<volScalarField> cellLevel
        (
            new volScalarField
            (
                IOobject
                (
                    "cellLevel",
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh,
                dimless,
                zeroGradientFvPatchScalarField::typeName
            )
        );
        mesh.objectRegistry::store(cellLevel);
    }

    volScalarField& cellLevel = 
        mesh.lookupObjectRef<volScalarField>("cellLevel");
    labelIOList cellLevelList
    (
        IOobject
        (
            "cellLevel",
            "constant",
            "polyMesh",
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );
    scalarField& intCellLevel = cellLevel.primitiveFieldRef();
    forAll(cellLevelList, cellI)
    {
        intCellLevel[cellI] = cellLevelList[cellI];
    }
    cellLevel.correctBoundaryConditions();
    Info << "Writing cellLevel" << endl;
    cellLevel.write();

    if (!mesh.foundObject<volScalarField>("pointLevel"))
    {
        autoPtr<pointScalarField> pointLevel
        (
            new pointScalarField
            (
                IOobject
                (
                    "pointLevel",
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                pointMesh::New(mesh),
                dimless,
                zeroGradientFvPatchScalarField::typeName
            )
        );
        mesh.objectRegistry::store(pointLevel);
    }

    pointScalarField& pointLevel = 
        mesh.lookupObjectRef<pointScalarField>("pointLevel");
    labelIOList pointLevelList
    (
        IOobject
        (
            "pointLevel",
            "constant",
            "polyMesh",
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );
    scalarField& intPointLevel = pointLevel.primitiveFieldRef();
    forAll(pointLevelList, pointI)
    {
        intPointLevel[pointI] = pointLevelList[pointI];
    }
    pointLevel.correctBoundaryConditions();
    Info << "Writing pointLevel" << endl;
    pointLevel.write();

    Info << "End\n" << endl;
    return 0;
}

// ************************************************************************* //
