#include "bit_manipulation.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "types.h"
#include "list.h"

typedef struct Gate Gate;
typedef struct TruthTable TruthTable;

typedef u64 (*Logic)(u64);

typedef enum {
    WIRE,
    NOT,
    AND,
    OR,
    XOR,
    NAND,
    NOR,
    XNOR,
} GateType;

struct Gate {
    size_t input_width;
    size_t output_width;
    Logic logic;
};

struct TruthTable {
    u64 (*rows)[2];
    size_t size;
    Gate gate;
};

LIST_DEF(GateList, Gate);
LIST_DEF(GateGraph, GateList);

typedef struct {
    GateGraph graph; 
} Circuit;

u64 not(u64 input)
{
    return ~BIT_AT(input, 0);
}

u64 and(u64 input)
{
    return BIT_AT(input, 0) & BIT_AT(input, 1);
}

u64 or(u64 input)
{
    return BIT_AT(input, 0) | BIT_AT(input, 1);
}

u64 xor(u64 input)
{
    return BIT_AT(input, 0) ^ BIT_AT(input, 1);
}

u64 nand(u64 input)
{
    return ~(BIT_AT(input, 0) & BIT_AT(input, 1));
}

u64 nor(u64 input)
{
    return ~(BIT_AT(input, 0) | BIT_AT(input, 1));
}

u64 xnor(u64 input)
{
    return ~(BIT_AT(input, 0) ^ BIT_AT(input, 1));
}

u64 in(u64 input)
{
    return BIT_AT(input, 0);
}

u64 adder(u64 input)
{
    u64 a = BIT_AT(input, 0);
    u64 b = BIT_AT(input, 1);
    u64 cin = BIT_AT(input, 2);
    u64 output = 0;
    u64 add1 = a ^ b;
    u64 add2 = add1 ^ cin;
    u64 cout = (a & b) | (cin & add1);
    BIT_SET(output, 0, add2);
    BIT_SET(output, 1, cout);
    return output;
}

TruthTable generate_truthtable(Gate gate)
{
    TruthTable tt = {0};
    tt.gate = gate;
    tt.size = pow(2, gate.input_width);
    tt.rows = malloc(sizeof(*tt.rows) * tt.size);
    u64 start = tt.size - 1;

    for (size_t i = 0; i < tt.size; i++) {
        tt.rows[i][0] = start - i; 
        tt.rows[i][1] = gate.logic(start - i); 
    }

    return tt;
}

void print_truthtable(TruthTable* tt, const char *name)
{
    printf("'%s' Truth Table:\n", name);
    for (size_t i = 0; i < tt->size; i++) {
        for (int j = tt->gate.input_width - 1; j >= 0; j--) {
            printf("%lld | ", BIT_AT(tt->rows[i][0], j));
        }
        printf("=> ");
        for (size_t j = 0; j < tt->gate.output_width; j++) {
            if (j < tt->gate.output_width - 1) {
                printf("%lld | ", BIT_AT(tt->rows[i][1], j));
            }
            else {
                printf("%lld", BIT_AT(tt->rows[i][1], j));
            }
        }
        printf("\n");
    }
}

Gate gate(GateType type)
{
    switch (type) {
        case WIRE:
            return (Gate){.input_width = 1, .output_width = 1, .logic = in};
        case NOT:
            return (Gate){.input_width = 1, .output_width = 1, .logic = not };
        case AND:
            return (Gate){.input_width = 2, .output_width = 1, .logic = and };
        case OR:
            return (Gate){.input_width = 2, .output_width = 1, .logic = or  };
        case XOR:
            return (Gate){.input_width = 2, .output_width = 1, .logic = xor };
        case NAND:
            return (Gate){.input_width = 2, .output_width = 1, .logic = nand};
        case NOR:
            return (Gate){.input_width = 2, .output_width = 1, .logic = nor };
        case XNOR:
            return (Gate){.input_width = 2, .output_width = 1, .logic = xnor};
    }

    return (Gate){0};
}

void test_truthtables()
{
    TruthTable in_table = generate_truthtable(gate(WIRE));
    print_truthtable(&in_table, "WIRE");
    TruthTable not_table = generate_truthtable(gate(NOT));
    print_truthtable(&not_table, "NOT");
    TruthTable and_table = generate_truthtable(gate(AND));
    print_truthtable(&and_table, "AND");
    TruthTable or_table = generate_truthtable(gate(OR));
    print_truthtable(&or_table, "OR");
    TruthTable xor_table = generate_truthtable(gate(XOR));
    print_truthtable(&xor_table, "XOR");
    TruthTable nand_table = generate_truthtable(gate(NAND));
    print_truthtable(&nand_table, "NAND");
    TruthTable nor_table = generate_truthtable(gate(NOR));
    print_truthtable(&nor_table, "NOR");
    TruthTable xnor_table = generate_truthtable(gate(XNOR));
    print_truthtable(&xnor_table, "XNOR");
    Gate adder_gate = {.input_width = 3, .output_width = 2, .logic = adder};
    TruthTable adder_table = generate_truthtable(adder_gate);
    print_truthtable(&adder_table, "ADDER");
}

int main(void)
{
    return 0;
}
