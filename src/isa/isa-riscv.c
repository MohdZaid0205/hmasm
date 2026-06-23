#include "isa.h"
#include <string.h>
#include <stdlib.h>

bool riscv_is_register(const char* token) {
    if (token[0] != 'a' && token[0] != 'x' && token[0] != 't' && token[0] != 's') return false;
    return true;
}

unsigned int riscv_validate_and_get_size(struct INSTRUCTION* inst) {
    return 4;
}

unsigned int riscv_encode_instruction(struct INSTRUCTION* inst, unsigned char* out_buffer, unsigned long current_address) {
    InstructionHandler handler = lookup_instruction("riscv", inst->mnemonic);
    if (handler) {
        handler(inst, out_buffer, current_address);
        return 4;
    }
    return 0;
}

const struct ASSEMBLER_ISA riscv_isa = {
    .name = "riscv",
    .desc = "RISC-V 32-bit Architecture Backend",
    .is_register = riscv_is_register,
    .validate_and_get_size = riscv_validate_and_get_size,
    .encode_instruction = riscv_encode_instruction
};

__attribute__((constructor)) static void register_riscv_isa(void) {
    register_isa(&riscv_isa);
}

static unsigned int parse_reg(const char* name) {
    if (name[0] == 'x') return atoi(name + 1);
    if (name[0] == 'a') return 10 + atoi(name + 1);
    if (name[0] == 't') return 5 + atoi(name + 1); // Simplification for RV32I t0-t2 etc.
    if (name[0] == 's') return 8 + atoi(name + 1); // Simplification for RV32I s0-s1 etc.
    return 0; 
}

static unsigned int parse_imm(const char* val) {
    if (!val) return 0;
    return strtol(val, NULL, 0);
}

static void write32(unsigned char* out, unsigned int inst) {
    out[0] = (inst >> 0) & 0xFF;
    out[1] = (inst >> 8) & 0xFF;
    out[2] = (inst >> 16) & 0xFF;
    out[3] = (inst >> 24) & 0xFF;
}

IMPLEMENT_INSTRUCTION(riscv, add) {
    SIGNATURE(REGISTER, REGISTER, REGISTER) {
        unsigned int rd = parse_reg(inst->rd.as.reg.name);
        unsigned int rs1 = parse_reg(inst->rs1.as.reg.name);
        unsigned int rs2 = parse_reg(inst->rs2.as.reg.name);
        unsigned int opcode = 0x33;
        unsigned int funct3 = 0x0;
        unsigned int funct7 = 0x0;
        unsigned int encoded = (funct7 << 25) | (rs2 << 20) | (rs1 << 15) | (funct3 << 12) | (rd << 7) | opcode;
        write32(out_buffer, encoded);
        return;
    }
    OTHERWISE(add, {
        INFORMATION_LN("  ", ISA_ISM_OPR, "REGISTER", "REGISTER", "REGISTER");
    })
}

IMPLEMENT_INSTRUCTION(riscv, addi) {
    SIGNATURE(REGISTER, REGISTER, IMMIDIATE) {
        unsigned int rd = parse_reg(inst->rd.as.reg.name);
        unsigned int rs1 = parse_reg(inst->rs1.as.reg.name);
        unsigned int imm = parse_imm(inst->rs2.as.imm.value) & 0xFFF;
        unsigned int opcode = 0x13;
        unsigned int funct3 = 0x0;
        unsigned int encoded = (imm << 20) | (rs1 << 15) | (funct3 << 12) | (rd << 7) | opcode;
        write32(out_buffer, encoded);
        return;
    }
    OTHERWISE(addi, {
        INFORMATION_LN("  ", ISA_ISM_OPR, "REGISTER", "REGISTER", "IMMIDIATE");
    })
}

IMPLEMENT_INSTRUCTION(riscv, li) {
    SIGNATURE(REGISTER, IMMIDIATE, NONE) {
        unsigned int rd = parse_reg(inst->rd.as.reg.name);
        unsigned int imm = parse_imm(inst->rs1.as.imm.value) & 0xFFF;
        unsigned int opcode = 0x13; // Uses addi
        unsigned int funct3 = 0x0;
        unsigned int rs1 = 0; // x0
        unsigned int encoded = (imm << 20) | (rs1 << 15) | (funct3 << 12) | (rd << 7) | opcode;
        write32(out_buffer, encoded);
        return;
    }
    OTHERWISE(li, {
        INFORMATION_LN("  ", ISA_ISM_OPR, "REGISTER", "IMMIDIATE", "NONE");
    })
}

IMPLEMENT_INSTRUCTION(riscv, la) {
    SIGNATURE(REGISTER, IMMIDIATE, NONE) {
        // Since we don't have a linker or symbol table in flat binary generation yet,
        // we'll just emit a dummy `addi rd, x0, 0` for now to stand in for `la`.
        // A true `la` requires `auipc` and `addi` with PC-relative addressing.
        unsigned int rd = parse_reg(inst->rd.as.reg.name);
        unsigned int opcode = 0x13; // addi
        unsigned int funct3 = 0x0;
        unsigned int rs1 = 0; // x0
        unsigned int imm = 0;
        unsigned int encoded = (imm << 20) | (rs1 << 15) | (funct3 << 12) | (rd << 7) | opcode;
        write32(out_buffer, encoded);
        return;
    }
    OTHERWISE(la, {
        INFORMATION_LN("  ", ISA_ISM_OPR, "REGISTER", "LABEL", "NONE");
    })
}

IMPLEMENT_INSTRUCTION(riscv, ecall) {
    SIGNATURE(NONE, NONE, NONE) {
        unsigned int opcode = 0x73;
        unsigned int encoded = (0 << 20) | (0 << 15) | (0 << 12) | (0 << 7) | opcode; // ecall is 000000000000 00000 000 00000 1110011
        write32(out_buffer, encoded);
        return;
    }
    OTHERWISE(ecall, {
        INFORMATION_LN("  ", ISA_ISM_OPR, "NONE", "NONE", "NONE");
    })
}
