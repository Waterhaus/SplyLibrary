#pragma once
#include "spline.h"
namespace sml 
{
	template<typename VectorSpaceType, std::size_t degree>
	spline<VectorSpaceType, degree>::spline() :
		_coefs(Vector<VectorSpaceType>::Zero(4u + degree - 2u)),
		_border({ 0., 1. }),
		_grid_size(4u) {}


	template<typename VectorSpaceType, std::size_t degree>
	spline<VectorSpaceType, degree>::spline(std::size_t grid_size, interval border) :
		_coefs(Vector<VectorSpaceType>::Zero(grid_size + degree - 2u)),
		_border(border),
		_grid_size(grid_size) {}

	template<typename VectorSpaceType, std::size_t degree>
	spline<VectorSpaceType, degree>::spline(const spline& f) :
		_coefs(f._coefs),
		_border(f._border),
		_grid_size(f._grid_size) {}

	template<typename U, typename T>
	T part_dot(const U& x, const std::vector<double>& y)
	{
		T tmp = 0. * x(0);
		for (int i = 0; i < y.size(); i++)
			tmp += x(i) * y[i];
		return tmp;
	}

	template<typename VectorSpaceType, std::size_t degree>
	VectorSpaceType spline<VectorSpaceType, degree>::operator()(double t) const
	{
		using vector = std::vector<double>;

		auto filter = [](double t)->double
		{
			if (t < 0.) return 0.;
			if (t > 1.) return 1.;
			return t;
		};
		/*������������ ���������. ������ 0 � VectorSpaceType*/
		VectorSpaceType sum = 0. * _coefs(0);

		const int index = cardinal::calculate_relative_index(t, _border, _grid_size);
		const auto& [a, b] = _border;
		const double inv_step = _border.get_inverse_step(_grid_size);
		const double step = _border.get_step(_grid_size);
		const double x = filter((t - (a + index * step)) * inv_step);

		const vector ksi_val = [x](int deg, int index, int grid_size) {

			auto val = cardinal::bsplvb(x, deg);

			if (index >= 0 && index < grid_size)
				return val;
			else {
				int delta = (index < 0) ? (index) : (index - grid_size + 1);

				/*���� �� �� ���������, ���������� ������ ������*/
				if (std::abs(delta) >= deg)
					return vector();

				if (delta < 0)
					return vector(val.begin() + std::abs(delta), val.end());
				else
					return vector(val.begin(), val.end() - delta);
			}
		}(degree, index, _grid_size);

		/*���� ������ � ��������� 0 <= index < grid_size
		* ����� ���������� ��������� �� _coefs ������� degree */
		
		
		auto block = _coefs.segment(index, degree);
		sum = part_dot<decltype(block), VectorSpaceType>(block, ksi_val);

		return sum;
	}

	template<typename VectorSpaceType, std::size_t degree>
	spline<VectorSpaceType, degree> spline<VectorSpaceType, degree>::operator+(const spline& other) const
	{
		spline<VectorSpaceType, degree> f(_grid_size, _border);

		f._coefs = _coefs + other._coefs;
	

		return std::move(f);
	}


}