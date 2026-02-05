# Adaptive Sampling in Monte Carlo Ray Tracing Using Evolutionary Hill Climbing
## Overview

Monte Carlo ray tracing is a physically based rendering technique that estimates pixel colors by randomly sampling light transport paths. While it produces high-quality images, it is computationally expensive because a large number of rays must be traced per pixel to reduce noise.

This project explores **adaptive per-pixel sampling** as an optimization problem. Instead of assigning the same number of samples to every pixel, we learn how to distribute samples across the image so that difficult regions receive more rays while easy regions receive fewer.

To achieve this, we treat the ray tracer as a black box and use an **evolutionary hill-climbing algorithm** to optimize the sampling pattern.

## Problem Context

Monte Carlo integration exhibits non-uniform variance across an image:

Smooth regions (sky, flat surfaces) converge quickly

Edges, silhouettes, and geometry boundaries require many more samples

Uniform sampling wastes computation on low-variance pixels.

The goal of this project is to learn where Monte Carlo integration is difficult and allocate samples accordingly.

## Optimization Formulation
### Decision Variables

A vector spp_map of length N (number of pixels)

Each element represents the number of samples per pixel (SPP)

### Objective Function

Minimize Mean Squared Error (MSE) between:

a high-quality reference image

a candidate image rendered with the current spp_map

### Constraints

1 ≤ spp ≤ max_spp for each pixel

### Optimization Method

(1+1) Evolutionary Strategy / Stochastic Hill Climbing

Mutation-only search with tournament selection

No crossover (not meaningful for per-pixel sampling maps)

## Methodology

Render a reference image using a high number of samples per pixel

Initialize random sampling maps (individuals)

Iteratively:

Render images using the candidate sampling maps

Evaluate fitness using MSE

Select the better individual

Apply small random mutations to generate a new candidate

After convergence, render the final image using the optimized sampling map

## Key Insight

The evolutionary process implicitly learns a variance map of the image:

Pixels that contribute more noise receive more samples

Easy pixels receive fewer samples

This allows near-reference image quality using significantly fewer total rays.

## Implementation Notes

Ray tracing core is based on Ray Tracing in One Weekend

The optimizer is implemented independently and treats the renderer as a black box

All optimization logic is contained in:

ga.h

fitness.h

## Time & Space Complexity (High Level)

Reference rendering: O(N · R_ref)

Per generation rendering: O(N · R_avg)

Total optimization: O(G · N · R_avg)

Memory usage: O(N)

Where:

N = number of pixels

R_ref = reference samples per pixel

R_avg = average samples per pixel during optimization

G = number of generations

## Output

High-quality reference image

Final GA-optimized image

Learned per-pixel sampling behavior
