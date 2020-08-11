/**
 * @file		StackRegisters.h
 * @author		Noam Rodrik
 * @description LR35902 main stack registers definition header.
 */
#ifndef __LR35902_STACK_REGISTERS_H__
#define __LR35902_STACK_REGISTERS_H__

#include <Core/Registers/RegisterBase.h>
#include <Core/Bus/Bus.h>

namespace Core
{
class SP_Register : public StackRegisterBase
{
public:
	SP_Register(RegisterType&& reg_value, IMemoryDeviceAccess& memory_context) :
		StackRegisterBase{std::move(reg_value)}, m_memory_context{memory_context} {}

	virtual ~SP_Register() = default;

public:
	virtual void Push(const address_t data) override
	{
		this->Push(static_cast<const data_t>((static_cast<address_t>(data) >> 8) & 0x00FF));
		this->Push(static_cast<const data_t>(data & 0x00FF));
	}

	virtual void Pop(address_t& data) override
	{
		data_t msb{0};
		data_t lsb{0};
		this->Pop(lsb);
		this->Pop(msb);
		data = (msb << 8) | lsb;
	}

	virtual void Pop(data_t& data) override
	{
		this->m_memory_context.Read(this->m_data++, data);
	}

	virtual void Push(const data_t data) override
	{
		this->m_memory_context.Write(--this->m_data, data);
	}

private:
	IMemoryDeviceAccess& m_memory_context;
};
} // Core

#endif // __LR35902_STACK_REGISTERS_H__
