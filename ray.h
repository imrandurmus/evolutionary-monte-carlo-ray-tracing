//
// Created by İmran Durmuş on 18.12.2025.
//

#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:
    ray() {}

    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    //The functions ray::origin() and ray::direction() both return an immutable reference to their members
    //Callers can either just use the reference directly, or make a mutable copy depending on their needs
    const point3& origin() const { return orig; }
    const vec3& direction() const { return dir; }

    point3 at(double t) const {
        return orig + t*dir;
    }

    private:
    point3 orig;
    vec3 dir;
};

#endif //RAY_H
