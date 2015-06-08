#include <stdio.h>
#include "speller.h"

int main(int arvc, char **argv) {

  char *to_check = argv[1];

  SpellResult *result = new_spell_result(to_check);

  if(result->is_correct) {
    puts("word is correct");
  } else {
    puts("word is incorrect");
  }


  const char *word;
  while ( (word = aspell_string_enumeration_next(result->possible_corrections)) != NULL )
  {
    printf("%s\n", word);
  }

  destroy_spell_result(result);

  return 0;
}



