#pragma once
#include <string>

namespace GMusicApi
{

/** Values that represent song qualities. */
enum class SongQuality
{
	High,	//! 320kbps
	Medium,	//! 160kbps
	Low,	//! 128kbps
};

inline std::string songQualityName(SongQuality songQuality)
{
	switch (songQuality)
	{
		case SongQuality::High:
		return "hi";

		case SongQuality::Medium:
		return "med";

		case SongQuality::Low:
		return "low";

		default:
		assert("Unknown song quality");
		return "";
	}
}

} // namespace GMusicApi