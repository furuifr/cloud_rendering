/**
 *    > Author:   UncP
 *    > Mail:     770778010@qq.com
 *    > Github:   https://www.github.com/UncP/Giraffe
 *    > Description:
 *
 *    > Created Time: 2016-10-21 13:45:34
**/

#include "../math/constant.hpp"
#include "area.hpp"

namespace Giraffe {

AreaLight::AreaLight(const Point3d &position, const Vector3d &direction,
	const Vector3d &intensity, double angle)
:Light(intensity), position_(position), direction_(-normalize(direction)),
 cos_max_(std::cos(radian(angle))) { }

Vector3d AreaLight::illuminate(Isect &isect) const
{
	Vector3d vec(position_ - isect.position());
	double dis = vec.length();
	vec /= dis;
	if (dot(direction_, vec) > cos_max_) {
		isect.setDistance(dis);
		return vec;
	}
	return Vector3d(0);
}

Vector3d AreaLight::radiance(const Isect &isect) const
{
	Vector3d direction(position_ - isect.position());

	return intensity_ * (1.0 / direction.length2());
}

std::shared_ptr<Light> createAreaLight(Slice &slice)
{
	Point3d position = slice.findPosition();
	Vector3d direction = slice.findVector();
	Vector3d intensity = slice.findVector();
	double angle = slice.findDouble();
	assert(slice.eof());
	return std::shared_ptr<Light>(new AreaLight(position, direction, intensity, angle));
}

} // namespace Giraffe
