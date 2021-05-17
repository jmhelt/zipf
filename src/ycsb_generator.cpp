#include "zipf/ycsb_generator.h"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

namespace zipf {

YCSBGenerator::YCSBGenerator(long long num_elements, double theta)
    : num_elements_{num_elements},
      theta_{theta},
      zeta2theta_{zeta(2, theta_)},
      alpha_{1.0 / (1.0 - theta_)},
      zetan_{zeta(num_elements_, theta_)},
      eta_{(1 - std::pow(2.0 / num_elements_, 1 - theta)) /
           (1 - zeta2theta_ / zetan_)},
      rd_{},
      gen_{rd_()},
      dis_{0.0, 1.0} {
    if (num_elements_ <= 0) {
        throw std::invalid_argument(
            "number of elements is not strictly positive: " +
            std::to_string(num_elements_));
    }

    if (theta_ <= 0) {
        throw std::invalid_argument("theta is not strictly positive: " +
                                    std::to_string(theta_));
    }

    if (theta_ == 1.0) {
        throw std::invalid_argument("theta cannot equal 1.0");
    }
}

YCSBGenerator::~YCSBGenerator() {}

uint64_t YCSBGenerator::sample() {
    double u = U();
    double uz = u * zetan_;

    if (uz < 1.0) {
        return 1;
    }

    if (uz < 1.0 + std::pow(0.5, theta_)) {
        return 2;
    }

    uint64_t ret = 1 + static_cast<uint64_t>((num_elements_)*std::pow(
                           (eta_ * u) - eta_ + 1, alpha_));
    return ret;
}

double YCSBGenerator::U() { return dis_(gen_); }

double YCSBGenerator::zeta(uint64_t num_elements, double theta) {
    double sum = 0;
    for (uint64_t i = 0; i < num_elements; i++) {
        sum += 1 / (std::pow(i + 1, theta));
    }

    return sum;
}

}  // namespace zipf