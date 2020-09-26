/**
 * @file		IORAM.h
 * @author		Noam Rodrik
 * @description LR35902 main IO RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_IO_RAM_H__
#define __LR35902_MEMORY_DEVICE_IO_RAM_H__

#include <Core/CPU/Interrupts/Registers/InterruptEnable.h>
#include <Core/CPU/Interrupts/Registers/InterruptFlag.h>
#include <Core/CPU/Timers/Registers/DividerRegister.h>
#include <Core/CPU/Timers/Registers/TimerCounter.h>
#include <Core/CPU/Timers/Registers/TimerControl.h>
#include <Core/CPU/Timers/Registers/TimerModulo.h>
#include <Core/GPU/Registers/LCDC_Control.h>
#include <Core/GPU/Registers/LCDC_Status.h>
#include <Core/Bus/Devices/CartridgeRAM.h>
#include <Core/GPU/Registers/OBP1.h>
#include <Core/GPU/Registers/OBP0.h>
#include <Core/GPU/Registers/BGP.h>
#include <Core/GPU/Registers/DMA.h>
#include <Core/GPU/Registers/SCX.h>
#include <Core/GPU/Registers/SCY.h>
#include <Core/GPU/Registers/LYC.h>
#include <Core/GPU/Registers/LY.h>
#include <Core/GPU/Registers/WY.h>
#include <Core/GPU/Registers/WX.h>
#include <Core/CPU/Timers/Timer.h>
#include <Core/Bus/RAMDevice.h>
#include <API/Definitions.h>
#include <mutex>

namespace Core
{
class IORAM : public RAMDevice<0xFF00, 0xFF7F>
{
public:
	IORAM(API::IMemoryDeviceAccess& memory_accessor) : RAMDevice{memory_accessor}
	{
		this->_memory[this->RelativeAddress(DividerRegister::DIVIDER_REGISTER_ADDRESS_LSB)] = DividerRegister::DIVIDER_REGISTER_DEFAULT_VALUE_LSB;
		this->_memory[this->RelativeAddress(DividerRegister::DIVIDER_REGISTER_ADDRESS)] = DividerRegister::DIVIDER_REGISTER_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(TimerCounter::TIMER_COUNTER_ADDRESS)] = TimerCounter::TIMER_COUNTER_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(TimerModulo::TIMER_MODULO_ADDRESS)] = TimerModulo::TIMER_MODULO_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(TimerControl::TIMER_CONTROL_ADDRESS)] = TimerControl::TIMER_CONTROL_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR10_ADDRESS)] = API::NR10_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR11_ADDRESS)] = API::NR11_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR12_ADDRESS)] = API::NR12_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR14_ADDRESS)] = API::NR14_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR21_ADDRESS)] = API::NR21_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR22_ADDRESS)] = API::NR22_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR24_ADDRESS)] = API::NR24_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR30_ADDRESS)] = API::NR30_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR31_ADDRESS)] = API::NR31_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR32_ADDRESS)] = API::NR32_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR33_ADDRESS)] = API::NR33_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR41_ADDRESS)] = API::NR41_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR42_ADDRESS)] = API::NR42_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR43_ADDRESS)] = API::NR43_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR44_ADDRESS)] = API::NR44_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR50_ADDRESS)] = API::NR50_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR51_ADDRESS)] = API::NR51_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(API::NR52_ADDRESS)] = API::NR52_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(LCDC_Control::LCDC_ADDRESS)] = LCDC_Control::LCDC_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(LCDC_Status::LCDC_ADDRESS)] = LCDC_Status::LCDC_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(SCY::SCY_ADDRESS)] = SCY::SCY_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(SCX::SCX_ADDRESS)] = SCX::SCX_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(LY::LY_ADDRESS)] = LY::LY_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(LYC::LYC_ADDRESS)] = LYC::LYC_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(DMA::DMA_ADDRESS)] = DMA::DMA_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(BGP::BGP_ADDRESS)] = BGP::BGP_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(OBP0::OBP0_ADDRESS)] = OBP0::OBP0_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(OBP1::OBP1_ADDRESS)] = OBP1::OBP1_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(WY::WY_ADDRESS)] = WY::WY_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(WX::WX_ADDRESS)] = WX::WX_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(InterruptFlag::INTERRUPT_FLAG_ADDRESS)] = InterruptFlag::INTERRUPT_FLAG_DEFAULT_VALUE;
	}

private:
	virtual bool Intercept(const API::address_t absolute_address, const API::data_t data) override
	{
		switch (absolute_address)
		{
			case (API::SERIAL_TRANSFER_CONTROL):
			{
#ifdef SERIAL_TRANSFER_PRINT_DEBUG
				Message("This is for testing purposes");
				if (data == (API::SERIAL_TRANSFER_START | API::SERIAL_TRANSFER_CLOCK_SOURCE))
				{
					API::data_t byte_read{0};
					SANITY(this->Read(API::SERIAL_TRANSFER_DATA, byte_read), "Failed reading serial data");
					MAIN_LOG_NO_ENTER("%c", byte_read);
				}
#endif
				break;
			}

			case (TimerModulo::TIMER_MODULO_ADDRESS):
			{
				// Writing also onto the timer counter!
				// NOTE: This can't be *Write* since it will cause an infinite loop!
				this->_memory[this->RelativeAddress(TimerCounter::TIMER_COUNTER_ADDRESS)] = data;

				break;
			}

			case (TimerCounter::TIMER_COUNTER_ADDRESS):
			{
				if (Timer::IsCounterOverflow(data))
				{
					// NOTE: This can't be *Write* since it will cause an infinite loop!
					this->_memory[this->RelativeAddress(absolute_address)] = this->_memory[this->RelativeAddress(TimerModulo::TIMER_MODULO_ADDRESS)];
					Timer::LaunchInterrupt();
					return true;
				}

				break;
			}

			case (TimerControl::TIMER_CONTROL_ADDRESS):
			{
				// Using only the allowed portion of the TAC.
				// NOTE: This can't be *Write* since it will cause an infinite loop!
				this->_memory[this->RelativeAddress(absolute_address)] = data & 0x07;
				return true;
			}

			case (DividerRegister::DIVIDER_REGISTER_ADDRESS):
			{
				// Writing to the divier register resets the divider timer.
				// NOTE: This can't be *Write* since it will cause an infinite loop!
				this->_memory[this->RelativeAddress(absolute_address)] = 0;
				// Also the LSB turns into 0.
				// NOTE: This can't be *Write* since it will cause an infinite loop!
				this->_memory[this->RelativeAddress(absolute_address - 1)] = 0;
				return true;
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

			case (LY::LY_ADDRESS):
			{
				// Writing will reset the counter.
				std::lock_guard lock{this->_mutex};
				this->_memory[this->RelativeAddress(absolute_address)] = 0;
				return true;
			
				break;
			}

			case (LCDC_Status::LCDC_ADDRESS):
			{
				// This might be accessed in multiple threads.
				std::lock_guard lock{this->_mutex};
				this->_memory[this->RelativeAddress(absolute_address)] = data;
				return true;

				break;
			}
		}

		return false;
	}

private:
	/**
	 * Since this is accessed from the PPU, this needs to be thread safe.
	 */
	void MaskLCDCStatus(API::data_t mask)
	{
		std::lock_guard lock{this->_mutex};

		this->_memory[this->RelativeAddress(LCDC_Status::LCDC_ADDRESS)] &= mask;
	}

private:
	std::mutex _mutex;

private:
	friend class MainPixelEngine;
	friend class PixelFIFO;
	friend class DeviceManager;
	friend class Timer;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_IO_RAM_H__
