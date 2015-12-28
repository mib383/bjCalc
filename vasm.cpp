#include "vasm.h"

namespace vasm{

#define X(name) #name,
char* cmd_names[]
{
    CMD_LIST
};
#undef X

}
