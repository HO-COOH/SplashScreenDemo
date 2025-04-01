#pragma once
#include "KeySpline.h"
#include <chrono>
#include <cmath>

struct SplineDoubleKeyFrame
{
	double value;
	std::chrono::milliseconds keyTime;
	KeySpline keySpline;

	double getValue(double baseValue, double keyFrameProgress) const
	{
		auto const splineProgress = keySpline.GetSplineProgress(keyFrameProgress);
		return std::lerp(baseValue, value, splineProgress);
	}
};
