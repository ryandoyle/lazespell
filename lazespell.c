#include <stdio.h>
#include <gtk/gtk.h>
#include "speller.h"



void entry_changed_event(gpointer *entry) {
    const char *word = gtk_entry_get_text(GTK_ENTRY(entry));
    printf("%s\n", word);

    SpellResult *result = new_spell_result(word);

    printf("CORRRECTIONS FOR: %s\n", word);

    const char *correction;
    while ( (correction = aspell_string_enumeration_next(result->possible_corrections)) != NULL )
    {
        printf("%s\n", correction);
    }

    destroy_spell_result(result);

}

int main(int argc, char **argv) {

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

    /* GTK stuffs */
    GtkWidget *entry;
    GtkWidget *window;

    gtk_init(&argc, &argv);

    entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry), "derps");

    g_signal_connect(entry, "changed", G_CALLBACK(entry_changed_event), entry);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    gtk_container_add(GTK_CONTAINER(window), entry);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    gtk_window_set_has_resize_grip(GTK_WINDOW(window), FALSE);

    gtk_widget_show_all (window);

    GtkEntryCompletion *completion;
    completion = gtk_entry_completion_new();
    gtk_entry_completion_set_text_column(completion, 0);
    gtk_entry_set_completion(GTK_ENTRY(entry), completion);

    GtkListStore *model;
    GtkTreeIter iter;
    model = gtk_list_store_new(1, G_TYPE_STRING);

    gtk_list_store_append(model, &iter);
    gtk_list_store_set(model, &iter, 0, "Doops", -1);

    gtk_list_store_append(model, &iter);
    gtk_list_store_set(model, &iter, 0, "Fooper", -1);

    gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(model));

    gtk_main();

    destroy_spell_result(result);

    return 0;
}



