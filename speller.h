#include <aspell.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char *word;
    int is_correct;
    AspellStringEnumeration *possible_corrections;

} SpellResult;

static AspellSpeller *speller;

SpellResult *new_spell_result(char *word);
void destroy_spell_result(SpellResult *result);
