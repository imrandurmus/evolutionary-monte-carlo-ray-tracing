//
// Created by İmran Durmuş on 5.01.2026.
//

#ifndef GA_H
#define GA_H


#include "ga.h"
#include <vector>
#include "rtweekend.h"


struct Individual {
    std::vector<int> spp_map;
    double fitness;
};


//Time Complexity: O(N)
// Space Complexity: O(N) N being the number pixels because it will resize the spp_map vector into that size
Individual rand_ind(int pixel_count, int max_spp) {
    Individual ind;

    //O(n) resizes the vector to the amount of pixels, so n being the amount of pixels
    ind.spp_map.resize(pixel_count);

    //O(n) n being the number of elements inside the spp_map vector
    for (int i = 0; i < pixel_count; i++) {
        ind.spp_map.at(i) = 1 + int(random_double() * max_spp);
    }

    //making the fitness the worst by default so that non-evaluated individuals don't sneak in
    ind.fitness = 1e30;

    return ind;
}

//Time Complexity: O(N) N being every gene which is every pixel
//Space Complexity: O(1) no change constant space
inline void mutate(Individual& ind, int max_spp)
{
    for (int i = 0; i < ind.spp_map.size(); i++) {
        if (random_double() < 0.5) {        // 50% chance per pixel
            int d = (random_double() < 0.5) ? -1 : 1;
            ind.spp_map.at(i) += d;

            // Clamp to valid range
            if (ind.spp_map.at(i) < 1) ind.spp_map.at(i) = 1;
            if (ind.spp_map.at(i) > max_spp) ind.spp_map.at(i) = max_spp;
        }
    }
}

//Time Complexity: O(1) just comparison
//Space Complexity: 0(1) no change in space
Individual tournament(const Individual& a, const Individual& b)
{
    return (a.fitness < b.fitness) ? a : b;
}


#endif //GA_H
