#pragma once

#undef M_PI
#define M_PI         3.14159265358979323846f
#define M_TWOPI		 6.28318530717958647692f
#define INV_PI       0.31830988618379067154f
#define INV_TWOPI    0.15915494309189533577f

#define Epsilon 1e-3f

inline float radToDeg(float rad) {
	return rad * (180.f / M_PI);
}

inline float degToRad(float deg) {
	return deg * (M_PI / 180.f);
}

inline float modAngle(float deg) {
	deg = fmodf(deg, 360);
	if (deg < 0)
		deg += 360.f;
	return deg;
}

inline float lerp(float t, float v1, float v2) {
	return (1.f - t) * v1 + t * v2;
}

inline float clamp(float v, float low, float high) {
	if (v < low) return low;
	if (v > high) return high;
	return v;
}

inline int clamp(int v, int low, int high) {
	if (v < low) return low;
	if (v > high) return high;
	return v;
}

inline float linearTween(float t, float b, float c, float d) {
	return c * t / d + b;
}

inline float easeInOutQuad(float t, float b, float c, float d) {
	t /= d / 2;
	if (t < 1) return c / 2 * t * t + b;
	t--;
	return -c / 2 * (t *  (t - 2) - 1) + b;
}

inline float norm(const sf::Vector2f& v) {
	return std::sqrt(v.x * v.x + v.y * v.y);
}

inline void normalize(sf::Vector2f& v) {
	float normV = norm(v);
	if (normV == 0) return;
	v.x /= normV;
	v.y /= normV;
}

inline sf::Vector2f normalized(const sf::Vector2f& v) {
	sf::Vector2f n = v;
	normalize(n);
	return n;
}

inline float angle(const sf::Vector2f& v1, const sf::Vector2f& v2) {
	float dot = v1.x * v2.x + v1.y * v2.y;
	float det = v1.x * v2.y - v1.y * v2.x;
	return std::atan2(det, dot);
}

inline float dist(const sf::Vector2f& v1, const sf::Vector2f& v2) {
	return norm(v1 - v2);
}

inline float randomFloat(float low, float high) {
	return low + static_cast<float> (rand()) / (static_cast<float> (RAND_MAX / (high - low)));
}

inline int round_int(float r) {
	return static_cast<int>((r > 0.0) ? (r + 0.5) : (r - 0.5));
}

inline bool epsIntersect(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
	return !(rect2.left + Epsilon >= rect1.left + rect1.width
		|| rect1.left + Epsilon >= rect2.left + rect2.width
		|| rect2.top + Epsilon >= rect1.top + rect1.height
		|| rect1.top + Epsilon >= rect2.top + rect2.height);
}

inline bool fastIntersect(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
	return !(rect2.left >= rect1.left + rect1.width
		|| rect1.left >= rect2.left + rect2.width
		|| rect2.top >= rect1.top + rect1.height
		|| rect1.top >= rect2.top + rect2.height);
}

inline bool fastIntersect(const sf::FloatRect& bb, const sf::Vector2f& center, float radius) {
	sf::Vector2f pc = center;
	if (pc.x > bb.left + bb.width) pc.x = bb.left + bb.width;
	if (pc.x < bb.left) pc.x = bb.left;
	if (pc.y > bb.top + bb.height) pc.y = bb.top + bb.height;
	if (pc.y < bb.top) pc.y = bb.top;

	return dist(pc, center) < radius;
}

inline std::string removeDigits(const std::string& s) {
	std::string removed = s;
	removed.erase(std::remove_if(removed.begin(), removed.end(), [](char c) {return isdigit(c); }), removed.end());
	return removed;
}

template <typename T>
inline bool contains(const std::set<T>& c, T item) {
	return c.find(item) != c.end();
}

template <typename T, typename U>
inline bool contains(const std::map<T, U>& c, T item) {
	return c.find(item) != c.end();
}

template <typename T>
inline bool contains(const std::vector<T>& c, T item) {
	return std::find(c.begin(), c.end(), item) != c.end();
}

// convenience method for updating time values 
// The frame time is subtracted from the time but 
// if the time falls below sf::Time::Zero, it is set to sf::Time::Zero.
inline void updateTime(sf::Time& time, const sf::Time& frameTime) {
	if (time == sf::Time::Zero) return;
	time -= frameTime;
	if (time < sf::Time::Zero) time = sf::Time::Zero;
}