#include "speller.h"

static void build_spell_checker() {
    AspellConfig *spell_config = new_aspell_config();
    AspellCanHaveError *aspell_possible_error = new_aspell_speller(spell_config);

    if (aspell_error_number(aspell_possible_error) != 0) {
        fprintf(stderr, aspell_error_message(aspell_possible_error));
    }
    else {
        speller = to_aspell_speller(aspell_possible_error);
    }
}

static AspellSpeller *get_spell_checker(void) {
    if(speller == NULL) {
        build_spell_checker();
    }
    return speller;
}

static int is_word_correct(char *word) {
    return aspell_speller_check(get_spell_checker(), word, -1) != 0;
}

static AspellStringEnumeration *get_possible_corrections(char *word) {
    const AspellWordList *suggestions = aspell_speller_suggest(get_spell_checker(), word, -1);
    return aspell_word_list_elements(suggestions);
}

SpellResult *new_spell_result(char *word) {
    SpellResult *result = malloc(sizeof(SpellResult));

    result->word = word;
    result->is_correct = is_word_correct(word);
    result->possible_corrections = get_possible_corrections(word);
    return result;
}


void destroy_spell_result(SpellResult *result) {
    if(result->possible_corrections != NULL) {
        delete_aspell_string_enumeration(result->possible_corrections);
    }

    free(result);
}


