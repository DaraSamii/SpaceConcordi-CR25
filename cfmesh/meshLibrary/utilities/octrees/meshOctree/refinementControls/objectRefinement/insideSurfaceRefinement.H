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

Class
    insideSurfaceRefinement

Description
    Refine boxes with centres contained inside the cone

SourceFiles
    insideSurfaceRefinement.C

\*---------------------------------------------------------------------------*/

#ifndef insideSurfaceRefinement_H
#define insideSurfaceRefinement_H

#include "objectRefinement.H"
#ifdef BLUECFD
#include "point.T.H"
#else
#include "point.H"
#endif
#include "typeInfo.H"

#include "triSurface.H"
#include "triSurfaceSearch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                Class insideSurfaceRefinement Declaration
\*---------------------------------------------------------------------------*/

class insideSurfaceRefinement
:
    public objectRefinement
{
    // Private data
        fileName closedSurfaceFile_;
        triSurface closedSurface_;
        autoPtr<triSurfaceSearch> closedSurfaceSearch_;

protected:

    // Protected member functions

        void initialiseSurfaceSearch();

public:

    //- Runtime type information
    TypeName("insideSurface");


    // Constructors

        //- Null construct
        insideSurfaceRefinement();

        //- Construct from name, cell size, surface file name
        insideSurfaceRefinement
        (
            const word& name,
            const scalar cellSize,
            const direction additionalRefLevels,
            const fileName& closedSurfaceFile
        );

        //- Construct from dictionary
        insideSurfaceRefinement(const word& name, const dictionary& dict);
        
        //- Construct and return a clone
        virtual autoPtr<objectRefinement> clone
        (
            const insideSurfaceRefinement& sr
        ) const
        {
            return autoPtr<objectRefinement>
            (
                new insideSurfaceRefinement
                (
                    sr.name(),
                    sr.cellSize(),
                    sr.additionalRefinementLevels(),
                    sr.closedSurfaceFile_
                )
            );
        }
    
    // Member Functions
    
        //- check if a boundBox intersects or is inside the object
        bool intersectsObject(const boundBox&) const;
        
    //- Return as dictionary of entries
        dictionary dict(bool ignoreType = false) const;

      // Write

        //- Write
        void write(Ostream&) const;

        //- Write dictionary
        void writeDict(Ostream&, bool subDict = true) const;
        
    // Member Operators

        //- assign from dictionary
        void operator=(const dictionary&);

    // IOstream Operators

        Ostream& operator<<(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
