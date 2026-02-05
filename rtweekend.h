//
// Created by İmran Durmuş on 29.12.2025.
//

#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <cstdint>


inline unsigned int g_seed = 12345;

inline void seed_rng(unsigned int s) {
    g_seed = s;
}

//C++ Std usings
using std::make_shared;
using std::shared_ptr;

//Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    //Returns a random real in [0,1)
    //return std::rand() / (RAND_MAX + 1.0);


    //We have to have the same cycle of random numbers so that the optimization is not based on luck
    g_seed = 1664525 * g_seed + 1013904223; // LCG
    return (g_seed & 0x00FFFFFF) / double(0x01000000);
}

inline double random_double(double min, double max) {
    //Returns a random real in [min,max)

    return min + (max - min) * random_double();
}

//Common Headers
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"



#endif //RTWEEKEND_H
