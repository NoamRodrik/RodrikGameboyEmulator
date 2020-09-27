/**
 * @file		Header.cpp
 * @author		Noam Rodrik
 * @description Main logic of header
 */
#include "Header.h"

namespace API
{
const CartridgeHeader::ROMSizeValue CartridgeHeader::ROMSize() const
{
	switch (CartridgeHeader::ROM())
	{
		case (ROMSizeType::_32_KB):
			return ROMSizeValue::_32_KB;
		case (ROMSizeType::_64_KB):
			return ROMSizeValue::_64_KB;
		case (ROMSizeType::_128_KB):
			return ROMSizeValue::_128_KB;
		case (ROMSizeType::_256_KB):
			return ROMSizeValue::_256_KB;
		case (ROMSizeType::_512_KB):
			return ROMSizeValue::_512_KB;
		case(ROMSizeType::_1000_KB):
			return ROMSizeValue::_1000_KB;
		case (ROMSizeType::_2000_KB):
			return ROMSizeValue::_2000_KB;
		case (ROMSizeType::_4000_KB):
			return ROMSizeValue::_4000_KB;
		case (ROMSizeType::_8000_KB):
			return ROMSizeValue::_8000_KB;
		case (ROMSizeType::_1100_KB):
			return ROMSizeValue::_1100_KB;
		case (ROMSizeType::_1200_KB):
			return ROMSizeValue::_1200_KB;
		case (ROMSizeType::_1500_KB):
			return ROMSizeValue::_1500_KB;
	}

	return ROMSizeValue::_2000_KB;
}

const CartridgeHeader::RAMSizeValue CartridgeHeader::RAMSize() const
{
	switch (CartridgeHeader::RAM())
	{
		case (RAMSizeType::NONE):
			return RAMSizeValue::NONE;
		case (RAMSizeType::_128_KB):
			return RAMSizeValue::_128_KB;
		case (RAMSizeType::_2_KB):
			return RAMSizeValue::_2_KB;
		case (RAMSizeType::_32_KB):
			return RAMSizeValue::_32_KB;
		case (RAMSizeType::_64_KB):
			return RAMSizeValue::_64_KB;
		case (RAMSizeType::_8_KB):
			return RAMSizeValue::_8_KB;
	}

	return RAMSizeValue::NONE;
}
}