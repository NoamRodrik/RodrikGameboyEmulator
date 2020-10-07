/**
 * @file		IORAM.h
 * @author		Noam Rodrik
 * @description LR35902 main IO RAM device header.
 */
#ifndef __LR35902_MEMORY_DEVICE_IO_RAM_H__
#define __LR35902_MEMORY_DEVICE_IO_RAM_H__

#include <Core/CPU/Interrupts/Registers/InterruptEnable.h>
#include <Core/CPU/Interrupts/Registers/InterruptFlag.h>
#include <Core/Timers/Registers/DividerRegister.h>
#include <Core/Joypad/Registers/JoypadRegister.h>
#include <Core/CPU/Interrupts/InterruptHandler.h>
#include <Core/Timers/Registers/TimerCounter.h>
#include <Core/Timers/Registers/TimerControl.h>
#include <Core/Timers/Registers/TimerModulo.h>
#include <Core/GPU/Registers/LCDC_Control.h>
#include <Core/GPU/Registers/LCDC_Status.h>
#include <Core/Bus/Devices/CartridgeRAM.h>
#include <Core/APU/Registers/NR10.h>
#include <Core/APU/Registers/NR11.h>
#include <Core/APU/Registers/NR12.h>
#include <Core/APU/Registers/NR13.h>
#include <Core/APU/Registers/NR14.h>
#include <Core/APU/Registers/NR21.h>
#include <Core/APU/Registers/NR22.h>
#include <Core/APU/Registers/NR23.h>
#include <Core/APU/Registers/NR24.h>
#include <Core/APU/Registers/NR30.h>
#include <Core/APU/Registers/NR31.h>
#include <Core/APU/Registers/NR32.h>
#include <Core/APU/Registers/NR33.h>
#include <Core/APU/Registers/NR34.h>
#include <Core/APU/Registers/NR41.h>
#include <Core/APU/Registers/NR42.h>
#include <Core/APU/Registers/NR43.h>
#include <Core/APU/Registers/NR44.h>
#include <Core/APU/Registers/NR50.h>
#include <Core/APU/Registers/NR51.h>
#include <Core/APU/Registers/NR52.h>
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
#include <Core/Joypad/Joypad.h>
#include <Core/Bus/RAMDevice.h>
#include <Core/Timers/Timer.h>
#include <API/Definitions.h>

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
		Message("Return this when needed");
		/*this->_memory[this->RelativeAddress(NR10::NR10_ADDRESS)] = NR10::NR10_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR11::NR11_ADDRESS)] = NR11::NR11_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR12::NR12_ADDRESS)] = NR12::NR12_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR13::NR13_ADDRESS)] = NR13::NR13_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR14::NR14_ADDRESS)] = NR14::NR14_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR21::NR21_ADDRESS)] = NR21::NR21_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR22::NR22_ADDRESS)] = NR22::NR22_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR23::NR23_ADDRESS)] = NR23::NR23_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR24::NR24_ADDRESS)] = NR24::NR24_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR30::NR30_ADDRESS)] = NR30::NR30_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR31::NR31_ADDRESS)] = NR31::NR31_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR32::NR32_ADDRESS)] = NR32::NR32_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR33::NR33_ADDRESS)] = NR33::NR33_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR34::NR34_ADDRESS)] = NR34::NR34_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR41::NR41_ADDRESS)] = NR41::NR41_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR42::NR42_ADDRESS)] = NR42::NR42_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR43::NR43_ADDRESS)] = NR43::NR43_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR44::NR44_ADDRESS)] = NR44::NR44_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR50::NR50_ADDRESS)] = NR50::NR50_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR51::NR51_ADDRESS)] = NR51::NR51_DEFAULT_VALUE;
		this->_memory[this->RelativeAddress(NR52::NR52_ADDRESS)] = NR52::NR52_DEFAULT_VALUE;*/
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
	virtual bool InterceptRead(const API::address_t absolute_address, API::data_t& data) const override
	{
		switch (absolute_address)
		{
			case (JoypadRegister::JOYPAD_REGISTER_ADDRESS):
			{
				API::data_t status = 0xC0 | (this->_memory[this->RelativeAddress(JoypadRegister::JOYPAD_REGISTER_ADDRESS)] & 0x30);
				switch (status & 0x30)
				{
					case (1 << static_cast<std::size_t>(Joypad::Mode::SELECT_DIRECTIONS)):
					{
						status |= Joypad::GetDirectionStatus();
						break;
					}

					case (1 << static_cast<std::size_t>(Joypad::Mode::SELECT_BUTTONS)):
					{
						status |= Joypad::GetButtonStatus();
						break;
					}

					default:
					{
						status |= 0x0F;
					}
				}

				data = status;

				return true;
				break;
			}
		}

		return false;
	}

	virtual bool InterceptWrite(const API::address_t absolute_address, const API::data_t data) override
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
				this->_memory[this->RelativeAddress(absolute_address)] = 0;
				return true;
			
				break;
			}

			case (LCDC_Control::LCDC_ADDRESS):
			{
				this->_memory[this->RelativeAddress(LCDC_Control::LCDC_ADDRESS)] = data;

				if (static_cast<LCDC_Control::Control>(data).IsLCDEnabled() &&
					!Processor::GetInstance().GetPPU()->IsLCDEnabled())
				{
					Processor::GetInstance().GetPPU()->EnableLCD();
				}
				// If the status has changed.
				else if (!static_cast<LCDC_Control::Control>(data).IsLCDEnabled() &&
					     Processor::GetInstance().GetPPU()->IsLCDEnabled() &&
					     Processor::GetInstance().GetPPU()->GetState() == PPUState::V_BLANK)
				{
					Processor::GetInstance().GetPPU()->DisableLCD();
				}

				return true;

				break;
			}

			case (LCDC_Status::LCDC_ADDRESS) :
			{
				// Don't allow the override of values in bits [0,2]
				this->_memory[this->RelativeAddress(LCDC_Status::LCDC_ADDRESS)] = (data & 0x78) | (this->_memory[this->RelativeAddress(LCDC_Status::LCDC_ADDRESS)] & 0x07);

				return true;
				break;
			}

			case (JoypadRegister::JOYPAD_REGISTER_ADDRESS):
			{
				this->_memory[this->RelativeAddress(JoypadRegister::JOYPAD_REGISTER_ADDRESS)] = 0xCF | (data & 0x30);

				return true;
				break;
			}

			case (DMA::DMA_ADDRESS):
			{
				this->_memory[this->RelativeAddress(DMA::DMA_ADDRESS)] = data;
				Processor::GetInstance().GetPPU()->InitiateDMA();
				return true;
				break;
			}
		}

		return false;
	}

protected:
	void MaskLCDCStatus(API::data_t mask)
	{
		this->_memory[this->RelativeAddress(LCDC_Status::LCDC_ADDRESS)] &= mask;
	}

private:
	friend class LCDRender;
	friend class PixelFIFO;
	friend class DeviceManager;
	friend class Timer;
	friend class Joypad;
};
} // Core

#endif // __LR35902_MEMORY_DEVICE_IO_RAM_H__
