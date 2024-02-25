import std;
import synodic.honesty.test.logger;

using namespace synodic::honesty;

static_assert(logger_type<logger::Console>, "The console logger must satisfy the logger concept");
