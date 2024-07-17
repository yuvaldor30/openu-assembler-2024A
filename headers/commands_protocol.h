#ifndef _COMMANDS_PROTOCOL_H
#define _COMMANDS_PROTOCOL_H

#include "validation.h"

static Command commandsList[CMD_AMOUNT] = {
    /*{"command syntax", command_id, operands_amount, source_operands_allowed, dest_operands_allowed }*/
    {"mov", CMD_MOV, 2, {{TRUE, TRUE, TRUE, TRUE}, {FALSE, TRUE, TRUE, TRUE}}},
    {"cmp", CMD_CMP, 2, {{TRUE, TRUE, TRUE, TRUE}, {TRUE, TRUE, TRUE, TRUE}}},
    {"add", CMD_ADD, 2, {{TRUE, TRUE, TRUE, TRUE}, {FALSE, TRUE, TRUE, TRUE}}},
    {"sub", CMD_SUB, 2, {{TRUE, TRUE, TRUE, TRUE}, {FALSE, TRUE, TRUE, TRUE}}},
    {"not", CMD_NOT, 1, {{FALSE, FALSE, FALSE, FALSE}, {FALSE, TRUE, TRUE, TRUE}}},
    {"clr", CMD_CLR, 1, {{FALSE, FALSE, FALSE, FALSE}, {FALSE, TRUE, TRUE, TRUE}}},
    {"lea", CMD_LEA, 2, {{FALSE, TRUE, TRUE, FALSE}, {FALSE, TRUE, TRUE, TRUE}}},
    {"inc", CMD_INC, 1, {{FALSE, FALSE, FALSE, FALSE}, {FALSE, TRUE, TRUE, TRUE}}},
    {"dec", CMD_DEC, 1, {{FALSE, FALSE, FALSE, FALSE}, {FALSE, TRUE, TRUE, TRUE}}},
    {"jmp", CMD_JMP, 1, {{FALSE, FALSE, FALSE, FALSE}, {FALSE, TRUE, FALSE, TRUE}}},
    {"bne", CMD_BNE, 1, {{FALSE, FALSE, FALSE, FALSE}, {FALSE, TRUE, FALSE, TRUE}}},
    {"red", CMD_RED, 1, {{FALSE, FALSE, FALSE, FALSE}, {FALSE, TRUE, TRUE, TRUE}}},
    {"prn", CMD_PRN, 1, {{FALSE, FALSE, FALSE, FALSE}, {TRUE, TRUE, TRUE, TRUE}}},
    {"jsr", CMD_JSR, 1, {{FALSE, FALSE, FALSE, FALSE}, {FALSE, TRUE, FALSE, TRUE}}},
    {"rts", CMD_RTS, 0, {{FALSE, FALSE, FALSE, FALSE}, {FALSE, FALSE, FALSE, FALSE}}},
    {"hlt", CMD_HLT, 0, {{FALSE, FALSE, FALSE, FALSE}, {FALSE, FALSE, FALSE, FALSE}}},
};

#endif