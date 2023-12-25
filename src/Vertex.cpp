#include "Vertex.h"
#include "HalfEdge.h"

std::vector<HalfEdge> isolated;

/**
 * Functionality: Checks if the vertex is a boundary vertex.
 * Returns:
 *     bool: Returns true if the vertex is a boundary vertex, false otherwise.
 */
bool Vertex::isIsolated() const
{
    return he == isolated.begin();
}