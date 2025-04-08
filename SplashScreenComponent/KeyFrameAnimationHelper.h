#pragma once

template<typename Derived, typename KeyFrameType, typename EasingKeyFrameType>
class KeyFrameAnimationHelper
{
	void insertKeyFrameHelper(auto&& keyFrame)
	{
		using Type = std::remove_cvref_t<decltype(keyFrame)>;
		if constexpr (std::is_same_v<Type, KeyFrameType>)
		{
			static_cast<Derived*>(this)->InsertKeyFrame(keyFrame.normalizedProgressKey, keyFrame.value);
		}
		else if constexpr (std::is_same_v<Type, EasingKeyFrameType>)
		{
			static_cast<Derived*>(this)->InsertKeyFrame(keyFrame.normalizedProgressKey, keyFrame.value, keyFrame.easingFunction);
		}
		else
		{
			static_assert("Invalid key frame type");
		}
	}
public:
	KeyFrameAnimationHelper(auto&&... keyFrames)
	{
		(insertKeyFrameHelper(keyFrames), ...);
	}
};