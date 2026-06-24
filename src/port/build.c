// Concrete build-version globals for the console build. Upstream generates this from
// build.c.in via CMake configure_file; the devkitPPC Makefile has no configure step,
// so the values are filled in directly here.
#include <libultraship/libultra.h>

const char gBuildVersion[] = "Ghostship GX (0.0.1)";
const u16 gBuildVersionMajor = 0;
const u16 gBuildVersionMinor = 0;
const u16 gBuildVersionPatch = 1;

const char gGitBranch[] = "gx-port";
const char gGitCommitHash[] = "";
const char gGitCommitTag[] = "";

const char gBuildTeam[] = "libultragx";
const char gBuildDate[] = __DATE__ " " __TIME__;
const char gBuildMakeOption[] = "";
