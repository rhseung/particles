#include <NumCpp.hpp>
#include "../Particles.hpp"

int main() {
    nc::NdArray<double> n = {1, 0, 0};
    nc::NdArray<double> r = {2, 3, 0};
    nc::NdArray<double> I = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    double m = 4;

    nc::NdArray<double> J = nc::append(n, nc::cross(r, n));
    nc::NdArray<double> M = nc::zeros<double>(6, 6);
    M.put(nc::Slice(3), nc::Slice(3), m * nc::eye<double>(3));
    M.put(M.cSlice(3), M.cSlice(3), I);

    nc::NdArray<double> v_old = {1, 2, 0};
    double h = 1. / 60;
    nc::NdArray<double> g = {0, 9.8, 0};
    auto lambda = -nc::linalg::inv(J * nc::linalg::inv(M) * J.transpose()) * J * (v_old + h * g);
    std::cout << lambda << std::endl;

    return 0;
}
