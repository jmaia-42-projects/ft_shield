/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_injection_elf64.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 14:58:19 by dhubleur          #+#    #+#             */
/*   Updated: 2024/06/12 22:32:48 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

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
	{
		printf("%s:%d\n", __FILE__, __LINE__);
		return false;
	}
	injection->fd = open(WOODY_TMP_FILE, O_CREAT | O_RDWR | O_TRUNC, 0755);
	if (injection->fd == -1)
	{
		printf("%s:%d\n", __FILE__, __LINE__);
		perror("salut");
		printf("hey\n");
		return false;
	}
	injection->file_size = file.size;
	if (lseek(injection->fd, injection->file_size-1, SEEK_SET) == -1)
	{
		printf("%s:%d\n", __FILE__, __LINE__);
		return false;
	}
	write(injection->fd, "", 1);
	injection->file_map = mmap(NULL, injection->file_size,  PROT_WRITE, MAP_SHARED, injection->fd, 0);
	if (injection->file_map == MAP_FAILED)
	{
		printf("%s:%d\n", __FILE__, __LINE__);
		return false;
	}
	memcpy(injection->file_map, file.map, file.size);
	t_file_elf64 output_file;
	output_file.header = (Elf64_Ehdr *)injection->file_map;
	output_file.sections = (Elf64_Shdr *)(injection->file_map + output_file.header->e_shoff);
	output_file.programs = (Elf64_Phdr *)(injection->file_map + output_file.header->e_phoff);

	code_cave = find_code_cave_elf64(output_file, get_payload_length());
	injection->payload_offset = use_code_cave_elf64(output_file.header, code_cave, get_payload_length(), injection);

	Elf64_Shdr *text_section = get_section(".text", output_file.header, output_file.sections);
	if (text_section == NULL)
	{
		printf("%s:%d\n", __FILE__, __LINE__);
		return false;
	}
	text_section->sh_flags |= SHF_WRITE;
	injection->encrypt_offset = text_section->sh_offset;
	injection->encrypt_size = text_section->sh_size;

	Elf64_Phdr *text_segment = NULL;
	for (size_t i = 0; i < output_file.header->e_phnum; i++)
	{
		if (output_file.programs[i].p_offset <= text_section->sh_offset && output_file.programs[i].p_offset + output_file.programs[i].p_filesz >= text_section->sh_offset + text_section->sh_size)
		{
			text_segment = &(output_file.programs[i]);
			break ;
		}
	}
	if (text_segment != NULL) {
		text_segment->p_flags |= PF_W | PF_R;
	}

	return true;
}
