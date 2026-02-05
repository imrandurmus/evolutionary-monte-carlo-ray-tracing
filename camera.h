//
// Created by İmran Durmuş on 4.01.2026.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

#include <vector>
#include <fstream>
#include <iostream>

//Implementations are taken from the book ray tracing in one weekend
class camera {
    public:
    double aspect_ratio = 16.0 / 9.0; //Ratio of image width over height
    int image_width = 400; //Rendered image width in pixel count
    int samples_per_pixel = 10; //Count of random samples for each pixel

    std::vector<int> spp_map;

    //This is the standard ray tracer, uniform rays
    //Time Complexity: O(N * R)
    //Space Complexity: O(1) no image stored, written directly to file
    void render (const hittable& world) {
        initialize();

        //Render
        std::string filename = "reference.ppm";

        std::ofstream out(filename);

        if (out.is_open()) {
            out << "P3\n" << image_width << " " << image_height << "\n255\n";

            for (int j= 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining " << (image_height - j) << ' ' << std::endl;
                for (int i = 0; i < image_width; i++) {
                    color pixel_color(0,0,0);
                    for(int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i,j);
                        pixel_color += ray_color(r, world);
                    }

                    write_color(out, pixel_samples_scale * pixel_color);
                }
            }
            std::clog << "\rDone.    \n";
            out.close();

            std::cout << "Image successfully rendered " << filename << "\n" ;

        }
        else {
            std::cout << "Unable to open file " << filename << "\n";
        }
    }

    //Time Complexity: O(N * Ravg) N being number of pixels and Ravg is the average amount of rays
    //Space Complexity: O(N)
    std::vector<color> render_to_buffer(const hittable& world) {
        initialize();

        std::vector<color> img;
        img.resize(image_width * image_height);

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining " << (image_height - j) << ' ' << std::endl;

            for (int i = 0; i < image_width; i++) {
                int spp = spp_for_pixel(i,j);
                double scale = 1.0/spp;

                color pixel_color(0,0,0);
                for (int s = 0; s < spp; s++) {
                    ray r = get_ray(i,j);
                    pixel_color += ray_color(r, world);
                }
                img.at(j * image_width + i) = pixel_color * scale;
            }

        }
        std::clog << "\rDone.    \n";
        return img;
    }


    //Time Complexity: O(N) N being the number of pixels
    //Space Complexity: O(1)
    void write_ppm(const std::string& filename, const std::vector<color>& image) {
        std::ofstream out(filename);
        if (!out.is_open()) {
            std::cerr << "Unable to open file " << filename << "\n";
            return;
        }

        out << "P3\n" << image_width << " " << image_height << "\n255\n";
        for (int j = 0; j < image_height; j++) {
            for (int i = 0; i < image_width; i++) {
                int idx = j * image_width + i;
                write_color(out,image.at(idx));
            }
        }
    }

private:
    int image_height; // Rendered image height
    double pixel_samples_scale; // Color scale factor for a sum of pixels
    point3 center;  // Camera center
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3 pixel_delta_u; // Offset to pixel to the right
    vec3 pixel_delta_v; // Offset to pixel below

    //Computes viewport geometry
    //Time Complexity: O(1)
    //Space Complexity: O(1)
    void initialize() {
        //Calculate the image height, and ensure that it's at least 1
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = point3(0, 0, 0);

        // Determine viewport dimensions.
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        //Calculate the horizontal and vertical delta vectors from pixel to pixel
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        //Calculate the upper left pixel
        auto viewport_upper_left = center
                                    -vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u+pixel_delta_v);
    }

    //VEctor math
    //Time Complexity: O(1)
    //Space Complexity: O(1)
    ray get_ray (int i, int j) const {
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around pixel location i,j

        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
                            + ((i + offset.x())* pixel_delta_u)
                            + ((j + offset.y())* pixel_delta_v) ;

        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    //One random vector
    //Time Complexity: O(1)
    //Space Complexity: O(1)
    vec3 sample_square() const {
        //Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(random_double()-0.5,random_double()-0.5,0);
    }

    //One ray object intersection test
    //Time Complexity: O(1)
    //Space Complexity: O(1)
    color ray_color(const ray& r, const hittable& world) const{
        hit_record rec;
        if(world.hit(r, interval(0,infinity), rec)) {
            return 0.5 * (rec.normal + color(1, 1, 1));
        }


        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }

    //Index look up
    //Time Complexity: O(1)
    //Space Complexity: O(1)
    int spp_for_pixel(int i, int j) const {
        if (spp_map.empty()) return samples_per_pixel;
        int idx = j * image_width + i;
        int v = spp_map.at(idx);
        return (v < 1) ? 1 : v;
    }
};

#endif //CAMERA_H
