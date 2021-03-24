#include <esppl_functions.h>

bool maccmp (uint8_t * mac1, uint8_t * mac2)
{
	for (int i = 0; i < ESPPL_MAC_LEN; i++)
	{
		if (mac1[i] != mac2[i])
		{
			return false;
		}
	}
	return true;
}

void maccpy (uint8_t * macS, uint8_t * macD)
{
	for (int i = 0; i < ESPPL_MAC_LEN; i++)
		macD[i] = macS[i];
}