#include "vector.hpp"

// Constructor and member function definitions
vec3::vec3(double i) : e{ i, i, i } {}
vec3::vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}
vec3::vec3(const vec3& v) : e{ v[0], v[1], v[2] } {}

vec3& vec3::operator=(const vec3& v) {
    if (this != &v) {
        e[0] = v.e[0];
        e[1] = v.e[1];
        e[2] = v.e[2];
    }
    return *this;
}

vec3::~vec3() = default;

double& vec3::operator[](int i) {
    if (i < 0 || i > 2) throw std::out_of_range("Index out of range");
    return e[i];
}

double vec3::operator[](int i) const {
    if (i < 0 || i > 2) throw std::out_of_range("Index out of range");
    return e[i];
}

double vec3::length() const {
    return std::sqrt(squared_length());
}

double vec3::squared_length() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

vec3 vec3::operator-() const {
    return vec3(-e[0], -e[1], -e[2]);
}


vec3& vec3::operator+=(const vec3& v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

vec3& vec3::operator+=(const double t) {
    e[0] += t;
    e[1] += t;
    e[2] += t;
    return *this;
}

vec3& vec3::operator-=(const vec3& v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}

vec3& vec3::operator-=(const double t) {
    e[0] -= t;
    e[1] -= t;
    e[2] -= t;
    return *this;
}

vec3& vec3::operator*=(const double t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

vec3& vec3::operator/=(const double t) {
    if (t == 0) throw std::runtime_error("Division by zero");
    e[0] /= t;
    e[1] /= t;
    e[2] /= t;
    return *this;
}

double vec3::x() const { return e[0]; }
double vec3::y() const { return e[1]; }
double vec3::z() const { return e[2]; }

// Non-member function definitions
double dot(const vec3& u, const vec3& v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

vec3 unit_vector(vec3 v) {
    return v / v.length();
}

vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u[1] * v[2] - u[2] * v[1],
        u[2] * v[0] - u[0] * v[2],
        u[0] * v[1] - u[1] * v[0]);
}

vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

vec3 operator+(const vec3& u, const double t) {
    return vec3(u[0] + t, u[1] + t, u[2] + t);
}

vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

vec3 operator-(const vec3& u, const double t) {
    return vec3(u[0] - t, u[1] - t, u[2] - t);
}

vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

vec3 operator*(const vec3& u, const double t) {
    return vec3(u[0] * t, u[1] * t, u[2] * t);
}

vec3 operator*(const double t, const vec3& u)
{
	return u * t;
}

vec3 operator/(const vec3& u, const vec3& v) {
    if (v[0] == 0 || v[1] == 0 || v[2] == 0) throw std::runtime_error("Division by zero in vector division");
    return vec3(u[0] / v[0], u[1] / v[1], u[2] / v[2]);
}

vec3 operator/(const vec3& u, const double t) {
    if (t == 0) throw std::runtime_error("Division by zero");
    return vec3(u[0] / t, u[1] / t, u[2] / t);
}

std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}
