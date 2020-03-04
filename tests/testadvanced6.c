#include <stdio.h>
#include <string.h>
#include "ijvm.h"
#include "testutil.h"

void run_calc_inp(char *input, char *expected)
{
    int res = init_ijvm("files/advanced/SimpleCalc.ijvm");
    assert(res != -1);

    char buf[128];
    FILE *f = tmpfile();
    fprintf(f, "%s", input);
    rewind(f);
    set_input(f);

    FILE *out_file = tmpfile();
    set_output(out_file);

    // Run program
    run();

    rewind(out_file);
    memset(buf, '\0', 128);
    fread(buf, 1, 127, out_file);

    // Compare output
    assert(strncmp(buf, expected, strlen(expected)) == 0);

    destroy_ijvm();

}

void test_rec_1()
{
    run_calc_inp("2!?.", "2\n");
}

void test_rec_2()
{
    run_calc_inp("7!?.", "5040\n");
}

void test_rec_3()
{
    run_calc_inp("8!?.", "40320\n");
}

int main()
{
    RUN_TEST(test_rec_1);
    RUN_TEST(test_rec_2);
    RUN_TEST(test_rec_3);
    return END_TEST();
}
