#include <gtk/gtk.h>
#include "speller.h"


#define MAXIMUM_SUGGESTIONS 10
#define MINIMUM_TO_START_MATCH 3
#define STYLE_CORRECT "correct"
#define STYLE_INCORRECT "incorrect"

static void entry_changed_event(GtkWidget *entry, GtkEntryCompletion *completion) {

    const char *word;
    GtkTreeModel *model;
    GtkTreeIter iter;
    SpellResult *result;

    word =  gtk_entry_get_text(GTK_ENTRY(entry));
    result = spell_result_new(word);
    model = gtk_entry_completion_get_model(completion);

    gtk_list_store_clear(GTK_LIST_STORE(model));


    if (strlen(word) >= MINIMUM_TO_START_MATCH) {
        if (spell_result_is_correct(result)) {
            gtk_widget_set_name(GTK_WIDGET(entry), "correct");
        } else {
            gtk_widget_set_name(GTK_WIDGET(entry), "incorrect");
        }
    } else {
        gtk_widget_set_name(GTK_WIDGET(entry), "");
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

    spell_result_destroy(result);
}

static void entry_activate(GtkEntry *entry, GtkClipboard  *clipboard) {
    gtk_clipboard_set_text(clipboard, gtk_entry_get_text(entry), gtk_entry_get_text_length(entry));
    gtk_clipboard_store(clipboard);
    gtk_main_quit();
}

static gboolean entry_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(widget);

    if(event->keyval == GDK_KEY_Escape) {
        /* Single escape: clear the text, double: quit completely */
        if (gtk_entry_get_text_length(entry) == 0) {
            gtk_main_quit();
        } else {
            gtk_entry_set_text(entry, "");
            /* Clear name do we don't have any CSS applied */
            gtk_widget_set_name(widget, "");
        }
    }
    return FALSE;
}

static gboolean noop_match(GtkEntryCompletion *completion,
                    const gchar *key,
                    GtkTreeIter *iter,
                    gpointer user_data) {
    return strlen(key) >= MINIMUM_TO_START_MATCH;
}

int main(int argc, char **argv) {

    /* GTK stuffs */
    GtkWidget *entry;
    GtkWidget *window;
    GtkListStore *model;
    GtkEntryCompletion *completion;
    GtkClipboard *clipboard;

    /* Colouring  */
    GtkCssProvider *css_provider;
    GdkDisplay *display;
    GdkScreen *screen;


    gtk_init(&argc, &argv);

    css_provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(css_provider),
            "#" STYLE_CORRECT " { background: #B6FFAD; }\n#" STYLE_INCORRECT " { background: #FFADAD; }\n", -1, NULL);

    g_object_unref(css_provider);



    model = gtk_list_store_new(1, G_TYPE_STRING);
    completion = gtk_entry_completion_new();

    clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);

    entry = gtk_entry_new();

    g_signal_connect(entry, "changed", G_CALLBACK(entry_changed_event), completion);
    g_signal_connect(entry, "key_press_event", G_CALLBACK(entry_key_press), NULL);
    g_signal_connect(entry, "activate", G_CALLBACK(entry_activate), clipboard);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    gtk_container_add(GTK_CONTAINER(window), entry);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);

    gtk_window_set_skip_taskbar_hint(GTK_WINDOW(window), TRUE);
    gtk_widget_show_all (window);


    gtk_entry_completion_set_text_column(completion, 0);
    gtk_entry_completion_set_inline_completion(completion, TRUE);
    gtk_entry_set_completion(GTK_ENTRY(entry), completion);

    gtk_entry_completion_set_match_func(completion, noop_match, NULL, NULL);
    gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(model));
    gtk_entry_completion_set_minimum_key_length(completion, MINIMUM_TO_START_MATCH);

    gtk_main();

    return 0;
}



