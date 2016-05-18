/**
 * \copyright
 * Copyright (c) 2012-2016, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 */

#include "FlipElements.h"

#include "MeshLib/Elements/Element.h"
#include "MeshLib/Node.h"
#include "MeshLib/Elements/Line.h"
#include "MeshLib/Elements/Tri.h"
#include "MeshLib/Elements/Quad.h"
#include "DuplicateMeshComponents.h"

namespace MeshLib
{

std::unique_ptr<MeshLib::Element> createFlippedElement(MeshLib::Element const& elem, std::vector<MeshLib::Node*> const& nodes)
{
    if (elem.getDimension()>2)
        return nullptr;

    unsigned const n_nodes (elem.getNNodes());
    MeshLib::Node** elem_nodes = new MeshLib::Node*[n_nodes];
    for (unsigned i=0; i<n_nodes; ++i)
        elem_nodes[i] = nodes[elem.getNode(i)->getID()];
    std::swap(elem_nodes[0], elem_nodes[1]);

    if (elem.getGeomType() == MeshElemType::LINE)
        return std::unique_ptr<MeshLib::Line>(new MeshLib::Line(elem_nodes, elem.getID()));
    else if (elem.getGeomType() == MeshElemType::TRIANGLE)
        return std::unique_ptr<MeshLib::Tri>(new MeshLib::Tri(elem_nodes, elem.getID()));
    else if (elem.getGeomType() == MeshElemType::QUAD)
    {
        std::swap(elem_nodes[2], elem_nodes[3]);
        return std::unique_ptr<MeshLib::Quad>(new MeshLib::Quad(elem_nodes, elem.getID()));
    }
    return nullptr;
}

std::unique_ptr<MeshLib::Mesh> createFlippedMesh(MeshLib::Mesh const& mesh)
{
    if (mesh.getDimension() > 2)
        return nullptr;

    std::vector<MeshLib::Node*> new_nodes (MeshLib::copyNodeVector(mesh.getNodes()));
    std::vector<MeshLib::Element*> const& elems (mesh.getElements());
    std::vector<MeshLib::Element*> new_elems;
    std::size_t n_elems (mesh.getNElements());
    new_elems.reserve(n_elems);

    for (std::size_t i=0; i<n_elems; ++i)
        new_elems.push_back(createFlippedElement(*elems[i], new_nodes).release());

    MeshLib::Properties new_props (mesh.getProperties());
    return std::unique_ptr<MeshLib::Mesh>(new MeshLib::Mesh("FlippedElementMesh", new_nodes, new_elems, new_props));
}

} // end namespace MeshLib

