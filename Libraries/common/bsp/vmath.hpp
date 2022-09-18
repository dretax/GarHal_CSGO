#pragma once
#include <cassert>
#include <cmath>

#ifndef EPSILON
#define EPSILON 1e-6
#endif

template<class T>
class Vector2
{
public:
	union
	{
		/**
		 * First element of vector, alias for X-coordinate.
		 */
		T x;

		/**
		 * First element of vector, alias for S-coordinate.
		 * For textures notation.
		 */
		T s;
	};

	union
	{
		/**
		 * Second element of vector, alias for Y-coordinate.
		 */
		T y;

		/**
		 * Second element of vector, alias for T-coordinate.
		 * For textures notation.
		 */
		T t;
	};

	//----------------[ constructors ]--------------------------
	/**
	 * Creates and sets to (0,0)
	 */
	Vector2()
		: x(0), y(0)
	{
	}

	/**
	 * Creates and sets to (x,y)
	 * @param nx initial x-coordinate value
	 * @param ny initial y-coordinate value
	 */
	Vector2(T nx, T ny)
		: x(nx), y(ny)
	{
	}

	/**
	 * Copy constructor.
	 * @param src Source of data for new created instance.
	 */
	Vector2(const Vector2<T>& src)
		: x(src.x), y(src.y)
	{
	}

	/**
	 * Copy casting constructor.
	 * @param src Source of data for new created instance.
	 */
	template<class FromT>
	Vector2(const Vector2<FromT>& src)
		: x(static_cast<T>(src.x)), y(static_cast<T>(src.y))
	{
	}

	//----------------[ access operators ]-------------------
	/**
	 * Copy casting operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	template<class FromT>
	Vector2<T>& operator=(const Vector2<FromT>& rhs)
	{
		x = static_cast<T>(rhs.x);
		y = static_cast<T>(rhs.y);
		return *this;
	}

	/**
	 * Copy operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator=(const Vector2<T>& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	/**
	 * Array access operator
	 * @param n Array index
	 * @return For n = 0, reference to x coordinate, else reference to y
	 * y coordinate.
	 */
	T& operator[](int n)
	{
		assert(n >= 0 && n <= 1);
		if (0 == n)
			return x;
		else
			return y;
	}

	/**
	 * Constant array access operator
	 * @param n Array index
	 * @return For n = 0, reference to x coordinate, else reference to y
	 * y coordinate.
	 */
	const T& operator[](int n) const
	{
		assert(n >= 0 && n <= 1);
		if (0 == n)
			return x;
		else
			return y;
	}

	//---------------[ vector aritmetic operator ]--------------
	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T> operator+(const Vector2<T>& rhs) const
	{
		return Vector2<T>(x + rhs.x, y + rhs.y);
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T> operator-(const Vector2<T>& rhs) const
	{
		return Vector2<T>(x - rhs.x, y - rhs.y);
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T> operator*(const Vector2<T>& rhs) const
	{
		return Vector2<T>(x * rhs.x, y * rhs.y);
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T> operator/(const Vector2<T>& rhs) const
	{
		return Vector2<T>(x / rhs.x, y / rhs.y);
	}

	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator+=(const Vector2<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	/**
	 * Substraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator-=(const Vector2<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator*=(const Vector2<T>& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator/=(const Vector2<T>& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		return *this;
	}

	//--------------[ scalar vector operator ]--------------------
	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T> operator+(T rhs) const
	{
		return Vector2<T>(x + rhs, y + rhs);
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T> operator-(T rhs) const
	{
		return Vector2<T>(x - rhs, y - rhs);
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T> operator*(T rhs) const
	{
		return Vector2<T>(x * rhs, y * rhs);
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T> operator/(T rhs) const
	{
		return Vector2<T>(x / rhs, y / rhs);
	}

	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator+=(T rhs)
	{
		x += rhs;
		y += rhs;
		return *this;
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator-=(T rhs)
	{
		x -= rhs;
		y -= rhs;
		return *this;
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator*=(T rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector2<T>& operator/=(T rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}

	//--------------[ equality operator ]------------------------
	/**
	 * Equality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @note Test of equality is based of threshold EPSILON value. To be two
	 * values equal, must satisfy this condition | lhs.x - rhs.y | < EPSILON,
	 * same for y-coordinate.
	 */
	bool operator==(const Vector2<T>& rhs) const
	{
		return (std::abs(x - rhs.x) < EPSILON) && (std::abs(y - rhs.y) < EPSILON);
	}

	/**
	 * Inequality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @return not (lhs == rhs) :-P
	 */
	bool operator!=(const Vector2<T>& rhs) const
	{
		return !(*this == rhs);
	}

	//-------------[ unary operations ]--------------------------
	/**
	 * Unary negate operator
	 * @return negated vector
	 */
	Vector2<T> operator-() const
	{
		return Vector2<T>(-x, -y);
	}

	//-------------[ size operations ]---------------------------
	/**
	 * get length of vector.
	 * @return lenght of vector
	 */
	T length() const
	{
		return (T)std::sqrt(x * x + y * y);
	}

	/**
	 * Normalize vector
	 */
	void normalize()
	{
		T s = length();
		x /= s;
		y /= s;
	}

	/**
	 * Return square of length.
	 * @return length ^ 2
	 * @note This method is faster then length(). For comparison
	 * of length of two vector can be used just this value, instead
	 * of more expensive length() method.
	 */
	T lengthSq() const
	{
		return x * x + y * y;
	}

	//--------------[ misc. operations ]-----------------------
	/**
	 * Linear interpolation of two vectors
	 * @param fact Factor of interpolation. For translation from position
	 * of this vector to vector r, values of factor goes from 0.0 to 1.0.
	 * @param r Second Vector for interpolation
	 * @note However values of fact parameter are reasonable only in interval
	 * [0.0 , 1.0], you can pass also values outside of this interval and you
	 * can get result (extrapolation?)
	 */
	Vector2<T> lerp(T fact, const Vector2<T>& r) const
	{
		return (*this) + (r - (*this)) * fact;
	}

	//-------------[ conversion ]-----------------------------
	/**
	 * Conversion to pointer operator
	 * @return Pointer to internally stored (in management of class Vector2<T>)
	 * used for passing Vector2<T> values to gl*2[fd] functions.
	 */
	operator T* ()
	{
		return (T*)this;
	}
	/**
	 * Conversion to pointer operator
	 * @return Constant Pointer to internally stored (in management of class Vector2<T>)
	 * used for passing Vector2<T> values to gl*2[fd] functions.
	 */
	operator const T* () const
	{
		return (const T*)this;
	}
};

//--------------------------------------
// Typedef shortcuts for 2D vector
//-------------------------------------
/// Two dimensional Vector of floats
typedef class Vector2<float> Vector2f;
/// Two dimensional Vector of doubles
typedef class Vector2<double> Vector2d;
/// Two dimensional Vector of ints
typedef class Vector2<int> Vector2i;

template<class T>
class Vector3
{
public:
	//T x, y, z;
	union
	{
		/**
		 * First element of vector, alias for X-coordinate.
		 */
		T x;

		/**
		 * First element of vector, alias for S-coordinate.
		 * For textures notation.
		 */
		T s;

		/**
		 * First element of vector, alias for R-coordinate.
		 * For color notation.
		 */
		T r;
	};

	union
	{
		/**
		 * Second element of vector, alias for Y-coordinate.
		 */
		T y;
		/**
		 * Second element of vector, alias for T-coordinate.
		 * For textures notation.
		 */
		T t;
		/**
		 * Second element of vector, alias for G-coordinate.
		 * For color notation.
		 */
		T g;
	};

	union
	{
		/**
		 * Third element of vector, alias for Z-coordinate.
		 */
		T z;

		/**
		 * Third element of vector, alias for U-coordinate.
		 * For textures notation.
		 */
		T u;
		/**
		 * Third element of vector, alias for B-coordinate.
		 * For color notation.
		 */
		T b;
	};

	//----------------[ constructors ]--------------------------
	/**
	 * Creates and sets to (0,0,0)
	 */
	Vector3()
		: x(0), y(0), z(0)
	{
	}

	/**
	 * Creates and sets to (x,y,z)
	 * @param nx initial x-coordinate value
	 * @param ny initial y-coordinate value
	 * @param nz initial z-coordinate value
	 */
	Vector3(T nx, T ny, T nz)
		: x(nx), y(ny), z(nz)
	{
	}

	/**
	 * Copy constructor.
	 * @param src Source of data for new created Vector3 instance.
	 */
	Vector3(const Vector3<T>& src)
		: x(src.x), y(src.y), z(src.z)
	{
	}

	/**
	 * Copy casting constructor.
	 * @param src Source of data for new created Vector3 instance.
	 */
	template<class FromT>
	Vector3(const Vector3<FromT>& src)
		: x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(src.z))
	{
	}

	//----------------[ access operators ]-------------------
	/**
	 * Copy operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator=(const Vector3<T>& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	/**
	 * Copy casting operator.
	 * @param rhs Right hand side argument of binary operator.
	 */
	template<class FromT>
	Vector3<T> operator=(const Vector3<FromT>& rhs)
	{
		x = static_cast<T>(rhs.x);
		y = static_cast<T>(rhs.y);
		z = static_cast<T>(rhs.z);
		return *this;
	}

	/**
	 * Array access operator
	 * @param n Array index
	 * @return For n = 0, reference to x coordinate, n = 1
	 * reference to y, else reference to z
	 * y coordinate.
	 */
	T& operator[](int n)
	{
		assert(n >= 0 && n <= 2);
		if (0 == n)
			return x;
		else if (1 == n)
			return y;
		else
			return z;
	}

	/**
	 * Constant array access operator
	 * @param n Array index
	 * @return For n = 0, reference to x coordinate, n = 1
	 * reference to y, else reference to z
	 * y coordinate.
	 */
	const T& operator[](int n) const
	{
		assert(n >= 0 && n <= 2);
		if (0 == n)
			return x;
		else if (1 == n)
			return y;
		else
			return z;
	}

	//---------------[ vector arithmetic operator ]--------------
	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator+(const Vector3<T>& rhs) const
	{
		return Vector3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator-(const Vector3<T>& rhs) const
	{
		return Vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator*(const Vector3<T>& rhs) const
	{
		return Vector3<T>(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator/(const Vector3<T>& rhs) const
	{
		return Vector3<T>(x / rhs.x, y / rhs.y, z / rhs.z);
	}

	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T>& operator+=(const Vector3<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T>& operator-=(const Vector3<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T>& operator*=(const Vector3<T>& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T>& operator/=(const Vector3<T>& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		return *this;
	}

	/**
	 * Dot product of two vectors.
	 * @param rhs Right hand side argument of binary operator.
	 */
	T dotProduct(const Vector3<T>& rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}

	/**
	 * Cross product operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> crossProduct(const Vector3<T>& rhs) const
	{
		return Vector3<T>(y * rhs.z - rhs.y * z, z * rhs.x - rhs.z * x, x * rhs.y - rhs.x * y);
	}

	//--------------[ scalar vector operator ]--------------------
	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator+(T rhs) const
	{
		return Vector3<T>(x + rhs, y + rhs, z + rhs);
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator-(T rhs) const
	{
		return Vector3<T>(x - rhs, y - rhs, z - rhs);
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator*(T rhs) const
	{
		return Vector3<T>(x * rhs, y * rhs, z * rhs);
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator/(T rhs) const
	{
		return Vector3<T>(x / rhs, y / rhs, z / rhs);
	}

	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T>& operator+=(T rhs)
	{
		x += rhs;
		y += rhs;
		z += rhs;
		return *this;
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T>& operator-=(T rhs)
	{
		x -= rhs;
		y -= rhs;
		z -= rhs;
		return *this;
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T>& operator*=(T rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T>& operator/=(T rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	//--------------[ Equality operator ]------------------------
	/**
	 * Equality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @note Test of equality is based of threshold EPSILON value. To be two
	 * values equal, must satisfy this condition | lhs.x - rhs.y | < EPSILON,
	 * same for y-coordinate, and z-coordinate.
	 */
	bool operator==(const Vector3<T>& rhs) const
	{
		return std::fabs(x - rhs.x) < 0.0001 && std::fabs(y - rhs.y) < 0.0001 && std::fabs(z - rhs.z) < 0.0001;
	}

	/**
	 * Inequality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @return not (lhs == rhs) :-P
	 */
	bool operator!=(const Vector3<T>& rhs) const
	{
		return !(*this == rhs);
	}

	//-------------[ unary operations ]--------------------------
	/**
	 * Unary negate operator
	 * @return negated vector
	 */
	Vector3<T> operator-() const
	{
		return Vector3<T>(-x, -y, -z);
	}

	//-------------[ size operations ]---------------------------
	/**
	 * get length of vector.
	 * @return lenght of vector
	 */
	T length() const
	{
		return (T)std::sqrt(x * x + y * y + z * z);
	}

	/**
	 * Return square of length.
	 * @return length ^ 2
	 * @note This method is faster then length(). For comparison
	 * of length of two vector can be used just this value, instead
	 * of more expensive length() method.
	 */
	T lengthSq() const
	{
		return x * x + y * y + z * z;
	}

	/**
	 * Normalize vector
	 */
	void normalize()
	{
		T s = length();
		x /= s;
		y /= s;
		z /= s;
	}

	//------------[ other operations ]---------------------------
	/**
	 * Rotate vector around three axis.
	 * @param ax Angle (in degrees) to be rotated around X-axis.
	 * @param ay Angle (in degrees) to be rotated around Y-axis.
	 * @param az Angle (in degrees) to be rotated around Z-axis.
	 */
	void rotate(T ax, T ay, T az)
	{
		T a = cos(DEG2RAD(ax));
		T b = sin(DEG2RAD(ax));
		T c = cos(DEG2RAD(ay));
		T d = sin(DEG2RAD(ay));
		T e = cos(DEG2RAD(az));
		T f = sin(DEG2RAD(az));
		T nx = c * e * x - c * f * y + d * z;
		T ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
		T nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
		x = nx;
		y = ny;
		z = nz;

	}

	/**
	 * Linear interpolation of two vectors
	 * @param fact Factor of interpolation. For translation from positon
	 * of this vector to vector r, values of factor goes from 0.0 to 1.0.
	 * @param r Second Vector for interpolation
	 * @note However values of fact parameter are reasonable only in interval
	 * [0.0 , 1.0], you can pass also values outside of this interval and you
	 * can get result (extrapolation?)
	 */
	Vector3<T> lerp(T fact, const Vector3<T>& r) const
	{
		return (*this) + (r - (*this)) * fact;
	}

	//-------------[ conversion ]-----------------------------

	/**
	 * Conversion to pointer operator
	 * @return Pointer to internally stored (in management of class Vector3<T>)
	 * used for passing Vector3<T> values to gl*3[fd] functions.
	 */
	operator T* ()
	{
		return (T*)this;
	}

	/**
	 * Conversion to pointer operator
	 * @return Constant Pointer to internally stored (in management of class Vector3<T>)
	 * used for passing Vector3<T> values to gl*3[fd] functions.
	 */
	operator const T* () const
	{
		return (const T*)this;
	}
};

/// Three dimensional Vector of floats
typedef Vector3<float> Vector3f;
/// Three dimensional Vector of doubles
typedef Vector3<double> Vector3d;
/// Three dimensional Vector of ints
typedef Vector3<int> Vector3i;


/**
 * Class for four dimensional vector.
  * There are four ways of accessing vector components.
 * Let's have <code>Vector4f v</code>, you can either:
 * <ul>
 * 	<li>access as position in projective space (x,y,z,w) &mdash; <code>v.x = v.y = v.z = v.w = 1;</code></li>
 * 	<li>access as texture coordinate (s,t,u,v) &mdash; <code>v.s = v.t = v.u = v.v = 1;</code></li>
 * 	<li>access as color (r,g,b,a) &mdash; <code>v.r = v.g = v.b = v.a = 1;</code></li>
 * 	<li>access via operator[] &mdash; <code>v[0] = v[1] = v[2] = v[3] = 1;</code></li>
 * </ul>
 */
template<class T>
class Vector4
{
public:

	union
	{
		/**
		 * First element of vector, alias for R-coordinate.
		 * For color notation.
		 */
		T r
			/**
			 * First element of vector, alias for X-coordinate.
			 */;
		T x;
	};

	union
	{
		/**
		 * Second element of vector, alias for G-coordinate.
		 * For color notation.
		 */
		T g;
		/**
		 * Second element of vector, alias for Y-coordinate.
		 */
		T y;
	};

	union
	{
		/**
		 * Third element of vector, alias for B-coordinate.
		 * For color notation.
		 */
		T b;
		/**
		 * Third element of vector, alias for Z-coordinate.
		 */
		T z;
	};

	union
	{
		/**
		 * Fourth element of vector, alias for A-coordinate.
		 * For color notation. This represnt aplha chanell
		 */
		T a;
		/**
		 * First element of vector, alias for W-coordinate.
		 * @note For vectors (such as normals) should be set to 0.0
		 * For vertices should be set to 1.0
		 */
		T w;
	};

	//----------------[ constructors ]--------------------------
	/**
	 * Creates and sets to (0,0,0,0)
	 */
	Vector4()
		: x(0), y(0), z(0), w(0)
	{
	}

	/**
	 * Creates and sets to (x,y,z,z)
	 * @param nx initial x-coordinate value (R)
	 * @param ny initial y-coordinate value (G)
	 * @param nz initial z-coordinate value (B)
	 * @param nw initial w-coordinate value (Alpha)
	 */
	Vector4(T nx, T ny, T nz, T nw)
		: x(nx), y(ny), z(nz), w(nw)
	{
	}

	/**
	 * Copy constructor.
	 * @param src Source of data for new created Vector4 instance.
	 */
	Vector4(const Vector4<T>& src)
		: x(src.x), y(src.y), z(src.z), w(src.w)
	{
	}

	/**
	 * Copy casting constructor.
	 * @param src Source of data for new created Vector4 instance.
	 */
	template<class FromT>
	Vector4(const Vector4<FromT>& src)
		: x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(src.z)), w(static_cast<T>(src.w))
	{
	}

	Vector4(const Vector3<T>& src, T w)
		: x(src.x), y(src.y), z(src.z), w(w)
	{}

	template <typename FromT>
	Vector4(const Vector3<FromT>& src, FromT w)
		: x(static_cast<T>(src.x)), y(static_cast<T>(src.y)), z(static_cast<T>(src.z)), w(static_cast<T>(w))
	{}

	//----------------[ access operators ]-------------------
	/**
	 * Copy operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator=(const Vector4<T>& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	/**
	 * Copy casting operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	template<class FromT>
	Vector4<T> operator=(const Vector4<FromT>& rhs)
	{
		x = static_cast<T>(rhs.x);
		y = static_cast<T>(rhs.y);
		z = static_cast<T>(rhs.z);
		w = static_cast<T>(rhs.w);
		return *this;
	}

	/**
	 * Array access operator
	 * @param n Array index
	 * @return For n = 0, reference to x coordinate, n = 1
	 * reference to y coordinate, n = 2 reference to z,
	 * else reference to w coordinate.
	 */
	T& operator[](int n)
	{
		assert(n >= 0 && n <= 3);
		if (0 == n)
			return x;
		else if (1 == n)
			return y;
		else if (2 == n)
			return z;
		else
			return w;
	}

	/**
	 * Array access operator
	 * @param n Array index
	 * @return For n = 0, reference to x coordinate, n = 1
	 * reference to y coordinate, n = 2 reference to z,
	 * else reference to w coordinate.
	 */
	const T& operator[](int n) const
	{
		assert(n >= 0 && n <= 3);
		if (0 == n)
			return x;
		else if (1 == n)
			return y;
		else if (2 == n)
			return z;
		else
			return w;
	}

	//---------------[ vector aritmetic operator ]--------------
	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator+(const Vector4<T>& rhs) const
	{
		return Vector4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator-(const Vector4<T>& rhs) const
	{
		return Vector4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator*(const Vector4<T> rhs) const
	{
		return Vector4<T>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator/(const Vector4<T>& rhs) const
	{
		return Vector4<T>(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
	}

	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T>& operator+=(const Vector4<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T>& operator-=(const Vector4<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T>& operator*=(const Vector4<T>& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T>& operator/=(const Vector4<T>& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		w /= rhs.w;
		return *this;
	}

	//--------------[ equiality operator ]------------------------
	/**
	 * Equality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @note Test of equality is based of threshold EPSILON value. To be two
	 * values equal, must satisfy this condition | lhs.x - rhs.y | < EPSILON,
	 * same for y-coordinate, z-coordinate, and w-coordinate.
	 */
	bool operator==(const Vector4<T>& rhs) const
	{
		return std::fabs(x - rhs.x) < EPSILON && std::fabs(y - rhs.y) < EPSILON && std::fabs(z - rhs.z) < EPSILON
			&& std::fabs(w - rhs.w) < EPSILON;
	}

	/**
	 * Inequality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @return not (lhs == rhs) :-P
	 */
	bool operator!=(const Vector4<T>& rhs) const
	{
		return !(*this == rhs);
	}

	//-------------[ unary operations ]--------------------------
	/**
	 * Unary negate operator
	 * @return negated vector
	 */
	Vector4<T> operator-() const
	{
		return Vector4<T>(-x, -y, -z, -w);
	}

	//--------------[ scalar vector operator ]--------------------

	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator+(T rhs) const
	{
		return Vector4<T>(x + rhs, y + rhs, z + rhs, w + rhs);
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator-(T rhs) const
	{
		return Vector4<T>(x - rhs, y - rhs, z - rhs, w - rhs);
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator*(T rhs) const
	{
		return Vector4<T>(x * rhs, y * rhs, z * rhs, w * rhs);
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator/(T rhs) const
	{
		return Vector4<T>(x / rhs, y / rhs, z / rhs, w / rhs);
	}

	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T>& operator+=(T rhs)
	{
		x += rhs;
		y += rhs;
		z += rhs;
		w += rhs;
		return *this;
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T>& operator-=(T rhs)
	{
		x -= rhs;
		y -= rhs;
		z -= rhs;
		w -= rhs;
		return *this;
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T>& operator*=(T rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T>& operator/=(T rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w /= rhs;
		return *this;
	}

	//-------------[ size operations ]---------------------------
	/**
	 * get length of vector.
	 * @return lenght of vector
	 */
	T length() const
	{
		return (T)std::sqrt(x * x + y * y + z * z + w * w);
	}

	/**
	 * Normalize vector
	 */
	void normalize()
	{
		T s = length();
		x /= s;
		y /= s;
		z /= s;
		w /= s;
	}

	/**
	 * Return square of length.
	 * @return length ^ 2
	 * @note This method is faster then length(). For comparison
	 * of length of two vector can be used just this value, instead
	 * of more expensive length() method.
	 */
	T lengthSq() const
	{
		return x * x + y * y + z * z + w * w;
	}

	//--------------[ misc. operations ]-----------------------
	/**
	 * Linear interpolation of two vectors
	 * @param fact Factor of interpolation. For translation from position
	 * of this vector to vector r, values of factor goes from 0.0 to 1.0.
	 * @param r Second Vector for interpolation
	 * @note However values of fact parameter are reasonable only in interval
	 * [0.0 , 1.0], you can pass also values outside of this interval and you
	 * can get result (extrapolation?)
	 */
	Vector4<T> lerp(T fact, const Vector4<T>& r) const
	{
		return (*this) + (r - (*this)) * fact;
	}

	//-------------[ conversion ]-----------------------------

	/**
	 * Conversion to pointer operator
	 * @return Pointer to internally stored (in management of class Vector4<T>)
	 * used for passing Vector4<T> values to gl*4[fd] functions.
	 */
	operator T* ()
	{
		return (T*)this;
	}

	/**
	 * Conversion to pointer operator
	 * @return Constant Pointer to internally stored (in management of class Vector4<T>)
	 * used for passing Vector4<T> values to gl*4[fd] functions.
	 */
	operator const T* () const
	{
		return (const T*)this;
	}

	/**
	 * gets 3D vector. Note that the output is divided by w coordinate to apply projection
	 * transform. If the w coordinate is equal to zero, the result is not divided.
	 * @return (x/w, y/w, z/w) iff w != 0 otherwise (x,y,z)
	 */
	Vector3<T> xyz() const
	{
		if (w == 0 || w == 1)
			return Vector3<T>(x, y, z);

		const T invW = 1.0 / w;
		return Vector3<T>(x * invW, y * invW, z * invW);
	}

};

/// Three dimensional Vector of floats
typedef Vector4<float> Vector4f;
/// Three dimensional Vector of doubles
typedef Vector4<double> Vector4d;
/// Three dimensional Vector of ints
typedef Vector4<int> Vector4i;

/**
 * Class for matrix 3x3.
 * @note Data stored in this matrix are in column major order. This arrangement suits OpenGL.
 * If you're using row major matrix, consider using fromRowMajorArray as way for construction
 * Matrix3<T> instance.
 */
template<class T>
class Matrix3
{
public:
	/// Data stored in column major order
	T data[9];

	//--------------------------[ constructors ]-------------------------------
	/**
	 * Creates identity matrix
	 */
	Matrix3()
	{
		for (int i = 0; i < 9; i++)
			data[i] = (i % 4) ? 0 : 1;
	}

	/**
	 * Copy matrix values from array (these data must be in column
	 * major order!)
	 */
	Matrix3(const T* dt)
	{
		std::memcpy(data, dt, sizeof(T) * 9);
	}

	/**
	 * Copy constructor.
	 * @param src Data source for new created instance of Matrix3
	 */
	Matrix3(const Matrix3<T>& src)
	{
		std::memcpy(data, src.data, sizeof(T) * 9);
	}

	/**
	 * Copy casting constructor.
	 * @param src Data source for new created instance of Matrix3
	 */
	template<class FromT>
	Matrix3(const Matrix3<FromT>& src)
	{
		for (int i = 0; i < 9; i++)
		{
			data[i] = static_cast<T>(src.data[i]);
		}
	}

	/**
	 * Resets matrix to be identity matrix
	 */
	void identity()
	{
		for (int i = 0; i < 9; i++)
			data[i] = (i % 4) ? 0 : 1;
	}

	/**
	 * Creates rotation matrix by rotation around axis.
	 * @param xDeg Angle (in degrees) of rotation around axis X.
	 * @param yDeg Angle (in degrees) of rotation around axis Y.
	 * @param zDeg Angle (in degrees) of rotation around axis Z.
	 */
	static Matrix3<T> createRotationAroundAxis(T xDeg, T yDeg, T zDeg)
	{
		T xRads(DEG2RAD(xDeg));
		T yRads(DEG2RAD(yDeg));
		T zRads(DEG2RAD(zDeg));

		Matrix3<T> ma, mb, mc;
		float ac = cos(xRads);
		float as = sin(xRads);
		float bc = cos(yRads);
		float bs = sin(yRads);
		float cc = cos(zRads);
		float cs = sin(zRads);

		ma.at(1, 1) = ac;
		ma.at(2, 1) = as;
		ma.at(1, 2) = -as;
		ma.at(2, 2) = ac;

		mb.at(0, 0) = bc;
		mb.at(2, 0) = -bs;
		mb.at(0, 2) = bs;
		mb.at(2, 2) = bc;

		mc.at(0, 0) = cc;
		mc.at(1, 0) = cs;
		mc.at(0, 1) = -cs;
		mc.at(1, 1) = cc;

		Matrix3<T> ret = ma * mb * mc;
		return ret;
	}

	/**
	 * Creates rotation matrix from ODE Matrix.
	 */
	template<class It>
	static Matrix3<T> fromOde(const It* mat)
	{
		Matrix3<T> ret;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				ret.at(i, j) = static_cast<T>(mat[j * 4 + i]);
			}
		}
		return ret;
	}

	/**
	 * Creates new matrix 3x3 from array that represents such matrix 3x3
	 * as array of tightly packed elements in row major order.
	 * @param arr An array of elements for 3x3 matrix in row major order.
	 * @return An instance of Matrix3<T> representing @a arr
	 */
	template<class FromT>
	static Matrix3<T> fromRowMajorArray(const FromT* arr)
	{
		const T retData[] =
		{ static_cast<T>(arr[0]), static_cast<T>(arr[3]), static_cast<T>(arr[6]), static_cast<T>(arr[1]),
				static_cast<T>(arr[4]), static_cast<T>(arr[7]), static_cast<T>(arr[2]), static_cast<T>(arr[5]),
				static_cast<T>(arr[8]) };

		return retData;
	}

	/**
	 * Creates new matrix 3x3 from array that represents such matrix 3x3
	 * as array of tightly packed elements in column major order.
	 * @param arr An array of elements for 3x3 matrix in column major order.
	 * @return An instance of Matrix3<T> representing @a arr
	 */
	template<class FromT>
	static Matrix3<T> fromColumnMajorArray(const FromT* arr)
	{
		const T retData[] =
		{ static_cast<T>(arr[0]), static_cast<T>(arr[1]), static_cast<T>(arr[2]), static_cast<T>(arr[3]),
				static_cast<T>(arr[4]), static_cast<T>(arr[5]), static_cast<T>(arr[6]), static_cast<T>(arr[7]),
				static_cast<T>(arr[8]) };

		return retData;
	}

	//---------------------[ equiality operators ]------------------------------
	/**
	 * Equality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @note Test of equality is based of threshold EPSILON value. To be two
	 * values equal, must satisfy this condition all elements of matrix
	 * | lhs[i] - rhs[i] | < EPSILON,
	 * same for y-coordinate, z-coordinate, and w-coordinate.
	 */
	bool operator==(const Matrix3<T>& rhs) const
	{
		for (int i = 0; i < 9; i++)
		{
			if (std::fabs(data[i] - rhs.data[i]) >= EPSILON)
				return false;
		}
		return true;
	}

	/**
	 * Inequality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @return not (lhs == rhs) :-P
	 */
	bool operator!=(const Matrix3<T>& rhs) const
	{
		return !(*this == rhs);
	}

	//---------------------[ access operators ]---------------------------------
	/**
	 * get reference to element at position (x,y).
	 * @param x Number of column (0..2)
	 * @param y Number of row (0..2)
	 */
	T& at(int x, int y)
	{
		assert(x >= 0 && x < 3);
		assert(y >= 0 && y < 3);
		return data[x * 3 + y];
	}

	/**
	 * get constant reference to element at position (x,y).
	 * @param x Number of column (0..2)
	 * @param y Number of row (0..2)
	 */
	const T& at(int x, int y) const
	{
		assert(x >= 0 && x < 3);
		assert(y >= 0 && y < 3);
		return data[x * 3 + y];
	}

	/**
	 * get reference to element at position (i,j), with math matrix notation
	 * @param i Number of row (1..3)
	 * @param j Number of column (1..3)
	 */
	T& operator()(int i, int j)
	{
		assert(i >= 1 && i <= 3);
		assert(j >= 1 && j <= 3);
		return data[(j - 1) * 3 + i - 1];
	}

	/**
	 * get constant reference to element at position (i,j), with math matrix notation
	 * @param i Number of row (1..3)
	 * @param j Number of column (1..3)
	 */
	const T& operator()(int i, int j) const
	{
		assert(i >= 1 && i <= 3);
		assert(j >= 1 && j <= 3);
		return data[(j - 1) * 3 + i - 1];
	}

	/**
	 * Copy operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix3<T>& operator=(const Matrix3<T>& rhs)
	{
		std::memcpy(data, rhs.data, sizeof(T) * 9);
		return *this;
	}

	/**
	 * Copy casting operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	template<class FromT>
	Matrix3<T>& operator=(const Matrix3<FromT>& rhs)
	{
		for (int i = 0; i < 9; i++)
		{
			data[i] = static_cast<T>(rhs.data[i]);
		}
		return *this;
	}

	/**
	 * Copy operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix3<T>& operator=(const T* rhs)
	{
		std::memcpy(data, rhs, sizeof(T) * 9);
		return *this;
	}

	/*Matrix3<T> & operator=(const double* m)
	 {
	 for (int i = 0; i < 9; i++) data[i] = (T)m[i];
	 return * this;
	 }*/

	 //--------------------[ matrix with matrix operations ]---------------------
	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	Matrix3<T> operator+(const Matrix3<T>& rhs) const
	{
		Matrix3<T> ret;
		for (int i = 0; i < 9; i++)
			ret.data[i] = data[i] + rhs.data[i];
		return ret;
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix3<T> operator-(const Matrix3<T>& rhs) const
	{
		Matrix3<T> ret;
		for (int i = 0; i < 9; i++)
			ret.data[i] = data[i] - rhs.data[i];
		return ret;
	}

	//--------------------[ matrix with scalar operations ]---------------------
	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix3<T> operator+(T rhs) const
	{
		Matrix3<T> ret;
		for (int i = 0; i < 9; i++)
			ret.data[i] = data[i] + rhs;
		return ret;
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix3<T> operator-(T rhs) const
	{
		Matrix3<T> ret;
		for (int i = 0; i < 9; i++)
			ret.data[i] = data[i] - rhs;
		return ret;
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix3<T> operator*(T rhs) const
	{
		Matrix3<T> ret;
		for (int i = 0; i < 9; i++)
			ret.data[i] = data[i] * rhs;
		return ret;
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix3<T> operator/(T rhs) const
	{
		Matrix3<T> ret;
		for (int i = 0; i < 9; i++)
			ret.data[i] = data[i] / rhs;
		return ret;
	}

	//--------------------[ multiply operators ]--------------------------------
	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator*(const Vector3<T>& rhs) const
	{
		return Vector3<T>(data[0] * rhs.x + data[3] * rhs.y + data[6] * rhs.z,
			data[1] * rhs.x + data[4] * rhs.y + data[7] * rhs.z,
			data[2] * rhs.x + data[5] * rhs.y + data[8] * rhs.z);
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix3<T> operator*(Matrix3<T> rhs) const
	{
		Matrix3<T> w;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				T n = 0;
				for (int k = 0; k < 3; k++)
					n += rhs.at(i, k) * at(k, j);
				w.at(i, j) = n;
			}
		}
		return w;

	}

	//---------------------------[ misc operations ]----------------------------
	/**
	 * Transpose matrix.
	 */
	Matrix3<T> transpose()
	{
		Matrix3<T> ret;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				ret.at(i, j) = at(j, i);
			}
		}
		return ret;
	}

	/**
	 * Linear interpolation of two matrices
	 * @param fact Factor of interpolation. For translation from positon
	 * of this matrix (lhs) to matrix rhs, values of factor goes from 0.0 to 1.0.
	 * @param rhs Second Matrix for interpolation
	 * @note However values of fact parameter are reasonable only in interval
	 * [0.0 , 1.0], you can pass also values outside of this interval and you
	 * can get result (extrapolation?)
	 */
	Matrix3<T> lerp(T fact, const Matrix3<T>& rhs) const
	{
		Matrix3<T> ret = (*this) + (rhs - (*this)) * fact;
		return ret;
	}

	T det()
	{
		return +at(0, 0) * at(1, 1) * at(2, 2) + at(0, 1) * at(1, 2) * at(2, 0) + at(0, 2) * at(1, 0) * at(2, 1)
			- at(0, 0) * at(1, 2) * at(2, 1) - at(0, 1) * at(1, 0) * at(2, 2) - at(0, 2) * at(1, 1) * at(2, 0);
	}

	/**
	 * Computes inverse matrix
	 * @return Inverse matrix of this matrix.
	 */
	Matrix3<T> inverse()
	{
		Matrix3<T> ret;
		ret.at(0, 0) = at(1, 1) * at(2, 2) - at(2, 1) * at(1, 2);
		ret.at(0, 1) = at(2, 1) * at(0, 2) - at(0, 1) * at(2, 2);
		ret.at(0, 2) = at(0, 1) * at(1, 2) - at(1, 1) * at(0, 2);
		ret.at(1, 0) = at(2, 0) * at(1, 2) - at(1, 0) * at(2, 2);
		ret.at(1, 1) = at(0, 0) * at(2, 2) - at(2, 0) * at(0, 2);
		ret.at(1, 2) = at(1, 0) * at(0, 2) - at(0, 0) * at(1, 2);
		ret.at(2, 0) = at(1, 0) * at(2, 1) - at(2, 0) * at(1, 1);
		ret.at(2, 1) = at(2, 0) * at(0, 1) - at(0, 0) * at(2, 1);
		ret.at(2, 2) = at(0, 0) * at(1, 1) - at(1, 0) * at(0, 1);
		return ret * (1.0f / det());
	}

	//-------------[ conversion ]-----------------------------

	/**
	 * Conversion to pointer operator
	 * @return Pointer to internally stored (in management of class Matrix3<T>)
	 * used for passing Matrix3<T> values to gl*[fd]v functions.
	 */
	operator T* ()
	{
		return (T*)data;
	}

	/**
	 * Conversion to pointer operator
	 * @return Constant Pointer to internally stored (in management of class Matrix3<T>)
	 * used for passing Matrix3<T> values to gl*[fd]v functions.
	 */
	operator const T* () const
	{
		return (const T*)data;
	}
};

/// Matrix 3x3 of floats
typedef Matrix3<float> Matrix3f;
/// Matrix 3x3 of doubles
typedef Matrix3<double> Matrix3d;
/// Matrix 3x3 of int
typedef Matrix3<int> Matrix3i;

/**
 * Class for matrix 4x4
 * @note Data stored in this matrix are in column major order. This arrangement suits OpenGL.
 * If you're using row major matrix, consider using fromRowMajorArray as way for construction
 * Matrix4<T> instance.
 */
template<class T>
class Matrix4
{
public:
	/// Data stored in column major order
	T data[16];

	//--------------------------[ constructors ]-------------------------------
	/**
	 *Creates identity matrix
	 */
	Matrix4()
	{
		for (int i = 0; i < 16; i++)
			data[i] = (i % 5) ? 0 : 1;
	}

	/**
	 * Copy matrix values from array (these data must be in column
	 * major order!)
	 */
	Matrix4(const T* dt)
	{
		std::memcpy(data, dt, sizeof(T) * 16);
	}

	/**
	 * Copy constructor.
	 * @param src Data source for new created instance of Matrix4.
	 */
	Matrix4(const Matrix4<T>& src)
	{
		std::memcpy(data, src.data, sizeof(T) * 16);
	}

	/**
	 * Copy casting constructor.
	 * @param src Data source for new created instance of Matrix4.
	 */
	template<class FromT>
	Matrix4(const Matrix4<FromT>& src)
	{
		for (int i = 0; i < 16; i++)
		{
			data[i] = static_cast<T>(src.data[i]);
		}
	}

	/**
	 * Resets matrix to be identity matrix
	 */
	void identity()
	{
		for (int i = 0; i < 16; i++)
			data[i] = (i % 5) ? 0 : 1;
	}

	/**
	 * Creates rotation matrix by rotation around axis.
	 * @param xDeg Angle (in degrees) of rotation around axis X.
	 * @param yDeg Angle (in degrees) of rotation around axis Y.
	 * @param zDeg Angle (in degrees) of rotation around axis Z.
	 */
	static Matrix4<T> createRotationAroundAxis(T xDeg, T yDeg, T zDeg)
	{
		T xRads(DEG2RAD(xDeg));
		T yRads(DEG2RAD(yDeg));
		T zRads(DEG2RAD(zDeg));

		Matrix4<T> ma, mb, mc;
		float ac = cos(xRads);
		float as = sin(xRads);
		float bc = cos(yRads);
		float bs = sin(yRads);
		float cc = cos(zRads);
		float cs = sin(zRads);

		ma.at(1, 1) = ac;
		ma.at(2, 1) = as;
		ma.at(1, 2) = -as;
		ma.at(2, 2) = ac;

		mb.at(0, 0) = bc;
		mb.at(2, 0) = -bs;
		mb.at(0, 2) = bs;
		mb.at(2, 2) = bc;

		mc.at(0, 0) = cc;
		mc.at(1, 0) = cs;
		mc.at(0, 1) = -cs;
		mc.at(1, 1) = cc;

		/*std::cout << "RotVec = " << a << "," << b << "," << c << std::endl;
		 std::cout << "Rx = " << std::endl << ma;
		 std::cout << "Ry = " << std::endl << mb;
		 std::cout << "Rz = " << std::endl << mc;*/

		Matrix4<T> ret = ma * mb * mc;
		//std::cout << "Result = " << std::endl << ma * (mb * mc);

		return ret;
	}

	/// Creates translation matrix
	/**
	 * Creates translation matrix.
	 * @param x X-direction translation
	 * @param y Y-direction translation
	 * @param z Z-direction translation
	 * @param w for W-coordinate translation (implicitly set to 1)
	 */
	static Matrix4<T> createTranslation(T x, T y, T z, T w = 1)
	{
		Matrix4 ret;
		ret.at(3, 0) = x;
		ret.at(3, 1) = y;
		ret.at(3, 2) = z;
		ret.at(3, 3) = w;

		return ret;
	}

	/**
	 * Create scale matrix with @a sx, @a sy, and @a sz
	 * being values of matrix main diagonal.
	 * @param sx Scale in X-axis
	 * @param sy Scale in Y-axis
	 * @param sz Scale in Z-axis
	 * @return Transform matrix 4x4 with scale transformation.
	 */
	static Matrix4<T> createScale(T sx, T sy, T sz)
	{
		Matrix4<T> ret;
		ret.at(0, 0) = sx;
		ret.at(1, 1) = sy;
		ret.at(2, 2) = sz;

		return ret;
	}

	/**
	 * Creates new view matrix to look from specified position @a eyePos to specified position @a centerPos
	 * @param eyePos A position of camera
	 * @param centerPos A position where camera looks-at
	 * @param upDir Direction of up vector
	 * @return Resulting view matrix that looks from and at specific position.
	 */
	static Matrix4<T> createLookAt(const Vector3<T>& eyePos, const Vector3<T>& centerPos, const Vector3<T>& upDir)
	{
		Vector3<T> forward, side, up;
		Matrix4<T> m;

		forward = centerPos - eyePos;
		up = upDir;

		forward.normalize();

		// Side = forward x up
		side = forward.crossProduct(up);
		side.normalize();

		// Recompute up as: up = side x forward
		up = side.crossProduct(forward);

		m.at(0, 0) = side.x;
		m.at(1, 0) = side.y;
		m.at(2, 0) = side.z;

		m.at(0, 1) = up.x;
		m.at(1, 1) = up.y;
		m.at(2, 1) = up.z;

		m.at(0, 2) = -forward.x;
		m.at(1, 2) = -forward.y;
		m.at(2, 2) = -forward.z;

		m = m * Matrix4<T>::createTranslation(-eyePos.x, -eyePos.y, -eyePos.z);
		return m;
	}


	/**
	 * Creates OpenGL compatible perspective projection according specified frustum parameters.
	 *
	 * @param left Specify the coordinate for the left vertical clipping plane,
	 * @param right Specify the coordinate for the right vertical clipping plane.
	 * @param bottom Specify the coordinate for the bottom horizontal clipping plane,
	 * @param top Specify the coordinate for the top horizontal clipping plane.
	 * @param zNear Specify the distance to the near clipping plane.  Distance must be positive.
	 * @param zFar Specify the distance to the far depth clipping plane.  Distance must be positive.
	 *
	 * @return Projection matrix for specified frustum.
	 */
	static Matrix4<T> createFrustum(T left, T right, T bottom, T top, T zNear, T zFar)
	{
		/*
		 *
		  2 zNear
		------------       0              A              0
		right - left
						2 zNear
			0         ------------        B              0
					  top - bottom
			0              0              C              D
			0              0              -1             0
													 A = (right + left) / (right - left)
													 B = (top + bottom) / (top - bottom)
													C = - (zFar + zNear) / (zFar - zNear)
													D = - (2 zFar zNear) / (zFar - zNear)
		 *
		 */
		Matrix4<T> ret;

		const T invWidth = 1.0 / (right - left);
		const T invHeight = 1.0 / (top - bottom);
		const T invDepth = 1.0 / (zFar - zNear);

		const T twoZNear = 2 * zNear;

		ret.at(0, 0) = twoZNear * invWidth;
		ret.at(1, 1) = twoZNear * invHeight;

		ret.at(2, 0) = (right + left) * invWidth;
		ret.at(2, 1) = (top + bottom) * invHeight;
		ret.at(2, 2) = -(zFar + zNear) * invDepth;
		ret.at(2, 3) = -1;

		ret.at(3, 2) = -twoZNear * zFar * invDepth;
		ret.at(3, 3) = 0;

		return ret;
	}

	/**
	 * Creates OpenGL compatible orthographic projection matrix.
	 * @param left Specify the coordinate for the left vertical clipping plane,
	 * @param right Specify the coordinate for the right vertical clipping plane.
	 * @param bottom Specify the coordinate for the bottom horizontal clipping plane,
	 * @param top Specify the coordinate for the top horizontal clipping plane.
	 * @param zNear Specify the distance to the nearer depth clipping plane.
	 *       This value is negative if the plane is to be behind the viewer,
	 * @param zFar Specify the distance to the farther depth clipping plane.
	 *       This value is negative if the plane is to be behind the viewer.
	 * @return Othrographic projection matrix.
	 */
	static Matrix4<T> createOrtho(T left, T right, T bottom, T top, T zNear, T zFar)
	{
		/*
			   2
		------------       0              0              tx
		right - left
						   2
			0         ------------        0              ty
					  top - bottom
										  -2
			0              0         ------------        tz
									  zFar-zNear
			0              0              0              1
	   where
													tx = - (right + left) / (right - left)
													ty = - (top + bottom) / (top - bottom)
													tz = - (zFar + zNear) / (zFar - zNear)
		 */

		const T invWidth = 1.0 / (right - left);
		const T invHeight = 1.0 / (top - bottom);
		const T invDepth = 1.0 / (zFar - zNear);

		Matrix4<T> ret;

		ret.at(0, 0) = 2 * invWidth;
		ret.at(1, 1) = 2 * invHeight;
		ret.at(2, 2) = -2 * invDepth;

		ret.at(3, 0) = -(right + left) * invWidth;
		ret.at(3, 1) = -(top + bottom) * invHeight;
		ret.at(3, 2) = -(zFar + zNear) * invDepth;

		return ret;
	}

	/**
	 * Creates new matrix 4x4 from array that represents such matrix 4x4
	 * as array of tightly packed elements in row major order.
	 * @param arr An array of elements for 4x4 matrix in row major order.
	 * @return An instance of Matrix4<T> representing @a arr
	 */
	template<class FromT>
	static Matrix4<T> fromRowMajorArray(const FromT* arr)
	{
		const T retData[] =
		{ static_cast<T>(arr[0]), static_cast<T>(arr[4]), static_cast<T>(arr[8]), static_cast<T>(arr[12]),
				static_cast<T>(arr[1]), static_cast<T>(arr[5]), static_cast<T>(arr[9]), static_cast<T>(arr[13]),
				static_cast<T>(arr[2]), static_cast<T>(arr[6]), static_cast<T>(arr[10]), static_cast<T>(arr[14]),
				static_cast<T>(arr[3]), static_cast<T>(arr[7]), static_cast<T>(arr[11]), static_cast<T>(arr[15]) };

		return retData;
	}

	/**
	 * Creates new matrix 4x4 from array that represents such matrix 4x4
	 * as array of tightly packed elements in column major order.
	 * @param arr An array of elements for 4x4 matrix in column major order.
	 * @return An instance of Matrix4<T> representing @a arr
	 */
	template<class FromT>
	static Matrix4<T> fromColumnMajorArray(const FromT* arr)
	{
		const T retData[] =
		{ static_cast<T>(arr[0]), static_cast<T>(arr[1]), static_cast<T>(arr[2]), static_cast<T>(arr[3]),
				static_cast<T>(arr[4]), static_cast<T>(arr[5]), static_cast<T>(arr[6]), static_cast<T>(arr[7]),
				static_cast<T>(arr[8]), static_cast<T>(arr[9]), static_cast<T>(arr[10]), static_cast<T>(arr[11]),
				static_cast<T>(arr[12]), static_cast<T>(arr[13]), static_cast<T>(arr[14]), static_cast<T>(arr[15]) };

		return retData;
	}

	//---------------------[ Equality operators ]------------------------------
	/**
	 * Equality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @note Test of equality is based of threshold EPSILON value. To be two
	 * values equal, must satisfy this condition all elements of matrix
	 * | lhs[i] - rhs[i] | < EPSILON,
	 * same for y-coordinate, z-coordinate, and w-coordinate.
	 */
	bool operator==(const Matrix4<T>& rhs) const
	{
		for (int i = 0; i < 16; i++)
		{
			if (std::fabs(data[i] - rhs.data[i]) >= EPSILON
				)
				return false;
		}
		return true;
	}

	/**
	 * Inequality test operator
	 * @param rhs Right hand side argument of binary operator.
	 * @return not (lhs == rhs) :-P
	 */
	bool operator!=(const Matrix4<T>& rhs) const
	{
		return !(*this == rhs);
	}

	//---------------------[ access operators ]---------------------------------
	/**
	 * get reference to element at postion (x,y).
	 * @param x Number of column (0..3)
	 * @param y Number of row (0..3)
	 */
	T& at(int x, int y)
	{
		assert(x >= 0 && x < 4);
		assert(y >= 0 && y < 4);
		return data[x * 4 + y];
	}

	/**
	 * get constant reference to element at position (x,y).
	 * @param x Number of column (0..3)
	 * @param y Number of row (0..3)
	 */
	const T& at(int x, int y) const
	{
		assert(x >= 0 && x < 4);
		assert(y >= 0 && y < 4);
		return data[x * 4 + y];
	}

	/**
	 * get reference to element at position (i,j), with math matrix notation
	 * @param i Number of row (1..4)
	 * @param j Number of column (1..4)
	 */
	T& operator()(int i, int j)
	{
		assert(i >= 1 && i <= 4);
		assert(j >= 1 && j <= 4);
		return data[(j - 1) * 4 + i - 1];
	}

	/**
	 * get constant reference to element at position (i,j), with math matrix notation
	 * @param i Number of row (1..4)
	 * @param j Number of column (1..4)
	 */
	const T& operator()(int i, int j) const
	{
		assert(i >= 1 && i <= 4);
		assert(j >= 1 && j <= 4);
		return data[(j - 1) * 4 + i - 1];
	}

	/**
	 * Sets translation part of matrix.
	 *
	 * @param v Vector of translation to be set.
	 */
	void setTranslation(const Vector3<T>& v)
	{
		at(3, 0) = v.x;
		at(3, 1) = v.y;
		at(3, 2) = v.z;
		at(3, 3) = 1;
	}

	Vector3<T> getTranslation() const
	{
		return Vector3<T>(at(3, 0), at(3, 1), at(3, 2));
	}

	/**
	 * Sets rotation part (matrix 3x3) of matrix.
	 *
	 * @param m Rotation part of matrix
	 */
	void setRotation(const Matrix3<T>& m)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				at(i, j) = m.at(i, j);
			}
		}
	}

	/**
	 * gets matrix scale
	 * @return Scales (i.e. first three values from matrix diagonal.
	 */
	Vector3<T> getScale() const
	{
		return Vector3<T>(at(0, 0), at(1, 1), at(2, 2));
	}

	/**
	 * Sets matrix uniform scale values
	 * @param s Uniform scale value
	 */
	void setScale(T s)
	{
		at(0, 0) = at(1, 1) = at(2, 2) = s;
	}

	/**
	 * Sets matrix scale for all axes.
	 * @param sx X-axis scale factor
	 * @param sy Y-axis scale factor
	 * @param sz Z-axis scale factor
	 */
	void setScale(T sx, T sy, T sz)
	{
		at(0, 0) = sx;
		at(1, 1) = sy;
		at(2, 2) = sz;
	}

	/**
	 * Sets matrix scale for all axes.
	 * @param s Scale factors for X, Y, and Z coordinate.
	 */
	void setScale(const Vector3<T>& s)
	{
		at(0, 0) = s.x;
		at(1, 1) = s.y;
		at(2, 2) = s.z;
	}

	/**
	 * Copy operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix4<T>& operator=(const Matrix4<T>& rhs)
	{
		std::memcpy(data, rhs.data, sizeof(T) * 16);
		return *this;
	}

	/**
	 * Copy casting operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	template<class FromT>
	Matrix4<T>& operator=(const Matrix4<FromT>& rhs)
	{
		for (int i = 0; i < 16; i++)
		{
			data[i] = static_cast<T>(rhs.data[i]);
		}
		return *this;
	}

	/**
	 * Copy operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix4<T>& operator=(const T* rhs)
	{
		std::memcpy(data, rhs, sizeof(T) * 16);
		return *this;
	}

	/*Matrix4<T> & operator=(const double* m)
	 {
	 for (int i = 0; i < 16; i++) data[i] = (T)m[i];
	 return * this;
	 }*/

	 //--------------------[ matrix with matrix operations ]---------------------
	 /**
	  * Addition operator
	  * @param rhs Right hand side argument of binary operator.
	  */
	Matrix4<T> operator+(const Matrix4<T>& rhs) const
	{
		Matrix4<T> ret;
		for (int i = 0; i < 16; i++)
			ret.data[i] = data[i] + rhs.data[i];
		return ret;
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix4<T> operator-(const Matrix4<T>& rhs) const
	{
		Matrix4<T> ret;
		for (int i = 0; i < 16; i++)
			ret.data[i] = data[i] - rhs.data[i];
		return ret;
	}

	//--------------------[ matrix with scalar operations ]---------------------
	/**
	 * Addition operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix4<T> operator+(T rhs) const
	{
		Matrix4<T> ret;
		for (int i = 0; i < 16; i++)
			ret.data[i] = data[i] + rhs;
		return ret;
	}

	/**
	 * Subtraction operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix4<T> operator-(T rhs) const
	{
		Matrix4<T> ret;
		for (int i = 0; i < 16; i++)
			ret.data[i] = data[i] - rhs;
		return ret;
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix4<T> operator*(T rhs) const
	{
		Matrix4<T> ret;
		for (int i = 0; i < 16; i++)
			ret.data[i] = data[i] * rhs;
		return ret;
	}

	/**
	 * Division operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix4<T> operator/(T rhs) const
	{
		Matrix4<T> ret;
		for (int i = 0; i < 16; i++)
			ret.data[i] = data[i] / rhs;
		return ret;
	}

	//--------------------[ multiply operators ]--------------------------------
	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector4<T> operator*(const Vector4<T>& rhs) const
	{
		return Vector4<T>(data[0] * rhs.x + data[4] * rhs.y + data[8] * rhs.z + data[12] * rhs.w,
			data[1] * rhs.x + data[5] * rhs.y + data[9] * rhs.z + data[13] * rhs.w,
			data[2] * rhs.x + data[6] * rhs.y + data[10] * rhs.z + data[14] * rhs.w,
			data[3] * rhs.x + data[7] * rhs.y + data[11] * rhs.z + data[15] * rhs.w);

	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Vector3<T> operator*(const Vector3<T>& rhs) const
	{
		return Vector3<T>(data[0] * rhs.x + data[4] * rhs.y + data[8] * rhs.z,
			data[1] * rhs.x + data[5] * rhs.y + data[9] * rhs.z,
			data[2] * rhs.x + data[6] * rhs.y + data[10] * rhs.z);
	}

	/**
	 * Multiplication operator
	 * @param rhs Right hand side argument of binary operator.
	 */
	Matrix4<T> operator*(Matrix4<T> rhs) const
	{
		Matrix4<T> w;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				T n = 0;
				for (int k = 0; k < 4; k++)
					n += rhs.at(i, k) * at(k, j);
				w.at(i, j) = n;
			}
		}
		return w;

	}

	//---------------------------[ misc operations ]----------------------------

	/**
	 * Computes determinant of matrix
	 * @return Determinant of matrix
	 * @note This function does 3 * 4 * 6 muls, 3 * 6 adds.
	 */
	T det()
	{

		return +at(3, 0) * at(2, 1) * at(1, 2) * at(0, 3) - at(2, 0) * at(3, 1) * at(1, 2) * at(0, 3)
			- at(3, 0) * at(1, 1) * at(2, 2) * at(0, 3) + at(1, 0) * at(3, 1) * at(2, 2) * at(0, 3)

			+ at(2, 0) * at(1, 1) * at(3, 2) * at(0, 3) - at(1, 0) * at(2, 1) * at(3, 2) * at(0, 3)
			- at(3, 0) * at(2, 1) * at(0, 2) * at(1, 3) + at(2, 0) * at(3, 1) * at(0, 2) * at(1, 3)

			+ at(3, 0) * at(0, 1) * at(2, 2) * at(1, 3) - at(0, 0) * at(3, 1) * at(2, 2) * at(1, 3)
			- at(2, 0) * at(0, 1) * at(3, 2) * at(1, 3) + at(0, 0) * at(2, 1) * at(3, 2) * at(1, 3)

			+ at(3, 0) * at(1, 1) * at(0, 2) * at(2, 3) - at(1, 0) * at(3, 1) * at(0, 2) * at(2, 3)
			- at(3, 0) * at(0, 1) * at(1, 2) * at(2, 3) + at(0, 0) * at(3, 1) * at(1, 2) * at(2, 3)

			+ at(1, 0) * at(0, 1) * at(3, 2) * at(2, 3) - at(0, 0) * at(1, 1) * at(3, 2) * at(2, 3)
			- at(2, 0) * at(1, 1) * at(0, 2) * at(3, 3) + at(1, 0) * at(2, 1) * at(0, 2) * at(3, 3)

			+ at(2, 0) * at(0, 1) * at(1, 2) * at(3, 3) - at(0, 0) * at(2, 1) * at(1, 2) * at(3, 3)
			- at(1, 0) * at(0, 1) * at(2, 2) * at(3, 3) + at(0, 0) * at(1, 1) * at(2, 2) * at(3, 3);

	}

	/**
	 * Computes inverse matrix
	 * @return Inverse matrix of this matrix.
	 * @note This is a little bit time consuming operation
	 * (16 * 6 * 3 muls, 16 * 5 adds + det() + mul() functions)
	 */
	Matrix4<T> inverse()
	{
		Matrix4<T> ret;

		ret.at(0, 0) = +at(2, 1) * at(3, 2) * at(1, 3) - at(3, 1) * at(2, 2) * at(1, 3) + at(3, 1) * at(1, 2) * at(2, 3)
			- at(1, 1) * at(3, 2) * at(2, 3) - at(2, 1) * at(1, 2) * at(3, 3) + at(1, 1) * at(2, 2) * at(3, 3);

		ret.at(1, 0) = +at(3, 0) * at(2, 2) * at(1, 3) - at(2, 0) * at(3, 2) * at(1, 3) - at(3, 0) * at(1, 2) * at(2, 3)
			+ at(1, 0) * at(3, 2) * at(2, 3) + at(2, 0) * at(1, 2) * at(3, 3) - at(1, 0) * at(2, 2) * at(3, 3);

		ret.at(2, 0) = +at(2, 0) * at(3, 1) * at(1, 3) - at(3, 0) * at(2, 1) * at(1, 3) + at(3, 0) * at(1, 1) * at(2, 3)
			- at(1, 0) * at(3, 1) * at(2, 3) - at(2, 0) * at(1, 1) * at(3, 3) + at(1, 0) * at(2, 1) * at(3, 3);

		ret.at(3, 0) = +at(3, 0) * at(2, 1) * at(1, 2) - at(2, 0) * at(3, 1) * at(1, 2) - at(3, 0) * at(1, 1) * at(2, 2)
			+ at(1, 0) * at(3, 1) * at(2, 2) + at(2, 0) * at(1, 1) * at(3, 2) - at(1, 0) * at(2, 1) * at(3, 2);

		ret.at(0, 1) = +at(3, 1) * at(2, 2) * at(0, 3) - at(2, 1) * at(3, 2) * at(0, 3) - at(3, 1) * at(0, 2) * at(2, 3)
			+ at(0, 1) * at(3, 2) * at(2, 3) + at(2, 1) * at(0, 2) * at(3, 3) - at(0, 1) * at(2, 2) * at(3, 3);

		ret.at(1, 1) = +at(2, 0) * at(3, 2) * at(0, 3) - at(3, 0) * at(2, 2) * at(0, 3) + at(3, 0) * at(0, 2) * at(2, 3)
			- at(0, 0) * at(3, 2) * at(2, 3) - at(2, 0) * at(0, 2) * at(3, 3) + at(0, 0) * at(2, 2) * at(3, 3);

		ret.at(2, 1) = +at(3, 0) * at(2, 1) * at(0, 3) - at(2, 0) * at(3, 1) * at(0, 3) - at(3, 0) * at(0, 1) * at(2, 3)
			+ at(0, 0) * at(3, 1) * at(2, 3) + at(2, 0) * at(0, 1) * at(3, 3) - at(0, 0) * at(2, 1) * at(3, 3);

		ret.at(3, 1) = +at(2, 0) * at(3, 1) * at(0, 2) - at(3, 0) * at(2, 1) * at(0, 2) + at(3, 0) * at(0, 1) * at(2, 2)
			- at(0, 0) * at(3, 1) * at(2, 2) - at(2, 0) * at(0, 1) * at(3, 2) + at(0, 0) * at(2, 1) * at(3, 2);

		ret.at(0, 2) = +at(1, 1) * at(3, 2) * at(0, 3) - at(3, 1) * at(1, 2) * at(0, 3) + at(3, 1) * at(0, 2) * at(1, 3)
			- at(0, 1) * at(3, 2) * at(1, 3) - at(1, 1) * at(0, 2) * at(3, 3) + at(0, 1) * at(1, 2) * at(3, 3);

		ret.at(1, 2) = +at(3, 0) * at(1, 2) * at(0, 3) - at(1, 0) * at(3, 2) * at(0, 3) - at(3, 0) * at(0, 2) * at(1, 3)
			+ at(0, 0) * at(3, 2) * at(1, 3) + at(1, 0) * at(0, 2) * at(3, 3) - at(0, 0) * at(1, 2) * at(3, 3);

		ret.at(2, 2) = +at(1, 0) * at(3, 1) * at(0, 3) - at(3, 0) * at(1, 1) * at(0, 3) + at(3, 0) * at(0, 1) * at(1, 3)
			- at(0, 0) * at(3, 1) * at(1, 3) - at(1, 0) * at(0, 1) * at(3, 3) + at(0, 0) * at(1, 1) * at(3, 3);

		ret.at(3, 2) = +at(3, 0) * at(1, 1) * at(0, 2) - at(1, 0) * at(3, 1) * at(0, 2) - at(3, 0) * at(0, 1) * at(1, 2)
			+ at(0, 0) * at(3, 1) * at(1, 2) + at(1, 0) * at(0, 1) * at(3, 2) - at(0, 0) * at(1, 1) * at(3, 2);

		ret.at(0, 3) = +at(2, 1) * at(1, 2) * at(0, 3) - at(1, 1) * at(2, 2) * at(0, 3) - at(2, 1) * at(0, 2) * at(1, 3)
			+ at(0, 1) * at(2, 2) * at(1, 3) + at(1, 1) * at(0, 2) * at(2, 3) - at(0, 1) * at(1, 2) * at(2, 3);

		ret.at(1, 3) = +at(1, 0) * at(2, 2) * at(0, 3) - at(2, 0) * at(1, 2) * at(0, 3) + at(2, 0) * at(0, 2) * at(1, 3)
			- at(0, 0) * at(2, 2) * at(1, 3) - at(1, 0) * at(0, 2) * at(2, 3) + at(0, 0) * at(1, 2) * at(2, 3);

		ret.at(2, 3) = +at(2, 0) * at(1, 1) * at(0, 3) - at(1, 0) * at(2, 1) * at(0, 3) - at(2, 0) * at(0, 1) * at(1, 3)
			+ at(0, 0) * at(2, 1) * at(1, 3) + at(1, 0) * at(0, 1) * at(2, 3) - at(0, 0) * at(1, 1) * at(2, 3);

		ret.at(3, 3) = +at(1, 0) * at(2, 1) * at(0, 2) - at(2, 0) * at(1, 1) * at(0, 2) + at(2, 0) * at(0, 1) * at(1, 2)
			- at(0, 0) * at(2, 1) * at(1, 2) - at(1, 0) * at(0, 1) * at(2, 2) + at(0, 0) * at(1, 1) * at(2, 2);

		return ret / det();
	}

	/**
	 * Transpose matrix.
	 */
	Matrix4<T> transpose()
	{
		Matrix4<T> ret;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				ret.at(i, j) = at(j, i);
			}
		}
		return ret;
	}

	/**
	 * Linear interpolation of two matrices
	 * @param fact Factor of interpolation. For translation from positon
	 * of this matrix (lhs) to matrix rhs, values of factor goes from 0.0 to 1.0.
	 * @param rhs Second Matrix for interpolation
	 * @note However values of fact parameter are reasonable only in interval
	 * [0.0 , 1.0], you can pass also values outside of this interval and you
	 * can get result (extrapolation?)
	 */
	Matrix4<T> lerp(T fact, const Matrix4<T>& rhs) const
	{
		Matrix4<T> ret = (*this) + (rhs - (*this)) * fact;
		return ret;
	}

	//-------------[ conversion ]-----------------------------
	/**
	 * Conversion to pointer operator
	 * @return Pointer to internally stored (in management of class Matrix4<T>)
	 * used for passing Matrix4<T> values to gl*[fd]v functions.
	 */
	operator T* ()
	{
		return (T*)data;
	}

	/**
	 * Conversion to pointer operator
	 * @return Constant Pointer to internally stored (in management of class Matrix4<T>)
	 * used for passing Matrix4<T> values to gl*[fd]v functions.
	 */
	operator const T* () const
	{
		return (const T*)data;
	}

};

/// Matrix 4x4 of floats
typedef Matrix4<float> Matrix4f;
/// Matrix 4x4 of doubles
typedef Matrix4<double> Matrix4d;
/// Matrix 4x4 of int
typedef Matrix4<int> Matrix4i;