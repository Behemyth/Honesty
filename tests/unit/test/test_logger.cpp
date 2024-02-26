import std;
import synodic.honesty.test;

using namespace synodic::honesty::test;

static_assert(logger_type<logger::Console>, "The console logger must satisfy the logger concept");
