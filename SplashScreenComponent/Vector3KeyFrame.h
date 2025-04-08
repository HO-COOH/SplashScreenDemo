#pragma once
#include "KeyFrameBase.h"
#include <winrt/Windows.Foundation.Numerics.h>

using Vector3KeyFrame = KeyFrameBase<winrt::Windows::Foundation::Numerics::float3>;
using EasingVector3KeyFrame = EasingKeyFrameBase<winrt::Windows::Foundation::Numerics::float3>;
