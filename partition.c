#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "partition.h"

struct gcm_info
{
    unsigned char *id;
    unsigned int disc_number;
};


/*
 * The magic number starts at byte 26
 */
bool CheckGCM(unsigned char buffer[])
{
	unsigned char magic_word[] = {0xC2, 0x33, 0x9F, 0x3D};
	return memcmp(buffer + 26, magic_word, 4) == 0;
}

/*
 * expects bytes 0-7 of the iso
 *
 * The first 6 bytes of the iso contain the disc id
 * The next byte is the disc number
 */
struct gcm_info * getGcmInfo(unsigned int buffer[])
{
	struct gcm_info *gcmInfo = malloc(sizeof(struct gcm_info));

	// the disc id comes from bytes 0 through 5
    unsigned char *id = malloc(sizeof(*id) * 6);
    memcpy(id, (char*)&buffer, 6);
	gcmInfo->id = id;

	// the disc number comes from byte 6 and 7
	gcmInfo->disc_number = buffer[6] + (buffer[7] << 8);
 	return gcmInfo;
}

bool IsUniform(unsigned char data[], int offset, int size, unsigned char value) {
    for (int i = 0; i < size; i++) {
        if (data[i + offset] != value) {
            return false;
        }
    }
    return true;
}