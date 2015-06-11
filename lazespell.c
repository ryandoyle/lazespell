#include <stdio.h>
#include <gtk/gtk.h>
#include "speller.h"

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

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    gtk_container_add(GTK_CONTAINER(window), entry);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    gtk_window_set_has_resize_grip(GTK_WINDOW(window), FALSE);

    gtk_widget_show(entry);
    gtk_widget_show(window);


    gtk_main();

    destroy_spell_result(result);

    return 0;
}



