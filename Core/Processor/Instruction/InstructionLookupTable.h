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
	STOP_RUNNING("An error has occurred!");
	return false;
};

Message("Need to increment PC after each instruction!");
static const std::array<Instruction, 0x10 * 0x10> INSTRUCTION_LOOKUP_TABLE =
{
    // 0x00 NOP
    // - - - -
    Instruction{NOP_0x00, 1, 4, 0, "NOP"},

    // 0x01 LD BC,d16
    // - - - -
    Instruction{LD_0x01, 3, 12, 0, "LD BC,d16"},

    // 0x02 LD (BC),A
    // - - - -
    Instruction{LD_0x02, 1, 8, 0, "LD (BC),A"},

    // 0x03 INC BC
    // - - - -
    Instruction{INC_0x03, 1, 8, 0, "INC BC"},

    // 0x04 INC B
    // Z 0 H -
    Instruction{INC_0x04, 1, 4, 0, "INC B"},

    // 0x05 DEC B
    // Z 1 H -
    Instruction{DEC_0x05, 1, 4, 0, "DEC B"},

    // 0x06 LD B,d8
    // - - - -
    Instruction{LD_0x06, 2, 8, 0, "LD B,d8"},

    // 0x07 RLCA
    // 0 0 0 C
    Instruction{RLCA_0x07, 1, 4, 0, "RLCA"},

    // 0x08 LD (a16),SP
    // - - - -
    Instruction{LD_0x08, 3, 20, 0, "LD (a16),SP"},

    // 0x09 ADD HL,BC
    // - 0 H C
    Instruction{ADD_0x09, 1, 8, 0, "ADD HL,BC"},

    // 0x0A LD A,(BC)
    // - - - -
    Instruction{LD_0x0A, 1, 8, 0, "LD A,(BC)"},

    // 0x0B DEC BC
    // - - - -
    Instruction{DEC_0x0B, 1, 8, 0, "DEC BC"},

    // 0x0C INC C
    // Z 0 H -
    Instruction{INC_0x0C, 1, 4, 0, "INC C"},

    // 0x0D DEC C
    // Z 1 H -
    Instruction{DEC_0x0D, 1, 4, 0, "DEC C"},

    // 0x0E LD C,d8
    // - - - -
    Instruction{LD_0x0E, 2, 8, 0, "LD C,d8"},

    // 0x0F RRCA
    // 0 0 0 C
    Instruction{RRCA_0x0F, 1, 4, 0, "RRCA"},

    // 0x10 STOP 0
    // - - - -
    Instruction{STOP_0x10, 2, 4, 0, "STOP 0"},

    // 0x11 LD DE,d16
    // - - - -
    Instruction{LD_0x11, 3, 12, 0, "LD DE,d16"},

    // 0x12 LD (DE),A
    // - - - -
    Instruction{LD_0x12, 1, 8, 0, "LD (DE),A"},

    // 0x13 INC DE
    // - - - -
    Instruction{INC_0x13, 1, 8, 0, "INC DE"},

    // 0x14 INC D
    // Z 0 H -
    Instruction{INC_0x14, 1, 4, 0, "INC D"},

    // 0x15 DEC D
    // Z 1 H -
    Instruction{DEC_0x15, 1, 4, 0, "DEC D"},

    // 0x16 LD D,d8
    // - - - -
    Instruction{LD_0x16, 2, 8, 0, "LD D,d8"},

    // 0x17 RLA
    // 0 0 0 C
    Instruction{RLA_0x17, 1, 4, 0, "RLA"},

    // 0x18 JR r8
    // - - - -
    Instruction{JR_0x18, 2, 12, 0, "JR r8"},

    // 0x19 ADD HL,DE
    // - 0 H C
    Instruction{ADD_0x19, 1, 8, 0, "ADD HL,DE"},

    // 0x1A LD A,(DE)
    // - - - -
    Instruction{LD_0x1A, 1, 8, 0, "LD A,(DE)"},

    // 0x1B DEC DE
    // - - - -
    Instruction{DEC_0x1B, 1, 8, 0, "DEC DE"},

    // 0x1C INC E
    // Z 0 H -
    Instruction{INC_0x1C, 1, 4, 0, "INC E"},

    // 0x1D DEC E
    // Z 1 H -
    Instruction{DEC_0x1D, 1, 4, 0, "DEC E"},

    // 0x1E LD E,d8
    // - - - -
    Instruction{LD_0x1E, 2, 8, 0, "LD E,d8"},

    // 0x1F RRA
    // 0 0 0 C
    Instruction{RRA_0x1F, 1, 4, 0, "RRA"},

    // 0x20 JR NZ,r8
    // - - - -
    Instruction{JR_0x20, 2, 12, 8, "JR NZ,r8"},

    // 0x21 LD HL,d16
    // - - - -
    Instruction{LD_0x21, 3, 12, 0, "LD HL,d16"},

    // 0x22 LD (HL+),A
    // - - - -
    Instruction{LD_0x22, 1, 8, 0, "LD (HL+),A"},

    // 0x23 INC HL
    // - - - -
    Instruction{INC_0x23, 1, 8, 0, "INC HL"},

    // 0x24 INC H
    // Z 0 H -
    Instruction{INC_0x24, 1, 4, 0, "INC H"},

    // 0x25 DEC H
    // Z 1 H -
    Instruction{DEC_0x25, 1, 4, 0, "DEC H"},

    // 0x26 LD H,d8
    // - - - -
    Instruction{LD_0x26, 2, 8, 0, "LD H,d8"},

    // 0x27 DAA
    // Z - 0 C
    Instruction{DAA_0x27, 1, 4, 0, "DAA"},

    // 0x28 JR Z,r8
    // - - - -
    Instruction{JR_0x28, 2, 12, 8, "JR Z,r8"},

    // 0x29 ADD HL,HL
    // - 0 H C
    Instruction{ADD_0x29, 1, 8, 0, "ADD HL,HL"},

    // 0x2A LD A,(HL+)
    // - - - -
    Instruction{LD_0x2A, 1, 8, 0, "LD A,(HL+)"},

    // 0x2B DEC HL
    // - - - -
    Instruction{DEC_0x2B, 1, 8, 0, "DEC HL"},

    // 0x2C INC L
    // Z 0 H -
    Instruction{INC_0x2C, 1, 4, 0, "INC L"},

    // 0x2D DEC L
    // Z 1 H -
    Instruction{DEC_0x2D, 1, 4, 0, "DEC L"},

    // 0x2E LD L,d8
    // - - - -
    Instruction{LD_0x2E, 2, 8, 0, "LD L,d8"},

    // 0x2F CPL
    // - 1 1 -
    Instruction{CPL_0x2F, 1, 4, 0, "CPL"},

    // 0x30 JR NC,r8
    // - - - -
    Instruction{JR_0x30, 2, 12, 8, "JR NC,r8"},

    // 0x31 LD SP,d16
    // - - - -
    Instruction{LD_0x31, 3, 12, 0, "LD SP,d16"},

    // 0x32 LD (HL-),A
    // - - - -
    Instruction{LD_0x32, 1, 8, 0, "LD (HL-),A"},

    // 0x33 INC SP
    // - - - -
    Instruction{INC_0x33, 1, 8, 0, "INC SP"},

    // 0x34 INC (HL)
    // Z 0 H -
    Instruction{INC_0x34, 1, 12, 0, "INC (HL)"},

    // 0x35 DEC (HL)
    // Z 1 H -
    Instruction{DEC_0x35, 1, 12, 0, "DEC (HL)"},

    // 0x36 LD (HL),d8
    // - - - -
    Instruction{LD_0x36, 2, 12, 0, "LD (HL),d8"},

    // 0x37 SCF
    // - 0 0 1
    Instruction{SCF_0x37, 1, 4, 0, "SCF"},

    // 0x38 JR C,r8
    // - - - -
    Instruction{JR_0x38, 2, 12, 8, "JR C,r8"},

    // 0x39 ADD HL,SP
    // - 0 H C
    Instruction{ADD_0x39, 1, 8, 0, "ADD HL,SP"},

    // 0x3A LD A,(HL-)
    // - - - -
    Instruction{LD_0x3A, 1, 8, 0, "LD A,(HL-)"},

    // 0x3B DEC SP
    // - - - -
    Instruction{DEC_0x3B, 1, 8, 0, "DEC SP"},

    // 0x3C INC A
    // Z 0 H -
    Instruction{INC_0x3C, 1, 4, 0, "INC A"},

    // 0x3D DEC A
    // Z 1 H -
    Instruction{DEC_0x3D, 1, 4, 0, "DEC A"},

    // 0x3E LD A,d8
    // - - - -
    Instruction{LD_0x3E, 2, 8, 0, "LD A,d8"},

    // 0x3F CCF
    // - 0 0 C
    Instruction{CCF_0x3F, 1, 4, 0, "CCF"},

    // 0x40 LD B,B
    // - - - -
    Instruction{LD_0x40, 1, 4, 0, "LD B,B"},

    // 0x41 LD B,C
    // - - - -
    Instruction{LD_0x41, 1, 4, 0, "LD B,C"},

    // 0x42 LD B,D
    // - - - -
    Instruction{LD_0x42, 1, 4, 0, "LD B,D"},

    // 0x43 LD B,E
    // - - - -
    Instruction{LD_0x43, 1, 4, 0, "LD B,E"},

    // 0x44 LD B,H
    // - - - -
    Instruction{LD_0x44, 1, 4, 0, "LD B,H"},

    // 0x45 LD B,L
    // - - - -
    Instruction{LD_0x45, 1, 4, 0, "LD B,L"},

    // 0x46 LD B,(HL)
    // - - - -
    Instruction{LD_0x46, 1, 8, 0, "LD B,(HL)"},

    // 0x47 LD B,A
    // - - - -
    Instruction{LD_0x47, 1, 4, 0, "LD B,A"},

    // 0x48 LD C,B
    // - - - -
    Instruction{LD_0x48, 1, 4, 0, "LD C,B"},

    // 0x49 LD C,C
    // - - - -
    Instruction{LD_0x49, 1, 4, 0, "LD C,C"},

    // 0x4A LD C,D
    // - - - -
    Instruction{LD_0x4A, 1, 4, 0, "LD C,D"},

    // 0x4B LD C,E
    // - - - -
    Instruction{LD_0x4B, 1, 4, 0, "LD C,E"},

    // 0x4C LD C,H
    // - - - -
    Instruction{LD_0x4C, 1, 4, 0, "LD C,H"},

    // 0x4D LD C,L
    // - - - -
    Instruction{LD_0x4D, 1, 4, 0, "LD C,L"},

    // 0x4E LD C,(HL)
    // - - - -
    Instruction{LD_0x4E, 1, 8, 0, "LD C,(HL)"},

    // 0x4F LD C,A
    // - - - -
    Instruction{LD_0x4F, 1, 4, 0, "LD C,A"},

    // 0x50 LD D,B
    // - - - -
    Instruction{LD_0x50, 1, 4, 0, "LD D,B"},

    // 0x51 LD D,C
    // - - - -
    Instruction{LD_0x51, 1, 4, 0, "LD D,C"},

    // 0x52 LD D,D
    // - - - -
    Instruction{LD_0x52, 1, 4, 0, "LD D,D"},

    // 0x53 LD D,E
    // - - - -
    Instruction{LD_0x53, 1, 4, 0, "LD D,E"},

    // 0x54 LD D,H
    // - - - -
    Instruction{LD_0x54, 1, 4, 0, "LD D,H"},

    // 0x55 LD D,L
    // - - - -
    Instruction{LD_0x55, 1, 4, 0, "LD D,L"},

    // 0x56 LD D,(HL)
    // - - - -
    Instruction{LD_0x56, 1, 8, 0, "LD D,(HL)"},

    // 0x57 LD D,A
    // - - - -
    Instruction{LD_0x57, 1, 4, 0, "LD D,A"},

    // 0x58 LD E,B
    // - - - -
    Instruction{LD_0x58, 1, 4, 0, "LD E,B"},

    // 0x59 LD E,C
    // - - - -
    Instruction{LD_0x59, 1, 4, 0, "LD E,C"},

    // 0x5A LD E,D
    // - - - -
    Instruction{LD_0x5A, 1, 4, 0, "LD E,D"},

    // 0x5B LD E,E
    // - - - -
    Instruction{LD_0x5B, 1, 4, 0, "LD E,E"},

    // 0x5C LD E,H
    // - - - -
    Instruction{LD_0x5C, 1, 4, 0, "LD E,H"},

    // 0x5D LD E,L
    // - - - -
    Instruction{LD_0x5D, 1, 4, 0, "LD E,L"},

    // 0x5E LD E,(HL)
    // - - - -
    Instruction{LD_0x5E, 1, 8, 0, "LD E,(HL)"},

    // 0x5F LD E,A
    // - - - -
    Instruction{LD_0x5F, 1, 4, 0, "LD E,A"},

    // 0x60 LD H,B
    // - - - -
    Instruction{LD_0x60, 1, 4, 0, "LD H,B"},

    // 0x61 LD H,C
    // - - - -
    Instruction{LD_0x61, 1, 4, 0, "LD H,C"},

    // 0x62 LD H,D
    // - - - -
    Instruction{LD_0x62, 1, 4, 0, "LD H,D"},

    // 0x63 LD H,E
    // - - - -
    Instruction{LD_0x63, 1, 4, 0, "LD H,E"},

    // 0x64 LD H,H
    // - - - -
    Instruction{LD_0x64, 1, 4, 0, "LD H,H"},

    // 0x65 LD H,L
    // - - - -
    Instruction{LD_0x65, 1, 4, 0, "LD H,L"},

    // 0x66 LD H,(HL)
    // - - - -
    Instruction{LD_0x66, 1, 8, 0, "LD H,(HL)"},

    // 0x67 LD H,A
    // - - - -
    Instruction{LD_0x67, 1, 4, 0, "LD H,A"},

    // 0x68 LD L,B
    // - - - -
    Instruction{LD_0x68, 1, 4, 0, "LD L,B"},

    // 0x69 LD L,C
    // - - - -
    Instruction{LD_0x69, 1, 4, 0, "LD L,C"},

    // 0x6A LD L,D
    // - - - -
    Instruction{LD_0x6A, 1, 4, 0, "LD L,D"},

    // 0x6B LD L,E
    // - - - -
    Instruction{LD_0x6B, 1, 4, 0, "LD L,E"},

    // 0x6C LD L,H
    // - - - -
    Instruction{LD_0x6C, 1, 4, 0, "LD L,H"},

    // 0x6D LD L,L
    // - - - -
    Instruction{LD_0x6D, 1, 4, 0, "LD L,L"},

    // 0x6E LD L,(HL)
    // - - - -
    Instruction{LD_0x6E, 1, 8, 0, "LD L,(HL)"},

    // 0x6F LD L,A
    // - - - -
    Instruction{LD_0x6F, 1, 4, 0, "LD L,A"},

    // 0x70 LD (HL),B
    // - - - -
    Instruction{LD_0x70, 1, 8, 0, "LD (HL),B"},

    // 0x71 LD (HL),C
    // - - - -
    Instruction{LD_0x71, 1, 8, 0, "LD (HL),C"},

    // 0x72 LD (HL),D
    // - - - -
    Instruction{LD_0x72, 1, 8, 0, "LD (HL),D"},

    // 0x73 LD (HL),E
    // - - - -
    Instruction{LD_0x73, 1, 8, 0, "LD (HL),E"},

    // 0x74 LD (HL),H
    // - - - -
    Instruction{LD_0x74, 1, 8, 0, "LD (HL),H"},

    // 0x75 LD (HL),L
    // - - - -
    Instruction{LD_0x75, 1, 8, 0, "LD (HL),L"},

    // 0x76 HALT
    // - - - -
    Instruction{HALT_0x76, 1, 4, 0, "HALT"},

    // 0x77 LD (HL),A
    // - - - -
    Instruction{LD_0x77, 1, 8, 0, "LD (HL),A"},

    // 0x78 LD A,B
    // - - - -
    Instruction{LD_0x78, 1, 4, 0, "LD A,B"},

    // 0x79 LD A,C
    // - - - -
    Instruction{LD_0x79, 1, 4, 0, "LD A,C"},

    // 0x7A LD A,D
    // - - - -
    Instruction{LD_0x7A, 1, 4, 0, "LD A,D"},

    // 0x7B LD A,E
    // - - - -
    Instruction{LD_0x7B, 1, 4, 0, "LD A,E"},

    // 0x7C LD A,H
    // - - - -
    Instruction{LD_0x7C, 1, 4, 0, "LD A,H"},

    // 0x7D LD A,L
    // - - - -
    Instruction{LD_0x7D, 1, 4, 0, "LD A,L"},

    // 0x7E LD A,(HL)
    // - - - -
    Instruction{LD_0x7E, 1, 8, 0, "LD A,(HL)"},

    // 0x7F LD A,A
    // - - - -
    Instruction{LD_0x7F, 1, 4, 0, "LD A,A"},

    // 0x80 ADD A,B
    // Z 0 H C
    Instruction{ADD_0x80, 1, 4, 0, "ADD A,B"},

    // 0x81 ADD A,C
    // Z 0 H C
    Instruction{ADD_0x81, 1, 4, 0, "ADD A,C"},

    // 0x82 ADD A,D
    // Z 0 H C
    Instruction{ADD_0x82, 1, 4, 0, "ADD A,D"},

    // 0x83 ADD A,E
    // Z 0 H C
    Instruction{ADD_0x83, 1, 4, 0, "ADD A,E"},

    // 0x84 ADD A,H
    // Z 0 H C
    Instruction{ADD_0x84, 1, 4, 0, "ADD A,H"},

    // 0x85 ADD A,L
    // Z 0 H C
    Instruction{ADD_0x85, 1, 4, 0, "ADD A,L"},

    // 0x86 ADD A,(HL)
    // Z 0 H C
    Instruction{ADD_0x86, 1, 8, 0, "ADD A,(HL)"},

    // 0x87 ADD A,A
    // Z 0 H C
    Instruction{ADD_0x87, 1, 4, 0, "ADD A,A"},

    // 0x88 ADC A,B
    // Z 0 H C
    Instruction{ADC_0x88, 1, 4, 0, "ADC A,B"},

    // 0x89 ADC A,C
    // Z 0 H C
    Instruction{ADC_0x89, 1, 4, 0, "ADC A,C"},

    // 0x8A ADC A,D
    // Z 0 H C
    Instruction{ADC_0x8A, 1, 4, 0, "ADC A,D"},

    // 0x8B ADC A,E
    // Z 0 H C
    Instruction{ADC_0x8B, 1, 4, 0, "ADC A,E"},

    // 0x8C ADC A,H
    // Z 0 H C
    Instruction{ADC_0x8C, 1, 4, 0, "ADC A,H"},

    // 0x8D ADC A,L
    // Z 0 H C
    Instruction{ADC_0x8D, 1, 4, 0, "ADC A,L"},

    // 0x8E ADC A,(HL)
    // Z 0 H C
    Instruction{ADC_0x8E, 1, 8, 0, "ADC A,(HL)"},

    // 0x8F ADC A,A
    // Z 0 H C
    Instruction{ADC_0x8F, 1, 4, 0, "ADC A,A"},

    // 0x90 SUB B
    // Z 1 H C
    Instruction{SUB_0x90, 1, 4, 0, "SUB B"},

    // 0x91 SUB C
    // Z 1 H C
    Instruction{SUB_0x91, 1, 4, 0, "SUB C"},

    // 0x92 SUB D
    // Z 1 H C
    Instruction{SUB_0x92, 1, 4, 0, "SUB D"},

    // 0x93 SUB E
    // Z 1 H C
    Instruction{SUB_0x93, 1, 4, 0, "SUB E"},

    // 0x94 SUB H
    // Z 1 H C
    Instruction{SUB_0x94, 1, 4, 0, "SUB H"},

    // 0x95 SUB L
    // Z 1 H C
    Instruction{SUB_0x95, 1, 4, 0, "SUB L"},

    // 0x96 SUB (HL)
    // Z 1 H C
    Instruction{SUB_0x96, 1, 8, 0, "SUB (HL)"},

    // 0x97 SUB A
    // Z 1 H C
    Instruction{SUB_0x97, 1, 4, 0, "SUB A"},

    // 0x98 SBC A,B
    // Z 1 H C
    Instruction{SBC_0x98, 1, 4, 0, "SBC A,B"},

    // 0x99 SBC A,C
    // Z 1 H C
    Instruction{SBC_0x99, 1, 4, 0, "SBC A,C"},

    // 0x9A SBC A,D
    // Z 1 H C
    Instruction{SBC_0x9A, 1, 4, 0, "SBC A,D"},

    // 0x9B SBC A,E
    // Z 1 H C
    Instruction{SBC_0x9B, 1, 4, 0, "SBC A,E"},

    // 0x9C SBC A,H
    // Z 1 H C
    Instruction{SBC_0x9C, 1, 4, 0, "SBC A,H"},

    // 0x9D SBC A,L
    // Z 1 H C
    Instruction{SBC_0x9D, 1, 4, 0, "SBC A,L"},

    // 0x9E SBC A,(HL)
    // Z 1 H C
    Instruction{SBC_0x9E, 1, 8, 0, "SBC A,(HL)"},

    // 0x9F SBC A,A
    // Z 1 H C
    Instruction{SBC_0x9F, 1, 4, 0, "SBC A,A"},

    // 0xA0 AND B
    // Z 0 1 0
    Instruction{AND_0xA0, 1, 4, 0, "AND B"},

    // 0xA1 AND C
    // Z 0 1 0
    Instruction{AND_0xA1, 1, 4, 0, "AND C"},

    // 0xA2 AND D
    // Z 0 1 0
    Instruction{AND_0xA2, 1, 4, 0, "AND D"},

    // 0xA3 AND E
    // Z 0 1 0
    Instruction{AND_0xA3, 1, 4, 0, "AND E"},

    // 0xA4 AND H
    // Z 0 1 0
    Instruction{AND_0xA4, 1, 4, 0, "AND H"},

    // 0xA5 AND L
    // Z 0 1 0
    Instruction{AND_0xA5, 1, 4, 0, "AND L"},

    // 0xA6 AND (HL)
    // Z 0 1 0
    Instruction{AND_0xA6, 1, 8, 0, "AND (HL)"},

    // 0xA7 AND A
    // Z 0 1 0
    Instruction{AND_0xA7, 1, 4, 0, "AND A"},

    // 0xA8 XOR B
    // Z 0 0 0
    Instruction{XOR_0xA8, 1, 4, 0, "XOR B"},

    // 0xA9 XOR C
    // Z 0 0 0
    Instruction{XOR_0xA9, 1, 4, 0, "XOR C"},

    // 0xAA XOR D
    // Z 0 0 0
    Instruction{XOR_0xAA, 1, 4, 0, "XOR D"},

    // 0xAB XOR E
    // Z 0 0 0
    Instruction{XOR_0xAB, 1, 4, 0, "XOR E"},

    // 0xAC XOR H
    // Z 0 0 0
    Instruction{XOR_0xAC, 1, 4, 0, "XOR H"},

    // 0xAD XOR L
    // Z 0 0 0
    Instruction{XOR_0xAD, 1, 4, 0, "XOR L"},

    // 0xAE XOR (HL)
    // Z 0 0 0
    Instruction{XOR_0xAE, 1, 8, 0, "XOR (HL)"},

    // 0xAF XOR A
    // Z 0 0 0
    Instruction{XOR_0xAF, 1, 4, 0, "XOR A"},

    // 0xB0 OR B
    // Z 0 0 0
    Instruction{OR_0xB0, 1, 4, 0, "OR B"},

    // 0xB1 OR C
    // Z 0 0 0
    Instruction{OR_0xB1, 1, 4, 0, "OR C"},

    // 0xB2 OR D
    // Z 0 0 0
    Instruction{OR_0xB2, 1, 4, 0, "OR D"},

    // 0xB3 OR E
    // Z 0 0 0
    Instruction{OR_0xB3, 1, 4, 0, "OR E"},

    // 0xB4 OR H
    // Z 0 0 0
    Instruction{OR_0xB4, 1, 4, 0, "OR H"},

    // 0xB5 OR L
    // Z 0 0 0
    Instruction{OR_0xB5, 1, 4, 0, "OR L"},

    // 0xB6 OR (HL)
    // Z 0 0 0
    Instruction{OR_0xB6, 1, 8, 0, "OR (HL)"},

    // 0xB7 OR A
    // Z 0 0 0
    Instruction{OR_0xB7, 1, 4, 0, "OR A"},

    // 0xB8 CP B
    // Z 1 H C
    Instruction{CP_0xB8, 1, 4, 0, "CP B"},

    // 0xB9 CP C
    // Z 1 H C
    Instruction{CP_0xB9, 1, 4, 0, "CP C"},

    // 0xBA CP D
    // Z 1 H C
    Instruction{CP_0xBA, 1, 4, 0, "CP D"},

    // 0xBB CP E
    // Z 1 H C
    Instruction{CP_0xBB, 1, 4, 0, "CP E"},

    // 0xBC CP H
    // Z 1 H C
    Instruction{CP_0xBC, 1, 4, 0, "CP H"},

    // 0xBD CP L
    // Z 1 H C
    Instruction{CP_0xBD, 1, 4, 0, "CP L"},

    // 0xBE CP (HL)
    // Z 1 H C
    Instruction{CP_0xBE, 1, 8, 0, "CP (HL)"},

    // 0xBF CP A
    // Z 1 H C
    Instruction{CP_0xBF, 1, 4, 0, "CP A"},

    // 0xC0 RET NZ
    // - - - -
    Instruction{RET_0xC0, 1, 20, 8, "RET NZ"},

    // 0xC1 POP BC
    // - - - -
    Instruction{POP_0xC1, 1, 12, 0, "POP BC"},

    // 0xC2 JP NZ,a16
    // - - - -
    Instruction{JP_0xC2, 3, 16, 12, "JP NZ,a16"},

    // 0xC3 JP a16
    // - - - -
    Instruction{JP_0xC3, 3, 16, 0, "JP a16"},

    // 0xC4 CALL NZ,a16
    // - - - -
    Instruction{CALL_0xC4, 3, 24, 12, "CALL NZ,a16"},

    // 0xC5 PUSH BC
    // - - - -
    Instruction{PUSH_0xC5, 1, 16, 0, "PUSH BC"},

    // 0xC6 ADD A,d8
    // Z 0 H C
    Instruction{ADD_0xC6, 2, 8, 0, "ADD A,d8"},

    // 0xC7 RST 00H
    // - - - -
    Instruction{RST_0xC7, 1, 16, 0, "RST 00H"},

    // 0xC8 RET Z
    // - - - -
    Instruction{RET_0xC8, 1, 20, 8, "RET Z"},

    // 0xC9 RET
    // - - - -
    Instruction{RET_0xC9, 1, 16, 0, "RET"},

    // 0xCA JP Z,a16
    // - - - -
    Instruction{JP_0xCA, 3, 16, 12, "JP Z,a16"},

    // 0xCB PREFIX CB
    // - - - -
    Instruction{PREFIX_0xCB, 1, 4, 0, "PREFIX CB"},

    // 0xCC CALL Z,a16
    // - - - -
    Instruction{CALL_0xCC, 3, 24, 12, "CALL Z,a16"},

    // 0xCD CALL a16
    // - - - -
    Instruction{CALL_0xCD, 3, 24, 0, "CALL a16"},

    // 0xCE ADC A,d8
    // Z 0 H C
    Instruction{ADC_0xCE, 2, 8, 0, "ADC A,d8"},

    // 0xCF RST 08H
    // - - - -
    Instruction{RST_0xCF, 1, 16, 0, "RST 08H"},

    // 0xD0 RET NC
    // - - - -
    Instruction{RET_0xD0, 1, 20, 8, "RET NC"},

    // 0xD1 POP DE
    // - - - -
    Instruction{POP_0xD1, 1, 12, 0, "POP DE"},

    // 0xD2 JP NC,a16
    // - - - -
    Instruction{JP_0xD2, 3, 16, 12, "JP NC,a16"},

    // 0xD3 INVALID
    Instruction{TERMINATE, 0, 0, 0, "ERROR"},

    // 0xD4 CALL NC,a16
    // - - - -
    Instruction{CALL_0xD4, 3, 24, 12, "CALL NC,a16"},

    // 0xD5 PUSH DE
    // - - - -
    Instruction{PUSH_0xD5, 1, 16, 0, "PUSH DE"},

    // 0xD6 SUB d8
    // Z 1 H C
    Instruction{SUB_0xD6, 2, 8, 0, "SUB d8"},

    // 0xD7 RST 10H
    // - - - -
    Instruction{RST_0xD7, 1, 16, 0, "RST 10H"},

    // 0xD8 RET C
    // - - - -
    Instruction{RET_0xD8, 1, 20, 8, "RET C"},

    // 0xD9 RETI
    // - - - -
    Instruction{RETI_0xD9, 1, 16, 0, "RETI"},

    // 0xDA JP C,a16
    // - - - -
    Instruction{JP_0xDA, 3, 16, 12, "JP C,a16"},

    // 0xDB INVALID
    Instruction{TERMINATE, 0, 0, 0, "ERROR"},

    // 0xDC CALL C,a16
    // - - - -
    Instruction{CALL_0xDC, 3, 24, 12, "CALL C,a16"},

    // 0xDD INVALID
    Instruction{TERMINATE, 0, 0, 0, "ERROR"},

    // 0xDE SBC A,d8
    // Z 1 H C
    Instruction{SBC_0xDE, 2, 8, 0, "SBC A,d8"},

    // 0xDF RST 18H
    // - - - -
    Instruction{RST_0xDF, 1, 16, 0, "RST 18H"},

    // 0xE0 LDH (a8),A
    // - - - -
    Instruction{LDH_0xE0, 2, 12, 0, "LDH (a8),A"},

    // 0xE1 POP HL
    // - - - -
    Instruction{POP_0xE1, 1, 12, 0, "POP HL"},

    // 0xE2 LD (C),A
    // - - - -
    Instruction{LD_0xE2, 2, 8, 0, "LD (C),A"},

    // 0xE3 INVALID
    Instruction{TERMINATE, 0, 0, 0, "ERROR"},

    // 0xE4 INVALID
    Instruction{TERMINATE, 0, 0, 0, "ERROR"},

    // 0xE5 PUSH HL
    // - - - -
    Instruction{PUSH_0xE5, 1, 16, 0, "PUSH HL"},

    // 0xE6 AND d8
    // Z 0 1 0
    Instruction{AND_0xE6, 2, 8, 0, "AND d8"},

    // 0xE7 RST 20H
    // - - - -
    Instruction{RST_0xE7, 1, 16, 0, "RST 20H"},

    // 0xE8 ADD SP,r8
    // 0 0 H C
    Instruction{ADD_0xE8, 2, 16, 0, "ADD SP,r8"},

    // 0xE9 JP (HL)
    // - - - -
    Instruction{JP_0xE9, 1, 4, 0, "JP (HL)"},

    // 0xEA LD (a16),A
    // - - - -
    Instruction{LD_0xEA, 3, 16, 0, "LD (a16),A"},

    // 0xEB INVALID
    Instruction{TERMINATE, 0, 0, 0, "ERROR"},

    // 0xEC INVALID
    Instruction{TERMINATE, 0, 0, 0, "ERROR"},

    // 0xED INVALID
    Instruction{TERMINATE, 0, 0, 0, "ERROR"},

    // 0xEE XOR d8
    // Z 0 0 0
    Instruction{XOR_0xEE, 2, 8, 0, "XOR d8"},

    // 0xEF RST 28H
    // - - - -
    Instruction{RST_0xEF, 1, 16, 0, "RST 28H"},

    // 0xF0 LDH A,(a8)
    // - - - -
    Instruction{LDH_0xF0, 2, 12, 0, "LDH A,(a8)"},

    // 0xF1 POP AF
    // - - - -
    Instruction{POP_0xF1, 1, 12, 0, "POP AF"},

    // 0xF2 LD A,(C)
    // - - - -
    Instruction{LD_0xF2, 2, 8, 0, "LD A,(C)"},

    // 0xF3 DI
    // - - - -
    Instruction{DI_0xF3, 1, 4, 0, "DI"},

    // 0xF4 INVALID
    Instruction{TERMINATE, 0, 0, 0, "ERROR"},

    // 0xF5 PUSH AF
    // - - - -
    Instruction{PUSH_0xF5, 1, 16, 0, "PUSH AF"},

    // 0xF6 OR d8
    // Z 0 0 0
    Instruction{OR_0xF6, 2, 8, 0, "OR d8"},

    // 0xF7 RST 30H
    // - - - -
    Instruction{RST_0xF7, 1, 16, 0, "RST 30H"},

    // 0xF8 LD HL,SP+r8
    // 0 0 H C
    Instruction{LD_0xF8, 2, 12, 0, "LD HL,SP+r8"},

    // 0xF9 LD SP,HL
    // - - - -
    Instruction{LD_0xF9, 1, 8, 0, "LD SP,HL"},

    // 0xFA LD A,(a16)
    // - - - -
    Instruction{LD_0xFA, 3, 16, 0, "LD A,(a16)"},

    // 0xFB EI
    // - - - -
    Instruction{EI_0xFB, 1, 4, 0, "EI"},

    // 0xFC INVALID
    Instruction{TERMINATE, 0, 0, 0, "ERROR"},

    // 0xFD INVALID
    Instruction{TERMINATE, 0, 0, 0, "ERROR"},

    // 0xFE CP d8
    // Z 1 H C
    Instruction{CP_0xFE, 2, 8, 0, "CP d8"},

    // 0xFF RST 38H
    // - - - -
    Instruction{RST_0xFF, 1, 16, 0, "RST 38H"}
};
} // Core

#endif // __INSTRUCTION_LOOKUP_TABLE_H__