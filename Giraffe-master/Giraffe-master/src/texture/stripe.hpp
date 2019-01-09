/**
 *    > Author:   UncP
 *    > Mail:     770778010@qq.com
 *    > Github:	  https://www.github.com/UncP/Giraffe
 *    > Description:
 *
 *    > Created Time: 2016-09-17 16:44:01
**/

#ifndef _STRIPE_TEXTURE_HPP_
#define _STRIPE_TEXTURE_HPP_

#include "../math/matrix.hpp"
#include "../core/texture.hpp"

namespace Giraffe {

class StripeTexture : public Texture
{
	public:
		StripeTexture(const Vector3d &color1, const Vector3d &color2, int axis,
									const double factor, const Matrix &transform)
		:color1_(color1), color2_(color2), transform_(transform), axis_(axis), factor_(factor) { }

		Vector3d evaluate(Vertex &vertex) const override;

	private:
		Vector3d color1_;
		Vector3d color2_;
		Matrix   transform_;
		int      axis_;
		double 	 factor_;
};

std::shared_ptr<Texture> createStripeTexture(Slice &slice);

} // namepsace Giraffe

#endif /* _STRIPE_TEXTURE_HPP_ */