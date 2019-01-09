/**
 *    > Author:   UncP
 *    > Mail:     770778010@qq.com
 *    > Github:   https://www.github.com/UncP/Giraffe
 *    > Description:
 *
 *    > Created Time: 2016-11-10 20:23:14
**/

#include <algorithm>

#include "cellular.hpp"

namespace Giraffe {

int CellularTexture::PoissonCount[256] = {
4,3,1,1,1,2,4,2,2,2,5,1,0,2,1,2,2,0,4,3,2,1,2,1,3,2,2,4,2,2,5,1,2,3,
2,2,2,2,2,3,2,4,2,5,3,2,2,2,5,3,3,5,2,1,3,3,4,4,2,3,0,4,2,2,2,1,3,2,
2,2,3,3,3,1,2,0,2,1,1,2,2,2,2,5,3,2,3,2,3,2,2,1,0,2,1,1,2,1,2,2,1,3,
4,2,2,2,5,4,2,4,2,2,5,4,3,2,2,5,4,3,3,3,5,2,2,2,2,2,3,1,1,4,2,1,3,3,
4,3,2,4,3,3,3,4,5,1,4,2,4,3,1,2,3,5,3,2,1,3,1,3,3,3,2,3,1,5,5,4,2,2,
4,1,3,4,1,5,3,3,5,3,4,3,2,2,1,1,1,1,1,2,4,5,4,5,4,2,1,5,1,1,2,3,3,3,
2,5,2,3,3,2,0,2,1,1,4,2,1,3,2,1,2,2,3,2,5,5,3,4,5,5,2,4,4,5,3,2,2,2,
1,4,2,3,3,4,2,5,4,2,4,2,2,2,4,5,3,2};

CellularTexture::CellularTexture(const Vector3d &color, int n_close, Distance type,
Combine combine):color_(color), n_close_(n_close), type_(type), combine_(combine) { }

double CellularTexture::distance(const Point3d &pos) const
{
	switch(type_) {
		case kEuclidean:
			return fabs(pos.x_) + fabs(pos.y_) + fabs(pos.z_);
		case kManhattan:
			return pos.x_ * pos.x_ + pos.y_ * pos.y_ + pos.z_ * pos.z_;
		case kSuperquadratic:
			return pow(fabs(pos.x_), 3) + pow(fabs(pos.y_), 3) + pow(fabs(pos.z_), 3);
		case kRadialManhattan:
			// A*fabs(dR)+B*fabs(dTheta)+C*dz
			return 3 * fabs(pos.x_) + 3 * fabs(pos.y_) + 3 * pos.z_;
		case kBiased:
			return 2 * pos.x_ * pos.x_ + 3 * pos.y_ * pos.y_ + 4 * pos.z_ * pos.z_;
	}
	assert(0);
}

Vector3d CellularTexture::combine(const std::vector<Neighbor> &neighbors) const
{
	switch(combine_) {
		case kDistance:
			return color_ * (neighbors[1].distance_-neighbors[0].distance_);
		case kClosest:
			return color_ * neighbors[0].distance_;
	}
	assert(0);
}

Vector3d CellularTexture::evaluate(Vertex &vertex) const
{
	const Point3d fpos = vertex.position() * (1.0 / n_close_);
	const Point3i ipos(	(int)std::floor(fpos.x_),
											(int)std::floor(fpos.y_),
											(int)std::floor(fpos.z_));
	std::vector<Neighbor> neighbors(n_close_, Neighbor());

	addSample(ipos, fpos, neighbors);

	double x2 = fpos.x_ - ipos.x_;
	double y2 = fpos.y_ - ipos.y_;
	double z2 = fpos.z_ - ipos.z_;
	double mx2 = (1.0 - x2) * (1.0 - x2);
	double my2 = (1.0 - y2) * (1.0 - y2);
	double mz2 = (1.0 - z2) * (1.0 - z2);
	x2 *= x2;
	y2 *= y2;
	z2 *= z2;
	if (x2 < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_-1, ipos.y_, ipos.z_), fpos, neighbors);
	if (y2 < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_, ipos.y_-1, ipos.z_), fpos, neighbors);
	if (z2 < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_, ipos.y_, ipos.z_-1), fpos, neighbors);
	if (mx2 < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_+1, ipos.y_, ipos.z_), fpos, neighbors);
	if (my2 < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_, ipos.y_+1, ipos.z_), fpos, neighbors);
	if (mz2 < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_, ipos.y_, ipos.z_+1), fpos, neighbors);

	if ((x2 + y2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_-1, ipos.y_-1, ipos.z_), fpos, neighbors);
	if ((x2 + z2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_-1, ipos.y_, ipos.z_-1), fpos, neighbors);
	if ((y2 + z2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_, ipos.y_-1, ipos.z_-1), fpos, neighbors);
	if ((mx2 + my2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_+1, ipos.y_+1, ipos.z_), fpos, neighbors);
	if ((mx2 + mz2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_+1, ipos.y_, ipos.z_+1), fpos, neighbors);
	if ((my2 + mz2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_, ipos.y_+1, ipos.z_+1), fpos, neighbors);
	if ((x2 + my2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_-1, ipos.y_+1, ipos.z_), fpos, neighbors);
	if ((x2 + mz2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_-1, ipos.y_, ipos.z_+1), fpos, neighbors);
	if ((y2 + mx2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_+1, ipos.y_-1, ipos.z_), fpos, neighbors);
	if ((y2 + mz2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_, ipos.y_-1, ipos.z_+1), fpos, neighbors);
	if ((z2 + mx2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_+1, ipos.y_, ipos.z_-1), fpos, neighbors);
	if ((z2 + my2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_, ipos.y_+1, ipos.z_-1), fpos, neighbors);

	if ((x2 + y2 + z2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_-1, ipos.y_-1, ipos.z_-1), fpos, neighbors);
	if ((x2 + y2 + mz2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_-1, ipos.y_-1, ipos.z_+1), fpos, neighbors);
	if ((x2 + my2 + z2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_-1, ipos.y_+1, ipos.z_-1), fpos, neighbors);
	if ((x2 + my2 + mz2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_-1, ipos.y_+1, ipos.z_+1), fpos, neighbors);
	if ((mx2 + y2 + z2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_+1, ipos.y_-1, ipos.z_-1), fpos, neighbors);
	if ((mx2 + y2 + mz2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_+1, ipos.y_-1, ipos.z_+1), fpos, neighbors);
	if ((mx2 + my2 + z2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_+1, ipos.y_+1, ipos.z_-1), fpos, neighbors);
	if ((mx2 + my2 + mz2) < neighbors[n_close_-1].distance_)
		addSample(Point3i(ipos.x_+1, ipos.y_+1, ipos.z_+1), fpos, neighbors);

	for (int i = 0; i != n_close_; ++i)
		neighbors[i].distance_ = std::sqrt(neighbors[i].distance_) * n_close_;

	return combine(neighbors);
}

void CellularTexture::addSample(const Point3i &ipos, const Point3d &fpos,
	std::vector<Neighbor> &neighbors) const
{
	static const double inv = 1.0 / 4294967296.0;

	#define next(seed) ((1402024253 * seed) + 586950981)
	#define get(seed) ((seed + 0.5) * inv)

	unsigned int seed = 702395077 * ipos.x_ + 915488749 * ipos.y_ + 2120969693 * ipos.z_;
	int count = PoissonCount[seed >> 24];
	seed = next(seed);

	Point3d f;
	for (int i = 0; i < count; ++i) {
		unsigned int id = seed;

		seed = next(seed), f.x_ = get(seed);
		seed = next(seed), f.y_ = get(seed);
		seed = next(seed), f.z_ = get(seed);

		Point3d d(ipos.x_ + f.x_ - fpos.x_,
							ipos.y_ + f.y_ - fpos.y_,
							ipos.z_ + f.z_ - fpos.z_);

		double d2 = distance(d);

		if (d2 < neighbors[n_close_-1].distance_) {
			neighbors[n_close_-1] = Neighbor(d2, d, id);
			std::sort(neighbors.begin(), neighbors.end());
		}
	}
	#undef next
	#undef get
}

std::shared_ptr<Texture> createCellularTexture(Slice &slice)
{
	Vector3d color = slice.findVector();
	int n_close = slice.findInteger();
	std::string s(slice.findString());
	CellularTexture::Distance distance = CellularTexture::kEuclidean;
	CellularTexture::Combine combine = CellularTexture::kDistance;
	if (s == "Euclidean")
		distance = CellularTexture::kEuclidean;
	else if (s == "Manhattan")
		distance = CellularTexture::kManhattan;
	else if (s == "Superquadratic")
		distance = CellularTexture::kSuperquadratic;
	else if (s == "Biased")
		distance = CellularTexture::kBiased;
	s = slice.findString();
	if (s == "Distance")
		combine = CellularTexture::kDistance;
	if (s == "Closest")
		combine = CellularTexture::kClosest;
	assert(slice.eof());
	return std::shared_ptr<Texture>(new CellularTexture(color, n_close, distance, combine));
}

} // namespace Giraffe
