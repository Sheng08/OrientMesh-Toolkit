#include "Edge.h"
#include "HalfEdge.h"
#include "Vertex.h"

/**
 * Functionality: Calculates the length of the edge.
 * Returns:
 *     double: Returns the length of the edge.
 */
double Edge::length() const
{
    Eigen::Vector3d a = he->vertex->position;
    Eigen::Vector3d b = he->flip->vertex->position;

    return (b-a).norm();
}