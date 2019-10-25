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
// LD reg, d8
auto LD_D8 = [](auto& reg)
{
	SANITY(memory_const.Read(IP_const, reg), "Failed reading from memory!");
};

// LD lower_reg&upper_reg, d16
auto LD_D16 = [](data_t& lower_reg, data_t& higher_reg)
{
	lower_reg = DataAt(IP_const);

	IP += 1;

	higher_reg = DataAt(IP_const);
};

// LD (a16), r16
auto LD_A16 = [](const address_t& r16)
{
	Message("What is R16?!?!!?");
	memory.Write(r16, IP_const);
};

// LD (reg_with_addr), reg_with_value
auto LD_REG_ADDR_FROM_REG = [](const auto& reg_with_addr, const auto& reg_with_value)
{
	memory.Write(reg_with_value, reg_with_addr);
};

// LD o_reg, (reg_with_addr)
auto LD_REG_FROM_REG_ADDR = [](const auto& reg_with_addr, auto& o_reg)
{
	o_reg = DataAt(reg_with_addr);
};

// LD o_reg, reg
auto LD_REG_FROM_REG = [](auto& o_reg, const auto& reg)
{
	o_reg = reg;
};

// 0x01 LD BC, d16
// - - - -
auto LD_0x01 = []()
{
	LD_D16(B, C);
};

// 0x02 LD (BC), A
// - - - -
auto LD_0x02 = []()
{
	LD_REG_ADDR_FROM_REG(BC_const, A_const);
};

// 0x06 LD B, d8
// - - - -
auto LD_0x06 = []()
{
	LD_D8(B);
};

// 0x08 LD (a16), SP
// - - - -
auto LD_0x08 = []()
{
	LD_A16(SP_const);
};

// 0x0A LD A, (BC)
// - - - -
auto LD_0x0A = []()
{
	LD_REG_FROM_REG_ADDR(BC_const, A);
};

// 0x0E LD C, d8
// - - - -
auto LD_0x0E = []()
{
	LD_D8(C);
};

// 0x11 LD DE, d16
// - - - -
auto LD_0x11 = []()
{
	LD_D16(D, E);
};

// 0x12 LD (DE), A
// - - - -
auto LD_0x12 = []()
{
	LD_REG_ADDR_FROM_REG(DE_const, A_const);
};

// 0x16 LD D,d8
// - - - -
auto LD_0x16 = []()
{
	LD_D8(D);
};

// 0x1A LD A,(DE)
// - - - -
auto LD_0x1A = []()
{
	LD_REG_FROM_REG_ADDR(DE_const, A);
};

// 0x1E LD E,d8
// - - - -
auto LD_0x1E = []()
{
	LD_D8(E);
};

// 0x21 LD HL,d16
// - - - -
auto LD_0x21 = []()
{
	LD_D16(H, L);
};

// 0x22 LD (HL+),A
// - - - -
auto LD_0x22 = []()
{
	// Put A into memory address HL. Increment HL.
	// Same as : LD (HL), A - INC HL	LD_REG_ADDR_FROM_REG(HL_const, A_const);
	INC_0x23();
};

// 0x26 LD H,d8
// - - - -
auto LD_0x26 = []()
{
	LD_D8(H);
};

// 0x2A LD A,(HL+)
// - - - -
auto LD_0x2A = []()
{
	// Put value at address HL into A. Increment HL.
	// Same as : LD A, (HL)-INC HL
	LD_REG_FROM_REG_ADDR(HL_const, A);
	INC_0x23();
};

// 0x2E LD L,d8
// - - - -
auto LD_0x2E = []()
{
	LD_D8(L);
};

// 0x31 LD SP,d16
// - - - -
auto LD_0x31 = []()
{
	LD_D8(SP);
	IP += 1;
	LD_D8(SP);
};

// 0x32 LD (HL-),A
// - - - -
auto LD_0x32 = []()
{
	// Put A into memory address HL. Decrement HL.
	// Same as : LD (HL), A - DEC HL	LD_REG_ADDR_FROM_REG(HL_const, A_const);
	DEC_0x2B();
};

// 0x36 LD (HL),d8
// - - - -
auto LD_0x36 = []()
{
	RunCommandAtAddress(HL_const, LD_D8);
};

// 0x3A LD A,(HL-)
// - - - -
auto LD_0x3A = []()
{
	// Put value at address HL into A. Decrement HL.
	// Same as : LD A, (HL)-DEC HL
	LD_REG_FROM_REG_ADDR(HL_const, A);
	DEC_0x2B();
};

// 0x3E LD A,d8
// - - - -
auto LD_0x3E = []()
{
	LD_D8(A);
};

// 0x40 LD B,B
// - - - -
auto LD_0x40 = []()
{
	LD_REG_FROM_REG(B, B_const);
};

// 0x41 LD B,C
// - - - -
auto LD_0x41 = []()
{
	LD_REG_FROM_REG(B, C_const);
};

// 0x42 LD B,D
// - - - -
auto LD_0x42 = []()
{
	LD_REG_FROM_REG(B, D_const);
};

// 0x43 LD B,E
// - - - -
auto LD_0x43 = []()
{
	LD_REG_FROM_REG(B, E_const);
};

// 0x44 LD B,H
// - - - -
auto LD_0x44 = []()
{
	LD_REG_FROM_REG(B, H_const);
};

// 0x45 LD B,L
// - - - -
auto LD_0x45 = []()
{
	LD_REG_FROM_REG(B, L_const);
};

// 0x46 LD B,(HL)
// - - - -
auto LD_0x46 = []()
{
	LD_REG_FROM_REG_ADDR(HL_const, B);
};

// 0x47 LD B,A
// - - - -
auto LD_0x47 = []()
{
	LD_REG_FROM_REG(B, A_const);
};

// 0x48 LD C,B
// - - - -
auto LD_0x48 = []()
{
	LD_REG_FROM_REG(C, B_const);
};

// 0x49 LD C,C
// - - - -
auto LD_0x49 = []()
{
	LD_REG_FROM_REG(C, C_const);
};

// 0x4A LD C,D
// - - - -
auto LD_0x4A = []()
{
	LD_REG_FROM_REG(C, D_const);
};

// 0x4B LD C,E
// - - - -
auto LD_0x4B = []()
{
	LD_REG_FROM_REG(C, E_const);
};

// 0x4C LD C,H
// - - - -
auto LD_0x4C = []()
{
	LD_REG_FROM_REG(C, H_const);
};

// 0x4D LD C,L
// - - - -
auto LD_0x4D = []()
{
	LD_REG_FROM_REG(C, L_const);
};

// 0x4E LD C,(HL)
// - - - -
auto LD_0x4E = []()
{
	LD_REG_FROM_REG_ADDR(HL_const, C);
};

// 0x4F LD C,A
// - - - -
auto LD_0x4F = []()
{
	LD_REG_FROM_REG(C, A_const);
};

// 0x50 LD D,B
// - - - -
auto LD_0x50 = []()
{
	LD_REG_FROM_REG(D, B_const);
};

// 0x51 LD D,C
// - - - -
auto LD_0x51 = []()
{
	LD_REG_FROM_REG(D, C_const);
};

// 0x52 LD D,D
// - - - -
auto LD_0x52 = []()
{
	LD_REG_FROM_REG(D, D_const);
};

// 0x53 LD D,E
// - - - -
auto LD_0x53 = []()
{
	LD_REG_FROM_REG(D, E_const);
};

// 0x54 LD D,H
// - - - -
auto LD_0x54 = []()
{
	LD_REG_FROM_REG(D, H_const);
};

// 0x55 LD D,L
// - - - -
auto LD_0x55 = []()
{
	LD_REG_FROM_REG(D, L_const);
};

// 0x56 LD D,(HL)
// - - - -
auto LD_0x56 = []()
{
	LD_REG_FROM_REG_ADDR(HL_const, D);
};

// 0x57 LD D,A
// - - - -
auto LD_0x57 = []()
{
	LD_REG_FROM_REG(D, A_const);
};

// 0x58 LD E,B
// - - - -
auto LD_0x58 = []()
{
	LD_REG_FROM_REG(E, B_const);
};

// 0x59 LD E,C
// - - - -
auto LD_0x59 = []()
{
	LD_REG_FROM_REG(E, C_const);
};

// 0x5A LD E,D
// - - - -
auto LD_0x5A = []()
{
	LD_REG_FROM_REG(E, D_const);
};

// 0x5B LD E,E
// - - - -
auto LD_0x5B = []()
{
	LD_REG_FROM_REG(E, E_const);
};

// 0x5C LD E,H
// - - - -
auto LD_0x5C = []()
{
	LD_REG_FROM_REG(E, H_const);
};

// 0x5D LD E,L
// - - - -
auto LD_0x5D = []()
{
	LD_REG_FROM_REG(E, L_const);
};

// 0x5E LD E,(HL)
// - - - -
auto LD_0x5E = []()
{
	LD_REG_FROM_REG_ADDR(HL_const, E);
};

// 0x5F LD E,A
// - - - -
auto LD_0x5F = []()
{
	LD_REG_FROM_REG(E, A_const);
};

// 0x60 LD H,B
// - - - -
auto LD_0x60 = []()
{
	LD_REG_FROM_REG(H, B_const);
};

// 0x61 LD H,C
// - - - -
auto LD_0x61 = []()
{
	LD_REG_FROM_REG(H, C_const);
};

// 0x62 LD H,D
// - - - -
auto LD_0x62 = []()
{
	LD_REG_FROM_REG(H, D_const);
};

// 0x63 LD H,E
// - - - -
auto LD_0x63 = []()
{
	LD_REG_FROM_REG(H, E_const);
};

// 0x64 LD H,H
// - - - -
auto LD_0x64 = []()
{
	LD_REG_FROM_REG(H, H_const);
};

// 0x65 LD H,L
// - - - -
auto LD_0x65 = []()
{
	LD_REG_FROM_REG(H, L_const);
};

// 0x66 LD H,(HL)
// - - - -
auto LD_0x66 = []()
{
	LD_REG_FROM_REG_ADDR(HL_const, H);
};

// 0x67 LD H,A
// - - - -
auto LD_0x67 = []()
{
	LD_REG_FROM_REG(H, A_const);
};

// 0x68 LD L,B
// - - - -
auto LD_0x68 = []()
{
	LD_REG_FROM_REG(L, B_const);
};

// 0x69 LD L,C
// - - - -
auto LD_0x69 = []()
{
	LD_REG_FROM_REG(L, C_const);
};

// 0x6A LD L,D
// - - - -
auto LD_0x6A = []()
{
	LD_REG_FROM_REG(L, D_const);
};

// 0x6B LD L,E
// - - - -
auto LD_0x6B = []()
{
	LD_REG_FROM_REG(L, E_const);
};

// 0x6C LD L,H
// - - - -
auto LD_0x6C = []()
{
	LD_REG_FROM_REG(L, H_const);
};

// 0x6D LD L,L
// - - - -
auto LD_0x6D = []()
{
	LD_REG_FROM_REG(L, L_const);
};

// 0x6E LD L,(HL)
// - - - -
auto LD_0x6E = []()
{
	LD_REG_FROM_REG_ADDR(HL_const, L);
};

// 0x6F LD L,A
// - - - -
auto LD_0x6F = []()
{
	LD_REG_FROM_REG(L, A_const);
};

// 0x70 LD (HL),B
// - - - -
auto LD_0x70 = []()
{
	LD_REG_ADDR_FROM_REG(HL_const, B_const);
};

// 0x71 LD (HL),C
// - - - -
auto LD_0x71 = []()
{
	LD_REG_ADDR_FROM_REG(HL_const, C_const);
};

// 0x72 LD (HL),D
// - - - -
auto LD_0x72 = []()
{
	LD_REG_ADDR_FROM_REG(HL_const, D_const);
};

// 0x73 LD (HL),E
// - - - -
auto LD_0x73 = []()
{
	LD_REG_ADDR_FROM_REG(HL_const, E_const);
};

// 0x74 LD (HL),H
// - - - -
auto LD_0x74 = []()
{
	LD_REG_ADDR_FROM_REG(HL_const, H_const);
};

// 0x75 LD (HL),L
// - - - -
auto LD_0x75 = []()
{
	LD_REG_ADDR_FROM_REG(HL_const, L_const);
};

// 0x77 LD (HL),A
// - - - -
auto LD_0x77 = []()
{
	LD_REG_ADDR_FROM_REG(HL_const, A_const);
};

// 0x78 LD A,B
// - - - -
auto LD_0x78 = []()
{
	LD_REG_FROM_REG(A, B_const);
};

// 0x79 LD A,C
// - - - -
auto LD_0x79 = []()
{
	LD_REG_FROM_REG(A, C_const);
};

// 0x7A LD A,D
// - - - -
auto LD_0x7A = []()
{
	LD_REG_FROM_REG(A, D_const);
};

// 0x7B LD A,E
// - - - -
auto LD_0x7B = []()
{
	LD_REG_FROM_REG(A, E_const);
};

// 0x7C LD A,H
// - - - -
auto LD_0x7C = []()
{
	LD_REG_FROM_REG(A, H_const);
};

// 0x7D LD A,L
// - - - -
auto LD_0x7D = []()
{
	LD_REG_FROM_REG(A, L_const);
};

// 0x7E LD A,(HL)
// - - - -
auto LD_0x7E = []()
{
	LD_REG_FROM_REG_ADDR(HL_const, A);
};

// 0x7F LD A,A
// - - - -
auto LD_0x7F = []()
{
	LD_REG_FROM_REG(A, A_const);
};

// 0xE2 LD (C),A
// - - - -
auto LD_0xE2 = []()
{
	LD_REG_ADDR_FROM_REG(C_const + 0xFF00, A_const);
};

// 0xEA LD (a16),A
// - - - -
auto LD_0xEA = []()
{
	Message("Endianty stuff need to check!");
	address_t immediate_address = 0;
	immediate_address = DataAt(IP) << 8;
	IP += 1;
	immediate_address |= DataAt(IP);

	LD_REG_ADDR_FROM_REG(immediate_address, A_const);
};

// 0xF2 LD A,(C)
// - - - -
auto LD_0xF2 = []()
{
	LD_REG_FROM_REG_ADDR(C_const + 0xFF00, A);
};

// 0xF8 LD HL,SP+r8
// 0 0 H C
auto LD_0xF8 = []()
{
	const r8_t right_hand_operand = D8_TO_R8(DataAt(IP_const));
	F.Clear(Flag::ZERO);
	F.Clear(Flag::SUB);

	if (right_hand_operand > 0)
	{
		F.MutateByCondition(Tools::CarryOnAddition(SP_const, static_cast<data_t>(right_hand_operand)), Flag::CARRY);
		F.MutateByCondition(Tools::HalfCarryOnAddition(SP_const, static_cast<data_t>(right_hand_operand)), Flag::HALF_CARRY);
	}
	else
	{
		F.MutateByCondition(Tools::CarryOnSubtraction(SP_const, static_cast<data_t>(right_hand_operand)), Flag::CARRY);
		F.MutateByCondition(Tools::HalfCarryOnSubtraction(SP_const, static_cast<data_t>(right_hand_operand)), Flag::HALF_CARRY);
	}
};

// 0xF9 LD SP,HL
// - - - -
auto LD_0xF9 = []()
{
	SP = HL_const;
};

// 0xFA LD A,(a16)
// - - - -
auto LD_0xFA = []()
{
	Message("Endianty stuff need to check!");
	address_t immediate_address = 0;
	immediate_address = DataAt(IP) << 8;
	IP += 1;
	immediate_address |= DataAt(IP);

	LD_REG_FROM_REG_ADDR(immediate_address, A);
};
} // Core

#endif // __LR35902_LD_H__