/**
 *    > Author:   UncP
 *    > Mail:     770778010@qq.com
 *    > Github:	  https://www.github.com/UncP/Giraffe
 *    > Description:
 *
 *    > Created Time: 2016-09-17 16:48:57
**/

#ifndef _MARBLE_TEXUTRE_HPP_
#define _MARBLE_TEXUTRE_HPP_

#include "../core/texture.hpp"

namespace Giraffe {

class MarbleTexture : public Texture
{
	public:
		MarbleTexture(const Vector3d &color1, const Vector3d &color2, const Vector3d &color3,
									double frequency)
		:color1_(color1), color2_(color2), color3_(color3), frequency_(frequency) { }

		Vector3d evaluate(Vertex &vertex) const override;

	private:
		Vector3d color1_;
		Vector3d color2_;
		Vector3d color3_;
		double   frequency_;
};

std::shared_ptr<Texture> createMarbleTexture(Slice &slice);

} // namespace Giraffe

#endif /* _MARBLE_TEXUTRE_HPP_ */