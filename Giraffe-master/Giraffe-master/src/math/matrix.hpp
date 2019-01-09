/**
 *    > Author:   UncP
 *    > Mail:     770778010@qq.com
 *    > Github:   https://www.github.com/UncP/Giraffe
 *    > Description:
 *
 *    > Created Time: 2016-06-22 19:42:57
**/

#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <memory.h>

#include "constant.hpp"
#include "point.hpp"
#include "vector.hpp"

namespace Giraffe {

class Matrix2
{
	public:
		static Matrix2 Identity;

		template <typename T>
		inline Point2<T> 	operator()(const Point2<T> &) const;

		Matrix2(double m00, double m10, double m01, double m11) {
			m_[0][0] = m00;
			m_[1][0] = m10;
			m_[0][1] = m01;
			m_[1][1] = m11;
		}

	private:
		double m_[2][2];
};

Matrix2 rotate2(const double);
Matrix2 shear2X(const double);
Matrix2 shear2Y(const double);

template <typename T>
inline Point2<T> Matrix2::operator()(const Point2<T> &v) const {
	double x = m_[0][0] * v.x_ + m_[1][0] * v.y_;
	double y = m_[0][1] * v.x_ + m_[1][1] * v.y_;
	return Point2<T>(x, y);
}

class Matrix
{
	public:
		static Matrix Identity;

		union {
			double m_[4][4];
			double _m[16];
		};

		Matrix() = default;
		Matrix(	double m00, double m10, double m20, double m30, \
						double m01, double m11, double m21, double m31, \
						double m02, double m12, double m22, double m32, \
						double m03, double m13, double m23, double m33  \
		);
		Matrix(const Matrix &that) {
			memcpy(this->m_, that.m_, 16 * sizeof(double));
		}
		Matrix& operator=(const Matrix &that) {
			memcpy(this->m_, that.m_, 16 * sizeof(double));
			return *this;
		}

		bool operator==(const Matrix &that) const {
			return memcmp(m_, that.m_, sizeof(double) * 16) == 0;
		}
		bool operator!=(const Matrix &that) const {
			return !operator==(that);
		}

		Matrix  operator*(const Matrix &) const;
		Matrix& operator*=(const Matrix &);

		template <typename T>
		inline Vector3<T> operator()(const Vector3<T> &) const;
		template <typename T>
		inline Point3<T> 	operator()(const Point3<T> &) const;

		friend std::ostream& operator<<(std::ostream &os, const Matrix &m) {
			os << "Matrix\n";
			for (size_t i = 0; i < 4; ++i) {
				for (size_t j = 0; j < 4; ++j)
					os << std::setw(12) << m.m_[j][i];
				os << std::endl;
			}
			os << "Matrix\n";
			return os;
		}
};

Matrix inverse(const Matrix &);
Matrix transform(const Vector3d &);
Matrix scale(const double, const double, const double);
Matrix rotateX(const double);
Matrix rotateY(const double);
Matrix rotateZ(const double);
Matrix perspective(const double, const double, const double);

template <typename T>
inline Vector3<T> Matrix::operator()(const Vector3<T> &p) const {
	double x = m_[0][0] * p.x_ + m_[1][0] * p.y_ + m_[2][0] * p.z_ + m_[3][0];
	double y = m_[0][1] * p.x_ + m_[1][1] * p.y_ + m_[2][1] * p.z_ + m_[3][1];
	double z = m_[0][2] * p.x_ + m_[1][2] * p.y_ + m_[2][2] * p.z_ + m_[3][2];
	return Vector3<T>(x, y, z);
}

template <typename T>
inline Point3<T> Matrix::operator()(const Point3<T> &v) const {
	double x = m_[0][0] * v.x_ + m_[1][0] * v.y_ + m_[2][0] * v.z_ + m_[3][0];
	double y = m_[0][1] * v.x_ + m_[1][1] * v.y_ + m_[2][1] * v.z_ + m_[3][1];
	double z = m_[0][2] * v.x_ + m_[1][2] * v.y_ + m_[2][2] * v.z_ + m_[3][2];
	double w = m_[0][3] * v.x_ + m_[1][3] * v.y_ + m_[2][3] * v.z_ + m_[3][3];
	assert(w);
	if (w != 1) {
		double invW = 1.0 / w;
		x *= invW;
		y *= invW;
		z *= invW;
	}
	return Point3<T>(x, y, z);
}

} // namespace Giraffe

#endif /* _MATRIX_HPP_ */