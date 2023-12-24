#include <Eigen/Core>
#include <iostream>

int main() {
    Eigen::Vector3f v(1,2,3);
    Eigen::Vector3f w(1,0,0);
    v += w;
    std::cout << "v = " << v << std::endl;
}