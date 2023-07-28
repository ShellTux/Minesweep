#include <cstdint>

using namespace std;

class Vector {
   public:
    Vector(void);
    Vector(const int64_t i, const int64_t j);
    int64_t get_i(void) const;
    int64_t get_j(void) const;
    bool operator==(const Vector other) const;
    bool operator<(const Vector other) const;
    Vector operator+(const Vector other);
    Vector& operator+=(const Vector& other);
    Vector constrain(const int64_t min_i, const int64_t max_i,
                     const int64_t min_j, const int64_t max_j);
    size_t magnitude_squared(void) const;
    ~Vector();

   private:
    int64_t i{0}, j{0};
};
