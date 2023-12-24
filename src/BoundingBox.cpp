#include <iostream>
#include <iomanip>
#include "BoundingBox.h"

#include "Eigen/Eigenvalues"
#include "Vertex.h"

BoundingBox::BoundingBox() : min(Eigen::Vector3d::Zero()),
                             max(Eigen::Vector3d::Zero()),
                             extent(Eigen::Vector3d::Zero()) {
}

BoundingBox::BoundingBox(const Eigen::Vector3d& min0, const Eigen::Vector3d& max0) : min(min0),
                                                                                     max(max0) {
    extent = max - min;
}

BoundingBox::BoundingBox(const Eigen::Vector3d& p) : min(p),
                                                     max(p) {
    extent = max - min;
}

// Overloading
void BoundingBox::expandToInclude(const Eigen::Vector3d& p) {
    if (min.x() > p.x()) min.x() = p.x();
    if (min.y() > p.y()) min.y() = p.y();
    if (min.z() > p.z()) min.z() = p.z();

    if (max.x() < p.x()) max.x() = p.x();
    if (max.y() < p.y()) max.y() = p.y();
    if (max.z() < p.z()) max.z() = p.z();

    extent = max - min;
}

// Overloading
void BoundingBox::expandToInclude(const BoundingBox& b) {
    if (min.x() > b.min.x()) min.x() = b.min.x();
    if (min.y() > b.min.y()) min.y() = b.min.y();
    if (min.z() > b.min.z()) min.z() = b.min.z();

    if (max.x() < b.max.x()) max.x() = b.max.x();
    if (max.y() < b.max.y()) max.y() = b.max.y();
    if (max.z() < b.max.z()) max.z() = b.max.z();

    extent = max - min;
}

int BoundingBox::maxDimension() const {
    if (type == "Oriented") {
        return -1;
    }

    int result = 0;
    if (extent.y() > extent.x()) result = 1;
    if (extent.z() > extent.y() && extent.z() > extent.x()) result = 2;

    return result;
}

bool BoundingBox::contains(const BoundingBox& boundingBox, double& dist) const {
    Eigen::Vector3d bMin = boundingBox.min;
    Eigen::Vector3d bMax = boundingBox.max;

    if (((min.x() <= bMin.x() && bMin.x() <= max.x()) || (bMin.x() <= min.x() && min.x() <= bMax.x())) &&
        ((min.y() <= bMin.y() && bMin.y() <= max.y()) || (bMin.y() <= min.y() && min.y() <= bMax.y())) &&
        ((min.z() <= bMin.z() && bMin.z() <= max.z()) || (bMin.z() <= min.z() && min.z() <= bMax.z()))) {
        Eigen::Vector3d v = ((min + max) / 2) - ((bMin + bMax) / 2);
        dist = v.norm();
        return true;
    }

    return false;
}

void BoundingBox::computeAxisAlignedBox(std::vector<Vertex>& vertices) {
    type = "Axis Aligned";

    min.setZero();
    max.setZero();

    for (VertexIter v = vertices.begin(); v != vertices.end(); v++) {
        expandToInclude(v->position);
    }


    std::cout << "(C++) Min: [" << std::fixed << std::setprecision(8) << min.transpose() << "]" << std::endl;
    std::cout << "(C++) Max: [" << std::fixed << std::setprecision(8) << max.transpose() << "]" << std::endl;
    std::cout << "(C++) Extent: [" << std::fixed << std::setprecision(8) << extent.transpose() << "]" << std::endl;
}

// Use PCA to compute the oriented bounding box
void BoundingBox::computeOrientedBox(std::vector<Vertex>& vertices) {
    type = "Oriented";
    orientedPoints.clear();

    // Compute the mean of the vertices
    Eigen::Vector3d center = Eigen::Vector3d::Zero();
    for (const auto& v : vertices) {
        center += v.position;
    }
    center /= static_cast<double>(vertices.size());

    // Adjust vertices based on the mean and compute covariance matrix
    Eigen::Matrix3d covariance = Eigen::Matrix3d::Zero();
    for (const auto& v : vertices) {
        Eigen::Vector3d adjustedPosition = v.position - center;
        covariance += adjustedPosition * adjustedPosition.transpose();
    }
    covariance /= static_cast<double>(vertices.size());

    // Perform PCA - compute the eigenvectors
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eigenSolver(covariance);
    Eigen::Matrix3d eigenVectors = eigenSolver.eigenvectors();

    // Initialize min and max values for each principal component
    Eigen::Vector3d minVals = Eigen::Vector3d::Constant(INFINITY);
    Eigen::Vector3d maxVals = Eigen::Vector3d::Constant(-INFINITY);

    // Project the vertices onto the principal components and find min/max
    for (const auto& v : vertices) {
        Eigen::Vector3d projected = eigenVectors.transpose() * (v.position - center);
        minVals = minVals.cwiseMin(projected);
        maxVals = maxVals.cwiseMax(projected);
    }

    // Construct the oriented bounding box points
    for (int i = 0; i < 3; ++i) {
        orientedPoints.push_back(center + eigenVectors.col(i) * minVals[i]);
        orientedPoints.push_back(center + eigenVectors.col(i) * maxVals[i]);
    }

    // Print the oriented points
    std::cout << "(C++) Oriented Point Xmin: [" << orientedPoints[0].x() << ", " << orientedPoints[0].y() << ", " << orientedPoints[0].z() << "]" << std::endl;
    std::cout << "(C++) Oriented Point Xmax: [" << orientedPoints[1].x() << ", " << orientedPoints[1].y() << ", " << orientedPoints[1].z() << "]" << std::endl;
    std::cout << "(C++) Oriented Point Ymin: [" << orientedPoints[2].x() << ", " << orientedPoints[2].y() << ", " << orientedPoints[2].z() << "]" << std::endl;
    std::cout << "(C++) Oriented Point Ymax: [" << orientedPoints[3].x() << ", " << orientedPoints[3].y() << ", " << orientedPoints[3].z() << "]" << std::endl;
    std::cout << "(C++) Oriented Point Zmin: [" << orientedPoints[4].x() << ", " << orientedPoints[4].y() << ", " << orientedPoints[4].z() << "]" << std::endl;
    std::cout << "(C++) Oriented Point Zmax: [" << orientedPoints[5].x() << ", " << orientedPoints[5].y() << ", " << orientedPoints[5].z() << "]" << std::endl;
}

/* Old compute have a little fault */
// void BoundingBox::computeOrientedBox(std::vector<Vertex>& vertices) {
//     type = "Oriented";
//     orientedPoints.clear();

//     // compute mean
//     Eigen::Vector3d center;
//     center.setZero();
//     for (VertexCIter v = vertices.begin(); v != vertices.end(); v++) {
//         center += v->position;
//     }
//     center /= (double)vertices.size();

//     // adjust for mean and compute covariance
//     Eigen::Matrix3d covariance;
//     covariance.setZero();
//     for (VertexIter v = vertices.begin(); v != vertices.end(); v++) {
//         Eigen::Vector3d pAdg = v->position - center;
//         covariance += pAdg * pAdg.transpose();
//     }
//     covariance /= (double)vertices.size();

//     // compute eigenvectors for the covariance matrix
//     Eigen::EigenSolver<Eigen::Matrix3d> solver(covariance);
//     Eigen::Matrix3d eigenVectors = solver.eigenvectors().real();

//     // project min and max points on each principal axis
//     double min1 = INFINITY, max1 = -INFINITY;
//     double min2 = INFINITY, max2 = -INFINITY;
//     double min3 = INFINITY, max3 = -INFINITY;
//     double d = 0.0;
//     eigenVectors.transpose();
//     for (VertexIter v = vertices.begin(); v != vertices.end(); v++) {
//         d = eigenVectors.row(0).dot(v->position);
//         if (min1 > d) min1 = d;
//         if (max1 < d) max1 = d;

//         d = eigenVectors.row(1).dot(v->position);
//         if (min2 > d) min2 = d;
//         if (max2 < d) max2 = d;

//         d = eigenVectors.row(2).dot(v->position);
//         if (min3 > d) min3 = d;
//         if (max3 < d) max3 = d;
//     }

//     // add points to vector
//     orientedPoints.push_back(eigenVectors.row(0) * min1);
//     orientedPoints.push_back(eigenVectors.row(0) * max1);
//     orientedPoints.push_back(eigenVectors.row(1) * min2);
//     orientedPoints.push_back(eigenVectors.row(1) * max2);
//     orientedPoints.push_back(eigenVectors.row(2) * min3);
//     orientedPoints.push_back(eigenVectors.row(2) * max3);


//     // Print orientedPoints
//     std::cout << "(C++) Oriented Point Xmin: [" << orientedPoints[0].x() << ", " << orientedPoints[0].y() << ", " << orientedPoints[0].z() << "]" << std::endl;
//     std::cout << "(C++) Oriented Point Xmax: [" << orientedPoints[1].x() << ", " << orientedPoints[1].y() << ", " << orientedPoints[1].z() << "]" << std::endl;
//     std::cout << "(C++) Oriented Point Ymin: [" << orientedPoints[2].x() << ", " << orientedPoints[2].y() << ", " << orientedPoints[2].z() << "]" << std::endl;
//     std::cout << "(C++) Oriented Point Ymax: [" << orientedPoints[3].x() << ", " << orientedPoints[3].y() << ", " << orientedPoints[3].z() << "]" << std::endl;
//     std::cout << "(C++) Oriented Point Zmin: [" << orientedPoints[4].x() << ", " << orientedPoints[4].y() << ", " << orientedPoints[4].z() << "]" << std::endl;
//     std::cout << "(C++) Oriented Point Zmax: [" << orientedPoints[5].x() << ", " << orientedPoints[5].y() << ", " << orientedPoints[5].z() << "]" << std::endl;
//     // for (const auto& point : orientedPoints) {
//     //     std::cout << "(C++) Oriented Point: (" << point.x() << ", " << point.y() << ", " << point.z() << ")" << std::endl;
//     // }

// }