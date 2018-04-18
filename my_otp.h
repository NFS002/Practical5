off_t fsize(const char *filename);
char* read_keyfile(struct gengetopt_args_info args);
char* read_input_file(struct gengetopt_args_info args);
char* xor_encypt(char* input, char* key, long long size);
void write_bytes_to_file(struct gengetopt_args_info args, char bytes[], long long size);
char* read_from_pipe(int fdes,long long size);
void write_to_pipe(int fdes, char bytes[], long long size);
