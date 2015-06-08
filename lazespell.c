#include <aspell.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
  char *word;
  int is_correct;
  AspellStringEnumeration *possible_corrections;

} SpellResult;

static int is_word_correct(char *word);
static AspellStringEnumeration *get_possible_corrections(char *word);


static SpellResult *new_spell_result(char *word) {
  SpellResult *result = malloc(sizeof(SpellResult));

  result->word = word;
  result->is_correct = is_word_correct(word);
  if(!result->is_correct) {
    result->possible_corrections = get_possible_corrections(word);
  }
  return result;
}


static void destroy_spell_result(SpellResult *result) {
  if(result->possible_corrections != NULL) {
    delete_aspell_string_enumeration(result->possible_corrections);
  }

  free(result);
}


static AspellSpeller *get_spell_checker(void) {

  AspellConfig *spell_config = new_aspell_config();
  AspellCanHaveError *aspell_errorable = new_aspell_speller(spell_config);
  AspellSpeller *spell_checker = 0;

  if (aspell_error_number(aspell_errorable) != 0) {
    fprintf(stderr, aspell_error_message(aspell_errorable));
  }
  else {
    spell_checker = to_aspell_speller(aspell_errorable);
  }
  return spell_checker;
}

static int is_word_correct(char *word) {
  AspellSpeller *speller;
  speller = get_spell_checker();

  return aspell_speller_check(speller, word, -1) != 0;
}

static AspellStringEnumeration *get_possible_corrections(char *word) {
  AspellSpeller *speller = get_spell_checker();

  const AspellWordList *suggestions = aspell_speller_suggest(speller, word, -1);
  return aspell_word_list_elements(suggestions);
}

int main(int arvc, char **argv) {

  char *to_check = argv[1];

  SpellResult *result = new_spell_result(to_check);

  if(result->is_correct) {
    puts("word is correct");
  } else {
    puts("word is incorrect");
  }


  const char * word;
  while ( (word = aspell_string_enumeration_next(result->possible_corrections)) != NULL )
  {
    printf("%s\n", word);
  }

  destroy_spell_result(result);

  return 0;
}



