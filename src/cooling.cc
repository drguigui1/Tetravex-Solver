#include "cooling.hh"

// ================================
// Multiplicative Monotonic Cooling
// ================================

// Exponential multiplicative cooling schedule
// Tk = T0 * alpha^k
// alpha between 0.8 and 0.9
// k = (log(Tk-1) - log(T0)) / log(alpha) + 1
float exp_mult_cooling(float tk_1, float t0, float alpha) {
    float k = (log(tk_1) - log(t0)) / log(alpha) + 1;
    return t0 * pow(alpha, k);
}

// Logarithmical multiplicative cooling
// Tk = T0 / (1 + alpha * log(1+k))
// alpha > 1
// k = exp(((T0 / Tk-1) - 1)1/alpha)
float log_mult_cooling(float tk_1, float t0, float alpha) {
    float k = exp((t0 / tk_1 - 1) * 1.0f/alpha);
    return t0 / (1.0f + alpha * log(1.0f + k));
}

// Linear multiplicative cooling schedule
// Tk = t0 / (1 + alpha * k)
// alpha > 0
float lin_mult_cooling(float t0, float alpha, int k) {
    return t0 / (1.0f + alpha * k);
}

// Quadratic multiplicative cooling schedule
// Tk = T0 / (1 + alpha * k^2)
// alpha > 0
float quadratic_mult_cooling(float t0, float alpha, int k) {
    return t0 / (1.0f + alpha * pow(k, 2));
}

// ================================
// Additive Monotonic Cooling
// ================================

// Linear additive cooling schedule
// Tk = Tn + (T0 - Tn) * ((n-k) / n)
// Tn: final temperature
// n: number of cooling cycles
float lin_additive_cooling(float t0, float tn, int n, int k) {
    return tn + (t0 - tn) * ((n - k) / n);
}

// Quadratic additive cooling schedule
// Tk = Tn + (T0 - Tn) * ((n-k) / n)^2
float quadratic_additive_cooling(float t0, float tn, int n, int k) {
    return tn + (t0 - tn) * pow(((n - k) / n), 2);
}

// Exponential additive cooling schedule
float exp_additive_cooling(float t0, float tn, int n, int k) {
    float f = 1.0f / (1 + exp((2 * log(t0 - tn) / n) * (k - 0.5 * n)));
    return tn + (t0 - tn) * f;
}

// Trigonometric additive cooling schedule
float trigo_additive_cooling(float t0, float tn, int n, int k) {
    float f = 1 + cos((k * M_PI) / n);
    return tn + 0.5 * (t0 - tn) * f;
}
