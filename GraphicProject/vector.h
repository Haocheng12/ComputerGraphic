
#pragma once

#include <cmath>
#include <algorithm> 
#include <iostream>

using namespace std;

class Vec3 {
public:
    union {
        float v[3];
        struct { float x, y, z; };
    };

    Vec3() : x(0), y(0), z(0) {} 
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {} 

    Vec3 operator+(const Vec3& pVec) const {
        return Vec3(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2]);
    }
    Vec3 operator-(const Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }
    Vec3& operator+=(const Vec3& pVec) {
        v[0] += pVec.v[0];
        v[1] += pVec.v[1];
        v[2] += pVec.v[2];
        return *this;
    }
    Vec3 operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }


    Vec3 normalize() const {
        float len = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
        if (len > 0.0f) {
            float invLen = 1.0f / len;
            return Vec3(v[0] * invLen, v[1] * invLen, v[2] * invLen);
        }
        return Vec3(0.0f, 0.0f, 0.0f); 
    }

    Vec3 operator-() const {
        return Vec3(-v[0], -v[1], -v[2]);
    }

    float normalize_GetLength() {
        float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
        if (length > 0.0f) {
            float invLen = 1.0f / length;
            v[0] *= invLen;
            v[1] *= invLen;
            v[2] *= invLen;
        }
        return length;
    }

    float Dot(const Vec3& pVec) const {
        return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];
    }

    Vec3 Cross(const Vec3& v1) const {
        return Vec3(
            v[1] * v1.v[2] - v[2] * v1.v[1],
            v[2] * v1.v[0] - v[0] * v1.v[2],
            v[0] * v1.v[1] - v[1] * v1.v[0]
        );
    }

    static Vec3 Max(const Vec3& v1, const Vec3& v2) {
        return Vec3(
            max(v1.v[0], v2.v[0]),
            max(v1.v[1], v2.v[1]),
            max(v1.v[2], v2.v[2])
        );
    }

    float Max() const {
        return max(x, max(y, z));
    }

    void print() const {
        cout << "Vec3(" << x << ", " << y << ", " << z << ")" << endl;
    }
};


class Vec4 {
public:
    union {
        float v[4];
        struct { float x, y, z, w; };
    };

  
    Vec4() : x(0), y(0), z(0), w(0) {} 
    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vec4 operator+(const Vec4& pVec) const {
        return Vec4(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2], v[3] + pVec.v[3]);
    }

    Vec4& operator+=(const Vec4& pVec) {
        v[0] += pVec.v[0];
        v[1] += pVec.v[1];
        v[2] += pVec.v[2];
        v[3] += pVec.v[3];
        return *this;
    }

    Vec4 operator-(const Vec4& pVec) const {
        return Vec4(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2], v[3] - pVec.v[3]);
    }

    Vec4 operator-() const {
        return Vec4(-v[0], -v[1], -v[2], -v[3]);
    }

    Vec4 operator*(float scalar) const {
        return Vec4(v[0] * scalar, v[1] * scalar, v[2] * scalar, v[3] * scalar);
    }

    Vec4 operator/(float scalar) const {
        if (scalar == 0.0f) {
            cerr << "Error: Division by zero!" << endl;
            return *this; 
        }
        float invScalar = 1.0f / scalar;
        return Vec4(v[0] * invScalar, v[1] * invScalar, v[2] * invScalar, v[3] * invScalar);
    }

    float Dot(const Vec4& pVec) const {
        return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2] + v[3] * pVec.v[3];
    }

    Vec4 normalize() const {
        float len = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
        if (len > 0.0f) {
            float invLen = 1.0f / len;
            return Vec4(v[0] * invLen, v[1] * invLen, v[2] * invLen, v[3] * invLen);
        }
        return Vec4(0.0f, 0.0f, 0.0f, 0.0f); 
    }

    float normalize_GetLength() {
        float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
        if (length > 0.0f) {
            float invLen = 1.0f / length;
            v[0] *= invLen;
            v[1] *= invLen;
            v[2] *= invLen;
            v[3] *= invLen;
        }
        return length;
    }

    static Vec4 Max(const Vec4& v1, const Vec4& v2) {
        return Vec4(
            max(v1.v[0], v2.v[0]),
            max(v1.v[1], v2.v[1]),
            max(v1.v[2], v2.v[2]),
            max(v1.v[3], v2.v[3])
        );
    }

    float Max() const {
        return max(max(x, y), max(z, w));
    }

    bool operator==(const Vec4& pVec) const {
        return (x == pVec.x && y == pVec.y && z == pVec.z && w == pVec.w);
    }

    void print() const {
        cout << "Vec4(" << x << ", " << y << ", " << z << ", " << w << ")" << endl;
    }
};



class Matrix
{
public:
    union
    {
      
        float a[4][4];
        float m[16];
    };
    Matrix() {
        identity();
    }
    Matrix(float x00, float x01, float x02, float x03, float x10, float x11, float x12, float x13, float x20, float x21, float x22, float x23, float x30, float x31, float x32, float x33) {
        a[0][0] = x00;
        a[0][1] = x01;
        a[0][2] = x02;
        a[0][3] = x03;
        a[1][0] = x10;
        a[1][1] = x11;
        a[1][2] = x12;
        a[1][3] = x13;
        a[2][0] = x20;
        a[2][1] = x21;
        a[2][2] = x22;
        a[2][3] = x23;
        a[3][0] = x30;
        a[3][1] = x31;
        a[3][2] = x32;
        a[3][3] = x33;
    }
    void identity() {
        memset(m, 0, 16 * sizeof(float));
        m[0] = 1.0f;
        m[5] = 1.0f;
        m[10] = 1.0f;
        m[15] = 1.0f;

    }

    Vec3 mulVec(const Vec3& v) const {
        return Vec3(
            v.x * a[0][0] + v.y * a[0][1] + v.z * a[0][2] + a[0][3],
            v.x * a[1][0] + v.y * a[1][1] + v.z * a[1][2] + a[1][3],
            v.x * a[2][0] + v.y * a[2][1] + v.z * a[2][2] + a[2][3]
        );
    }

    static Matrix tanslation(const Vec3& v) {
        Matrix mat;
        mat.a[0][3] = v.x;
        mat.a[1][3] = v.y;
        mat.a[2][3] = v.z;
        return mat;
    }
    static Matrix scaling(const Vec3& v) {
        Matrix mat;
        mat.m[0] = v.x;
        mat.m[5] = v.y;
        mat.m[15] = v.z;
        return mat;
    }
    static Matrix rotateX(float theta) {
        Matrix mat;
        float ct = cosf(theta);
        float st = sinf(theta);
        mat.m[5] = ct;
        return mat;
    }
    Matrix invert()
    {
        Matrix inv;
        inv.m[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
        inv.m[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
        inv.m[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
        inv.m[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
        inv.m[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
        inv.m[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
        inv.m[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
        inv.m[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
        inv.m[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
        inv.m[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
        inv.m[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
        inv.m[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
        inv.m[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
        inv.m[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
        inv.m[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
        inv.m[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
        float det = m[0] * inv.m[0] + m[1] * inv.m[4] + m[2] * inv.m[8] + m[3] * inv.m[12];
        if (det == 0) {
            // Handle this case
        }
        det = 1.0 / det;
        for (int i = 0; i < 16; i++) {
            inv.m[i] = inv.m[i] * det;
        }
        return inv;
    }

};


class Spherical {
public:
    
    float theta; 
    float phi;   
    float r;     
    Spherical() : theta(0), phi(0), r(1) {}

    
    Spherical(float t, float p) : theta(t), phi(p), r(1) {}

    
    static Spherical fromCartesian(float x, float y, float z) {
        float r = sqrt(x * x + y * y + z * z);
        float theta = acos(z / r);         
        float phi = atan2(y, x);          
        return Spherical(theta, phi);
    }

    
    void toCartesian(float& x, float& y, float& z) const {
        x = sin(theta) * cos(phi);  
        y = sin(theta) * sin(phi);  
        z = cos(theta);            
    }

  
    void print() const {
        std::cout << "Spherical Coordinates (theta, phi, r): ("
            << theta << ", " << phi << ", " << r << ")\n";
    }

    
    void printCartesian() const {
        float x, y, z;
        toCartesian(x, y, z);
        std::cout << "Cartesian Coordinates (x, y, z): ("
            << x << ", " << y << ", " << z << ")\n";
    }
};

class Colour {

   

public:
    float r, g, b, a;
    Colour() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {} 
    Colour(float red, float green, float blue, float alpha = 1.0f)
        : r(red), g(green), b(blue), a(alpha) {}

    Colour operator+(const Colour& colour) const {
        return Colour(r + colour.r, g + colour.g, b + colour.b, a + colour.a);
    }

    Colour operator*(const Colour& colour) const {
        return Colour(r * colour.r, g * colour.g, b * colour.b, a * colour.a);
    }

    Colour operator*(const float scalar) const {
        return Colour(r * scalar, g * scalar, b * scalar, a * scalar);
    }

    Colour operator/(const float scalar) const {
        if (scalar == 0) {
            std::cerr << "Error: Division by zero!" << std::endl;
            return *this; // Return the current color if division by zero
        }
        return Colour(r / scalar, g / scalar, b / scalar, a / scalar);
    }

    void print() const {
        std::cout << "Colour(r: " << r << ", g: " << g
            << ", b: " << b << ", a: " << a << ")\n";
    }
};

class ShadingFrame {
public:
    Vec3 x, y, z; 

    ShadingFrame(const Vec3& normal) {
        z = normal.normalize();
        Vec3 arbitrary = (fabs(z.x) > 0.9f) ? Vec3(0, 1, 0) : Vec3(1, 0, 0);
        y = arbitrary.Cross(z).normalize();
        x = z.Cross(y).normalize();
    }

    Vec3 toLocal(const Vec3& v) const {
        return Vec3(
            v.Dot(x),
            v.Dot(y),
            v.Dot(z)
        );
    }

    Vec3 toWorld(const Vec3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    void printBasis() const {
        cout << "Basis X: "; x.print();
        cout << "Basis Y: "; y.print();
        cout << "Basis Z: "; z.print();
    }
};

class Quaternion {
public:
    float w, x, y, z;

    Quaternion() : w(1), x(0), y(0), z(0) {}
    Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}
    Quaternion(float w, const Vec3& v) : w(w), x(v.x), y(v.y), z(v.z) {}

    static Quaternion fromAxisAngle(const Vec3& axis, float angle) {
        float halfAngle = angle * 0.5f;
        float sinHalfAngle = sinf(halfAngle);
        return Quaternion(cosf(halfAngle), axis.x * sinHalfAngle, axis.y * sinHalfAngle, axis.z * sinHalfAngle);
    }

    float norm() const {
        return sqrtf(w * w + x * x + y * y + z * z);
    }

    Quaternion normalize() const {
        float n = norm();
        if (n > 0.0f) {
            float invNorm = 1.0f / n;
            return Quaternion(w * invNorm, x * invNorm, y * invNorm, z * invNorm);
        }
        return Quaternion(1, 0, 0, 0); // Default unit quaternion
    }

    Quaternion conjugate() const {
        return Quaternion(w, -x, -y, -z);
    }

    Quaternion inverse() const {
        return conjugate() * (1.0f / (norm() * norm()));
    }
    Quaternion operator+(const Quaternion& q) const {
        return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
    }

    Quaternion operator-(const Quaternion& q) const {
        return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
    }
    Quaternion operator*(const Quaternion& q) const {
        return Quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
        );
    }

    Quaternion operator*(float scalar) const {
        return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);
    }

    static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t) {
        float dot = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;

        if (dot < 0.0f) {
            dot = -dot;
            return slerp(q1, q2 * -1.0f, t);
        }

        const float threshold = 0.9995f;
        if (dot > threshold) {
            Quaternion result = (q1 * (1.0f - t) + q2 * t).normalize();
            return result;
        }

        float theta_0 = acosf(dot);
        float theta = theta_0 * t;
        Quaternion q3 = (q2 - q1 * dot).normalize();
        return q1 * cosf(theta) + q3 * sinf(theta);
    }

    void toMatrix3x3(float matrix[3][3]) const {
        float xx = x * x, yy = y * y, zz = z * z;
        float xy = x * y, xz = x * z, yz = y * z;
        float wx = w * x, wy = w * y, wz = w * z;

        matrix[0][0] = 1.0f - 2.0f * (yy + zz);
        matrix[0][1] = 2.0f * (xy - wz);
        matrix[0][2] = 2.0f * (xz + wy);

        matrix[1][0] = 2.0f * (xy + wz);
        matrix[1][1] = 1.0f - 2.0f * (xx + zz);
        matrix[1][2] = 2.0f * (yz - wx);

        matrix[2][0] = 2.0f * (xz - wy);
        matrix[2][1] = 2.0f * (yz + wx);
        matrix[2][2] = 1.0f - 2.0f * (xx + yy);
    }

    void print() const {
        cout << "Quaternion(" << w << ", " << x << ", " << y << ", " << z << ")" << endl;
    }
};

