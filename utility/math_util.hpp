#pragma once

namespace ishihalib {

constexpr double PI = 3.141592653589793238462643383279502884;

static inline double deg_to_rad(double deg) { return deg * PI / 180.; }
static inline float deg_to_rad(float deg) { return deg * static_cast<float>(PI) / 180.f; }

static inline double rad_to_deg(double rad) { return rad * 180. / PI; }
static inline float rad_to_deg(float rad) { return rad * 180.f / static_cast<float>(PI); }

template <typename T>
static inline T narrowRange(T x, T max, T min) {
	if (x > max) return max;
	if (x < min) return min;
	return x;
}

static inline double normalize_Rad(double rad) {  //-PI<rad<PIに直す
	while (rad <= -PI) rad += 2 * PI;
	while (rad > PI) rad -= 2 * PI;
	return rad;
}

static inline double normalize_Deg(double deg) {  //-180<deg<=180に直す
	while (deg <= -180.) deg += 2 * 180.;
	while (deg > 180.) deg -= 2 * 180;
	return deg;
}
};	// namespace ishihalib