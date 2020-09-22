#pragma once

/// \file Matrix.h
/// Matrix class description

#include "Vector2.h"
#include "Rectangle.h"
#include <array>
#include <algorithm>

//! namespace yang Contains all Yangine code
namespace yang
{

/// \class Matrix
/// Describes a 3x3 Matrix. Used as 2D transformation Matrix.
class Matrix
{
public:

    /// \brief Default constructor
    /// Constructs an identity 3x3 matrix
    Matrix()
    {
        m_matrix = { 1,0,0,
                     0,1,0,
                     0,0,1 };
    }

    /// \brief Construct a matrix from 9 elements
    /// \param a00 Element [0,0] of the matrix
    /// \param a01 Element [0,1] of the matrix
    /// \param a02 Element [0,2] of the matrix
    /// \param a10 Element [1,0] of the matrix
    /// \param a11 Element [1,1] of the matrix
    /// \param a12 Element [1,2] of the matrix
    /// \param a20 Element [2,0] of the matrix
    /// \param a21 Element [2,1] of the matrix
    /// \param a22 Element [2,2] of the matrix
    Matrix(float a00, float a01, float a02,
        float a10, float a11, float a12,
        float a20, float a21, float a22)
    {
        m_matrix = { a00, a01, a02,
                    a10, a11, a12,
                    a20, a21, a22 };
    }

    /// \brief Combines a scaling transform matrix with current
    /// 
    /// Modifies the current transform matrix and returns reference to itself 
    ///
    /// \param factors - Scaling factors
    /// \return Reference to this
    Matrix& Scale(const FVec2& factors)
    {
        return Combine(Matrix(factors.x, 0, 0,
                       0, factors.y, 0,
                       0, 0, 1));
    }

    Matrix& Scale(const FVec2& factors, const FVec2& center)
    {
        return Combine(Matrix(factors.x, 0, center.x * (1 - factors.x),
                              0, factors.y, center.y * (1 - factors.y),
                              0, 0, 1));
    }

    /// \brief Combines a rotation transform matrix with current
    /// 
    /// Modifies the current transform matrix and returns reference to itself 
    ///
    /// \param factors - angle to rotate in radians
    /// \return Reference to this
    Matrix& Rotate(float angle)
    {
        const float cos = std::cosf(angle);
        const float sin = std::sinf(angle);

        return Combine(Matrix(cos, -sin, 0,
                              sin, cos, 0,
                              0, 0, 1));

    }

    /// \brief Combines a rotation transform matrix with current
    /// 
    /// Modifies the current transform matrix and returns reference to itself 
    /// Mathematically the same as Translate(-center).Rotate(angle).Translate(center), but more efficient
    ///
    /// \param factors - angle to rotate in radians
    /// \param center - point to rotate about
    /// \return Reference to this
    Matrix& Rotate(float angle, const FVec2& center)
    {
        const float cos = std::cosf(angle);
        const float sin = std::sinf(angle);

        return Combine(Matrix(cos, -sin, center.x * (1 - cos) + center.y * sin,
                              sin, cos, center.y * (1 - cos) - center.x * sin,
                              0, 0, 1));
    }


    /// \brief Combines a translation transform matrix with current
    /// 
    /// Modifies the current transform matrix and returns reference to itself 
    ///
    /// \param offset - translation offset
    /// \return Reference to this
    Matrix& Translate(const FVec2& offset)
    {
        return Combine(Matrix(1, 0, offset.x,
            0, 1, offset.y,
            0, 0, 1));
    }

    /// \brief Combines a transform matrix with current
    /// 
    /// Mathematically identical to matrix multiplication (other * this)
    /// Modifies the current transform matrix and returns reference to itself 
    ///
    /// \param other - Matrix to combine with this
    /// \return Reference to this
    Matrix& Combine(const Matrix& other)
    {
        const std::array<float, 9>& b = m_matrix;
        const std::array<float, 9>& a = other.m_matrix;

        *this = Matrix(a[0] * b[0] + a[1] * b[3] + a[2] * b[6],
            a[0] * b[1] + a[1] * b[4] + a[2] * b[7],
            a[0] * b[2] + a[1] * b[5] + a[2] * b[8],

            a[3] * b[0] + a[4] * b[3] + a[5] * b[6],
            a[3] * b[1] + a[4] * b[4] + a[5] * b[7],
            a[3] * b[2] + a[4] * b[5] + a[5] * b[8],

            a[6] * b[0] + a[7] * b[3] + a[8] * b[6],
            a[6] * b[1] + a[7] * b[4] + a[8] * b[7],
            a[6] * b[2] + a[7] * b[5] + a[8] * b[8]);

        return *this;
    }

    /// \brief Applies transformation to a point on t a plane
    /// 
    /// Mathematically identical to (matrix * point)
    ///
    /// \param point - Point to transform
    /// \return Transformed point
    FVec2 TransformPoint(FVec2 point) const
    {
        return Vector2(m_matrix[0] * point.x + m_matrix[1] * point.y + m_matrix[2],
                       m_matrix[3] * point.x + m_matrix[4] * point.y + m_matrix[5]);
    }

    FRect TransformAARect(FRect rect) const
    {
        const FVec2 points[4] = 
        {
            TransformPoint(FVec2(rect.x, rect.y)),
            TransformPoint(FVec2(rect.x, rect.y + rect.height)),
            TransformPoint(FVec2(rect.x + rect.width, rect.y)),
            TransformPoint(FVec2(rect.x + rect.width, rect.y + rect.height))
        };

        float minX = points[0].x;
        float minY = points[0].y;
        float maxX = points[0].x;
        float maxY = points[0].y;
        for (int i = 1; i < 4; ++i)
        {
            if (points[i].x < minX)
                minX = points[i].x;
            else if (points[i].x > maxX) 
                maxX = points[i].x;

            if (points[i].y < minY)    
                minY = points[i].y;
            else if (points[i].y > maxY) 
                maxY = points[i].y;
        }

        return FRect(minX, minY, maxX - minX, maxY - minY);
    }

private:
    std::array<float, 9> m_matrix;
};

}