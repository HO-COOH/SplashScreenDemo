#pragma once
#include <d2d1.h>
#include <stdexcept>
#include <algorithm>

/*
	General form:
		P(t) =	P0(-t^3 + 3t^2 - 3t + 1) +
				P1(3t^3 - 6t^2 + 3t) + 
				P2(-3t^3 + 3t^2) +
				P3(t^3)
	and we have P0 and P3 fixed at (0,0) and (1,1), so 
		P(t) =
			P1(3t^3 - 6t^2 + 3t) + 
			P2(-3t^3 + 3t^2) +
			(1,1) (t^3)
		= 
			3P1*t(t^2-2t+1) + 
			3P2*t^2(1-t) + 
			t^3
		= 3P1*t(t-1)^2 + 
			3P2*t^2(1-t) + 
			t^3
		Let:
			Bx = 3*P1.x
			Cx = 3*P2.x
			Cx_Bx = 2 * (Cx - Bx)
			Three_Cx = 3 - Cx
		So:
			P(t).x = Bx * t(t-1)^2 + Cx * t^2(1-t) + t^3
				   = Bx * t(1-t)^2 + Cx * t^2(1-t) + t^3
			
			P'(t).x = Bx * (3t^2 - 4t + 1) + Cx * (2t - 3t^2) + 3t^2 
					= Bx * (1 + t^2 - 2t) + 2* (Cx - Bx) * (t - t^2) + (3 - Cx) t^2
					= Bx * (1 - t) ^ 2 + 2 * Cx_Bx * (1-t)*t + Three_Cx * t^2
*/
class KeySpline
{
	D2D1_POINT_2F controlPoint1{};
	D2D1_POINT_2F controlPoint2{ .x = 1.0f, .y = 1.0f };

	double bx;
	double cx;
	double cx_bx;
	double three_cx;

	double by;
	double cy;

	constexpr static auto epsilon = 0.000001;
	constexpr static auto accuracy = 0.001;

	std::pair<double, double> getXAndDx(double t) const
	{
		auto const one_minus_t = 1.0 - t;
		auto const t_squared = t * t;
		auto const one_minus_t_squared = one_minus_t * one_minus_t;

		return {
			bx * t * one_minus_t * one_minus_t + cx * t_squared * one_minus_t + t_squared * t,
			bx * one_minus_t_squared + cx_bx * one_minus_t * t + three_cx * t_squared
		};
	}
	
	// get t from x(time), that is the equation P(t).x = x, that is P(t).x - x = 0
	double getTFromX(double x) const
	{
		if (x < 0 || x > 1)
			throw std::invalid_argument{ "0 <= x <= 1" };

		if (x == 0)
			return 0;

		if (x == 1)
			return 1;

		double top = 1, bottom = 0, t = 0;
		while (top - bottom > epsilon)
		{
			auto const [x0, dx0] = getXAndDx(t);
			if (x0 > x)
				top = t;
			else
				bottom = t;

			auto absdx = abs(dx0);
			if (abs(x0 - x) < accuracy * absdx)
				return t;

			if (absdx > epsilon)
			{
				auto const next = t - (x0 - x) / dx0;

				if (next >= top)
					t = (t + top) / 2;
				else if (next <= bottom)
					t = (t + bottom) / 2;
				else
					t = next;
			}
			else
				t = (top + bottom) / 2;
		}
	}

	double getBezierValue(double t) const
	{
		auto const one_minus_t = 1 - t;
		auto const t_squared = t * t;
		return by * t * one_minus_t * one_minus_t + cy * t_squared * one_minus_t + t_squared * t;
	}

	void cacheCoefficient()
	{
		bx = 3 * controlPoint1.x;
		cx = 3 * controlPoint2.x;
		cx_bx = 2 * (cx - bx);
		three_cx = 3 - cx;

		by = 3 * controlPoint1.y;
		cy = 3 * controlPoint2.y;
	}
public:
	double GetSplineProgress(double progress) const
	{
		return getBezierValue(getTFromX(progress));
	}

	KeySpline(D2D1_POINT_2F controlPoint1) : controlPoint1{ controlPoint1 }
	{
		cacheCoefficient();
	}

	KeySpline(D2D1_POINT_2F controlPoint1, D2D1_POINT_2F controlPoint2) : controlPoint1{ controlPoint1 }, controlPoint2{ controlPoint2 }
	{
		cacheCoefficient();
	}
};
