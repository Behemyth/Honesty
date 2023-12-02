import std;
import synodic.honesty.test;

using namespace synodic::honesty;
using namespace synodic::honesty::literals;

// TODO: Exception testing
/*
 *"exceptions/aborts"_test = [] {
expect(throws<std::runtime_error>([] { throw std::runtime_error{""}; }))
<< "throws runtime_error";
expect(throws([] { throw 0; })) << "throws any exception";
expect(nothrow([]{})) << "doesn't throw";
expect(aborts([] { assert(false); }));
};
 */
