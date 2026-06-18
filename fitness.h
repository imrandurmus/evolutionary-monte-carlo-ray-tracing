//
// Created by İmran Durmuş on 5.01.2026.
//

#ifndef FITNESS_H
#define FITNESS_H


#include "fitness.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "color.h"

//Calculates the Mean Squared Error
// Time Complexity O(N) n being the number of elements inside the vector, which depends on the size of the image in our instance the size of the image is not going to change
// Space Complexity: O(1) constant there is nothing scaling size-wise in this function.
double compute_mse(const std::vector<color>& ref,
                            const std::vector<color>& cand) {
    double error = 0.0;
    int n = ref.size();
    color a;
    color b;
    for(int i = 0; i < n; i++) {
        color a = ref.at(i);
        color b = cand.at(i);
        vec3 e = a - b;
        error += e.x()*e.x() + e.y()*e.y() + e.z()*e.z();
    }
    return error / n;
}


//Takes the image stored inside the memory to a .ppm file
//Time Complexity: O(N) N being width*height which is also the amount of pixels, the code goes through all the pixels in the buffer
//and writes them into a file
//Space Complexity: O(1) there's no size scaling with anything, the output file might grow depending on the N but that's not space complexity
void save_ppm(const std::vector<color>& img,
              int width,
              int height,
              const std::string& filename)
{
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Failed to write " << filename << "\n";
        return;
    }

    out << "P3\n" << width << " " << height << "\n255\n";

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            write_color(out, img.at(j * width + i));
        }
    }

    out.close();
    std::cout << "Saved " << filename << "\n";
}

void save_spp_map(const std::vector<int>& spp_map,
                  int width,
                  int height,
                  int max_spp,
                  const std::string& filename)
{
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Failed to write " << filename << "\n";
        return;
    }

    out << "P3\n" << width << " " << height << "\n255\n";

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int spp = spp_map.at(j * width + i);
            if (spp < 1) spp = 1;
            if (spp > max_spp) spp = max_spp;

            int gray = int(255.999 * double(spp) / max_spp);
            out << gray << " " << gray << " " << gray << "\n";
        }
    }

    out.close();
    std::cout << "Saved " << filename << "\n";
}


#endif //FITNESS_H
