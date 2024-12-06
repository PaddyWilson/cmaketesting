#include "UUIDGen.h"

uint64_t UUID::Generate()
{
	return uniformDistribution(s_Engine);
}
