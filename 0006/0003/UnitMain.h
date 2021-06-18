//---------------------------------------------------------------------------

#ifndef UnitMainH
#define UnitMainH

#include "csp.h"
//---------------------------------------------------------------------------
using __MSG = int;    // data

using CHAN = csp::chan<std::wstring, __MSG>;
using CHAN_PTR = std::shared_ptr<CHAN>;

using MUX = csp::mux<std::wstring, __MSG>;
using MUX_PTR = std::shared_ptr<MUX>;

using FORK = csp::fork<std::wstring, __MSG>;
using FORK_PTR = std::shared_ptr<FORK>;

using SINK = csp::sink<std::wstring, __MSG>;
using SINK_PTR = std::shared_ptr<SINK>;

using MSG = CHAN::MSG;
//---------------------------------------------------------------------------
const int N = 4;			// Dimension
//---------------------------------------------------------------------------
// Main functions of system processes
void doP(int pid, int& x, CHAN_PTR in, CHAN_PTR out);
void doQ(int pid, int& x, CHAN_PTR in, CHAN_PTR out);
//---------------------------------------------------------------------------
#endif

