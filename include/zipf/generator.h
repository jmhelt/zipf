#ifndef ZIPF_GENERATOR
#define ZIPF_GENERATOR

#include <cstdint>

namespace zipf {

class Generator {
   public:
    virtual ~Generator() {}
    virtual uint64_t sample() = 0;
};

}  // namespace zipf

#endif