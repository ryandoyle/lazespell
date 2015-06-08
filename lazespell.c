#include <aspell.h>
#include <stdio.h>

int main(int arvc, char **argv) {
  AspellConfig *spell_config = new_aspell_config();

  AspellCanHaveError *possible_err = new_aspell_speller(spell_config);
  AspellSpeller *spell_checker = 0;
  if (aspell_error_number(possible_err) != 0) {
    puts(aspell_error_message(possible_err));
  }
  else {
    spell_checker = to_aspell_speller(possible_err);
  }

  char *to_check = argv[1];

  if(aspell_speller_check(spell_checker, to_check, -1) != 0) {
    puts("word is correct");
  } else {
    puts("word is incorrect");  
  }


  const AspellWordList *suggestions = aspell_speller_suggest(spell_checker, to_check, -1);
  AspellStringEnumeration * elements = aspell_word_list_elements(suggestions);
  const char * word;
  while ( (word = aspell_string_enumeration_next(elements)) != NULL )
  {
    printf("%s\n", word);
  }
  delete_aspell_string_enumeration(elements);

  return 0;

}
