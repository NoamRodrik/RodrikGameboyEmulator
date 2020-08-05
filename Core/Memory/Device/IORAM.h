/**
 * @file		IORAM.h
 * @author		Noam Rodrik
 * @description LR35902 main IO RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_IO_RAM_H__
#define __LR35902_MEMORY_DEVICE_IO_RAM_H__

#include <Core/Processor/Timer/Registers/TimerModulo.h>
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
		// Default value for the interrupt flag
		this->m_memory[GetFixedAddress(InterruptFlag::INTERRUPT_FLAG_ADDRESS)] = InterruptFlag::INTERRUPT_FLAG_DEFAULT_VALUE;
	}

	virtual bool Read(const address_t absolute_address, data_t& result) const override
	{
		result = this->m_memory[GetFixedAddress(absolute_address)];
		return true;
	}

	virtual void Write(const data_t data, const address_t absolute_address) override
	{
		if (this->ApplyChanges(data, absolute_address))
		{
			this->m_memory[GetFixedAddress(absolute_address)] = data;
		}
	}

	virtual bool Read(const address_t absolute_address, address_t& result) const override
	{
		result = this->m_memory[GetFixedAddress(absolute_address)] | (this->m_memory[GetFixedAddress(absolute_address)] << 8);
		return true;
	}

	virtual void Write(const address_t data, const address_t absolute_address) override
	{
		if (this->ApplyChanges(data & 0x00FF, absolute_address) &&
			this->ApplyChanges((data & 0xFF00) >> 8, absolute_address + 1))
		{
			this->m_memory[GetFixedAddress(absolute_address)] = data & 0x00FF;
			this->m_memory[GetFixedAddress(absolute_address) + 1] = (data & 0xFF00 >> 8);
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
	bool ApplyChanges(const data_t data, const address_t address)
	{
		switch (address)
		{
			case (SERIAL_TRANSFER_CONTROL):
			{
#if _DEBUG
				if (data == (SERIAL_TRANSFER_START | SERIAL_TRANSFER_CLOCK_SOURCE))
				{
					data_t byte_read{0};
					SANITY(this->Read(SERIAL_TRANSFER_DATA, byte_read), "Failed reading serial data");
					VISUAL_STUDIO_OUTPUT_VIEW_PRINT(byte_read);
				}
#endif
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
				// No special treatment
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
