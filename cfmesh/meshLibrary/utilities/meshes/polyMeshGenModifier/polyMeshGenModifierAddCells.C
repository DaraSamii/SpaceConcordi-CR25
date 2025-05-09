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

Description

\*---------------------------------------------------------------------------*/

#include "polyMeshGenModifier.H"
#include "VRWGraphList.H"
#include "demandDrivenData.H"

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void polyMeshGenModifier::addCells(const LongList<faceList>& cellFaces)
{
    Info << "Adding cells to the mesh" << endl;

    faceListPMG& faces = mesh_.faces_;
    cellListPMG& cells = mesh_.cells_;
    labelIOList& cellLevel = mesh_.cellLevel_;

    VRWGraph& pointFaces = this->pointFaces();

    //- start adding cells into the mesh
    label nFaces = faces.size();
    label nCells = cells.size();

    forAll(cellFaces, cI)
    {
        const faceList& facesInCell = cellFaces[cI];

        label fI(0);
        cell c(cellFaces[cI].size());

        forAll(facesInCell, faceI)
        {
            const face& f = facesInCell[faceI];

            const label pointI = f[0];

            label fLabel(-1);
            forAllRow(pointFaces, pointI, pfI)
            {
                const label faceI = pointFaces(pointI, pfI);

                if( faces[faceI] == f )
                {
                    fLabel = faceI;
                    break;
                }
            }

            if( fLabel == -1 )
            {
                faces.append(f);
                c[fI++] = nFaces;

                forAll(f, pI)
                    pointFaces.append(f[pI], nFaces);

                ++nFaces;
            }
            else
            {
                c[fI++] = fLabel;
            }
        }

        cells.append(c);
        cellLevel.append(-1); //TODO: Get cell level from neighbour?
        ++nCells;
    }

    this->clearOut();
    mesh_.clearOut();

    Info << "Finished adding cells to the mesh" << endl;
}

void polyMeshGenModifier::addCells(const VRWGraphList& cellFaces)
{
    Info << "Adding " << cellFaces.size() << " cells to the mesh" << endl;

    faceListPMG& faces = mesh_.faces_;
    cellListPMG& cells = mesh_.cells_;
    labelIOList& cellLevel = mesh_.cellLevel_;

    VRWGraph& pointFaces = this->pointFaces();

    //- start adding cells into the mesh
    label nFaces = faces.size();
    label nCells = cells.size();

# ifdef USE_OMP

    // SMP-friendly algorithm:
    // 1. Search for dups of new faces in existing faces
    // 2. If not found, add point->cell-face mapping for new faces
    // 3. Loop through new cell-faces, add if only one found in
    //    point->cell-face mapping; or, if two found, add only if we are on the
    //    lower numbered cell.

    boolList pointLocked(pointFaces.size(), false);

    cells.setSize(nCells+cellFaces.size());
    cellLevel.setSize(nCells+cellFaces.size(), -1);

    VRWGraph newPointCell(pointFaces.size());
    VRWGraph newPointCellFace(pointFaces.size());

    # pragma omp parallel for schedule(dynamic)
    forAll(cellFaces, cI)
    {
        cell c(cellFaces.sizeOfGraph(cI));

        forAll(c, fI)
        {
            face f(cellFaces.sizeOfRow(cI, fI));
            forAll(f, pI)
                f[pI] = cellFaces(cI, fI, pI);

            const label pointI = f[0];

            label fLabel(-1);
            forAllRow(pointFaces, pointI, pfI)
            {
                const label faceI = pointFaces(pointI, pfI);

                if (faces[faceI] == f)
                {
                    fLabel = faceI;
                    break;
                }
            }

            if (fLabel == -1)
            {
                // Resize VRWGraph can re-size the underlying data so needs
                // to be critical
                # pragma omp critical
                forAll(f, pI)
                {
                    newPointCell.append(f[pI], cI);
                    newPointCellFace.append(f[pI], fI);
                }
            }
            else
            {
                c[fI] = fLabel;
            }
        }

        cells[nCells+cI] = c;
    }

    # pragma omp parallel for schedule(dynamic)
    forAll(cellFaces, cI)
    {
        cell& c = cells[nCells+cI];

        forAll(c, fI)
        {
            face f(cellFaces.sizeOfRow(cI, fI));
            forAll(f, pI)
                f[pI] = cellFaces(cI, fI, pI);

            const label pointI = f[0];

            bool foundSelf = false;
            label cIMatch = -1;
            label fIMatch = -1;
            forAllRow(newPointCell, pointI, pI)
            {
                const label cI2 = newPointCell(pointI, pI);
                const label fI2 = newPointCellFace(pointI, pI);
                if (!foundSelf && cI2 == cI && fI2 == fI)
                {
                    foundSelf = true;
                    if (cIMatch != -1)
                        break;
                }
                else
                {
                    face face2(cellFaces.sizeOfRow(cI2, fI2));
                    forAll(face2, pI)
                        face2[pI] = cellFaces(cI2, fI2, pI);
                    if (face2 == f)
                    {
                        cIMatch = cI2;
                        fIMatch = fI2;
                        if (foundSelf)
                            break;
                    }
                }
            }

            #ifdef FULLDEBUG
            if (cIMatch != -1 && !foundSelf)
                FatalErrorInFunction << "Unexpected error" << exit(FatalError);
            #endif
            if (foundSelf && (cIMatch == -1 || cI < cIMatch))
            {
                label faceI;
                # pragma omp critical
                {
                    faceI = nFaces;
                    faces.append(f);
                    ++nFaces;
                    forAll(f, pI)
                        pointFaces.append(f[pI], faceI);
                }
                c[fI] = faceI;
                if (cIMatch != -1)
                    cells[nCells+cIMatch][fIMatch] = faceI;
            }
        }
    }

    nCells = cells.size();

#else //USE_OMP

    forAll(cellFaces, cI)
    {
        faceList facesInCell(cellFaces.sizeOfGraph(cI));
        forAll(facesInCell, fI)
        {
            facesInCell[fI].setSize(cellFaces.sizeOfRow(cI, fI));

            forAll(facesInCell[fI], pI)
                facesInCell[fI][pI] = cellFaces(cI, fI, pI);
        }

        label fI(0);
        cell c(facesInCell.size());

        forAll(facesInCell, faceI)
        {
            const face& f = facesInCell[faceI];

            const label pointI = f[0];

            label fLabel(-1);
            forAllRow(pointFaces, pointI, pfI)
            {
                const label faceI = pointFaces(pointI, pfI);

                if( faces[faceI] == f )
                {
                    fLabel = faceI;
                    break;
                }
            }

            if( fLabel == -1 )
            {
                faces.append(f);
                c[fI++] = nFaces;

                forAll(f, pI)
                    pointFaces.append(f[pI], nFaces);

                ++nFaces;
            }
            else
            {
                c[fI++] = fLabel;
            }
        }

        cells.append(c);
        cellLevel.append(-1); //TODO: Get cellLevel from neighbouring cell?
        ++nCells;
    }

#endif //USE_OMP

    this->clearOut();
    mesh_.clearOut();

    Info << "Finished adding cells to the mesh" << endl;
}

void polyMeshGenModifier::addCell(const faceList& cellFaces)
{
    faceListPMG& faces = this->facesAccess();
    cellListPMG& cells = this->cellsAccess();
    labelIOList& cellLevel = mesh_.cellLevel_;

    label nFaces = faces.size();

    VRWGraph& pointFaces = this->pointFaces();

    cell c(cellFaces.size());
    label fI(0);

    forAll(cellFaces, faceI)
    {
        const face& f = cellFaces[faceI];

        const label pointI = f[0];

        label fLabel(-1);
        forAllRow(pointFaces, pointI, pfI)
        {
            const label faceI = pointFaces(pointI, pfI);

            if( faces[faceI] == f )
            {
                fLabel = faceI;
                break;
            }
        }

        if( fLabel == -1 )
        {
            faces.append(f);
            c[fI++] = nFaces;

            forAll(f, pI)
                pointFaces.append(f[pI], nFaces);

            ++nFaces;
        }
        else
        {
            c[fI++] = fLabel;
        }
    }

    cells.append(c);
    cellLevel.append(-1); //TODO: Get cellLevel from adjacent cell?
    mesh_.clearOut();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
