#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define VALID_ENCRYPTING "Valid encrypting with k = "
#define INVALID_ENCRYPTING "Invalid encrypting"
#define ARRAY_LENGTH 10
#define NUM_OF_LETTERS 2
#define ASCII_A 65
#define ASCII_Z 90
#define ASCII_S_A 97
#define ASCII_S_Z 122
#define NUM_OF_ABC 26
#define VALID_ENCODE "encode"
#define VALID_DECODE "decode"
#define VALID_CHECK "check"
#define CORRECT_ARGS_CHECK 4
#define CORRECT_ARGS_EN_DECODE 5
#define INVALID_COMMAND "The given command is invalid"
#define INVALID_NUM_OF_ARGS_EN_DECODE "Usage: cipher <encode|decode> <k> " \
"<source path file> <output path file>"
#define INVALID_FILE "The given file is invalid"
#define INVALID_NUM_OF_ARGS_CHECK "Usage: cipher <check> <source path file> " \
"<output path file>"
#define ERROR_LETTER 'e'
#define NON_LETTER_NUM -100

// The helper function gets k - the encrypting key, input path of the input
//file and the output path of the output file. It calculates the encryption
// that is needed to every char that it receives from the files. It is used by
// the "encode" and "decode" functions.
void helper (int k, char input_path[], char output_path[])
{
  FILE *input_file = NULL;
  input_file = fopen (input_path, "r");
  FILE *output_file;
  output_file = fopen (output_path, "w");
  char file_store[ARRAY_LENGTH];
  while (fgets (file_store, NUM_OF_LETTERS, input_file) != NULL)
    {
      unsigned char uc_file_store = file_store[0];
      int ascii_char = uc_file_store;
      if ((ascii_char >= ASCII_A) && (ascii_char <= ASCII_Z))
        {
          ascii_char = ascii_char + k;
          if (ascii_char < ASCII_A)
            {
              ascii_char = ascii_char + NUM_OF_ABC;
            }
          else if (ascii_char > ASCII_Z)
            {
              ascii_char = ascii_char - NUM_OF_ABC;
            }
        }
      else if ((ascii_char >= ASCII_S_A) && (ascii_char <= ASCII_S_Z))
        {
          ascii_char = ascii_char + k;
          if (ascii_char < ASCII_S_A)
            {
              ascii_char = ascii_char + NUM_OF_ABC;
            }
          else if (ascii_char > ASCII_S_Z)
            {
              ascii_char = ascii_char - NUM_OF_ABC;
            }
        }
      unsigned char char_convert = ascii_char;
      fprintf (output_file, "%c", char_convert);
    }
  fclose (output_file);
  fclose (input_file);
}

// This function receives encryption key, output and input file paths,
// encodes a string from a file and returns noting.
// if there is no output exists the function creates one
void encode (int k, char input_path[], char output_path[])
{
  helper (k % NUM_OF_ABC, input_path, output_path);
}

// This function receives encryption key, output and input file paths decodes a
// string from a file and returns noting.
// If there is no output exists the function creates one
void decode (int k, char input_path[], char output_path[])
{
  int new_k = -k;
  helper (new_k % NUM_OF_ABC, input_path, output_path);
}

// This function receives output and input file paths and returns the
// the encryption key. It returns a number [0,25] that
// represents the encryption key, or -100 if there is no key (0)
int guess_the_k (char input_path[], char output_path[])
{
  FILE *kelet = NULL;
  kelet = fopen (input_path, "r");
  FILE *pelet = NULL;
  pelet = fopen (output_path, "r");
  char kelet_arr[ARRAY_LENGTH];
  char pelet_arr[ARRAY_LENGTH];
  int guessed_k;
  while (fgets (kelet_arr, NUM_OF_LETTERS, kelet) != NULL)
    {
      while (fgets (pelet_arr, NUM_OF_LETTERS, pelet) != NULL)
        {
          unsigned char kelet_u_char = kelet_arr[0];
          unsigned char pelet_u_char = pelet_arr[0];
          int kelet_int = kelet_u_char;
          int pelet_int = pelet_u_char;
          if ((((kelet_int >= ASCII_A) && (kelet_int <= ASCII_Z)) ||
               ((kelet_int >= ASCII_S_A) && (kelet_int <= ASCII_S_Z))) &&
              (((pelet_int >= ASCII_A) && (pelet_int <= ASCII_Z)) ||
               ((pelet_int >= ASCII_S_A) && (pelet_int <= ASCII_S_Z))))
            {
              if (kelet_int > pelet_int)
                {
                  guessed_k = NUM_OF_ABC - (kelet_int - pelet_int);
                }
              else if (kelet_int == pelet_int)
                {
                  guessed_k = 0;
                }
              else
                {
                  guessed_k = pelet_int - kelet_int;
                }
              fclose (pelet);
              fclose (kelet);
              return guessed_k;
            }
          else
            {
              break;
            }
        }
    }
  fclose (pelet);
  fclose (kelet);
  return NON_LETTER_NUM;
}

// This function receives output and input file paths and checks if the
// encryption is correct.
// It returns a number [0,25] that represents the encryption key
char check (char input_path[], char output_path[])
{
  int guessed_k = guess_the_k (input_path, output_path);
  if (guessed_k == NON_LETTER_NUM)
    {
      guessed_k = 0;
    }
  FILE *kelet = NULL;
  kelet = fopen (input_path, "r");
  FILE *pelet = NULL;
  pelet = fopen (output_path, "r");
  char kelet_arr[ARRAY_LENGTH];
  char pelet_arr[ARRAY_LENGTH];
  int size_kelet = 0;
  int size_pelet = 0;
  while (fgets (kelet_arr, NUM_OF_LETTERS, kelet) != NULL)
    {
      size_kelet += 1;
    }
  while (fgets (pelet_arr, NUM_OF_LETTERS, pelet) != NULL)
    {
      size_pelet += 1;
    }
  fclose (pelet);
  fclose (kelet);
  if (size_pelet != size_kelet)
    {
      return ERROR_LETTER;
    }
  if (size_kelet == 0 && size_pelet == 0)
    {
      return 0;
    }
  FILE *kelet_2 = NULL;
  kelet_2 = fopen (input_path, "r");
  FILE *pelet_2 = NULL;
  pelet_2 = fopen (output_path, "r");
  while (fgets (kelet_arr, NUM_OF_LETTERS, kelet_2) != NULL)
    {
      while (fgets (pelet_arr, NUM_OF_LETTERS, pelet_2) != NULL)
        {
          unsigned char kelet_char_2 = kelet_arr[0];
          unsigned char pelet_char_2 = pelet_arr[0];
          int kelet_int_2 = kelet_char_2;
          int pelet_int_2 = pelet_char_2;
          if (((kelet_int_2 >= ASCII_A) && (kelet_int_2 <= ASCII_Z)))
            {
              if ((pelet_int_2 >= ASCII_A) && (pelet_int_2 <= ASCII_Z))
                {
                  if (kelet_int_2 > pelet_int_2)
                    {
                      if (guessed_k == (NUM_OF_ABC -
                                        (kelet_int_2 - pelet_int_2)))
                        {
                          break;
                        }
                      else
                        {
                          fclose (pelet_2);
                          fclose (kelet_2);
                          return ERROR_LETTER;
                        }
                    }
                  else if (pelet_int_2 > kelet_int_2)
                    {
                      if (guessed_k == (pelet_int_2 - kelet_int_2))
                        {
                          break;
                        }
                      else
                        {
                          fclose (pelet_2);
                          fclose (kelet_2);
                          return ERROR_LETTER;
                        }
                    }
                  else
                    {
                      if (guessed_k == 0)
                        {
                          break;
                        }
                      else
                        {
                          fclose (pelet_2);
                          fclose (kelet_2);
                          return ERROR_LETTER;
                        }
                    }
                }
            }
          else if (((kelet_int_2 >= ASCII_S_A) && (kelet_int_2 <= ASCII_S_Z)))
            {
              if ((pelet_int_2 >= ASCII_S_A) && (pelet_int_2 <= ASCII_S_Z))
                {
                  if (kelet_int_2 > pelet_int_2)
                    {
                      if (guessed_k == (NUM_OF_ABC -
                                        (kelet_int_2 - pelet_int_2)))
                        {
                          break;
                        }
                      else
                        {
                          fclose (pelet_2);
                          fclose (kelet_2);
                          return ERROR_LETTER;
                        }
                    }
                  else if (pelet_int_2 > kelet_int_2)
                    {
                      if (guessed_k == (pelet_int_2 - kelet_int_2))
                        {
                          break;
                        }
                      else
                        {
                          fclose (pelet_2);
                          fclose (kelet_2);
                          return ERROR_LETTER;
                        }
                    }
                  else
                    {
                      if (guessed_k == 0)
                        {
                          break;
                        }
                      else
                        {
                          fclose (pelet_2);
                          fclose (kelet_2);
                          return ERROR_LETTER;
                        }
                    }
                }
              else
                {
                  fclose (pelet_2);
                  fclose (kelet_2);
                  return ERROR_LETTER;
                }
            }
          else if (((pelet_int_2 >= ASCII_S_A) && (pelet_int_2 <= ASCII_S_Z))
                   || ((pelet_int_2 >= ASCII_A) && (pelet_int_2 <= ASCII_Z)) ||
                   (kelet_char_2 != pelet_char_2))
            {
              fclose (pelet_2);
              fclose (kelet_2);
              return ERROR_LETTER;
            }
          break;
        }
    }
  if ((fgets (pelet_arr, NUM_OF_LETTERS, pelet_2) == NULL) &&
      (fgets (kelet_arr, NUM_OF_LETTERS, kelet_2) == NULL))
    {
      fclose (pelet_2);
      fclose (kelet_2);
      return (char) guessed_k;
    }
  fclose (pelet_2);
  fclose (kelet_2);
  return ERROR_LETTER;
}

// This function checks if a path of a file is exists. If so, it returns 1,
// otherwise it returns 0.
int file_existence (char *path)
{
  FILE *input_file = NULL;
  if ((input_file = fopen (path, "r")))
    {
      fclose (input_file);
      return 1;
    }
  return 0;
}

int main (int argc, char *argv[])
{
  if (argc <= 1)
    {
      fprintf (stderr, INVALID_COMMAND "\n");
      return EXIT_FAILURE;
    }
  int encode_valid = strcmp (argv[1], VALID_ENCODE);
  int decode_valid = strcmp (argv[1], VALID_DECODE);
  int check_valid = strcmp (argv[1], VALID_CHECK);
  // Checks if the command is correct
  if (encode_valid && decode_valid && check_valid)
    {
      fprintf (stderr, INVALID_COMMAND "\n");
      return EXIT_FAILURE;
    }
  if (encode_valid == 0 || decode_valid == 0)
    {
      //Checks if there is a correct number of arguments for en\decode
      if (argc != CORRECT_ARGS_EN_DECODE)
        {
          fprintf (stderr, INVALID_NUM_OF_ARGS_EN_DECODE "\n");
          return EXIT_FAILURE;
        }
      int file_valid = file_existence (argv[3]);
      //Checks if the input file exists
      if (file_valid == 0)
        {
          fprintf (stderr, INVALID_FILE "\n");
          return EXIT_FAILURE;
        }
      int a = atoi (argv[2]);
      //Executes "encode" function if the user entered the right args
      if (encode_valid == 0)
        {
          encode (a, argv[3], argv[4]);
        }
      //Executes "decode" function if the user entered the right args
      if (decode_valid == 0)
        {
          decode (a, argv[3], argv[4]);
        }
    }
  //Checks if the user asked for "check" function
  if (check_valid == 0)
    {
      //Checks if there is a correct number of arguments for check function
      if (argc != CORRECT_ARGS_CHECK)
        {
          fprintf (stderr, INVALID_NUM_OF_ARGS_CHECK "\n");
          return EXIT_FAILURE;
        }
      int file_valid_2 = file_existence (argv[2]);
      int file_valid_3 = file_existence (argv[3]);
      //Checks if the input file exists
      if (file_valid_2 == 0)
        {
          fprintf (stderr, INVALID_FILE "\n");
          return EXIT_FAILURE;
        }
      if (file_valid_3 == 0)
        {
          fprintf (stderr, INVALID_FILE "\n");
          return EXIT_FAILURE;
        }
      //Executes "check" function if the user entered the right args
      char check_result = check (argv[2], argv[3]);
      int guessed_k = (int) check_result;
      if (check_result == ERROR_LETTER)
        {
          fprintf (stdout, "%s\n", INVALID_ENCRYPTING);
        }
      else
        {
          fprintf (stdout, "%s%d\n", VALID_ENCRYPTING, guessed_k);
        }
    }
  return EXIT_SUCCESS;
}
