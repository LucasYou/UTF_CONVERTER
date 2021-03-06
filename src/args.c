#include "debug.h"
#include "utf.h"
#include "wrappers.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int opterr;
int optopt;
int optind;
char *optarg;

state_t *program_state;

void
parse_args(int argc, char *argv[])
{

  int i;
  char option;
  int counter = 0;
  int checker = 0;


  char *joined_argv;
  joined_argv = join_string_array(argc, argv);
  info("argc: %d argv: %s", argc, joined_argv);
  free(joined_argv);


  program_state = Calloc(1, sizeof(state_t));
  for (i = 0; i < argc; i++) {
    debug("%d opterr: %d", i, opterr);
    debug("%d optind: %d", i, optind);
    debug("%d optopt: %d", i, optopt);
    debug("%d argv[optind]: %s", i, argv[optind]);

    if ((option = getopt(argc, argv, "+e:")) != -1) {
      //printf("%d\n", option);
      switch (option) {
        case 'e':
        {
          optarg = *(argv+i+2);
          info("Encoding Argument: %s", optarg);
          if ((program_state->encoding_to = determine_format(optarg)) == 0){
            goto errorcase;
          }

          program_state->encoding_to = determine_format(optarg);
          break;
        }

        case '?':
        {
          if (optopt != 'h'){
            fprintf(stderr, KRED "-%c is not a supported argument\n" KNRM, optopt);
            goto errorcase;
          }
          else if(optopt == 'h') {
            free(program_state);
            USAGE(argv[0]);
            exit(EXIT_SUCCESS);
          }
          break;
        }

        default: {
          break;
        }
      }
    }//end of if
    else if(argv[optind] != NULL)
    {
      if(counter == 2)
        goto errorcase;

      if (program_state->in_file == NULL) {
        program_state->in_file = argv[optind];
        if( strcmp(argv[optind], argv[optind+1]) == 0)
          checker = 1;
      }
      else if(program_state->out_file == NULL)
      {
        program_state->out_file = argv[optind];
      }
      optind++;
      counter++;
    }
  }//end of for
  if((argc < 5) | (argc > 6))
    goto errorcase;
  if(checker == 1)
    goto errorcase;

  goto success;

errorcase:
  free(program_state);
  USAGE(argv[0]);
  exit(EXIT_FAILURE);

success:
  //exit(EXIT_SUCCESS);
  return;
}

format_t
determine_format(char *argument)
{

  if (strcmp(argument, STR_UTF16LE) == 0)
    return UTF16LE;
  if (strcmp(argument, STR_UTF16BE) == 0)
    return UTF16BE;
  if (strcmp(argument, STR_UTF8) == 0)
    return UTF8;
  //printf("%s\n", argument);
  return 0;
}

/* char* --> const char* */
const char*
bom_to_string(format_t bom){
  switch(bom){
    case UTF8: return STR_UTF8;
    case UTF16BE: return STR_UTF16BE;
    case UTF16LE: return STR_UTF16LE;
  }
  return "UNKNOWN";
}

char*
join_string_array(int count, char *array[])
{
  char *ret;
  int len = 0;
  int cur_str_len;
  int str_len;
  str_len = array_size(count, array);

  ret = malloc(str_len+1);

  for (int i = 0; i < count; i++) {
    cur_str_len = strlen(array[i]);
    memcpy(ret + len, array[i], cur_str_len);
    len += cur_str_len;
    memcpy(ret + len, " ", 1);
    len += 1;
  }

  return ret;
}

int
array_size(int count, char *array[])
{
  int i, sum = 0;
  for (i = 0; i < count; i++) {
    sum += strlen(array[i]);
  }
  sum += count;
  sum --;
  return sum;
}

void
print_state()
{
//errorcase:
  if (program_state == NULL) {
    error("program_state is %p", (void*)program_state);
    exit(EXIT_FAILURE);
  }
  info("program_state {\n"
         "  format_t encoding_to = 0x%X;\n"
         "  format_t encoding_from = 0x%X;\n"
         "  char *in_file = '%s';\n"
         "  char *out_file = '%s';\n"
         "};\n",
         program_state->encoding_to, program_state->encoding_from,
         program_state->in_file, program_state->out_file);

/*
  printf("encoding_from: %.4x\n", program_state->encoding_from);
  printf("encoding_to: %.4x\n", program_state->encoding_to);
  printf("bom_length: %d\n", program_state->bom_length);
  printf("in_file: %s\n", program_state->in_file);
  printf("out_file: %s\n", program_state->out_file);
*/
}
