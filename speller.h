#include <aspell.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    const char *word;
    int is_correct;
    AspellStringEnumeration *possible_corrections;

} SpellResult;

static AspellSpeller *speller;

SpellResult *spell_result_new(const char *word);
void destroy_spell_result(SpellResult *result);
int spell_result_is_correct(SpellResult *result);
const char *spell_result_suggestion_iterator(SpellResult *result);