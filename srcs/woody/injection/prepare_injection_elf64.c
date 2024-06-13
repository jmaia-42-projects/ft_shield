/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_injection_elf64.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damien <damien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 14:58:19 by dhubleur          #+#    #+#             */
/*   Updated: 2024/06/14 13:14:36 by damien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

const char signature[SIGNATURE_SIZE] = {0x10, 0x10, 0x10};

char *get_section_name(Elf64_Ehdr *header, Elf64_Shdr *section_headers, int index)
{
	Elf64_Shdr *section_header = &section_headers[index];
	Elf64_Shdr *section_names = &section_headers[header->e_shstrndx];
	return ((char *)header + section_names->sh_offset + section_header->sh_name);
}

Elf64_Shdr *get_section(char *name, Elf64_Ehdr *header, Elf64_Shdr *section_headers)
{
	int i;

	i = 0;
	while (i < header->e_shnum - 1)
	{
		if (strcmp(name, get_section_name(header, section_headers, i)) == 0)
			return (&section_headers[i]);
		i++;
	}
	return (NULL);
}

bool	prepare_injection_elf64(t_file file, t_injection *injection)
{
	t_file_elf64 file_efl64 = *((t_file_elf64 *)file.specific_file);
	Elf64_Phdr *code_cave = find_code_cave_elf64(file_efl64, get_payload_length());
	injection->old_entrypoint = file_efl64.header->e_entry;
	if (code_cave == NULL)
		return false;
	injection->fd = open(WOODY_TMP_FILE, O_CREAT | O_RDWR | O_TRUNC, 0755);
	if (injection->fd == -1)
		return false;
	injection->file_size = file.size;
	if (lseek(injection->fd, injection->file_size-1, SEEK_SET) == -1)
		return false;
	write(injection->fd, "", 1);
	injection->file_map = mmap(NULL, injection->file_size,  PROT_WRITE, MAP_SHARED, injection->fd, 0);
	if (injection->file_map == MAP_FAILED)
		return false;
	memcpy(injection->file_map, file.map, file.size);
	t_file_elf64 output_file;
	output_file.header = (Elf64_Ehdr *)injection->file_map;
	output_file.sections = (Elf64_Shdr *)(injection->file_map + output_file.header->e_shoff);
	output_file.programs = (Elf64_Phdr *)(injection->file_map + output_file.header->e_phoff);

	code_cave = find_code_cave_elf64(output_file, get_payload_length());
	injection->payload_offset = use_code_cave_elf64(output_file.header, code_cave, get_payload_length(), injection);

	Elf64_Phdr *first_load_segment = NULL;
	for (int i = 0; i < output_file.header->e_phnum; i++)
	{
		if (output_file.programs[i].p_type == PT_LOAD)
		{
			first_load_segment = &output_file.programs[i];
			break;
		}
	}
	injection->signature_offset = first_load_segment->p_offset;
	injection->signature_segment_size = first_load_segment->p_memsz;

	return true;
}

bool check_signature_present(t_injection injection) {
	char *after_section = (char *)injection.file_map + injection.signature_offset + injection.signature_segment_size;
	for (int i = 0; i < SIGNATURE_SIZE; i++)
	{
		if (after_section[i] != signature[i])
			return false;
	}
	return true;
}