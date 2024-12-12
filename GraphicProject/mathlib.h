
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
            
            return Vec3(v[0] / len, v[1] /len, v[2]/len);
        }
        return Vec3(0.0f, 0.0f, 0.0f); 
    }

    Vec3 operator-() const {
        return Vec3(-v[0], -v[1], -v[2]);
    }




    float Dot(const Vec3& pVec) const {
        return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];
    }

    Vec3 cross(const Vec3& other) const {
        return Vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
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
    Vec4 divideByW() const {
        if (w == 0.0f) {
            cerr << "Warning: Division by zero for homogeneous vector!" << endl;
            return *this;
        }
        return Vec4(x / w, y / w, z / w, 1.0f);
    }
};



class Matrix
{
public:
    union
    {
      
        float m[4][4];
        
    };
    Matrix() {
        identity();
    }
    Matrix(float x00, float x01, float x02, float x03, float x10, float x11, float x12, float x13, float x20, float x21, float x22, float x23, float x30, float x31, float x32, float x33) {
        m[0][0] = x00;
        m[0][1] = x01;
        m[0][2] = x02;
        m[0][3] = x03;
        m[1][0] = x10;
        m[1][1] = x11;
        m[1][2] = x12;
        m[1][3] = x13;
        m[2][0] = x20;
        m[2][1] = x21;
        m[2][2] = x22;
        m[2][3] = x23;
        m[3][0] = x30;
        m[3][1] = x31;
        m[3][2] = x32;
        m[3][3] = x33;
    }
    void identity() {
      
        memset(m, 0, 16 * sizeof(float));
        m[0][0] = 1.0f;
        m[1][1] = 1.0f;
        m[2][2] = 1.0f;
        m[3][3] = 1.0f;

    }

    Vec3 mulVec(const Vec3& v) const {
        return Vec3(
            v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + m[0][3],
            v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + m[1][3],
            v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + m[2][3]
        );
    }

    
     
    
   
    Matrix operator*(const Matrix& other) const {
        Matrix result;
        for (int i = 0; i < 4; ++i) { // Row of A
            for (int j = 0; j < 4; ++j) { // Column of B
                result.m[i][j] = 0;
                for (int k = 0; k < 4; ++k) { // Iterating over row-column pair
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }

    

    static Matrix translation(const Vec3& v) {
        Matrix mat;
        mat.m[0][3] = v.x;
        mat.m[1][3] = v.y;
        mat.m[2][3] = v.z;
        return mat;
    }
    static Matrix translation(float x, float y, float z) {
        Matrix result;
        result.m[0][3] = x;
        result.m[1][3] = y;
        result.m[2][3] = z;
        result.m[3][3] = 1.0f; // Homogeneous coordinate
        return result;
    }
    static Matrix scaling(const Vec3& scale) {
        Matrix result;
        result.m[0][0] = scale.x;
        result.m[1][1] = scale.y;
        result.m[2][2] = scale.z;
        result.m[3][3] = 1.0f; // Homogeneous coordinate
        return result;
    }
    static Matrix scaling(float uniformScale) {
        return scaling(Vec3(uniformScale, uniformScale, uniformScale));
    }

   
    static Matrix perspective(float fov, float aspect, float n, float f) {
        Matrix mat;
        float radians = 3.14159265359f / 180.0f * fov;
        float tanHalfFov = tanf(radians / 2.0f);

        mat.m[0][0] = 1.0f / (aspect * tanHalfFov);
        mat.m[1][1] = 1.0f / tanHalfFov;
        mat.m[2][2] = -(f + n) / (n-f);
        mat.m[3][2] = (2.0f * f * n) / (n - f);
        mat.m[2][3] = 1.0f;
        mat.m[3][3] = 0.0f;

        return mat;
    }
    //static Matrix lookAt(const Vec3& eye, const Vec3& target, const Vec3& up) {
    //    Vec3 forward = (target - eye).normalize();  // Forward = (eye - target)
    //    Vec3 right = up.cross(forward).normalize();  // Right = up x forward
    //    Vec3 correctedUp = forward.cross(right);    // CorrectedUp = right x forward

    //    Matrix view;

    //    // Construct the view matrix
    //    view.m[0][0] = right.x;
    //    view.m[0][1] = correctedUp.x;
    //    view.m[0][2] = -forward.x;
    //    view.m[0][3] = 0.0f;

    //    view.m[1][0] = right.y;
    //    view.m[1][1] = correctedUp.y;
    //    view.m[1][2] = -forward.y;
    //    view.m[1][3] = 0.0f;

    //    view.m[2][0] = right.z;
    //    view.m[2][1] = correctedUp.z;
    //    view.m[2][2] = -forward.z;
    //    view.m[2][3] = 0.0f;

    //    // Handle the small precision error for the dot product term
    //    const float epsilon = 1e-6f; // small threshold for precision errors
    //    view.m[3][0] = -right.Dot(eye);
    //    view.m[3][1] = -correctedUp.Dot(eye);
    //    view.m[3][2] = forward.Dot(eye);

    //    // If the value is too small, treat it as 0
    //    if (fabs(view.m[3][2]) < epsilon) {
    //        view.m[3][2] = 0.0f;
    //    }

    //    view.m[3][3] = 1.0f;

    //    return view;
    //}

    static Matrix lookAt(const Vec3& eye, const Vec3& target, const Vec3& up) {
        Vec3 forward = (eye - target).normalize();  // Forward = (eye - target)
        Vec3 right = up.cross(forward);  // Right = up x forward
        Vec3 correctedUp = forward.cross(right);    // CorrectedUp = right x forward

        Matrix view;

        // Construct the view matrix
        view.m[0][0] = right.x;
        view.m[0][1] = correctedUp.x;
        view.m[0][2] = forward.x;
        view.m[0][3] = 0.0f;

        view.m[1][0] = right.y;
        view.m[1][1] = correctedUp.y;
        view.m[1][2] = forward.y;
        view.m[1][3] = 0.0f;

        view.m[2][0] = right.z;
        view.m[2][1] = correctedUp.z;
        view.m[2][2] = forward.z;
        view.m[2][3] = 0.0f;

        // Handle the small precision error for the dot product term
        const float epsilon = 1e-6f; // small threshold for precision errors
        view.m[3][0] = eye.x;
        view.m[3][1] = eye.y;
        view.m[3][2] = eye.z;

        // If the value is too small, treat it as 0
        if (fabs(view.m[3][2]) < epsilon) {
            view.m[3][2] = 0.0f;
        }

        view.m[3][3] = 1.0f;

        return view;
    }

     Matrix transpose() const {
         Matrix result;
         for (int i = 0; i < 4; ++i) {
             for (int j = 0; j < 4; ++j) {
                 result.m[i][j] = m[j][i];
             }
         }
         return result;
     }

     

    void print() const {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                cout << m[i][j] << " ";
            }
            cout << endl;
        }
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
        y = arbitrary.cross(z).normalize();
        x = z.cross(y).normalize();
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
    float dot(const Quaternion& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }
    static Quaternion slerp(const Quaternion& start, const Quaternion& end, float t) {
        Quaternion q1 = start;
        Quaternion q2 = end;

        // Compute the dot product
        float dot = q1.dot(q2);

        // Ensure shortest path by flipping the sign if necessary
        if (dot < 0.0f) {
            dot = -dot;
            q2 = Quaternion(-q2.x, -q2.y, -q2.z, -q2.w);
        }

        const float epsilon = 0.001f; // Avoid division by zero or small-angle approximation

        // If the quaternions are very close, linearly interpolate
        if (dot > 1.0f - epsilon) {
            return Quaternion(
                q1.x * (1.0f - t) + q2.x * t,
                q1.y * (1.0f - t) + q2.y * t,
                q1.z * (1.0f - t) + q2.z * t,
                q1.w * (1.0f - t) + q2.w * t
            ).normalize();
        }

        // Compute the angle and its sine
        float theta_0 = acos(dot);        // Angle between the two quaternions
        float theta = theta_0 * t;       // Interpolated angle
        float sin_theta_0 = sin(theta_0);
        float sin_theta = sin(theta);

        // Compute weights
        float weight1 = sin(theta_0 - theta) / sin_theta_0;
        float weight2 = sin(theta) / sin_theta_0;

        // Compute the result quaternion
        return Quaternion(
            q1.x * weight1 + q2.x * weight2,
            q1.y * weight1 + q2.y * weight2,
            q1.z * weight1 + q2.z * weight2,
            q1.w * weight1 + q2.w * weight2
        );
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
    Matrix toMatrix() const {
        Matrix result;

        float xx = x * x, yy = y * y, zz = z * z;
        float xy = x * y, xz = x * z, yz = y * z;
        float wx = w * x, wy = w * y, wz = w * z;

        result.m[0][0] = 1.0f - 2.0f * (yy + zz);
        result.m[0][1] = 2.0f * (xy - wz);
        result.m[0][2] = 2.0f * (xz + wy);
        result.m[0][3] = 0.0f;

        result.m[1][0] = 2.0f * (xy + wz);
        result.m[1][1] = 1.0f - 2.0f * (xx + zz);
        result.m[1][2] = 2.0f * (yz - wx);
        result.m[1][3] = 0.0f;

        result.m[2][0] = 2.0f * (xz - wy);
        result.m[2][1] = 2.0f * (yz + wx);
        result.m[2][2] = 1.0f - 2.0f * (xx + yy);
        result.m[2][3] = 0.0f;

        result.m[3][0] = 0.0f;
        result.m[3][1] = 0.0f;
        result.m[3][2] = 0.0f;
        result.m[3][3] = 1.0f;

        return result;
    }

    void print() const {
        cout << "Quaternion(" << w << ", " << x << ", " << y << ", " << z << ")" << endl;
    }
};

