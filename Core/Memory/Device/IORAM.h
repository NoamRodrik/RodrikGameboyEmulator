/**
 * @file		IORAM.h
 * @author		Noam Rodrik
 * @description LR35902 main IO RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_IO_RAM_H__
#define __LR35902_MEMORY_DEVICE_IO_RAM_H__

#include <Core/Processor/Timer/Registers/TimerControl.h>
#include <Core/Processor/Timer/Registers/TimerModulo.h>
#include <Core/Interrupts/Registers/InterruptEnable.h>
#include <Core/API/Memory/Device/MemoryDeviceBase.h>
#include <Core/Interrupts/Registers/InterruptFlag.h>
#include <Core/Memory/Device/CartridgeRAM.h>
#include <Core/Processor/Timer/Timer.h>
#include <Core/API/Definitions.h>
#include <Core/Memory/Memory.h>

namespace Core
{
class DeviceManagerBase;
} // Core

namespace Core
{
class IORAM : public MemoryDeviceBase
{
public:
	constexpr IORAM(DeviceManagerBase& device_manager) : MemoryDeviceBase{START_ADDRESS, END_ADDRESS, device_manager}, m_memory{}
	{
		// Default values
		this->m_memory[GetFixedAddress(TIMER_COUNTER_ADDRESS)] = TimerCounter::TIMER_COUNTER_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(TIMER_MODULO_ADDRESS)] = TimerModulo::TIMER_MODULO_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(TIMER_CONTROL_ADDRESS)] = TimerControl::TIMER_CONTROL_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR10_ADDRESS)] = NR10_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR11_ADDRESS)] = NR11_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR12_ADDRESS)] = NR12_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR14_ADDRESS)] = NR14_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR21_ADDRESS)] = NR21_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR22_ADDRESS)] = NR22_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR24_ADDRESS)] = NR24_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR30_ADDRESS)] = NR30_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR31_ADDRESS)] = NR31_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR32_ADDRESS)] = NR32_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR33_ADDRESS)] = NR33_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR41_ADDRESS)] = NR41_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR42_ADDRESS)] = NR42_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR43_ADDRESS)] = NR43_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR44_ADDRESS)] = NR44_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR50_ADDRESS)] = NR50_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR51_ADDRESS)] = NR51_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(NR52_ADDRESS)] = NR52_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(LCDC_ADDRESS)] = LCDC_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(SCY_ADDRESS)] = SCY_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(SCX_ADDRESS)] = SCX_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(LYC_ADDRESS)] = BGP_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(OBP0_ADDRESS)] = OBP0_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(OBP1_ADDRESS)] = OBP1_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(WY_ADDRESS)] = WY_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(WX_ADDRESS)] = WX_DEFAULT_VALUE;
		this->m_memory[GetFixedAddress(InterruptFlag::INTERRUPT_FLAG_ADDRESS)] = InterruptFlag::INTERRUPT_FLAG_DEFAULT_VALUE;
	}

	virtual bool Read(const address_t absolute_address, data_t& result) const override
	{
		result = this->m_memory[GetFixedAddress(absolute_address)];
		return true;
	}

	virtual void Write(const address_t absolute_address, const data_t data) override
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
	static constexpr address_t GetFixedAddress(const address_t address) { return address - START_ADDRESS; }
	virtual uint8_t* GetMemoryPointer() override { return this->m_memory.GetMemoryPointer(); }

private:
	bool ManipulateData(const data_t data, const address_t address)
	{
		switch (address)
		{
			case (SERIAL_TRANSFER_CONTROL):
			{
				Message("This is for testing purposes");
				if (data == (SERIAL_TRANSFER_START | SERIAL_TRANSFER_CLOCK_SOURCE))
				{
					data_t byte_read{0};
					SANITY(this->Read(SERIAL_TRANSFER_DATA, byte_read), "Failed reading serial data");
					SECONDARY_OUTPUT(byte_read);
				}

				break;
			}

			case (TIMER_MODULO_ADDRESS):
			{
				// Writing also onto the timer counter!
				this->m_memory[GetFixedAddress(TIMER_COUNTER_ADDRESS)] = data;

				break;
			}

			case (TIMER_COUNTER_ADDRESS):
			{
				if (Timer::IsCounterOverflow(data))
				{
					Timer::CounterOverflowInterrupt();
					TimerModulo modulo{};
					this->m_memory[GetFixedAddress(TIMER_COUNTER_ADDRESS)] = modulo;
					return false;
				}

				break;
			}

			case (TIMER_CONTROL_ADDRESS):
			{
				// Using only the allowed portion of the TAC.
				this->m_memory[GetFixedAddress(address)] = data & 0x07;
				return false;
				break;
			}

			case (DIVIDER_REGISTER_ADDRESS):
			{
				// Writing to the divier register resets the divider timer.
				this->m_memory[GetFixedAddress(address)] = 0;
				return false;
				break;
			}

			case (OVERRIDE_BOOTROM_ADDRESS):
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
	Memory<SIZE> m_memory;

private:
	friend class DeviceManager;
	friend class Timer;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_IO_RAM_H__
