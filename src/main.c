#include "debug.h"
#include "utf.h"
#include "wrappers.h"
#include <stdlib.h>


int
main(int argc, char *argv[])
{
  int infile, outfile, in_flags, out_flags;
  FILE *file;

  parse_args(argc, argv);
  check_bom();

  print_state();
  in_flags = O_RDONLY;
  out_flags = O_WRONLY | O_CREAT;

  if( (file = fopen(program_state->out_file, "w"))){
    remove(program_state->out_file);
    fclose(file);
  }
  infile = Open(program_state->in_file, in_flags);
  outfile = Open(program_state->out_file, out_flags);

  //lseek(SEEK_SET, program_state->bom_length, infile);
  lseek(infile, program_state->bom_length, SEEK_SET);
  get_encoding_function()(infile, outfile);
  //printf("%d\n", x);

/*
  printf("%d\n", infile);
  */

  /*
  if(program_state != NULL) {
    close((int)program_state);
  }
  //I think this is how this works
  free((void*)outfile);
  free((void*)infile);
  */
  //free(file);
  free(program_state);
  return EXIT_SUCCESS;
}
