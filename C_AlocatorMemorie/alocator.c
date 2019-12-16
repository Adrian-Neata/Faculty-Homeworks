//Neata Adrian
//311CB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint8_t* memory;
uint32_t memory_size;
uint32_t nr_blocks;
uint32_t first = 0;
uint32_t last = 0;
uint32_t block_position = 0;

int hextodec(uint32_t i)
{
	return memory[i] + memory[i + 1] * 256 + memory[i + 2] * 65536 + memory[i + 3] * 4294967296;
}
void dectohex(uint32_t i, uint32_t n)
{
	memory[i] = n % 4294967296 % 65536 % 256;
	memory[i + 1] = n % 4294967296 % 65536 / 256;
	memory[i + 2] = n % 4294967296 / 65536;
	memory[i + 3] = n / 4294967296;
}

void DUMP()
{
	uint32_t i = 0;
	for(i = 0; i < memory_size; i++)
	{
		if(i % 8 == 0 && i % 16 != 0)
			printf(" ");
		else if(i % 16 == 0)
			printf("%08X\t", i);
		if(i % 16 != 15 && i != memory_size - 1)
			printf("%02X ", memory[i]);
		if(i % 16 == 15 || i == memory_size - 1)
			printf("%02X\n", memory[i]);
	}
}

void FREE(uint32_t index)
{
	uint32_t size, i;
	index = index - 12;
	size = hextodec(index + 8);
	if(index != first)
		dectohex(hextodec(index + 4), hextodec(index));
	else first = hextodec(index);
	if(index != last)
		dectohex(hextodec(index) + 4, hextodec(index + 4));
	else last = hextodec(index + 4);
	for(i = index; i < index + size + 12; i++)
		memory[i] = 0;
	nr_blocks--;
}
void ALLOC(uint32_t size)
{
	uint32_t pos1, pos2, pos3, size1, ok = 0;
	if(first >= size + 12 || (!nr_blocks && memory_size >= size + 12))
	{
		dectohex(0, first);
		dectohex(8, size);
		first = 0;
		printf("12\n");
		nr_blocks++;
		block_position = 12;
		ok = 1;
	}
	//case first block or empty space at the start
	if(!ok && hextodec(first))
	{
		pos1 = first;
		size1 = hextodec(first + 8);
		pos2 = hextodec(pos1);
		while(pos2 - pos1 - size1 - 12 < size + 12 && hextodec(pos2))
		{
			pos1 = pos2;
			size1 = hextodec(pos2 + 8);
			pos2 = hextodec(pos2);
		}
		if(pos2 - pos1 - size1 - 12 >= size + 12)
		{
			pos3 = pos1 + size1 + 12;
			dectohex(pos3, pos2);
			dectohex(pos3 + 4, pos1);
			dectohex(pos3 + 8, size);
			dectohex(pos1, pos3);
			dectohex(pos2 + 4, pos3);
			nr_blocks++;
			printf("%u\n", pos3 + 12);
			block_position = pos3 + 12;
			ok = 1;
		}
	}
	//case enough space between blocks
	if(memory_size - last - 12 - hextodec(last + 8) >= size + 12 && !ok)
	{
		pos3 = last + hextodec(last + 8) + 12;
		dectohex(pos3 + 4, last);
		dectohex(pos3 + 8, size);
		dectohex(last, pos3);
		printf("%u\n", pos3 + 12);
		block_position = pos3 + 12;
		last = pos3;
		ok = 1;
		nr_blocks++;
	}
	//case at the end of the block
	if(!ok) 
	{
		printf("0\n");
		block_position = 0;
	}
}

void FILL(uint32_t index, uint32_t size, uint32_t value)
{
	uint32_t ok = 1, j, size_block;
	index = index - 12;
	while(ok)
	{
		if(!hextodec(index) || !size) 
			ok = 0;
		size_block = hextodec(index + 8);
		j = index + 12;
		while(j < index + 12 + size_block && size)
		{
			size--;
			memory[j] = value;
			j++;
		}
		index = hextodec(index);
	}
}

void ALLOCALIGNED(uint32_t size, uint32_t align)
{
	uint32_t pos1, pos2, pos3, size1, ok = 0, n = 1;
	while(n * align < 12)
		n++;
	if(first >= n * align + size || (!nr_blocks && memory_size >= n * align + size))
	{
		dectohex(n * align - 12, first);
		dectohex(n * align - 4, size);
		first = n * align - 12;
		printf("%d\n", n * align);
		if(!nr_blocks)
			last = n * align - 12;
		nr_blocks++;
		ok = 1;
	}
	//case first block or empty space at the start
	if(!ok && hextodec(first))
	{
		n = 0;
		pos1 = first;
		size1 = hextodec(first + 8);
		pos2 = hextodec(pos1);
		while(n * align < pos1 + size1 + 24)
			n++;
		while((int)(pos2 - n * align) < (int)(size) && hextodec(pos2))
		{
			pos1 = pos2;
			size1 = hextodec(pos2 + 8);
			pos2 = hextodec(pos2);
			while(n * align < pos1 + size1 + 24)
				n++;
		}
		if((int)(pos2 - n * align) >= (int)(size))
		{
			pos3 = n * align - 12;
			dectohex(pos3, pos2);
			dectohex(pos3 + 4, pos1);
			dectohex(pos3 + 8, size);
			dectohex(pos1, pos3);
			dectohex(pos2 + 4, pos3);
			nr_blocks++;
			printf("%u\n", pos3 + 12);
			ok = 1;
		}
	}
	//case enough space between blocks
	n = 1;
	while(n * align < last + hextodec(last + 8) + 24)
		n++;
	if((int)(memory_size - n * align) >= (int)(size) && !ok)
	{
		pos3 = n * align - 12;
		dectohex(pos3 + 4, last);
		dectohex(pos3 + 8, size);
		dectohex(last, pos3);
		printf("%u\n", pos3 + 12);
		last = pos3;
		ok = 1;
		nr_blocks++;
	}
	//case at the end of the block
	if(!ok) 
	{
		printf("0\n");
		block_position = 0;
	}
}

void REALLOC(uint32_t index, uint32_t size)
{
	uint8_t* block;
	uint32_t i, block_size;
	block_size = hextodec(index - 4);
	block = malloc(size * sizeof(unsigned char));
	for(i = 0; i < size; i++)
		block[i] = 0;
	i = 0;
	while(i < size && i < block_size)
	{
		block[i] = memory[index + i];
		i++;
	}
	FREE(index);
	ALLOC(size);
	if(block_position != 0)
	{
		i = 0;
		while(i < size && i < block_size)
		{
			memory[block_position + i] = block[i];
			i++;
		}
	}
	free(block);
}

void parse_command(char* cmd)
{
	const char* delims = " \n";

	char* cmd_name = strtok(cmd, delims);
	if (!cmd_name) {
		goto invalid_command;
	}

	if (strcmp(cmd_name, "INITIALIZE") == 0) {
		char* size_str = strtok(NULL, delims);
		if (!size_str) {
			goto invalid_command;
		}
		memory_size = atoi(size_str);
		uint32_t i;
		nr_blocks = 0;
		memory = malloc(memory_size * sizeof(unsigned char));
		for(i = 0; i < memory_size; i++)
			memory[i] = 0;
	} else if (strcmp(cmd_name, "FINALIZE") == 0) {
		free(memory);
		last = 0;
		first = 0;
		nr_blocks = 0;

	} else if (strcmp(cmd_name, "DUMP") == 0) {
		DUMP();

	} else if (strcmp(cmd_name, "ALLOC") == 0) {
		char* size_str = strtok(NULL, delims);
		if (!size_str) {
			goto invalid_command;
		}
		uint32_t size = atoi(size_str);
		if(size + 12 > memory_size) printf("0\n");
		else ALLOC(size);

	} else if (strcmp(cmd_name, "FREE") == 0) {
		char* index_str = strtok(NULL, delims);
		if (!index_str) {
			goto invalid_command;
		}
		uint32_t index = atoi(index_str);
		FREE(index);

	} else if (strcmp(cmd_name, "FILL") == 0) {
		char* index_str = strtok(NULL, delims);
		if (!index_str) {
			goto invalid_command;
		}
		uint32_t index = atoi(index_str);
		char* size_str = strtok(NULL, delims);
		if (!size_str) {
			goto invalid_command;
		}
		uint32_t size = atoi(size_str);
		char* value_str = strtok(NULL, delims);
		if (!value_str) {
			goto invalid_command;
		}
		uint32_t value = atoi(value_str);
		FILL(index, size, value);

	} else if (strcmp(cmd_name, "ALLOCALIGNED") == 0) {
		char* size_str = strtok(NULL, delims);
		if (!size_str) {
			goto invalid_command;
		}
		uint32_t size = atoi(size_str);
		char* align_str = strtok(NULL, delims);
		if (!align_str) {
			goto invalid_command;
		}
		uint32_t align = atoi(align_str);
		ALLOCALIGNED(size, align);

	} else if (strcmp(cmd_name, "REALLOC") == 0) {
		char* index_str = strtok(NULL, delims);
		if (!index_str) {
			goto invalid_command;
		}
		uint32_t index = atoi(index_str);
		char* size_str = strtok(NULL, delims);
		if (!size_str) {
			goto invalid_command;
		}
		uint32_t size = atoi(size_str);
		REALLOC(index, size);

	} else {
		goto invalid_command;
	}

	return;

invalid_command:
	printf("Invalid command: %s\n", cmd);
	free(memory);
	exit(1);
}


int main(void)
{
	ssize_t read;
	char* line = NULL;
	size_t len;
	/* parse input line by line */
	while ((read = getline(&line, &len, stdin)) != -1) {
		printf("%s", line);
		parse_command(line);
	}

	free(line);
	return 0;
}
