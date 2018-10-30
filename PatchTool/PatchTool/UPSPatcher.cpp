#include "pch.h"
#include "UPSPatcher.h"

// ?????
template <typename CharT>
static uintmax_t UPSPatcher::ReadVariableWidthInteger(const std::vector<CharT> &Data, std::size_t &Position)
{
	uintmax_t result = 0, shift = 0;

	for (;;) {
		uint8_t octet = Data[Position];
		++Position;
		if (octet & 0x80) {
			result += (octet & 0x7f) << shift;
			break;
		}
		result += (octet | 0x80) << shift;
		shift += 7;
	}

	return result;
}

UPSPatcher::UPSPatcher()
{
}

UPSPatcher::~UPSPatcher()
{
}
