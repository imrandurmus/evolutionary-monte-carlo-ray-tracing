//
// Created by İmran Durmuş on 7.12.2025.
//

#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color) {
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    //Translate the [0,1] component values to the byte range [0,255].
    static const interval intensity(0.000,0.999);

    int rbyte = int(255.999 * intensity.clamp(r));
    int gbyte = int(255.999 * intensity.clamp(g));
    int bbyte = int(255.999 * intensity.clamp(b));

    out << rbyte << " " << gbyte << " " << bbyte << '\n';

}

#endif //COLOR_H
