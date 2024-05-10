#ifndef SHA256_H
# define SHA256_H

# include <stdint.h>
# include <stdbool.h>
# include <string.h>
# include <stdlib.h>

void sha256_string(char *string, uint8_t *result);
bool convert_hash_to_string(uint8_t *hash, int hash_length, char **buffer);

typedef struct {
    uint8_t input[64];
	uint32_t datalen;
	unsigned long long bitlen;
	uint32_t buffer[8];
	uint8_t digest[32];
}	_sha256_context_t;

#endif