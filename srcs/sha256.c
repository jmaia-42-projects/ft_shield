#include "sha256.h"

#define A 0x6a09e667
#define B 0xbb67ae85
#define C 0x3c6ef372
#define D 0xa54ff53a
#define E 0x510e527f
#define F 0x9b05688c
#define G 0x1f83d9ab
#define H 0x5be0cd19

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (rotateRight(x,2) ^ rotateRight(x,13) ^ rotateRight(x,22))
#define EP1(x) (rotateRight(x,6) ^ rotateRight(x,11) ^ rotateRight(x,25))
#define SIG0(x) (rotateRight(x,7) ^ rotateRight(x,18) ^ ((x) >> 3))
#define SIG1(x) (rotateRight(x,17) ^ rotateRight(x,19) ^ ((x) >> 10))

static const uint32_t k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

static uint32_t rotateRight(uint32_t x, uint32_t n){
	return (x >> n) | (x << (32 - n));
}

void _sha256_init(_sha256_context_t *context)
{
	context->datalen = 0;
	context->bitlen = 0;

	context->buffer[0] = A;
	context->buffer[1] = B;
	context->buffer[2] = C;
	context->buffer[3] = D;
	context->buffer[4] = E;
	context->buffer[5] = F;
	context->buffer[6] = G;
	context->buffer[7] = H;
}

static void sha256_do_a_round(_sha256_context_t *context, uint8_t *input)
{
	uint32_t a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (input[j] << 24) | (input[j + 1] << 16) | (input[j + 2] << 8) | (input[j + 3]);
	for ( ; i < 64; ++i)
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

	a = context->buffer[0];
	b = context->buffer[1];
	c = context->buffer[2];
	d = context->buffer[3];
	e = context->buffer[4];
	f = context->buffer[5];
	g = context->buffer[6];
	h = context->buffer[7];

	for (i = 0; i < 64; ++i) {
		t1 = h + EP1(e) + CH(e,f,g) + k[i] + m[i];
		t2 = EP0(a) + MAJ(a,b,c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	context->buffer[0] += a;
	context->buffer[1] += b;
	context->buffer[2] += c;
	context->buffer[3] += d;
	context->buffer[4] += e;
	context->buffer[5] += f;
	context->buffer[6] += g;
	context->buffer[7] += h;
}

static void _sha256_fill_and_run_if_complete(_sha256_context_t *context, uint8_t *input, uint64_t input_len)
{
	uint64_t i;

	for (i = 0; i < input_len; ++i) {
		context->input[context->datalen] = input[i];
		context->datalen++;
		if (context->datalen == 64) {
			sha256_do_a_round(context, context->input);
			context->bitlen += 512;
			context->datalen = 0;
		}
	}
}

static void _sha256_padd_and_finalize(_sha256_context_t *context)
{
	uint32_t i;

	i = context->datalen;

	if (context->datalen < 56) {
		context->input[i++] = 0x80;
		while (i < 56)
			context->input[i++] = 0x00;
	}
	else {
		context->input[i++] = 0x80;
		while (i < 64)
			context->input[i++] = 0x00;
		sha256_do_a_round(context, context->input);
		memset(context->input, 0, 56);
	}

	context->bitlen += context->datalen * 8;
	context->input[63] = context->bitlen;
	context->input[62] = context->bitlen >> 8;
	context->input[61] = context->bitlen >> 16;
	context->input[60] = context->bitlen >> 24;
	context->input[59] = context->bitlen >> 32;
	context->input[58] = context->bitlen >> 40;
	context->input[57] = context->bitlen >> 48;
	context->input[56] = context->bitlen >> 56;
	sha256_do_a_round(context, context->input);

	for (i = 0; i < 4; ++i) {
		context->digest[i]      = (context->buffer[0] >> (24 - i * 8)) & 0x000000ff;
		context->digest[i + 4]  = (context->buffer[1] >> (24 - i * 8)) & 0x000000ff;
		context->digest[i + 8]  = (context->buffer[2] >> (24 - i * 8)) & 0x000000ff;
		context->digest[i + 12] = (context->buffer[3] >> (24 - i * 8)) & 0x000000ff;
		context->digest[i + 16] = (context->buffer[4] >> (24 - i * 8)) & 0x000000ff;
		context->digest[i + 20] = (context->buffer[5] >> (24 - i * 8)) & 0x000000ff;
		context->digest[i + 24] = (context->buffer[6] >> (24 - i * 8)) & 0x000000ff;
		context->digest[i + 28] = (context->buffer[7] >> (24 - i * 8)) & 0x000000ff;
	}

}

void sha256_string(char *string, uint8_t *result)
{
	_sha256_context_t context;

	_sha256_init(&context);
	_sha256_fill_and_run_if_complete(&context, (uint8_t *)string, strlen(string));
	_sha256_padd_and_finalize(&context);

	memcpy(result, context.digest, 32);	
}

bool convert_hash_to_string(uint8_t *hash, int hash_length, char **buffer)
{
	*buffer = malloc(hash_length * 2 + 1);
	if (!*buffer)
		return (false);
	for (int i = 0; i < hash_length; i++)
	{
		(*buffer)[i * 2] = "0123456789abcdef"[hash[i] / 16];
		(*buffer)[i * 2 + 1] = "0123456789abcdef"[hash[i] % 16];
	}
	(*buffer)[hash_length * 2] = '\0';
	return (true);
}