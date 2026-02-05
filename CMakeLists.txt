cmake_minimum_required(VERSION 3.29)
project(Ray_Tracer)

set(CMAKE_CXX_STANDARD 20)

add_executable(Ray_Tracer main.cpp
        vec3.h
        color.h
        ray.h
        hittable.h
        sphere.h
        hittable_list.h
        rtweekend.h
        interval.h
        camera.h
        fitness.cpp
        fitness.h
        ga.cpp
        ga.h
)
