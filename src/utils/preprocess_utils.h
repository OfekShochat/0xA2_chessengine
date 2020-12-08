#pragma once

#define DBG_LEVEL_HIGH TRUE

#if DEBUG
#if DBG_LEVEL_HIGH
#define DBG_VERBOSE(a) a
#endif
#define DBG_ALWAYS(a) a
#else 
#define DBG_VERBOSE(a)
#define DBG_ALWAYS(a)
#endif