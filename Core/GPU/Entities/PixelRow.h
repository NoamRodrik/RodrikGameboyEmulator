/**
 * @file		PixelRow.h
 * @author		Noam Rodrik
 * @description LR35902 pixel row object for the PPU
 */
#ifndef __LR35902_PIXEL_ROW_H__
#define __LR35902_PIXEL_ROW_H__

#include <Core/GPU/Definitions.h>
#include <Core/CPU/Processor.h>
#include <API/Definitions.h>
#include <Tools/Tools.h>

namespace Core
{
/**
 * Rows are described in this form:
 * Each pixel has two bits, (ranging from 0x00 to 0x03).
 * 
 * Upper: 0|0|0|0|0|0|1|0
 * Lower: 1|1|1|1|1|1|1|1
 * 
 * Each column is a bit, but data is read as a row.
 * Thus there are 8 pixels in one row.
 */
class PixelRow
{
public:
	constexpr PixelRow() = default;
	constexpr PixelRow(API::data_t upper, API::data_t lower) : m_upper{upper},
															   m_lower{lower} {}
	PixelRow(API::address_t address)
	{
		SANITY(this->LoadPixelRow(address), "Failed loading pixel row");
	}

	~PixelRow() = default;

public:
	bool LoadPixelRow(API::address_t address)
	{
		return Processor::GetInstance().GetMemory().Read(address, m_upper) &&
		       Processor::GetInstance().GetMemory().Read(address + 1, m_lower);
	}

	constexpr auto GetColorByIndex(uint8_t index) const
	{
		SANITY(index <= 7, "Can't get an index higher than bits");
		return PaletteColor{(m_upper >> index) & 0x01 | (((m_lower >> index) & 0x01) << 1)};
	}

public:
	static constexpr auto PIXEL_COUNT{8};

private:
	API::data_t m_upper{0};
	API::data_t m_lower{0};
};
}

#endif // __LR35902_PIXEL_ROW_H__