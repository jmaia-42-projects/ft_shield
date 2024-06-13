#ifndef WOODY_H
# define WOODY_H

# include <elf.h>
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/mman.h>
# include <unistd.h>

# define WOODY_TMP_FILE "/tmp/ft_shield_new_init"
# define SIGNATURE_SIZE 3

typedef enum e_file_type
{
	NONE,
	ELF64,
	ELF32,
}	t_file_type;

typedef struct s_file_elf64
{
	Elf64_Ehdr	*header;
	Elf64_Shdr	*sections;
	Elf64_Phdr	*programs;
}	t_file_elf64;

typedef struct s_file
{
	t_file_type	type;
	int			fd;
	void		*map;
	off_t		size;
	void		*specific_file;
}	t_file;

typedef struct s_injection
{
	int 	fd;
	char	*file_map;
	off_t	file_size;
	off_t	signature_offset;
	int		signature_segment_size;
	size_t	old_entrypoint;
	size_t 	new_entrypoint;
	off_t	payload_offset;
}	t_injection;

bool woody(char *filename);

bool get_file_type(char *name, t_file *file);
void close_file(t_file file);
bool get_specific_file(t_file *file);

bool parse_elf64(int fd, t_file *file);
void inject(t_injection injection);

bool prepare_injection(t_file file, t_injection *injection);
bool prepare_injection_elf64(t_file file, t_injection *injection);
void end_injection(t_injection injection);

Elf64_Phdr *find_code_cave_elf64(t_file_elf64 file_elf64, size_t payload_size);
size_t use_code_cave_elf64(Elf64_Ehdr *header, Elf64_Phdr *code_cave_header, size_t payload_size, t_injection *injection);
size_t get_payload_length();

bool check_signature_present(t_injection injection);

#endif
