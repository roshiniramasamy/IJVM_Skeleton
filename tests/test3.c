#include <stdio.h>
#include "ijvm.h"
#include "testutil.h"

void test_goto1()
{
    int res = init_ijvm("files/task3/GOTO1.ijvm");
    assert(res != -1);

    set_null_output();

    step();
    assert(tos() == 0x31);
    step();
    step();
    step();
    assert(tos() == 0x33);
    step();
    destroy_ijvm();
}

void test_goto2()
{
    int res = init_ijvm("files/task3/GOTO2.ijvm");
    assert(res != -1);

    set_null_output();

    step();
    assert(tos() == 0x31);
    step();
    step();
    step();
    assert(tos() == 0x33);
    step();
    step();
    step();
    assert(tos() == 0x32);
    step();
    destroy_ijvm();
}

void test_ifeq1()
{
    int res = init_ijvm("files/task3/IFEQ1.ijvm");
    assert(res != -1);
    set_null_output();

    int startpc = 0;

    // L1
    step();
    assert(tos() == 0x5);

    // L2 iteration 1
    steps(6);
    assert(tos() == 0x4);
    steps(3);
    startpc = get_program_counter();

    // L2 iteration 2
    steps(6);
    assert(tos() == 0x3);
    steps(3);
    assert(startpc == get_program_counter());

    // L2 iteration 3
    steps(6);
    assert(tos() == 0x2);
    steps(3);
    assert(startpc == get_program_counter());

    // L2 iteration 4
    steps(6);
    assert(tos() == 0x1);
    steps(3);
    assert(startpc == get_program_counter());

    // L2 iteration 5
    steps(6);
    assert(tos() == 0x0);
    steps(2);

    // Should have branched to L3
    assert(startpc + 15 == get_program_counter());

    destroy_ijvm();
}

void test_iflt1()
{
    int res = init_ijvm("files/task3/IFLT1.ijvm");
    assert(res != -1);

    set_null_output();


    int oldpc = get_program_counter();

    // L1
    steps(2);
    assert(oldpc + 5 == get_program_counter());
    oldpc = get_program_counter();

    // L2
    steps(2);
    assert(oldpc + 5 == get_program_counter());
    oldpc = get_program_counter();

    // L3
    steps(2);
    assert(oldpc + 5 == get_program_counter());
    oldpc = get_program_counter();

    // L4
    steps(4);
    assert(oldpc + 19 == get_program_counter());
    steps(1);
    assert(tos() == 0x37);
    destroy_ijvm();
}

void test_ificmpeq1()
{
    int res = init_ijvm("files/task3/IFICMPEQ1.ijvm");
    assert(res != -1);

    set_null_output();

    int oldpc = get_program_counter();

    // L1
    steps(3);
    assert(oldpc + 7 == get_program_counter());
    oldpc = get_program_counter();

    // L2
    steps(5);
    assert(oldpc + 10 == get_program_counter());
    oldpc = get_program_counter();

    // L3
    steps(4);
    assert(oldpc + 8 == get_program_counter());
    oldpc = get_program_counter();

    // L4
    for (int i = 0; i < 3; i++) {
        steps(3);
        assert(oldpc == get_program_counter());
        oldpc = get_program_counter();
    }

    // L6
    steps(2);
    assert(oldpc + 9 == get_program_counter());
    steps(2);
    assert(tos() == 0x13);
    destroy_ijvm();
}

int main()
{
    RUN_TEST(test_goto1);
    RUN_TEST(test_goto2);
    RUN_TEST(test_ifeq1);
    RUN_TEST(test_iflt1);
    RUN_TEST(test_ificmpeq1);
    return END_TEST();
}
