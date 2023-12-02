import synodic.honesty.test;

static_assert(synodic::honesty::expect(true));
static_assert(not synodic::honesty::expect(false));
