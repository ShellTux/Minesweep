#include "vector.hpp"

Vector::Vector(void) {
    j = 0;
    j = 0;
};

Vector::Vector(const int64_t row, const int64_t col) {
    i = row;
    j = col;
};

int64_t Vector::get_i(void) const { return i; }
int64_t Vector::get_j(void) const { return j; }

bool Vector::operator==(const Vector other) const {
    return i == other.i && j == other.j;
};

bool Vector::operator<(const Vector other) const {
    return magnitude_squared() < other.magnitude_squared();
}

Vector Vector::operator+(const Vector other) {
    return Vector(i + other.i, j + other.j);
}

Vector& Vector::operator+=(const Vector& other) {
    i += other.i;
    j += other.j;
    return *this;
}

Vector Vector::constrain(const int64_t min_i, const int64_t max_i,
                         const int64_t min_j, const int64_t max_j) {
    i = i < min_i ? min_i : i;
    i = i > max_i ? max_i : i;
    j = j < min_j ? min_j : j;
    j = j > max_j ? max_j : j;
    return *this;
}

size_t Vector::magnitude_squared(void) const { return i * i + j * j; }

Vector::~Vector(){};
