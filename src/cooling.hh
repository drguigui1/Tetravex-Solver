#pragma once

#include <cmath>

// http://what-when-how.com/artificial-intelligence/a-comparison-of-cooling-schedules-for-simulated-annealing-artificial-intelligence/

enum CoolingType {
    EXP_MULT,
    LOG_MULT,
    LIN_MULT,
    QUADRATIC_MULT,
    LIN_ADDITIVE,
    QUADRATIC_ADDITIVE,
    EXP_ADDITIVE,
    TRIGO_ADDITIVE
};

// ================================
// Multiplicative Monotonic Cooling
// ================================

// Exponential multiplicative cooling schedule
float exp_mult_cooling(float tk_1, float t0, float alpha);

// Logarithmical multiplicative cooling schedule
float log_mult_cooling(float tk_1, float t0, float alpha);

// Linear multiplicative cooling schedule
float lin_mult_cooling(float tk_1, float t0, float alpha);

// Quadratic multiplicative cooling schedule
float quadratic_mult_cooling(float t0, float alpha, int k);

// ================================
// Additive Monotonic Cooling
// ================================

// Linear additive cooling schedule
float lin_additive_cooling(float t0, float tn, int n, int k);

// Quadratic additive cooling schedule
float quadratic_additive_cooling(float t0, float tn, int n, int k);

// Exponential additive cooling schedule
float exp_additive_cooling(float t0, float tn, int n, int k);

// Trigonometric additive cooling schedule
float trigo_additive_cooling(float t0, float tn, int n, int k);
