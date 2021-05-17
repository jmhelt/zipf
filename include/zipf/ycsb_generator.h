#ifndef ZIPF_YCSB_GENERATOR
#define ZIPF_YCSB_GENERATOR

#include <cstdint>
#include <random>

#include "generator.h"

namespace zipf {

/**
 * This is a port of the Zipf generator used in the YCSB benchmark, which is
 * based on an algorithm by Jim Gray.
 *
 * A generator of a zipfian distribution. It produces a sequence of items, such
 * that some items are more popular than others, according to a zipfian
 * distribution. When you construct an instance of this class, you specify the
 * number of items in the set to draw from, either by specifying an itemcount
 * (so that the sequence is of items from 0 to itemcount-1) or by specifying a
 * min and a max (so that the sequence is of items from min to max inclusive).
 * After you construct the instance, you can change the number of items by
 * calling nextInt(itemcount) or nextLong(itemcount).
 *
 * Note that the popular items will be clustered together, e.g. item 0 is the
 * most popular, item 1 the second most popular, and so on (or min is the most
 * popular, min+1 the next most popular, etc.) If you don't want this
 * clustering, and instead want the popular items scattered throughout the item
 * space, then use ScrambledZipfKeySelector instead.
 *
 * Be aware: initializing this generator may take a uint64_t time if there are
 * lots of items to choose from (e.g. over a minute for 100 million objects).
 * This is because certain mathematical values need to be computed to properly
 * generate a zipfian skew, and one of those values (zeta) is a sum sequence
 * from 1 to n, where n is the itemcount. Note that if you increase the number
 * of items in the set, we can compute a new zeta incrementally, so it should be
 * fast unless you have added millions of items. However, if you decrease the
 * number of items, we recompute zeta from scratch, so this can take a uint64_t
 * time.
 *
 * The algorithm used here is from "Quickly Generating Billion-Record Synthetic
 * Databases", Jim Gray et al, SIGMOD 1994.
 */
class YCSBGenerator : public Generator {
   public:
    YCSBGenerator(long long num_elements, double exponent);
    ~YCSBGenerator();

    uint64_t sample() override;

   private:
    /** Number of elements. */
    const long long num_elements_;
    /** Exponent parameter of the distribution. */
    const double theta_;
    /**
     * Computed parameters for generating the distribution.
     */
    double zeta2theta_, alpha_, zetan_, eta_;

    /** Underlying source of randomness. */
    std::random_device rd_;
    /** Underlying random number generator */
    std::mt19937 gen_;
    /** Uniform generator */
    std::uniform_real_distribution<> dis_;

    double U();

    static double zeta(uint64_t num_elements, double theta);
};

}  // namespace zipf

#endif