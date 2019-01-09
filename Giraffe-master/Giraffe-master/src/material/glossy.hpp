/**
 *    > Author:   UncP
 *    > Mail:     770778010@qq.com
 *    > Github:   https://www.github.com/UncP/Giraffe
 *    > Description:
 *
 *    > Created Time: 2016-10-31 18:18:12
**/

#ifndef _GLOSSY_MATERIAL_HPP_
#define _GLOSSY_MATERIAL_HPP_

#include "../core/material.hpp"

namespace Giraffe {

class Glossy : public Material
{
	public:

		Glossy(Type type, const Vector3d &color, double roughness)
		:Material(Type(type | kDiffuse)), color_(color),
		 sin_(sin(radian(roughness))), cos_(cos(radian(roughness)))  {
			assert(type == kGlossy);
		}

		Color sample(const Vector3d &out, Vector3d &in, const Vector3d &normal, double &pdf)override;
	private:
		using Material::type_;
		Vector3d color_;
		double sin_;
		double cos_;
};

} // namespace Giraffe

#endif /* _GLOSSY_MATERIAL_HPP_ */