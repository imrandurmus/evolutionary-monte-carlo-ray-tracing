#include <iostream>

#include <fstream> // is the file stream it both inputs and outputs file contents

#include <cmath> // for the math functions

//These are the functions of the ray tracer which I mainly implemented from the book
// in this project we are trying to optimize the ray tracer and we will treat it as an external part of the project
//Though I will include "camera.h" unlike the other headers because it is utilized quite often
#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

//These are the actual project headers, the optimization part
#include "ga.h"
#include "fitness.h"





int main() {
    // Scene
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1),0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1),100));

    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 128;

    // Render the reference image
    // Time Complexity: O(N * Rref)
    // N = number of pixels, Rref = samples per pixel for reference
    // Space Complexity: O(N) for storing the reference image
    cam.samples_per_pixel = 1000;
    auto reference = cam.render_to_buffer(world);

    int image_height = int(cam.image_width / cam.aspect_ratio);
    int pixel_count = cam.image_width * image_height;
    int max_spp = 120;


    //Time Complexity: O(N) N being the amount of pixels per individual
    Individual a = rand_ind(pixel_count, max_spp);
    Individual b = rand_ind(pixel_count, max_spp);


    // Genetic Algorithm loop
    // Runs G generations
    // Time Complexity per generation: O(N * Ravg)
    // Ravg = average samples per pixel during GA
    // Total Time Complexity: O(G * N * Ravg)
    // Space Complexity: O(N) for images and individuals
    for (int gen = 0; gen < 2; gen++) {
//5.73256e-05
        //5.1133e-05

        // Render individual A
        // Time Complexity: O(N * Ravg)
        // Space Complexity: O(N)
        cam.spp_map = a.spp_map;
        auto imgA = cam.render_to_buffer(world);
        a.fitness = compute_mse(reference, imgA);


        // Render individual B
        // Time Complexity: O(N * Ravg)
        // Space Complexity: O(N)
        cam.spp_map = b.spp_map;
        auto imgB = cam.render_to_buffer(world);
        b.fitness = compute_mse(reference, imgB);

        // Tournament selection
        // Time Complexity: O(1)
        // Space Complexity: O(1)
        Individual winner = tournament(a,b);

        // Mutation
        // Time Complexity: O(N)
        // Space Complexity: O(1)
        Individual child = winner;
        mutate(child, max_spp);

        // Replace population
        // Time Complexity: O(1)
        // Space Complexity: O(1)
        a = winner;
        b = child;

        std::cout << "Generation " << gen
                  << " best error = " << winner.fitness << "\n";
    }

    // Select final best individual
    // Time Complexity: O(1)
    // Space Complexity: O(1)
    Individual best = tournament(a, b);

    // Render final GA optimized image
    // Time Complexity: O(N * Ravg)
    // Space Complexity: O(N)
    cam.spp_map = best.spp_map;
    auto final_img = cam.render_to_buffer(world);

    // Save final image to file
    // Time Complexity: O(N)
    // Space Complexity: O(1)
    save_ppm(final_img, cam.image_width, image_height, "ga_result.ppm");


}
