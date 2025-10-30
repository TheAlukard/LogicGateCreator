#include "bit_manipulation.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

typedef struct GateDesc GateDesc;
typedef struct TruthTable TruthTable;

typedef uint64_t (*Logic)(uint64_t);

typedef enum {
    IN,
    NOT,
    AND,
    OR,
    XOR,
    NAND,
    NOR,
    XNOR,
} GateType;

struct GateDesc {
    size_t input_width;
    size_t output_width;
    Logic logic;
};

struct TruthTable {
    uint64_t (*rows)[2];
    size_t size;
    GateDesc gate;
};

typedef struct {
    int i;
} Circuit;

uint64_t not(uint64_t input)
{
    return ~BIT_AT(input, 0);
}

uint64_t and(uint64_t input)
{
    return BIT_AT(input, 0) & BIT_AT(input, 1);
}

uint64_t or(uint64_t input)
{
    return BIT_AT(input, 0) | BIT_AT(input, 1);
}

uint64_t xor(uint64_t input)
{
    return BIT_AT(input, 0) ^ BIT_AT(input, 1);
}

uint64_t nand(uint64_t input)
{
    return ~(BIT_AT(input, 0) & BIT_AT(input, 1));
}

uint64_t nor(uint64_t input)
{
    return ~(BIT_AT(input, 0) | BIT_AT(input, 1));
}

uint64_t xnor(uint64_t input)
{
    return ~(BIT_AT(input, 0) ^ BIT_AT(input, 1));
}

uint64_t in(uint64_t input)
{
    return BIT_AT(input, 0);
}

uint64_t adder(uint64_t input)
{
    uint64_t a = BIT_AT(input, 0);
    uint64_t b = BIT_AT(input, 1);
    uint64_t cin = BIT_AT(input, 2);
    uint64_t output = 0;
    uint64_t add1 = a ^ b;
    uint64_t add2 = add1 ^ cin;
    uint64_t cout = (a & b) | (cin & add1);
    BIT_SET(output, 0, add2);
    BIT_SET(output, 1, cout);
    return output;
}

TruthTable generate_truthtable(GateDesc gate)
{
    TruthTable tt = {0};
    tt.gate = gate;
    tt.size = pow(2, gate.input_width);
    tt.rows = malloc(sizeof(*tt.rows) * tt.size);
    uint64_t start = tt.size - 1;

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

GateDesc gate(GateType type)
{
    switch (type) {
        case IN:
            return (GateDesc){.input_width = 1, .output_width = 1, .logic = in};
        case NOT:
            return (GateDesc){.input_width = 1, .output_width = 1, .logic = not };
        case AND:
            return (GateDesc){.input_width = 2, .output_width = 1, .logic = and };
        case OR:
            return (GateDesc){.input_width = 2, .output_width = 1, .logic = or  };
        case XOR:
            return (GateDesc){.input_width = 2, .output_width = 1, .logic = xor };
        case NAND:
            return (GateDesc){.input_width = 2, .output_width = 1, .logic = nand};
        case NOR:
            return (GateDesc){.input_width = 2, .output_width = 1, .logic = nor };
        case XNOR:
            return (GateDesc){.input_width = 2, .output_width = 1, .logic = xnor};
    }

    return (GateDesc){0};
}

void test_truthtables()
{
    TruthTable in_table = generate_truthtable(gate(IN));
    print_truthtable(&in_table, "IN");
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
    GateDesc adder_gate = {.input_width = 3, .output_width = 2, .logic = adder};
    TruthTable adder_table = generate_truthtable(adder_gate);
    print_truthtable(&adder_table, "ADDER");
}

int main(void)
{

    return 0;
}
