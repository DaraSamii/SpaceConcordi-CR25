/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | cfMesh: A library for mesh generation
   \\    /   O peration     | Author: Franjo Juretic (franjo.juretic@c-fields.com)
    \\  /    A nd           | Copyright (C) Creative Fields, Ltd.
     \\/     M anipulation  | Copyright (C) 2019 Oliver Oxtoby
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

\*---------------------------------------------------------------------------*/

#include "insideSurfaceRefinement.H"
#include "addToRunTimeSelectionTable.H"
#include "boundBox.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{

defineTypeNameAndDebug(insideSurfaceRefinement, 0);
addToRunTimeSelectionTable(objectRefinement, insideSurfaceRefinement, dictionary);

// * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * * * //

void insideSurfaceRefinement::initialiseSurfaceSearch()
{
    closedSurface_ = triSurface(closedSurfaceFile_);
    closedSurfaceSearch_.reset(new triSurfaceSearch(closedSurface_));
    // Force it to construct the octree and preallocate data to prevent OpenMP races
    closedSurface_.edges();
    closedSurface_.localFaces();
    closedSurface_.localPoints();
    closedSurface_.edgeFaces();
    closedSurface_.pointEdges();
    closedSurface_.faceNormals();
    closedSurface_.pointNormals();
    closedSurface_.faceEdges();
    closedSurfaceSearch_->tree();
    closedSurfaceSearch_->tree().getVolumeType(point(0,0,0));
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

insideSurfaceRefinement::insideSurfaceRefinement()
:
    objectRefinement()
{}

insideSurfaceRefinement::insideSurfaceRefinement
(
    const word& name,
    const scalar cellSize,
    const direction additionalRefLevels,
    const fileName& closedSurfaceFile
)
:
    objectRefinement(),
    closedSurfaceFile_(closedSurfaceFile)
{
    setName(name);
    setCellSize(cellSize);
    setAdditionalRefinementLevels(additionalRefLevels);
    initialiseSurfaceSearch();
}

insideSurfaceRefinement::insideSurfaceRefinement
(
    const word& name,
    const dictionary& dict
)
:
    objectRefinement(name, dict)
{
    this->operator=(dict);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

bool insideSurfaceRefinement::intersectsObject(const boundBox& bb) const
{
    //- check if the centre is inside the closed surface
    const point c = (bb.max() + bb.min()) / 2.0;

    pointField pf(1, c);
    boolList bl(closedSurfaceSearch_->calcInside(pf));
    return bl[0];
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

dictionary insideSurfaceRefinement::dict(bool /*ignoreType*/) const
{
    dictionary dict;

    if( additionalRefinementLevels() == 0 && cellSize() >= 0.0 )
    {
        dict.add("cellSize", cellSize());
    }
    else
    {
        dict.add("additionalRefinementLevels", additionalRefinementLevels());
    }

    dict.add("type", type());

    dict.add("closedSurfaceFile", closedSurfaceFile_);

    return dict;
}

void insideSurfaceRefinement::write(Ostream& os) const
{
    os  << " type:   " << type()
        << " closedSurfaceFile: " << closedSurfaceFile_;
}

void insideSurfaceRefinement::writeDict(Ostream& os, bool subDict) const
{
    if( subDict )
    {
        os << indent << token::BEGIN_BLOCK << incrIndent << nl;
    }
    
    if( additionalRefinementLevels() == 0 && cellSize() >= 0.0 )
    {
        os.writeKeyword("cellSize") << cellSize() << token::END_STATEMENT << nl;
    }
    else
    {
        os.writeKeyword("additionalRefinementLevels")
                << additionalRefinementLevels()
                << token::END_STATEMENT << nl;
    }

    // only write type for derived types
    if( type() != typeName_() )
    {
        os.writeKeyword("type") << type() << token::END_STATEMENT << nl;
    }

    os.writeKeyword("closedSurfaceFile") << closedSurfaceFile_ 
        << token::END_STATEMENT << nl;
    
    if( subDict )
    {
        os << decrIndent << indent << token::END_BLOCK << endl;
    }
}

void insideSurfaceRefinement::operator=(const dictionary& d)
{
    // allow as embedded sub-dictionary "coordinateSystem"
    const dictionary& dict =
    (
        d.found(typeName_())
      ? d.subDict(typeName_())
      : d
    );

    if( dict.found("closedSurfaceFile") )
    {
        dict.lookup("closedSurfaceFile") >> closedSurfaceFile_;
        initialiseSurfaceSearch();
    }
    else
    {
        FatalErrorIn
        (
            "void insideSurfaceRefinement::operator=(const dictionary& d)"
        ) << "Entry closedSurfaceFile is not specified!" << exit(FatalError);
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Ostream& insideSurfaceRefinement::operator<<(Ostream& os) const
{
    os << "name " << name() << nl;
    os << "cell size " << cellSize() << nl;
    os << "additionalRefinementLevels " << additionalRefinementLevels() << endl;

    write(os);
    return os;
}
        
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
        
} // End namespace Foam

// ************************************************************************* //
