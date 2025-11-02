#include "bit_manipulation.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "types.h"
#include "list.h"

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

typedef struct {
    size_t input_width;
    size_t output_width;
    size_t i;
    Logic logic;
} Gate;

LIST_DEF(GateList, Gate);
LIST_DEF(GateWiring, Gate*);
LIST_DEF(GateGraph, GateWiring);

typedef struct {
    u64 *outputs;
    size_t size;
    Gate gate;
} TruthTable;

typedef struct {
    GateList inputs;
    GateList gates;
    GateList outputs;
} Blueprint;

typedef struct {
    Blueprint blueprint;
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

u64 wire(u64 input)
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
    tt.outputs = (u64*)malloc(sizeof(*tt.outputs) * tt.size);

    for (size_t i = 0; i < tt.size; i++) {
        tt.outputs[i] = gate.logic(i);
    }

    return tt;
}

void destroy_truthtable(TruthTable *tt)
{
    free(tt->outputs);
    tt->outputs = NULL;
}

void print_truthtable(TruthTable* tt, const char *name, bool destroy)
{
    printf("'%s' Truth Table:\n", name);
    for (i64 i = tt->size - 1; i >= 0; i--) {
        for (i64 j = tt->gate.input_width - 1; j >= 0; j--) {
            printf("%llu ", BIT_AT(i, j));
            printf("| ");
        }
        printf("=> ");
        for (size_t j = 0; j < tt->gate.output_width; j++) {
            printf("%llu ", BIT_AT(tt->outputs[i], j));
            if (j < tt->gate.output_width - 1) printf("| ");
        }
        printf("\n");
    }

    if (destroy) destroy_truthtable(tt);
}

Gate gate(GateType type)
{
    switch (type) {
        case WIRE:
            return (Gate){.i = 0, .input_width = 1, .output_width = 1, .logic = wire};
        case NOT:
            return (Gate){.i = 0, .input_width = 1, .output_width = 1, .logic = not };
        case AND:
            return (Gate){.i = 0, .input_width = 2, .output_width = 1, .logic = and };
        case OR:
            return (Gate){.i = 0, .input_width = 2, .output_width = 1, .logic = or  };
        case XOR:
            return (Gate){.i = 0, .input_width = 2, .output_width = 1, .logic = xor };
        case NAND:
            return (Gate){.i = 0, .input_width = 2, .output_width = 1, .logic = nand};
        case NOR:
            return (Gate){.i = 0, .input_width = 2, .output_width = 1, .logic = nor };
        case XNOR:
            return (Gate){.i = 0, .input_width = 2, .output_width = 1, .logic = xnor};
    }

    return (Gate){0};
}

Circuit half_adder_circuit()
{
    Circuit half_adder = {0};
    Blueprint blueprint;
    list_push(&blueprint.inputs, gate(WIRE));
    list_push(&blueprint.gates, gate(XOR));
    list_push(&blueprint.gates, gate(AND));
    list_push(&blueprint.outputs, gate(WIRE));

    GateWiring xor_wiring;
    list_push(&xor_wiring, &blueprint.gates.items[0]);
    list_push(&xor_wiring, &blueprint.inputs.items[0]);
    list_push(&xor_wiring, &blueprint.outputs.items[0]);

    GateWiring and_wiring;
    list_push(&xor_wiring, &blueprint.gates.items[1]);
    list_push(&xor_wiring, &blueprint.inputs.items[0]);
    list_push(&xor_wiring, &blueprint.outputs.items[0]);

    list_push(&half_adder.graph, xor_wiring);
    list_push(&half_adder.graph, and_wiring);

    return half_adder;
}

u64 wire_run(GateWiring *wiring, u64 input)
{
    u64 output = 0;


    return output;
}

u64 circuit_run(Circuit *circuit, u64 input)
{
    u64 output = 0;
    size_t output_i = 0;

    for (size_t i = 0; i < circuit->graph.count; i++) {
        GateWiring *wiring = &circuit->graph.items[i];
        for (size_t i = 0; i < wiring->items[0]->input_width; i++) {
            
        }
    }

    return output;
}

void test_truthtables()
{
    TruthTable wire_table = generate_truthtable(gate(WIRE));
    print_truthtable(&wire_table, "WIRE", true);
    TruthTable not_table = generate_truthtable(gate(NOT));
    print_truthtable(&not_table, "NOT", true);
    TruthTable and_table = generate_truthtable(gate(AND));
    print_truthtable(&and_table, "AND", true);
    TruthTable or_table = generate_truthtable(gate(OR));
    print_truthtable(&or_table, "OR", true);
    TruthTable xor_table = generate_truthtable(gate(XOR));
    print_truthtable(&xor_table, "XOR", true);
    TruthTable nand_table = generate_truthtable(gate(NAND));
    print_truthtable(&nand_table, "NAND", true);
    TruthTable nor_table = generate_truthtable(gate(NOR));
    print_truthtable(&nor_table, "NOR", true);
    TruthTable xnor_table = generate_truthtable(gate(XNOR));
    print_truthtable(&xnor_table, "XNOR", true);
    Gate adder_gate = {.input_width = 3, .output_width = 2, .logic = adder};
    TruthTable adder_table = generate_truthtable(adder_gate);
    print_truthtable(&adder_table, "ADDER", true);
}

int main(void)
{
    test_truthtables();

    return 0;
}
