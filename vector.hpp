#pragma once

#include <iostream>
#include <stdexcept>
#include <cmath>

class vec3 {
public:
    vec3(double i = 0.0);
    vec3(double e0, double e1, double e2);
    vec3(const vec3& v);

    vec3& operator=(const vec3& v);

    ~vec3();

    double& operator[](int i);
    double operator[](int i) const;

    double length() const;
    double squared_length() const;

    vec3 operator-() const;

    vec3& operator+=(const vec3& v);
    vec3& operator+=(const double t);
    vec3& operator-=(const vec3& v);
    vec3& operator-=(const double t);
    vec3& operator*=(const double t);
    vec3& operator/=(const double t);

    double x() const;
    double y() const;
    double z() const;

private:
    double e[3];
};

// Non-member function declarations
double dot(const vec3& u, const vec3& v);
vec3 unit_vector(vec3 v);
vec3 cross(const vec3& u, const vec3& v);

vec3 operator+(const vec3& u, const vec3& v);
vec3 operator+(const vec3& u, const double t);
vec3 operator-(const vec3& u, const vec3& v);
vec3 operator-(const vec3& u, const double t);
vec3 operator*(const vec3& u, const vec3& v);
vec3 operator*(const vec3& u, const double t);
vec3 operator*(const double t, const vec3& u);
vec3 operator/(const vec3& u, const vec3& v);
vec3 operator/(const vec3& u, const double t);

std::ostream& operator<<(std::ostream& out, const vec3& v);
