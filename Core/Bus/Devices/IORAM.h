/**
 * @file		IORAM.h
 * @author		Noam Rodrik
 * @description LR35902 main IO RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_IO_RAM_H__
#define __LR35902_MEMORY_DEVICE_IO_RAM_H__

#include <Core/CPU/Interrupts/Registers/InterruptEnable.h>
#include <Core/CPU/Interrupts/Registers/InterruptFlag.h>
#include <Core/CPU/Timer/Registers/TimerControl.h>
#include <API/Memory/Device/IMemoryDeviceAccess.h>
#include <Core/CPU/Timer/Registers/TimerModulo.h>
#include <API/Memory/Device/IMemoryDevice.h>
#include <Core/Bus/Devices/CartridgeRAM.h>
#include <Core/CPU/Timer/Timer.h>
#include <API/Definitions.h>
#include <API/Memory/Memory.h>

namespace Core
{
class IORAM : public API::IMemoryDevice
{
public:
	constexpr IORAM(API::IMemoryDeviceAccess& memory_accessor) : API::IMemoryDevice{START_ADDRESS, END_ADDRESS, memory_accessor }, m_memory{}
	{
		// Default values
		this->m_memory[GetFixedAddress(API::TIMER_COUNTER_ADDRESS)] = TimerCounter::TIMER_COUNTER_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::TIMER_MODULO_ADDRESS)] = TimerModulo::TIMER_MODULO_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::TIMER_CONTROL_ADDRESS)] = TimerControl::TIMER_CONTROL_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR10_ADDRESS)] = API::NR10_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR11_ADDRESS)] = API::NR11_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR12_ADDRESS)] = API::NR12_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR14_ADDRESS)] = API::NR14_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR21_ADDRESS)] = API::NR21_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR22_ADDRESS)] = API::NR22_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR24_ADDRESS)] = API::NR24_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR30_ADDRESS)] = API::NR30_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR31_ADDRESS)] = API::NR31_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR32_ADDRESS)] = API::NR32_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR33_ADDRESS)] = API::NR33_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR41_ADDRESS)] = API::NR41_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR42_ADDRESS)] = API::NR42_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR43_ADDRESS)] = API::NR43_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR44_ADDRESS)] = API::NR44_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR50_ADDRESS)] = API::NR50_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR51_ADDRESS)] = API::NR51_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::NR52_ADDRESS)] = API::NR52_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::LCDC_ADDRESS)] = API::LCDC_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::SCY_ADDRESS)] = API::SCY_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::SCX_ADDRESS)] = API::SCX_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::LYC_ADDRESS)] = API::BGP_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::OBP0_ADDRESS)] = API::OBP0_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::OBP1_ADDRESS)] = API::OBP1_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::WY_ADDRESS)] = API::WY_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(API::WX_ADDRESS)] = API::WX_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(InterruptFlag::INTERRUPT_FLAG_ADDRESS)] = InterruptFlag::INTERRUPT_FLAG_DEFAULT_VALUE;
	}

	virtual bool Read(const API::address_t absolute_address, API::data_t& result) const override
	{
		result = this->m_memory[GetFixedAddress(absolute_address)];
		return true;
	}

	virtual void Write(const API::address_t absolute_address, const API::data_t data) override
	{
		if (this->ManipulateData(data, absolute_address))
		{
			this->m_memory[GetFixedAddress(absolute_address)] = data;
		}
	}

public:
	static constexpr uint16_t START_ADDRESS = 0xFF00;
	static constexpr uint16_t END_ADDRESS = 0xFF7F;
	static constexpr size_t   SIZE = END_ADDRESS - START_ADDRESS + 1;

protected:
	static constexpr API::address_t GetFixedAddress(const API::address_t address) { return address - START_ADDRESS; }
	virtual uint8_t* GetMemoryPointer() override { return this->m_memory.GetMemoryPointer(); }

private:
	bool ManipulateData(const API::data_t data, const API::address_t address)
	{
		switch (address)
		{
			case (API::SERIAL_TRANSFER_CONTROL):
			{
#ifdef TESTING
				Message("This is for testing purposes");
				if (data == (API::SERIAL_TRANSFER_START | API::SERIAL_TRANSFER_CLOCK_SOURCE))
				{
					API::data_t byte_read{0};
					SANITY(this->Read(API::SERIAL_TRANSFER_DATA, byte_read), "Failed reading serial data");
					SECONDARY_OUTPUT(byte_read);
				}
#endif
				break;
			}

			case (API::TIMER_MODULO_ADDRESS):
			{
				// Writing also onto the timer counter!
				this->m_memory[GetFixedAddress(API::TIMER_COUNTER_ADDRESS)] = data;

				break;
			}

			case (API::TIMER_COUNTER_ADDRESS):
			{
				if (Timer::IsCounterOverflow(data))
				{
					Timer::CounterOverflowInterrupt();
					TimerModulo modulo{};
					this->m_memory[GetFixedAddress(API::TIMER_COUNTER_ADDRESS)] = modulo;
					return false;
				}

				break;
			}

			case (API::TIMER_CONTROL_ADDRESS):
			{
				// Using only the allowed portion of the TAC.
				this->m_memory[GetFixedAddress(address)] = data & 0x07;
				return false;
				break;
			}

			case (API::DIVIDER_REGISTER_ADDRESS):
			{
				// Writing to the divier register resets the divider timer.
				this->m_memory[GetFixedAddress(address)] = 0;
				return false;
				break;
			}

			case (API::OVERRIDE_BOOTROM_ADDRESS):
			{
				// Once this is written with 0x1, we can read the software BOOT-ROM section.
				if (data == 0x01)
				{
					static_cast<CartridgeRAM*>(Processor::GetInstance().GetMemory().GetDeviceAtAddress(CartridgeRAM::START_ADDRESS))->CoverSystemBoot();
				}

				break;
			}
		}

		return true;
	}
	
private:
	API::Memory<SIZE> m_memory;

private:
	friend class DeviceManager;
	friend class Timer;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_IO_RAM_H__
