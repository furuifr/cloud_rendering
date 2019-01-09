/**
 *    > Author:   UncP
 *    > Mail:     770778010@qq.com
 *    > Github:   https://www.github.com/UncP/Giraffe
 *    > Description:
 *
 *    > Created Time: 2016-10-31 18:22:40
**/

#include "../utility/sampler.hpp"
#include "glossy.hpp"

namespace Giraffe {

Color Glossy::sample(const Vector3d &out, Vector3d &in, const Vector3d &normal, double &pdf)
{
	Vector3d u, v, w(normalize(out - (2 * dot(out, normal)) * normal));

	if (std::fabs(w.x_) > 0.1)
		u = normalize(cross(Vector3d(0, 1, 0), w));
	else
		u = normalize(cross(Vector3d(1, 0, 0), w));
	v = normalize(cross(w, u));

	double a = Sampler::get1D(), phi = DOU_PI * a;

	in = normalize((sin_*std::cos(phi)*u) + (sin_*std::sin(phi)*v) + (cos_*w));

	return color_;
}

} // namespace Giraffe
