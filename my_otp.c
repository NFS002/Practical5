 /* System header files*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>

/* User header files */
#include "cmdline.h"
#include "my_otp.h"

/* Macros  - Default buffer size, if reading from stdin */
#define  BSIZE 1024

off_t fsize(const char *filename) {
    struct stat st;
    if (!stat(filename, &st))
        return st.st_size;
    return BSIZE;
}

char* read_input_file(struct gengetopt_args_info args) {
  FILE* fp = NULL;
  off_t file_size = -1;
  if (args.input_filename_given && strcmp("stdin",args.input_filename_arg)) {
    fp = fopen(args.input_filename_arg,"rb");
    if (fp == NULL) {
      perror("Error in opening file");
      /* sends SIGKILL to all */
      exit(-1);
    }
    file_size = fsize(args.input_filename_arg);
  }
  else {
    fp = stdin;
    file_size = BSIZE;
  }
  char* input_buf = malloc(file_size);
  fread(input_buf, 1, file_size, fp);
  input_buf[file_size] = 0;
  fclose(fp);
  if (file_size == -1) exit(-1);
  return input_buf;
}

char* read_keyfile(struct gengetopt_args_info args) {
  FILE* fptr = NULL;
  off_t ksize = fsize(args.keyfile_arg);
  char* key_buf = malloc(ksize);
  fptr = fopen(args.keyfile_arg,"rb");
  if (fptr == NULL) {
    /* sends SIGKILL to all */
    perror("Error in opening keyfile");
    exit(-1);
  }
  fread(key_buf, 1, ksize, fptr);
  fclose(fptr);
  return key_buf;
}

char* xor_encypt(char* input, char* key, long long size) {
  char* msg_enc_in = malloc(size);
  for (int i = 0; i < size; i++)
    msg_enc_in[i] = input[i] ^ key[i];
  return msg_enc_in;
}

void write_bytes_to_file(struct gengetopt_args_info args, char bytes[], long long size) {
  FILE* enc_msg_fptr = NULL;
  if (args.output_filename_given && strcmp("stdout",args.output_filename_arg)) {
    enc_msg_fptr = fopen(args.output_filename_arg,"wb");
    if ((enc_msg_fptr) == NULL) {
      perror("Error in opening ouput file");
      exit(-1);
    }
  }
  else enc_msg_fptr = stdout;
  fwrite(bytes, 1, size, enc_msg_fptr);
  fclose(enc_msg_fptr);
}

void write_to_pipe(int fdes, char bytes[], long long size) {
  ssize_t bytes_written = write(fdes, bytes, size);
  if (bytes_written == -1) {
    perror("Error writing into pipe");
      /* sends SIGKILL to all */
    exit(-1);
  }
  if (bytes_written < size)
    printf("Error occured in writing pipe, only %zd were written\n",bytes_written);
}

char* read_from_pipe(int fdes,long long size) {
  char* msg_enc_out = malloc(size);
  ssize_t bytes_read = read(fdes, msg_enc_out, size);   //read pipe
  if (bytes_read == -1) {
    perror("Error reading from pipe");
    /* sends SIGKILL to all */
    exit(-1);
  }
  if (bytes_read < size)
    printf("Error occured in reading from pipe, only %zd were read\n",bytes_read);
  return msg_enc_out;
}

int main (int argc, char *argv[])
{
    struct                  gengetopt_args_info args;

  if (cmdline_parser (argc, argv, &args) != -1) { /* If correct aguments supplied, continue with program,
                                                     If not, exit. */

    long long               input_buf_size = fsize (args.input_filename_arg),
                            key_size       = fsize (args.keyfile_arg);

                            if (key_size < input_buf_size) {
                              puts("Invalid key size");
                              exit(-1);
                            }

      int                     cpid, cpid_2, fdes[2]; pipe(fdes); cpid = fork();

  if(cpid < 0) { /* Error */

      perror("main 111: fork");

  }

  if (cpid != 0) { /* Parent */

      cpid_2 = fork();

      if(cpid_2 < 0) { /* Error */

      perror("main 116: fork");
    }
    else if (cpid_2 != 0)
    { /* Parent -  wait for both child processes to complete */

      while(wait(NULL) > 0) ;

    }
    else
    {   /* Second child - read from pipe
        Waiting for the first process to wiote to the pipe is not necessary,
        since the call to read blocks until data is available :
        while(kill(cpid, 0) != -1 && errno != ESRCH) ; */

      char* msg_enc_out                     = read_from_pipe(fdes[0],input_buf_size);               /* read pipe */
                                              write_bytes_to_file(args,msg_enc_out,input_buf_size); /* Write bytes */
    }
  }
  else
  {   /* 1st Child - write to pipe */
      char* input_buf                        = read_input_file(args);        /* read input file */
      char* key_buf                          = read_keyfile(args);           /* read keyfile */
      char* msg_enc_in                       = xor_encypt(input_buf,key_buf,input_buf_size); /* Encrypt message */

      write_to_pipe(fdes[1], msg_enc_in, input_buf_size);                    /* write pipe */

  }
  return 0;
  }
}
