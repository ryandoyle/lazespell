#include <gtk/gtk.h>
#include "speller.h"


#define MAXIMUM_SUGGESTIONS 10

void entry_changed_event(GtkWidget *entry, GtkEntryCompletion *completion) {

    const char *word;
    GtkTreeModel *model;
    GtkTreeIter iter;
    SpellResult *result;

    word =  gtk_entry_get_text(GTK_ENTRY(entry));
    result = spell_result_new(word);
    model = gtk_entry_completion_get_model(completion);

    gtk_list_store_clear(GTK_LIST_STORE(model));


    if(spell_result_is_correct(result)) {
        GdkRGBA green;
        green.alpha = 1.0;
        green.blue = 0.4;
        green.green = 1.0;
        green.red = 0.4;
        gtk_widget_override_background_color(entry, GTK_STATE_FLAG_NORMAL, &green);
    } else {
        GdkRGBA red;
        red.alpha = 1.0;
        red.blue = 0.6;
        red.green = 0.6;
        red.red = 0.9;
        gtk_widget_override_background_color(entry, GTK_STATE_FLAG_NORMAL, &red);
    }

    const char *correction;
    int iteration = 0;
    while ((correction = spell_result_suggestion_iterator(result)) != NULL)
    {
        if(iteration++ == MAXIMUM_SUGGESTIONS)
            break;
        gtk_list_store_append(GTK_LIST_STORE(model), &iter);
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, correction, -1);
    }

    destroy_spell_result(result);
}

gboolean noop_match(GtkEntryCompletion *completion,
                    const gchar *key,
                    GtkTreeIter *iter,
                    gpointer user_data) {
    return TRUE;
}

int main(int argc, char **argv) {

    /* GTK stuffs */
    GtkWidget *entry;
    GtkWidget *window;
    GtkListStore *model;
    GtkEntryCompletion *completion;

    gtk_init(&argc, &argv);

    model = gtk_list_store_new(1, G_TYPE_STRING);
    completion = gtk_entry_completion_new();


    entry = gtk_entry_new();

    g_signal_connect(entry, "changed", G_CALLBACK(entry_changed_event), completion);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    gtk_container_add(GTK_CONTAINER(window), entry);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    gtk_window_set_has_resize_grip(GTK_WINDOW(window), FALSE);

    gtk_widget_show_all (window);


    gtk_entry_completion_set_text_column(completion, 0);
    gtk_entry_set_completion(GTK_ENTRY(entry), completion);

    gtk_entry_completion_set_match_func(completion, noop_match, NULL, NULL);
    gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(model));
    gtk_entry_completion_set_minimum_key_length(completion, 3);

    gtk_main();

    return 0;
}



