/**
 * @file        LD.h
 * @author      Noam Rodrik
 * @description Implementation to the LD instruction(s).
 */
#ifndef __LR35902_LD_H__
#define __LR35902_LD_H__

#include <Core/Processor/Processor.h>
#include <Core/Processor/Instruction/Shortcuts.h>
#include <Core/API/Registers/RegisterPair.h>
#include "DEC.h"
#include "INC.h"

namespace Core
{
// LD o_reg, data
auto LD_REG_WITH_DATA = [](auto& o_reg, const auto& data)
{
	o_reg = data;
	return true;
};

// LD reg_pair, d16
auto LD_D16 = [](auto& reg_pair)
{
	return LD_REG_WITH_DATA(reg_pair, D16());
};

// LD (addr), value
auto LD_VALUE_TO_ADDR = [](const address_t& addr, const data_t& value)
{
	memory.Write(addr, value);
	return true;
};

// LD (a16), reg
auto LD_A16 = [](const address_t& reg)
{
	LD_VALUE_TO_ADDR(A16(), static_cast<const data_t>(reg & 0x00FF));
	LD_VALUE_TO_ADDR(A16() + 1, static_cast<const data_t>((static_cast<const address_t>(reg & 0xFF00) >> 8) & 0x00FF));
	return true;
};

// LD o_reg, (addr)
auto LD_REG_FROM_MEMORY_ADDRESS = [](const auto& addr, auto& o_reg)
{
	o_reg = READ_DATA_AT(addr);
	return true;
};

// 0x01 LD BC, d16
// - - - -
auto LD_0x01 = []()
{
	return LD_D16(BC);
};

// 0x02 LD (BC), A
// - - - -
auto LD_0x02 = []()
{
	return LD_VALUE_TO_ADDR(BC_const, A_const);
};

// 0x06 LD B, d8
// - - - -
auto LD_0x06 = []()
{
	B = D8();
	return true;
};

// 0x08 LD (a16), SP
// - - - -
auto LD_0x08 = []()
{
	return LD_A16(SP_const);
};

// 0x0A LD A, (BC)
// - - - -
auto LD_0x0A = []()
{
	return LD_REG_FROM_MEMORY_ADDRESS(BC_const, A);
};

// 0x0E LD C, d8
// - - - -
auto LD_0x0E = []()
{
	C = D8();
	return true;
};

// 0x11 LD DE, d16
// - - - -
auto LD_0x11 = []()
{
	return LD_D16(DE);
};

// 0x12 LD (DE), A
// - - - -
auto LD_0x12 = []()
{
	return LD_VALUE_TO_ADDR(DE_const, A_const);
};

// 0x16 LD D,d8
// - - - -
auto LD_0x16 = []()
{
	D = D8();
	return true;
};

// 0x1A LD A,(DE)
// - - - -
auto LD_0x1A = []()
{
	return LD_REG_FROM_MEMORY_ADDRESS(DE_const, A);
};

// 0x1E LD E,d8
// - - - -
auto LD_0x1E = []()
{
	E = D8();
	return true;
};

// 0x21 LD HL,d16
// - - - -
auto LD_0x21 = []()
{
	return LD_D16(HL);
};

// 0x22 LD (HL+),A
// - - - -
auto LD_0x22 = []()
{
	// Put A into memory address HL. Increment HL.
	// Same as : LD (HL), A - INC HL
	SANITY(LD_VALUE_TO_ADDR(HL_const, A_const), "Fail loading memory to reg");
	INC_0x23();
	return true;
};

// 0x26 LD H,d8
// - - - -
auto LD_0x26 = []()
{
	H = D8();
	return true;
};

// 0x2A LD A,(HL+)
// - - - -
auto LD_0x2A = []()
{
	// Put value at address HL into A. Increment HL.
	// Same as : LD A, (HL) & INC HL
	SANITY(LD_REG_FROM_MEMORY_ADDRESS(HL_const, A), "Failed to load HL register to A");
	INC_0x23();
	return true;
};

// 0x2E LD L,d8
// - - - -
auto LD_0x2E = []()
{
	L = D8();
	return true;
};

// 0x31 LD SP,d16
// - - - -
auto LD_0x31 = []()
{
	return LD_REG_WITH_DATA(SP, D16());
};

// 0x32 LD (HL-),A
// - - - -
auto LD_0x32 = []()
{
	// Put A into memory address HL. Decrement HL.
	// Same as : LD (HL), A && DEC HL
	LD_VALUE_TO_ADDR(HL_const, A_const);
	DEC_0x2B();
	return true;
};

// 0x36 LD (HL),d8
// - - - -
auto LD_0x36 = []()
{
	memory.Write(HL_const, D8());
	return true;
};

// 0x3A LD A,(HL-)
// - - - -
auto LD_0x3A = []()
{
	// Put value at address HL into A. Pre-Decrement HL.
	// Same as : LD A, (HL) && DEC HL
	LD_REG_FROM_MEMORY_ADDRESS(HL_const, A);
	DEC_0x2B();
	return true;
};

// 0x3E LD A,d8
// - - - -
auto LD_0x3E = []()
{
	A = D8();
	return true;
};

// 0x40 LD B,B
// - - - -
auto LD_0x40 = []()
{
	return LD_REG_WITH_DATA(B, B_const);
};

// 0x41 LD B,C
// - - - -
auto LD_0x41 = []()
{
	return LD_REG_WITH_DATA(B, C_const);
};

// 0x42 LD B,D
// - - - -
auto LD_0x42 = []()
{
	return LD_REG_WITH_DATA(B, D_const);
};

// 0x43 LD B,E
// - - - -
auto LD_0x43 = []()
{
	return LD_REG_WITH_DATA(B, E_const);
};

// 0x44 LD B,H
// - - - -
auto LD_0x44 = []()
{
	return LD_REG_WITH_DATA(B, H_const);
};

// 0x45 LD B,L
// - - - -
auto LD_0x45 = []()
{
	return LD_REG_WITH_DATA(B, L_const);
};

// 0x46 LD B,(HL)
// - - - -
auto LD_0x46 = []()
{
	return LD_REG_FROM_MEMORY_ADDRESS(HL_const, B);
};

// 0x47 LD B,A
// - - - -
auto LD_0x47 = []()
{
	return LD_REG_WITH_DATA(B, A_const);
};

// 0x48 LD C,B
// - - - -
auto LD_0x48 = []()
{
	return LD_REG_WITH_DATA(C, B_const);
};

// 0x49 LD C,C
// - - - -
auto LD_0x49 = []()
{
	return LD_REG_WITH_DATA(C, C_const);
};

// 0x4A LD C,D
// - - - -
auto LD_0x4A = []()
{
	return LD_REG_WITH_DATA(C, D_const);
};

// 0x4B LD C,E
// - - - -
auto LD_0x4B = []()
{
	return LD_REG_WITH_DATA(C, E_const);
};

// 0x4C LD C,H
// - - - -
auto LD_0x4C = []()
{
	return LD_REG_WITH_DATA(C, H_const);
};

// 0x4D LD C,L
// - - - -
auto LD_0x4D = []()
{
	return LD_REG_WITH_DATA(C, L_const);
};

// 0x4E LD C,(HL)
// - - - -
auto LD_0x4E = []()
{
	return LD_REG_FROM_MEMORY_ADDRESS(HL_const, C);
};

// 0x4F LD C,A
// - - - -
auto LD_0x4F = []()
{
	return LD_REG_WITH_DATA(C, A_const);
};

// 0x50 LD D,B
// - - - -
auto LD_0x50 = []()
{
	return LD_REG_WITH_DATA(D, B_const);
};

// 0x51 LD D,C
// - - - -
auto LD_0x51 = []()
{
	return LD_REG_WITH_DATA(D, C_const);
};

// 0x52 LD D,D
// - - - -
auto LD_0x52 = []()
{
	return LD_REG_WITH_DATA(D, D_const);
};

// 0x53 LD D,E
// - - - -
auto LD_0x53 = []()
{
	return LD_REG_WITH_DATA(D, E_const);
};

// 0x54 LD D,H
// - - - -
auto LD_0x54 = []()
{
	return LD_REG_WITH_DATA(D, H_const);
};

// 0x55 LD D,L
// - - - -
auto LD_0x55 = []()
{
	return LD_REG_WITH_DATA(D, L_const);
};

// 0x56 LD D,(HL)
// - - - -
auto LD_0x56 = []()
{
	return LD_REG_FROM_MEMORY_ADDRESS(HL_const, D);
};

// 0x57 LD D,A
// - - - -
auto LD_0x57 = []()
{
	return LD_REG_WITH_DATA(D, A_const);
};

// 0x58 LD E,B
// - - - -
auto LD_0x58 = []()
{
	return LD_REG_WITH_DATA(E, B_const);
};

// 0x59 LD E,C
// - - - -
auto LD_0x59 = []()
{
	return LD_REG_WITH_DATA(E, C_const);
};

// 0x5A LD E,D
// - - - -
auto LD_0x5A = []()
{
	return LD_REG_WITH_DATA(E, D_const);
};

// 0x5B LD E,E
// - - - -
auto LD_0x5B = []()
{
	return LD_REG_WITH_DATA(E, E_const);
};

// 0x5C LD E,H
// - - - -
auto LD_0x5C = []()
{
	return LD_REG_WITH_DATA(E, H_const);
};

// 0x5D LD E,L
// - - - -
auto LD_0x5D = []()
{
	return LD_REG_WITH_DATA(E, L_const);
};

// 0x5E LD E,(HL)
// - - - -
auto LD_0x5E = []()
{
	return LD_REG_FROM_MEMORY_ADDRESS(HL_const, E);
};

// 0x5F LD E,A
// - - - -
auto LD_0x5F = []()
{
	return LD_REG_WITH_DATA(E, A_const);
};

// 0x60 LD H,B
// - - - -
auto LD_0x60 = []()
{
	return LD_REG_WITH_DATA(H, B_const);
};

// 0x61 LD H,C
// - - - -
auto LD_0x61 = []()
{
	return LD_REG_WITH_DATA(H, C_const);
};

// 0x62 LD H,D
// - - - -
auto LD_0x62 = []()
{
	return LD_REG_WITH_DATA(H, D_const);
};

// 0x63 LD H,E
// - - - -
auto LD_0x63 = []()
{
	return LD_REG_WITH_DATA(H, E_const);
};

// 0x64 LD H,H
// - - - -
auto LD_0x64 = []()
{
	return LD_REG_WITH_DATA(H, H_const);
};

// 0x65 LD H,L
// - - - -
auto LD_0x65 = []()
{
	return LD_REG_WITH_DATA(H, L_const);
};

// 0x66 LD H,(HL)
// - - - -
auto LD_0x66 = []()
{
	return LD_REG_FROM_MEMORY_ADDRESS(HL_const, H);
};

// 0x67 LD H,A
// - - - -
auto LD_0x67 = []()
{
	return LD_REG_WITH_DATA(H, A_const);
};

// 0x68 LD L,B
// - - - -
auto LD_0x68 = []()
{
	return LD_REG_WITH_DATA(L, B_const);
};

// 0x69 LD L,C
// - - - -
auto LD_0x69 = []()
{
	return LD_REG_WITH_DATA(L, C_const);
};

// 0x6A LD L,D
// - - - -
auto LD_0x6A = []()
{
	return LD_REG_WITH_DATA(L, D_const);
};

// 0x6B LD L,E
// - - - -
auto LD_0x6B = []()
{
	return LD_REG_WITH_DATA(L, E_const);
};

// 0x6C LD L,H
// - - - -
auto LD_0x6C = []()
{
	return LD_REG_WITH_DATA(L, H_const);
};

// 0x6D LD L,L
// - - - -
auto LD_0x6D = []()
{
	return LD_REG_WITH_DATA(L, L_const);
};

// 0x6E LD L,(HL)
// - - - -
auto LD_0x6E = []()
{
	return LD_REG_FROM_MEMORY_ADDRESS(HL_const, L);
};

// 0x6F LD L,A
// - - - -
auto LD_0x6F = []()
{
	return LD_REG_WITH_DATA(L, A_const);
};

// 0x70 LD (HL),B
// - - - -
auto LD_0x70 = []()
{
	return LD_VALUE_TO_ADDR(HL_const, B_const);
};

// 0x71 LD (HL),C
// - - - -
auto LD_0x71 = []()
{
	return LD_VALUE_TO_ADDR(HL_const, C_const);
};

// 0x72 LD (HL),D
// - - - -
auto LD_0x72 = []()
{
	return LD_VALUE_TO_ADDR(HL_const, D_const);
};

// 0x73 LD (HL),E
// - - - -
auto LD_0x73 = []()
{
	return LD_VALUE_TO_ADDR(HL_const, E_const);
};

// 0x74 LD (HL),H
// - - - -
auto LD_0x74 = []()
{
	return LD_VALUE_TO_ADDR(HL_const, H_const);
};

// 0x75 LD (HL),L
// - - - -
auto LD_0x75 = []()
{
	return LD_VALUE_TO_ADDR(HL_const, L_const);
};

// 0x77 LD (HL),A
// - - - -
auto LD_0x77 = []()
{
	return LD_VALUE_TO_ADDR(HL_const, A_const);
};

// 0x78 LD A,B
// - - - -
auto LD_0x78 = []()
{
	return LD_REG_WITH_DATA(A, B_const);
};

// 0x79 LD A,C
// - - - -
auto LD_0x79 = []()
{
	return LD_REG_WITH_DATA(A, C_const);
};

// 0x7A LD A,D
// - - - -
auto LD_0x7A = []()
{
	return LD_REG_WITH_DATA(A, D_const);
};

// 0x7B LD A,E
// - - - -
auto LD_0x7B = []()
{
	return LD_REG_WITH_DATA(A, E_const);
};

// 0x7C LD A,H
// - - - -
auto LD_0x7C = []()
{
	return LD_REG_WITH_DATA(A, H_const);
};

// 0x7D LD A,L
// - - - -
auto LD_0x7D = []()
{
	return LD_REG_WITH_DATA(A, L_const);
};

// 0x7E LD A,(HL)
// - - - -
auto LD_0x7E = []()
{
	return LD_REG_FROM_MEMORY_ADDRESS(HL_const, A);
};

// 0x7F LD A,A
// - - - -
auto LD_0x7F = []()
{
	return LD_REG_WITH_DATA(A, A_const);
};

// 0xE2 LD (C),A
// - - - -
auto LD_0xE2 = []()
{
	return LD_VALUE_TO_ADDR(C_const + ZERO_PAGE_ADDRESS, A_const);
};

// 0xEA LD (a16),A
// - - - -
auto LD_0xEA = []()
{
	return LD_VALUE_TO_ADDR(D16(), A_const);
};

// 0xF2 LD A,(C)
// - - - -
auto LD_0xF2 = []()
{
	return LD_REG_FROM_MEMORY_ADDRESS(C_const + ZERO_PAGE_ADDRESS, A);
};

// 0xF8 LD HL,SP+r8
// 0 0 H C
auto LD_0xF8 = []()
{
	F.MutateByCondition((static_cast<int32_t>(SP_const) + static_cast<int32_t>(R8())) & 0x10000, Flag::CARRY);
	F.MutateByCondition((static_cast<int32_t>(SP_const & 0x0F) + static_cast<int32_t>(R8())) & 0x0100, Flag::HALF_CARRY);

	F.Clear(Flag::ZERO);
	F.Clear(Flag::SUB);

	return LD_REG_WITH_DATA(HL, static_cast<address_t>(SP_const + R8()));
};

// 0xF9 LD SP,HL
// - - - -
auto LD_0xF9 = []()
{
	return LD_REG_WITH_DATA(SP, HL_const);
};

// 0xFA LD A,(a16)
// - - - -
auto LD_0xFA = []()
{
	return LD_REG_FROM_MEMORY_ADDRESS(D16(), A);
};
} // Core

#endif // __LR35902_LD_H__