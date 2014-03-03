/**
 * \file
 * \author Lars Bilke
 * \date   2009-10-27
 * \brief  Definition of the Vector3 class.
 *         From: http://www.strout.net/info/coding/classlib/intro.html
 *         with modifications to derive from TemplatePoint
 *
 * \copyright
 * Copyright (c) 2013, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 */

#ifndef VECTOR3_H
#define VECTOR3_H

// MathLib
#include "TemplatePoint.h"

// MathLib
#include "MathTools.h"

#include <cmath>
#include <iostream>

namespace MathLib
{
/**
 * The Vector3 class defines a three-dimensional vector, with appropriate
 * operators.
 */
template <class T>
class TemplateVector3 : public MathLib::TemplatePoint<T>
{
public:
	/**
	 * Default constructor. All coordinates are set to zero.
	 */
	TemplateVector3() : TemplatePoint<T>() {}

	TemplateVector3(T x0, T x1, T x2)
	{
		this->_x[0] = x0;
		this->_x[1] = x1;
		this->_x[2] = x2;
	}

	/**
	 * Copy constructor.
	 */
	TemplateVector3(TemplateVector3<T> const& v) :
		TemplatePoint<T>(v)
	{}

	/**
	 * Construct Vector3 from TemplatePoint.
	 */
	TemplateVector3(TemplatePoint<T,3> const& p) :
		TemplatePoint<T>(p)
	{}

	/** constructs the vector v=(b-a) from the given points */
	TemplateVector3(const MathLib::TemplatePoint<T> &a, const MathLib::TemplatePoint<T> &b) :
		MathLib::TemplatePoint<T>()
	{
		this->_x[0] = b[0] - a[0];
		this->_x[1] = b[1] - a[1];
		this->_x[2] = b[2] - a[2];
	}

	~TemplateVector3() {}

	// vector arithmetic
	TemplateVector3 operator+(TemplateVector3 const& v) const
	{
		return TemplateVector3(this->_x[0]+v[0], this->_x[1]+v[1], this->_x[2]+v[2]);
	}

	TemplateVector3 operator-(TemplateVector3 const& v) const
	{
		return TemplateVector3(this->_x[0]-v[0], this->_x[1]-v[1], this->_x[2]-v[2]);
	}

	TemplateVector3& operator+=(TemplateVector3 const& v)
	{
		for (std::size_t i(0); i < 3; i++) this->_x[i] += v[i];
		return *this;
	}

	TemplateVector3& operator-=(const TemplateVector3 & pV)
	{
		for (std::size_t i(0); i < 3; i++) this->_x[i] -= pV[i];
		return *this;
	}

	/** scalarProduct, implementation of scalar product,
	 * sometimes called dot or inner product.
	 */
	template <typename T1>
	friend T1 scalarProduct(TemplateVector3<T1> const& v, TemplateVector3<T1> const& w);

	/** crossProduct: implementation of cross product,
	 * sometimes called outer product.
	 */
	template <typename T1>
	friend TemplateVector3<T1> crossProduct(
		TemplateVector3<T1> const& v,
		TemplateVector3<T1> const& w);

	/// Cross product with another vector
	TemplateVector3 Cross( const TemplateVector3 & pV ) const
	{ return *this * pV; }

	friend TemplateVector3 Cross( const TemplateVector3 & p1, const TemplateVector3 & p2 )
	{ return p1 * p2; }

	TemplateVector3 operator*(double pR) const   // * a scalar
	{
		return TemplateVector3(this->x[0] * pR, this->x[1] * pR, this->x[2] * pR);
	}

	friend TemplateVector3 operator*(double pR, const TemplateVector3 & pV)
	{
		return TemplateVector3 ( pV[0] * pR, pV[1] * pR, pV[2] * pR );
	}

	TemplateVector3& operator*=(double pR)
	{
		for (std::size_t i(0); i < 3; i++) this->_x[i] *= pR;
		return *this;
	}

	/// Returns the squared length
	double LenSqr(void) const
	{
		return scalarProduct<double,3> (this->getCoords (), this->getCoords ());
	}

	/// Returns the length
	double Length(void) const
	{ return sqrt(LenSqr()); }

	/// Projection (component of *this parallel to pV).
	/// Note: component perpendicular to pV is:  *this - Proj(pV)
	TemplateVector3 Proj(const TemplateVector3 & pV)
	{ TemplateVector3 out( pV * (this->Dot(pV) / pV.LenSqr()) ); return out; }

	/// Cosine of the angle between two vectors:
	double CosAng(const TemplateVector3& pV)
	{ return this->Dot(pV) / (Length() * pV.Length()); }

	/// Comparison if equal
	bool operator==( const TemplateVector3 & pV) const
	{
		return std::fabs(this->_x[0] - pV[0]) < sqrt(std::numeric_limits<double>::min()) &&
		       std::fabs(this->_x[1] - pV[1]) < sqrt(std::numeric_limits<double>::min()) &&
		       std::fabs(this->_x[2] - pV[2]) < sqrt(std::numeric_limits<double>::min());
	}

	/// Comparison if not equal
	bool operator!=( const TemplateVector3 & pV) const
	{
		return !(pV == this);
	}
};

template <typename T>
T scalarProduct(TemplateVector3<T> const& v, TemplateVector3<T> const& w)
{
	return v._x[0] * w._x[0] + v._x[1] * w._x[1] + v._x[2] * w._x[2];
}

template <typename T1>
TemplateVector3<T1> crossProduct(
		TemplateVector3<T1> const& v,
		TemplateVector3<T1> const& w)
{
	return TemplateVector3<T1>(
			v._x[1] * w._x[2] - v._x[2] * w._x[1],
			v._x[2] * w._x[0] - v._x[0] * w._x[2],
			v._x[0] * w._x[1] - v._x[1] * w._x[0]);
}

typedef TemplateVector3<double> Vector3;
}

#endif // VECTOR3_H
