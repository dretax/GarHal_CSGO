/**
 * @author     frk, ReactiioN
 * @date       08.03.2016
 * @visit      https://github.com/frk1
 *             https://thefrk.pw
 *             https://www.unknowncheats.me/forum/members/1067779.html
 *
 *             https://github.com/ReactiioN1337
 *             https://reactiion.pw
 *             https://www.unknowncheats.me/forum/members/264622.html
 */
#pragma once

#include <array>
#include <string>
#include <ostream>
#include <algorithm>

template < typename T, size_t T_Rows, size_t T_Cols >
class Matrix
{
public:
    using array_t = std::array< T, T_Rows* T_Cols >;
    using matrix_t = Matrix< T, T_Rows, T_Cols >;
    using col_matrix_t = Matrix< T, 1, T_Cols >;
    using row_matrix_t = Matrix< T, T_Rows, 1 >;

private:
    array_t m_cValues;

public:
    Matrix(void)
    {
        static_assert(std::is_arithmetic< T >::value, "Type T has to be arithmetic!");
        static_assert(T_Rows >= 1 && T_Cols >= 1, "Minimal row and col dimensions are 1.");
        m_cValues.fill(static_cast<T>(0));
    }

    ~Matrix(void)
    {
    }

    explicit Matrix(std::array< T, (T_Rows* T_Cols) > cValues)
        : m_cValues(cValues)
    {
        static_assert(std::is_arithmetic< T >::value, "Type T has to be arithmetic!");
    }

    template < typename... K >
    Matrix(K ... args)
    {
        static_assert(std::is_arithmetic< T >::value, "Type T has to be arithmetic!");
        m_cValues = std::array< T, T_Rows* T_Cols > { static_cast<T>(args)... };
    }

    constexpr static size_t rows(void)
    {
        return T_Rows;
    }

    constexpr static size_t cols(void)
    {
        return T_Cols;
    }

    constexpr static size_t size(void)
    {
        return T_Rows * T_Cols;
    }

    constexpr static bool isVector(void)
    {
        return T_Rows == 1 || T_Cols == 1;
    }

    bool empty(void) const
    {
        return all_of(m_cValues.begin(), m_cValues.end(), [](T i)
            {
                return i == static_cast<T>(0);
            });
    }

    col_matrix_t row(size_t iRow) const
    {
        std::array< T, T_Cols > values;

        for (size_t i = 0; i < T_Cols; ++i)
            values.at(i) = at(iRow, i);

        return col_matrix_t{ values };
    }

    row_matrix_t col(size_t iCol) const
    {
        std::array< T, T_Rows > values;

        for (size_t i = 0; i < T_Rows; ++i)
            values.at(i) = at(i, iCol);

        return row_matrix_t{ values };
    }

    void row(size_t iRow, col_matrix_t mat)
    {
        for (size_t i = 0; i < T_Cols; ++i)
            at(iRow, i) = mat(i);
    }

    void col(size_t iCol, row_matrix_t mat)
    {
        for (size_t i = 0; i < T_Rows; ++i)
            at(i, iCol) = mat(i);
    }

    T& at(size_t i)
    {
        return m_cValues.at(i);
    }

    const T& at(size_t i) const
    {
        return m_cValues.at(i);
    }

    T& at(size_t iRow, size_t iCol)
    {
        return m_cValues.at(iRow * T_Cols + iCol);
    }

    const T& at(size_t iRow, size_t iCol) const
    {
        return m_cValues.at(iRow * T_Cols + iCol);
    }

    T& operator () (size_t i)
    {
        return at(i);
    }

    const T& operator () (size_t i) const
    {
        return at(i);
    }

    T& operator () (size_t iRow, size_t iCol)
    {
        return at(iRow, iCol);
    }

    const T& operator () (size_t iRow, size_t iCol) const
    {
        return at(iRow, iCol);
    }

    matrix_t operator + (T other) const
    {
        auto buf = *this;
        buf += other;
        return buf;
    }

    matrix_t operator - (T other) const
    {
        auto buf = *this;
        buf -= other;
        return buf;
    }

    matrix_t operator * (T other) const
    {
        auto buf = *this;
        buf *= other;
        return buf;
    }

    matrix_t operator / (T other) const
    {
        auto buf = *this;
        buf /= other;
        return buf;
    }

    matrix_t& operator += (T other)
    {
        for (auto& val : m_cValues)
            val += other;

        return *this;
    }

    matrix_t& operator -= (T other)
    {
        for (auto& val : m_cValues)
            val -= other;

        return *this;
    }

    matrix_t& operator *= (T other)
    {
        for (auto& val : m_cValues)
            val *= other;

        return *this;
    }

    matrix_t& operator /= (T other)
    {
        for (auto& val : m_cValues)
            val /= other;

        return *this;
    }

    matrix_t operator + (const matrix_t& other) const
    {
        auto buf = *this;

        for (size_t i = 0; i < size(); ++i)
            buf(i) += other(i);

        return buf;
    }

    matrix_t operator - (const matrix_t& other) const
    {
        auto buf = *this;

        for (size_t i = 0; i < size(); ++i)
            buf(i) -= other(i);

        return buf;
    }

    matrix_t& operator += (const matrix_t& other)
    {
        for (size_t i = 0; i < size(); ++i)
            at(i) += other(i);

        return (*this);
    }

    bool operator == (const matrix_t& other) const
    {
        for (size_t i = 0; i < size(); ++i)
            if (at(i) != other.at(i))
                return false;

        return true;
    }

    bool operator != (const matrix_t& other) const
    {
        return !((*this) == other);
    }

    matrix_t& operator -= (const matrix_t& other)
    {
        for (size_t i = 0; i < size(); ++i)
            at(i) -= other(i);

        return (*this);
    }

    T norm(void) const
    {
        static_assert(matrix_t::isVector(), "Matrix::norm can only be used on vectors!");

        T buf = static_cast<T>(0);

        for (auto v : m_cValues)
            buf += (v * v);

        return sqrt(buf);
    }

    void normalize(void)
    {
        static_assert(matrix_t::isVector(), "Matrix::normalize can only be used on vectors!");
        (*this) /= this->norm();
    }

    matrix_t normalized(void) const
    {
        static_assert(matrix_t::isVector(), "Matrix::normalized can only be used on vectors!");

        auto buf = *this;
        buf.normalize();
        return buf;
    }

    friend std::ostream& operator <<(std::ostream& os, const matrix_t& v)
    {
        for (size_t i = 0; i < T_Rows; ++i)
        {
            for (size_t j = 0; j < T_Cols; j++)
                os << v.at(i, j) << "\t";

            os << "\n";
        }

        return os;
    }

    template < typename K, size_t OROWS, size_t OCOLS >
    T dot(const Matrix< K, OROWS, OCOLS >& other) const
    {
        static_assert(matrix_t::isVector() && Matrix< K, OROWS, OCOLS >::isVector(), "Matrix::dot can only be used on vectors!");

        T buf = static_cast<T>(0);

        for (size_t i = 0; i < (std::min)(size(), other.size()); ++i)
            buf += at(i) * other.at(i);

        return buf;
    }

    matrix_t cross(const matrix_t& other) const
    {
        static_assert(matrix_t::size() == 3, "Matrix::cross can only be called for 3 dimensional vectors!");

        return{
          at(1) * other.at(2) - at(2) * other.at(1),
          at(2) * other.at(0) - at(0) * other.at(2),
          at(0) * other.at(1) - at(1) * other.at(0),
        };
    }

    matrix_t ncross(const matrix_t& other) const
    {
        static_assert(matrix_t::size() == 3, "Matrix::ncross can only be called for 3 dimensional vectors!");

        return cross(other).normalized();
    }
};