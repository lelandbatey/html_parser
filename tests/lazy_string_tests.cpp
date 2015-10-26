#include "../include/dbg.hpp"
#include "../include/minunit.hpp"

// Due to how the makefile for this project works, each test is compiled
// against all the objects that have a test of some kind. So if there are
// object files for `foo`, `bar`, and `baz`, but there are only tests for `bar`
// and `baz`, then the tests executables for `bar` and `baz` will only be
// compiled agains `bar.o` and `baz.o`. However, there may be functionality in
// `foo` that `baz` needs in order to run correctly. That means when attempting
// to create the `baz_test` executable, because `baz_test` is only linked
// against `bar.o` and `baz.o`, the functionality that `baz.o` depends on that
// is in `foo.o` will not be defined, and the linking process will fail.
//
// To work around this, we create a dummy `foo_test.cpp` file, so now all tests
// will be linked against `foo.o`, `bar.o`, and `baz.o`. 

const char* dummy(){
	return NULL;
}

const char* all_tests(){
	mu_suite_start();

	mu_run_test(dummy);
	return NULL;
}

RUN_TESTS(all_tests);
