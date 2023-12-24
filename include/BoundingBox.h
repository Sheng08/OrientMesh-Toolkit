#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "Types.h"

class BoundingBox {
public:
    // Default constructor
    BoundingBox();

    // Constructor initializing with specified min and max components
    BoundingBox(const Eigen::Vector3d& min0, const Eigen::Vector3d& max0);

    // Constructor initializing with a single point
    BoundingBox(const Eigen::Vector3d& p);

    // Destructor
    virtual ~BoundingBox(){};

    // Expand bounding box to include a point
    void expandToInclude(const Eigen::Vector3d& p);

    // Expand bounding box to include another bounding box
    void expandToInclude(const BoundingBox& b);

    // Return the maximum dimension of the bounding box
    int maxDimension() const;

    // Check if this bounding box contains another bounding box and calculate the distance
    bool contains(const BoundingBox& boundingBox, double& dist) const;

    // Compute an axis-aligned bounding box from a set of vertices
    void computeAxisAlignedBox(std::vector<Vertex>& vertices);

    // Compute an oriented bounding box using principal component analysis from a set of vertices
    void computeOrientedBox(std::vector<Vertex>& vertices);

    // Member variables
    Eigen::Vector3d min;
    Eigen::Vector3d max;
    Eigen::Vector3d extent;
    std::vector<Eigen::Vector3d> orientedPoints;
    std::string type;
};

#endif // BOUNDING_BOX_H
