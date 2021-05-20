
#include <unistd.h>

#include <iostream>
#include <map>
#include <string>

#include "zipf/rejection_inversion_generator.h"
#include "zipf/ycsb_generator.h"

void print_usage(char* argv[]) {
    std::cout << "Usage: " << argv[0] << " [-h]"
              << " -g <generator: ycsb,rejinv>"
              << " -e <num_elements>"
              << " -s <skew>"
              << " -n <num_samples>" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string generator = "";
    long long num_elements = -1;
    long long num_samples = -1;
    double skew = -1.0;

    char c;
    while ((c = getopt(argc, argv, "hg:n:e:s:n:")) != -1) {
        switch (c) {
            case 'g':
                generator = optarg;
                continue;
            case 'e':
                num_elements = std::stoll(optarg);
                continue;
            case 's':
                skew = std::stod(optarg);
                continue;
            case 'n':
                num_samples = std::stoll(optarg);
                continue;
            case -1:
                break;
            case '?':
            case 'h':
            default:
                print_usage(argv);
                return 1;
        }
    }

    if (num_elements <= 0 || skew <= 0.0 || num_samples <= 0) {
        print_usage(argv);
        return 1;
    }

    zipf::Generator* gen = nullptr;

    if (generator == "ycsb") {
        gen = new zipf::YCSBGenerator(num_elements, skew);
    } else if (generator == "rejinv") {
        gen = new zipf::RejectionInverstionGenerator(num_elements, skew);
    } else {
        print_usage(argv);
        return 1;
    }

    std::map<uint64_t, uint64_t> counts;
    for (long long i = 0; i < num_samples; ++i) {
        uint64_t sample = gen->sample();
        counts[sample] += 1;
    }

    for (long long i = 1; i <= num_elements; ++i) {
        auto search = counts.find(static_cast<uint64_t>(i));
        if (search == counts.end()) {
            std::cout << std::to_string(i) << " " << std::to_string(0)
                      << std::endl;
        } else {
            std::cout << std::to_string(i) << " "
                      << std::to_string(search->second) << std::endl;
        }
    }

    delete gen;

    return 0;
}