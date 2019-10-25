/**
 * @file		InstructionLookupTable.h
 * @author		Noam Rodrik
 * @description A lookup table for all instruction sequences.
 */
#ifndef __INSTRUCTION_LOOKUP_TABLE_H__
#define __INSTRUCTION_LOOKUP_TABLE_H__

#include <Core/Processor/Instruction/Instruction.h>
#include <Tools/Tools.h>
#include <array>

/* Instruction includes */
#include <Core/Processor/Instruction/Generated/NOP.h>
#include <Core/Processor/Instruction/Generated/LD.h>
#include <Core/Processor/Instruction/Generated/INC.h>
#include <Core/Processor/Instruction/Generated/DEC.h>
#include <Core/Processor/Instruction/Generated/RLCA.h>
#include <Core/Processor/Instruction/Generated/ADD.h>
#include <Core/Processor/Instruction/Generated/RRCA.h>
#include <Core/Processor/Instruction/Generated/STOP.h>
#include <Core/Processor/Instruction/Generated/RLA.h>
#include <Core/Processor/Instruction/Generated/JR.h>
#include <Core/Processor/Instruction/Generated/RRA.h>
#include <Core/Processor/Instruction/Generated/DAA.h>
#include <Core/Processor/Instruction/Generated/CPL.h>
#include <Core/Processor/Instruction/Generated/SCF.h>
#include <Core/Processor/Instruction/Generated/CCF.h>
#include <Core/Processor/Instruction/Generated/HALT.h>
#include <Core/Processor/Instruction/Generated/ADC.h>
#include <Core/Processor/Instruction/Generated/SUB.h>
#include <Core/Processor/Instruction/Generated/SBC.h>
#include <Core/Processor/Instruction/Generated/AND.h>
#include <Core/Processor/Instruction/Generated/XOR.h>
#include <Core/Processor/Instruction/Generated/OR.h>
#include <Core/Processor/Instruction/Generated/CP.h>
#include <Core/Processor/Instruction/Generated/RET.h>
#include <Core/Processor/Instruction/Generated/POP.h>
#include <Core/Processor/Instruction/Generated/JP.h>
#include <Core/Processor/Instruction/Generated/CALL.h>
#include <Core/Processor/Instruction/Generated/PUSH.h>
#include <Core/Processor/Instruction/Generated/RST.h>
#include <Core/Processor/Instruction/Generated/PREFIX.h>
#include <Core/Processor/Instruction/Generated/RETI.h>
#include <Core/Processor/Instruction/Generated/LDH.h>
#include <Core/Processor/Instruction/Generated/DI.h>
#include <Core/Processor/Instruction/Generated/EI.h>

namespace Core
{
auto TERMINATE = []()
{
	Message("To change to macro!");
	printf("An error has occurred!");
	abort();
};

Message("Complete the prefix table!");
Message("Need to increment PC after each instruction!");
static const std::array<Instruction, 0x10 * 0x10> INSTRUCTION_LOOKUP_TABLE =
{
    // 0x00 NOP
    // - - - -
    Instruction{NOP_0x00, 1, 4},

    // 0x01 LD BC,d16
    // - - - -
    Instruction{LD_0x01, 3, 12},

    // 0x02 LD (BC),A
    // - - - -
    Instruction{LD_0x02, 1, 8},

    // 0x03 INC BC
    // - - - -
    Instruction{INC_0x03, 1, 8},

    // 0x04 INC B
    // Z 0 H -
    Instruction{INC_0x04, 1, 4},

    // 0x05 DEC B
    // Z 1 H -
    Instruction{DEC_0x05, 1, 4},

    // 0x06 LD B,d8
    // - - - -
    Instruction{LD_0x06, 2, 8},

    // 0x07 RLCA
    // 0 0 0 C
    Instruction{RLCA_0x07, 1, 4},

    // 0x08 LD (a16),SP
    // - - - -
    Instruction{LD_0x08, 3, 20},

    // 0x09 ADD HL,BC
    // - 0 H C
    Instruction{ADD_0x09, 1, 8},

    // 0x0A LD A,(BC)
    // - - - -
    Instruction{LD_0x0A, 1, 8},

    // 0x0B DEC BC
    // - - - -
    Instruction{DEC_0x0B, 1, 8},

    // 0x0C INC C
    // Z 0 H -
    Instruction{INC_0x0C, 1, 4},

    // 0x0D DEC C
    // Z 1 H -
    Instruction{DEC_0x0D, 1, 4},

    // 0x0E LD C,d8
    // - - - -
    Instruction{LD_0x0E, 2, 8},

    // 0x0F RRCA
    // 0 0 0 C
    Instruction{RRCA_0x0F, 1, 4},

    // 0x10 STOP 0
    // - - - -
    Instruction{STOP_0x10, 2, 4},

    // 0x11 LD DE,d16
    // - - - -
    Instruction{LD_0x11, 3, 12},

    // 0x12 LD (DE),A
    // - - - -
    Instruction{LD_0x12, 1, 8},

    // 0x13 INC DE
    // - - - -
    Instruction{INC_0x13, 1, 8},

    // 0x14 INC D
    // Z 0 H -
    Instruction{INC_0x14, 1, 4},

    // 0x15 DEC D
    // Z 1 H -
    Instruction{DEC_0x15, 1, 4},

    // 0x16 LD D,d8
    // - - - -
    Instruction{LD_0x16, 2, 8},

    // 0x17 RLA
    // 0 0 0 C
    Instruction{RLA_0x17, 1, 4},

    // 0x18 JR r8
    // - - - -
    Instruction{JR_0x18, 2, 12},

    // 0x19 ADD HL,DE
    // - 0 H C
    Instruction{ADD_0x19, 1, 8},

    // 0x1A LD A,(DE)
    // - - - -
    Instruction{LD_0x1A, 1, 8},

    // 0x1B DEC DE
    // - - - -
    Instruction{DEC_0x1B, 1, 8},

    // 0x1C INC E
    // Z 0 H -
    Instruction{INC_0x1C, 1, 4},

    // 0x1D DEC E
    // Z 1 H -
    Instruction{DEC_0x1D, 1, 4},

    // 0x1E LD E,d8
    // - - - -
    Instruction{LD_0x1E, 2, 8},

    // 0x1F RRA
    // 0 0 0 C
    Instruction{RRA_0x1F, 1, 4},

    // 0x20 JR NZ,r8
    // - - - -
    Instruction{JR_0x20, 2, 12, 8},

    // 0x21 LD HL,d16
    // - - - -
    Instruction{LD_0x21, 3, 12},

    // 0x22 LD (HL+),A
    // - - - -
    Instruction{LD_0x22, 1, 8},

    // 0x23 INC HL
    // - - - -
    Instruction{INC_0x23, 1, 8},

    // 0x24 INC H
    // Z 0 H -
    Instruction{INC_0x24, 1, 4},

    // 0x25 DEC H
    // Z 1 H -
    Instruction{DEC_0x25, 1, 4},

    // 0x26 LD H,d8
    // - - - -
    Instruction{LD_0x26, 2, 8},

    // 0x27 DAA
    // Z - 0 C
    Instruction{DAA_0x27, 1, 4},

    // 0x28 JR Z,r8
    // - - - -
    Instruction{JR_0x28, 2, 12, 8},

    // 0x29 ADD HL,HL
    // - 0 H C
    Instruction{ADD_0x29, 1, 8},

    // 0x2A LD A,(HL+)
    // - - - -
    Instruction{LD_0x2A, 1, 8},

    // 0x2B DEC HL
    // - - - -
    Instruction{DEC_0x2B, 1, 8},

    // 0x2C INC L
    // Z 0 H -
    Instruction{INC_0x2C, 1, 4},

    // 0x2D DEC L
    // Z 1 H -
    Instruction{DEC_0x2D, 1, 4},

    // 0x2E LD L,d8
    // - - - -
    Instruction{LD_0x2E, 2, 8},

    // 0x2F CPL
    // - 1 1 -
    Instruction{CPL_0x2F, 1, 4},

    // 0x30 JR NC,r8
    // - - - -
    Instruction{JR_0x30, 2, 12, 8},

    // 0x31 LD SP,d16
    // - - - -
    Instruction{LD_0x31, 3, 12},

    // 0x32 LD (HL-),A
    // - - - -
    Instruction{LD_0x32, 1, 8},

    // 0x33 INC SP
    // - - - -
    Instruction{INC_0x33, 1, 8},

    // 0x34 INC (HL)
    // Z 0 H -
    Instruction{INC_0x34, 1, 12},

    // 0x35 DEC (HL)
    // Z 1 H -
    Instruction{DEC_0x35, 1, 12},

    // 0x36 LD (HL),d8
    // - - - -
    Instruction{LD_0x36, 2, 12},

    // 0x37 SCF
    // - 0 0 1
    Instruction{SCF_0x37, 1, 4},

    // 0x38 JR C,r8
    // - - - -
    Instruction{JR_0x38, 2, 12, 8},

    // 0x39 ADD HL,SP
    // - 0 H C
    Instruction{ADD_0x39, 1, 8},

    // 0x3A LD A,(HL-)
    // - - - -
    Instruction{LD_0x3A, 1, 8},

    // 0x3B DEC SP
    // - - - -
    Instruction{DEC_0x3B, 1, 8},

    // 0x3C INC A
    // Z 0 H -
    Instruction{INC_0x3C, 1, 4},

    // 0x3D DEC A
    // Z 1 H -
    Instruction{DEC_0x3D, 1, 4},

    // 0x3E LD A,d8
    // - - - -
    Instruction{LD_0x3E, 2, 8},

    // 0x3F CCF
    // - 0 0 C
    Instruction{CCF_0x3F, 1, 4},

    // 0x40 LD B,B
    // - - - -
    Instruction{LD_0x40, 1, 4},

    // 0x41 LD B,C
    // - - - -
    Instruction{LD_0x41, 1, 4},

    // 0x42 LD B,D
    // - - - -
    Instruction{LD_0x42, 1, 4},

    // 0x43 LD B,E
    // - - - -
    Instruction{LD_0x43, 1, 4},

    // 0x44 LD B,H
    // - - - -
    Instruction{LD_0x44, 1, 4},

    // 0x45 LD B,L
    // - - - -
    Instruction{LD_0x45, 1, 4},

    // 0x46 LD B,(HL)
    // - - - -
    Instruction{LD_0x46, 1, 8},

    // 0x47 LD B,A
    // - - - -
    Instruction{LD_0x47, 1, 4},

    // 0x48 LD C,B
    // - - - -
    Instruction{LD_0x48, 1, 4},

    // 0x49 LD C,C
    // - - - -
    Instruction{LD_0x49, 1, 4},

    // 0x4A LD C,D
    // - - - -
    Instruction{LD_0x4A, 1, 4},

    // 0x4B LD C,E
    // - - - -
    Instruction{LD_0x4B, 1, 4},

    // 0x4C LD C,H
    // - - - -
    Instruction{LD_0x4C, 1, 4},

    // 0x4D LD C,L
    // - - - -
    Instruction{LD_0x4D, 1, 4},

    // 0x4E LD C,(HL)
    // - - - -
    Instruction{LD_0x4E, 1, 8},

    // 0x4F LD C,A
    // - - - -
    Instruction{LD_0x4F, 1, 4},

    // 0x50 LD D,B
    // - - - -
    Instruction{LD_0x50, 1, 4},

    // 0x51 LD D,C
    // - - - -
    Instruction{LD_0x51, 1, 4},

    // 0x52 LD D,D
    // - - - -
    Instruction{LD_0x52, 1, 4},

    // 0x53 LD D,E
    // - - - -
    Instruction{LD_0x53, 1, 4},

    // 0x54 LD D,H
    // - - - -
    Instruction{LD_0x54, 1, 4},

    // 0x55 LD D,L
    // - - - -
    Instruction{LD_0x55, 1, 4},

    // 0x56 LD D,(HL)
    // - - - -
    Instruction{LD_0x56, 1, 8},

    // 0x57 LD D,A
    // - - - -
    Instruction{LD_0x57, 1, 4},

    // 0x58 LD E,B
    // - - - -
    Instruction{LD_0x58, 1, 4},

    // 0x59 LD E,C
    // - - - -
    Instruction{LD_0x59, 1, 4},

    // 0x5A LD E,D
    // - - - -
    Instruction{LD_0x5A, 1, 4},

    // 0x5B LD E,E
    // - - - -
    Instruction{LD_0x5B, 1, 4},

    // 0x5C LD E,H
    // - - - -
    Instruction{LD_0x5C, 1, 4},

    // 0x5D LD E,L
    // - - - -
    Instruction{LD_0x5D, 1, 4},

    // 0x5E LD E,(HL)
    // - - - -
    Instruction{LD_0x5E, 1, 8},

    // 0x5F LD E,A
    // - - - -
    Instruction{LD_0x5F, 1, 4},

    // 0x60 LD H,B
    // - - - -
    Instruction{LD_0x60, 1, 4},

    // 0x61 LD H,C
    // - - - -
    Instruction{LD_0x61, 1, 4},

    // 0x62 LD H,D
    // - - - -
    Instruction{LD_0x62, 1, 4},

    // 0x63 LD H,E
    // - - - -
    Instruction{LD_0x63, 1, 4},

    // 0x64 LD H,H
    // - - - -
    Instruction{LD_0x64, 1, 4},

    // 0x65 LD H,L
    // - - - -
    Instruction{LD_0x65, 1, 4},

    // 0x66 LD H,(HL)
    // - - - -
    Instruction{LD_0x66, 1, 8},

    // 0x67 LD H,A
    // - - - -
    Instruction{LD_0x67, 1, 4},

    // 0x68 LD L,B
    // - - - -
    Instruction{LD_0x68, 1, 4},

    // 0x69 LD L,C
    // - - - -
    Instruction{LD_0x69, 1, 4},

    // 0x6A LD L,D
    // - - - -
    Instruction{LD_0x6A, 1, 4},

    // 0x6B LD L,E
    // - - - -
    Instruction{LD_0x6B, 1, 4},

    // 0x6C LD L,H
    // - - - -
    Instruction{LD_0x6C, 1, 4},

    // 0x6D LD L,L
    // - - - -
    Instruction{LD_0x6D, 1, 4},

    // 0x6E LD L,(HL)
    // - - - -
    Instruction{LD_0x6E, 1, 8},

    // 0x6F LD L,A
    // - - - -
    Instruction{LD_0x6F, 1, 4},

    // 0x70 LD (HL),B
    // - - - -
    Instruction{LD_0x70, 1, 8},

    // 0x71 LD (HL),C
    // - - - -
    Instruction{LD_0x71, 1, 8},

    // 0x72 LD (HL),D
    // - - - -
    Instruction{LD_0x72, 1, 8},

    // 0x73 LD (HL),E
    // - - - -
    Instruction{LD_0x73, 1, 8},

    // 0x74 LD (HL),H
    // - - - -
    Instruction{LD_0x74, 1, 8},

    // 0x75 LD (HL),L
    // - - - -
    Instruction{LD_0x75, 1, 8},

    // 0x76 HALT
    // - - - -
    Instruction{HALT_0x76, 1, 4},

    // 0x77 LD (HL),A
    // - - - -
    Instruction{LD_0x77, 1, 8},

    // 0x78 LD A,B
    // - - - -
    Instruction{LD_0x78, 1, 4},

    // 0x79 LD A,C
    // - - - -
    Instruction{LD_0x79, 1, 4},

    // 0x7A LD A,D
    // - - - -
    Instruction{LD_0x7A, 1, 4},

    // 0x7B LD A,E
    // - - - -
    Instruction{LD_0x7B, 1, 4},

    // 0x7C LD A,H
    // - - - -
    Instruction{LD_0x7C, 1, 4},

    // 0x7D LD A,L
    // - - - -
    Instruction{LD_0x7D, 1, 4},

    // 0x7E LD A,(HL)
    // - - - -
    Instruction{LD_0x7E, 1, 8},

    // 0x7F LD A,A
    // - - - -
    Instruction{LD_0x7F, 1, 4},

    // 0x80 ADD A,B
    // Z 0 H C
    Instruction{ADD_0x80, 1, 4},

    // 0x81 ADD A,C
    // Z 0 H C
    Instruction{ADD_0x81, 1, 4},

    // 0x82 ADD A,D
    // Z 0 H C
    Instruction{ADD_0x82, 1, 4},

    // 0x83 ADD A,E
    // Z 0 H C
    Instruction{ADD_0x83, 1, 4},

    // 0x84 ADD A,H
    // Z 0 H C
    Instruction{ADD_0x84, 1, 4},

    // 0x85 ADD A,L
    // Z 0 H C
    Instruction{ADD_0x85, 1, 4},

    // 0x86 ADD A,(HL)
    // Z 0 H C
    Instruction{ADD_0x86, 1, 8},

    // 0x87 ADD A,A
    // Z 0 H C
    Instruction{ADD_0x87, 1, 4},

    // 0x88 ADC A,B
    // Z 0 H C
    Instruction{ADC_0x88, 1, 4},

    // 0x89 ADC A,C
    // Z 0 H C
    Instruction{ADC_0x89, 1, 4},

    // 0x8A ADC A,D
    // Z 0 H C
    Instruction{ADC_0x8A, 1, 4},

    // 0x8B ADC A,E
    // Z 0 H C
    Instruction{ADC_0x8B, 1, 4},

    // 0x8C ADC A,H
    // Z 0 H C
    Instruction{ADC_0x8C, 1, 4},

    // 0x8D ADC A,L
    // Z 0 H C
    Instruction{ADC_0x8D, 1, 4},

    // 0x8E ADC A,(HL)
    // Z 0 H C
    Instruction{ADC_0x8E, 1, 8},

    // 0x8F ADC A,A
    // Z 0 H C
    Instruction{ADC_0x8F, 1, 4},

    // 0x90 SUB B
    // Z 1 H C
    Instruction{SUB_0x90, 1, 4},

    // 0x91 SUB C
    // Z 1 H C
    Instruction{SUB_0x91, 1, 4},

    // 0x92 SUB D
    // Z 1 H C
    Instruction{SUB_0x92, 1, 4},

    // 0x93 SUB E
    // Z 1 H C
    Instruction{SUB_0x93, 1, 4},

    // 0x94 SUB H
    // Z 1 H C
    Instruction{SUB_0x94, 1, 4},

    // 0x95 SUB L
    // Z 1 H C
    Instruction{SUB_0x95, 1, 4},

    // 0x96 SUB (HL)
    // Z 1 H C
    Instruction{SUB_0x96, 1, 8},

    // 0x97 SUB A
    // Z 1 H C
    Instruction{SUB_0x97, 1, 4},

    // 0x98 SBC A,B
    // Z 1 H C
    Instruction{SBC_0x98, 1, 4},

    // 0x99 SBC A,C
    // Z 1 H C
    Instruction{SBC_0x99, 1, 4},

    // 0x9A SBC A,D
    // Z 1 H C
    Instruction{SBC_0x9A, 1, 4},

    // 0x9B SBC A,E
    // Z 1 H C
    Instruction{SBC_0x9B, 1, 4},

    // 0x9C SBC A,H
    // Z 1 H C
    Instruction{SBC_0x9C, 1, 4},

    // 0x9D SBC A,L
    // Z 1 H C
    Instruction{SBC_0x9D, 1, 4},

    // 0x9E SBC A,(HL)
    // Z 1 H C
    Instruction{SBC_0x9E, 1, 8},

    // 0x9F SBC A,A
    // Z 1 H C
    Instruction{SBC_0x9F, 1, 4},

    // 0xA0 AND B
    // Z 0 1 0
    Instruction{AND_0xA0, 1, 4},

    // 0xA1 AND C
    // Z 0 1 0
    Instruction{AND_0xA1, 1, 4},

    // 0xA2 AND D
    // Z 0 1 0
    Instruction{AND_0xA2, 1, 4},

    // 0xA3 AND E
    // Z 0 1 0
    Instruction{AND_0xA3, 1, 4},

    // 0xA4 AND H
    // Z 0 1 0
    Instruction{AND_0xA4, 1, 4},

    // 0xA5 AND L
    // Z 0 1 0
    Instruction{AND_0xA5, 1, 4},

    // 0xA6 AND (HL)
    // Z 0 1 0
    Instruction{AND_0xA6, 1, 8},

    // 0xA7 AND A
    // Z 0 1 0
    Instruction{AND_0xA7, 1, 4},

    // 0xA8 XOR B
    // Z 0 0 0
    Instruction{XOR_0xA8, 1, 4},

    // 0xA9 XOR C
    // Z 0 0 0
    Instruction{XOR_0xA9, 1, 4},

    // 0xAA XOR D
    // Z 0 0 0
    Instruction{XOR_0xAA, 1, 4},

    // 0xAB XOR E
    // Z 0 0 0
    Instruction{XOR_0xAB, 1, 4},

    // 0xAC XOR H
    // Z 0 0 0
    Instruction{XOR_0xAC, 1, 4},

    // 0xAD XOR L
    // Z 0 0 0
    Instruction{XOR_0xAD, 1, 4},

    // 0xAE XOR (HL)
    // Z 0 0 0
    Instruction{XOR_0xAE, 1, 8},

    // 0xAF XOR A
    // Z 0 0 0
    Instruction{XOR_0xAF, 1, 4},

    // 0xB0 OR B
    // Z 0 0 0
    Instruction{OR_0xB0, 1, 4},

    // 0xB1 OR C
    // Z 0 0 0
    Instruction{OR_0xB1, 1, 4},

    // 0xB2 OR D
    // Z 0 0 0
    Instruction{OR_0xB2, 1, 4},

    // 0xB3 OR E
    // Z 0 0 0
    Instruction{OR_0xB3, 1, 4},

    // 0xB4 OR H
    // Z 0 0 0
    Instruction{OR_0xB4, 1, 4},

    // 0xB5 OR L
    // Z 0 0 0
    Instruction{OR_0xB5, 1, 4},

    // 0xB6 OR (HL)
    // Z 0 0 0
    Instruction{OR_0xB6, 1, 8},

    // 0xB7 OR A
    // Z 0 0 0
    Instruction{OR_0xB7, 1, 4},

    // 0xB8 CP B
    // Z 1 H C
    Instruction{CP_0xB8, 1, 4},

    // 0xB9 CP C
    // Z 1 H C
    Instruction{CP_0xB9, 1, 4},

    // 0xBA CP D
    // Z 1 H C
    Instruction{CP_0xBA, 1, 4},

    // 0xBB CP E
    // Z 1 H C
    Instruction{CP_0xBB, 1, 4},

    // 0xBC CP H
    // Z 1 H C
    Instruction{CP_0xBC, 1, 4},

    // 0xBD CP L
    // Z 1 H C
    Instruction{CP_0xBD, 1, 4},

    // 0xBE CP (HL)
    // Z 1 H C
    Instruction{CP_0xBE, 1, 8},

    // 0xBF CP A
    // Z 1 H C
    Instruction{CP_0xBF, 1, 4},

    // 0xC0 RET NZ
    // - - - -
    Instruction{RET_0xC0, 1, 20, 8},

    // 0xC1 POP BC
    // - - - -
    Instruction{POP_0xC1, 1, 12},

    // 0xC2 JP NZ,a16
    // - - - -
    Instruction{JP_0xC2, 3, 16, 12},

    // 0xC3 JP a16
    // - - - -
    Instruction{JP_0xC3, 3, 16},

    // 0xC4 CALL NZ,a16
    // - - - -
    Instruction{CALL_0xC4, 3, 24, 12},

    // 0xC5 PUSH BC
    // - - - -
    Instruction{PUSH_0xC5, 1, 16},

    // 0xC6 ADD A,d8
    // Z 0 H C
    Instruction{ADD_0xC6, 2, 8},

    // 0xC7 RST 00H
    // - - - -
    Instruction{RST_0xC7, 1, 16},

    // 0xC8 RET Z
    // - - - -
    Instruction{RET_0xC8, 1, 20, 8},

    // 0xC9 RET
    // - - - -
    Instruction{RET_0xC9, 1, 16},

    // 0xCA JP Z,a16
    // - - - -
    Instruction{JP_0xCA, 3, 16, 12},

    // 0xCB PREFIX CB
    // - - - -
    Instruction{PREFIX_0xCB, 1, 4},

    // 0xCC CALL Z,a16
    // - - - -
    Instruction{CALL_0xCC, 3, 24, 12},

    // 0xCD CALL a16
    // - - - -
    Instruction{CALL_0xCD, 3, 24},

    // 0xCE ADC A,d8
    // Z 0 H C
    Instruction{ADC_0xCE, 2, 8},

    // 0xCF RST 08H
    // - - - -
    Instruction{RST_0xCF, 1, 16},

    // 0xD0 RET NC
    // - - - -
    Instruction{RET_0xD0, 1, 20, 8},

    // 0xD1 POP DE
    // - - - -
    Instruction{POP_0xD1, 1, 12},

    // 0xD2 JP NC,a16
    // - - - -
    Instruction{JP_0xD2, 3, 16, 12},

    // 0xD3 INVALID
    Instruction{TERMINATE, 1, 4},

    // 0xD4 CALL NC,a16
    // - - - -
    Instruction{CALL_0xD4, 3, 24, 12},

    // 0xD5 PUSH DE
    // - - - -
    Instruction{PUSH_0xD5, 1, 16},

    // 0xD6 SUB d8
    // Z 1 H C
    Instruction{SUB_0xD6, 2, 8},

    // 0xD7 RST 10H
    // - - - -
    Instruction{RST_0xD7, 1, 16},

    // 0xD8 RET C
    // - - - -
    Instruction{RET_0xD8, 1, 20, 8},

    // 0xD9 RETI
    // - - - -
    Instruction{RETI_0xD9, 1, 16},

    // 0xDA JP C,a16
    // - - - -
    Instruction{JP_0xDA, 3, 16, 12},

    // 0xDB INVALID
    Instruction{TERMINATE, 1, 4},

    // 0xDC CALL C,a16
    // - - - -
    Instruction{CALL_0xDC, 3, 24, 12},

    // 0xDD INVALID
    Instruction{TERMINATE, 1, 4},

    // 0xDE SBC A,d8
    // Z 1 H C
    Instruction{SBC_0xDE, 2, 8},

    // 0xDF RST 18H
    // - - - -
    Instruction{RST_0xDF, 1, 16},

    // 0xE0 LDH (a8),A
    // - - - -
    Instruction{LDH_0xE0, 2, 12},

    // 0xE1 POP HL
    // - - - -
    Instruction{POP_0xE1, 1, 12},

    // 0xE2 LD (C),A
    // - - - -
    Instruction{LD_0xE2, 2, 8},

    // 0xE3 INVALID
    Instruction{TERMINATE, 1, 4},

    // 0xE4 INVALID
    Instruction{TERMINATE, 1, 4},

    // 0xE5 PUSH HL
    // - - - -
    Instruction{PUSH_0xE5, 1, 16},

    // 0xE6 AND d8
    // Z 0 1 0
    Instruction{AND_0xE6, 2, 8},

    // 0xE7 RST 20H
    // - - - -
    Instruction{RST_0xE7, 1, 16},

    // 0xE8 ADD SP,r8
    // 0 0 H C
    Instruction{ADD_0xE8, 2, 16},

    // 0xE9 JP (HL)
    // - - - -
    Instruction{JP_0xE9, 1, 4},

    // 0xEA LD (a16),A
    // - - - -
    Instruction{LD_0xEA, 3, 16},

    // 0xEB INVALID
    Instruction{TERMINATE, 1, 4},

    // 0xEC INVALID
    Instruction{TERMINATE, 1, 4},

    // 0xED INVALID
    Instruction{TERMINATE, 1, 4},

    // 0xEE XOR d8
    // Z 0 0 0
    Instruction{XOR_0xEE, 2, 8},

    // 0xEF RST 28H
    // - - - -
    Instruction{RST_0xEF, 1, 16},

    // 0xF0 LDH A,(a8)
    // - - - -
    Instruction{LDH_0xF0, 2, 12},

    // 0xF1 POP AF
    // - - - -
    Instruction{POP_0xF1, 1, 12},

    // 0xF2 LD A,(C)
    // - - - -
    Instruction{LD_0xF2, 2, 8},

    // 0xF3 DI
    // - - - -
    Instruction{DI_0xF3, 1, 4},

    // 0xF4 INVALID
    Instruction{TERMINATE, 1, 4},

    // 0xF5 PUSH AF
    // - - - -
    Instruction{PUSH_0xF5, 1, 16},

    // 0xF6 OR d8
    // Z 0 0 0
    Instruction{OR_0xF6, 2, 8},

    // 0xF7 RST 30H
    // - - - -
    Instruction{RST_0xF7, 1, 16},

    // 0xF8 LD HL,SP+r8
    // 0 0 H C
    Instruction{LD_0xF8, 2, 12},

    // 0xF9 LD SP,HL
    // - - - -
    Instruction{LD_0xF9, 1, 8},

    // 0xFA LD A,(a16)
    // - - - -
    Instruction{LD_0xFA, 3, 16},

    // 0xFB EI
    // - - - -
    Instruction{EI_0xFB, 1, 4},

    // 0xFC INVALID
    Instruction{TERMINATE, 1, 4},

    // 0xFD INVALID
    Instruction{TERMINATE, 1, 4},

    // 0xFE CP d8
    // Z 1 H C
    Instruction{CP_0xFE, 2, 8},

    // 0xFF RST 38H
    // - - - -
    Instruction{RST_0xFF, 1, 16}
};
} // Core

#endif // __INSTRUCTION_LOOKUP_TABLE_H__