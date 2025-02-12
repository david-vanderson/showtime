#include <gtk/gtk.h>

GtkBox* hbox;
GtkLabel* label_hello;
GtkLabel* label_entry;
GtkEntry* entry;
GtkButton* button;

char name[50] = "Alice";
bool editing = false;

void stop_editing() {
    editing = false;

    GtkEntryBuffer* buffer = gtk_entry_get_buffer(entry);
    strncpy(name, gtk_entry_buffer_get_text(buffer), 50);

    g_object_ref(label_entry);
    gtk_box_remove(hbox, (GtkWidget*) label_entry);
    g_object_ref(entry);
    gtk_box_remove(hbox, (GtkWidget*) entry);

    char temp[70];
    snprintf(temp, 70, "Hello:      %s", name);
    gtk_label_set_label(label_hello, temp);
    gtk_box_append(hbox, (GtkWidget*) label_hello);

    gtk_button_set_label(button, "Edit");
}

void entry_enter(GtkWidget* origin, gpointer data) {
    stop_editing();
}

void button_click(GtkWidget* origin, gpointer data) {
    if (editing) {
        stop_editing();
    } else {
        editing = true;

        g_object_ref(label_hello);
        gtk_box_remove(hbox, (GtkWidget*) label_hello);

        GtkEntryBuffer* buffer = gtk_entry_get_buffer(entry);
        gtk_entry_buffer_set_text(buffer, name, -1);

        gtk_box_append(hbox, (GtkWidget*) label_entry);
        gtk_box_append(hbox, (GtkWidget*) entry);

        gtk_button_set_label(button, "Done");
    }
}

void activate (GtkApplication* app, gpointer data) {
    GtkWidget *window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Gtk C");
    gtk_window_set_default_size (GTK_WINDOW (window), 200, 160);

    GtkBox* box = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child((GtkWindow*) window, (GtkWidget*) box);

    hbox = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append((GtkBox*) box, (GtkWidget*) hbox);

    char temp[70];
    snprintf(temp, 70, "Hello:      %s", name);
    label_hello = (GtkLabel*) gtk_label_new(temp);
    g_object_set(label_hello, "margin-start", 10, "margin-top", 10, "margin-end", 10, "margin-bottom", 10, NULL);
    gtk_box_append(hbox, (GtkWidget*) label_hello);

    label_entry = (GtkLabel*) gtk_label_new("Hello:");
    g_object_set(label_entry, "margin-start", 10, "margin-top", 10, "margin-end", 10, "margin-bottom", 10, NULL);

    entry = (GtkEntry*) gtk_entry_new();
    gtk_editable_set_max_width_chars((GtkEditable*) entry, 10);
    g_signal_connect(entry, "activate", G_CALLBACK(entry_enter), NULL);

    button = (GtkButton*) gtk_button_new();
    gtk_button_set_label(button, "Edit");
    g_object_set(button, "margin-start", 10, "margin-top", 10, "margin-end", 10, "margin-bottom", 10, NULL);
    gtk_widget_set_halign ((GtkWidget*)button, GTK_ALIGN_START);
    g_signal_connect(button, "clicked", G_CALLBACK(button_click), NULL);
    gtk_box_append((GtkBox*) box, (GtkWidget*) button);

    gtk_window_present (GTK_WINDOW (window));

	//gtk_window_set_interactive_debugging(TRUE);
}

int main (int argc, char **argv) {
    GtkApplication* app = gtk_application_new("com.github.david-vanderson.demo", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return 0;
}

