#include "Face.h"
#include "HalfEdge.h"
#include "Vertex.h"

/**
 * Functionality: Checks if the face is a boundary face.
 * Returns:
 *     bool: Returns true if the face is a boundary face, false otherwise.
 */
bool Face::isBoundary() const
{
    return he->onBoundary;
}

/**
 * Functionality: Calculates the area of the face.
 * Returns:
 *     double: Returns the area of the face.
 */
double Face::area() const
{
    if (isBoundary()) {
        return 0;
    }

    return 0.5 * normal().norm();
}

/**
 * Functionality: Calculates the normal of the face.
 * Returns:
 *     Eigen::Vector3d: Returns the normal of the face.
 */
Eigen::Vector3d Face::normal() const
{
    Eigen::Vector3d a = he->vertex->position;
    Eigen::Vector3d b = he->next->vertex->position;
    Eigen::Vector3d c = he->next->next->vertex->position;

    Eigen::Vector3d v1 = a - b;
    Eigen::Vector3d v2 = c - b;

    return v1.cross(v2);
}