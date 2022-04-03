#include <gtk/gtk.h>
#include <stdio.h>

GtkWidget *dialog;
GtkFileChooser *chooser;
GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
gint res;
GtkWidget *window;
GtkWidget *image;
GtkWidget *SaveButton;
GtkWidget *FileChooser;
GtkBuilder *Builder;

gboolean on_FileChoosing_file_set(GtkFileChooserButton *f, gpointer user_data);
gboolean on_SaveButton_clicked(GtkButton *f, gpointer user_data); 
void update_preview_cb (GtkFileChooser *file_chooser, gpointer data);


int main(int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    Builder = gtk_builder_new_from_file("GUI.glade");

    window = GTK_WIDGET(gtk_builder_get_object(Builder, "MyWindow"));

    dialog = gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(window), action, 
    "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL);

    chooser = GTK_FILE_CHOOSER(dialog);                                         
    gtk_file_chooser_set_current_name (chooser, "Saving Image"); 

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    SaveButton = GTK_WIDGET(gtk_builder_get_object(Builder, "Save"));
    FileChooser = GTK_WIDGET(gtk_builder_get_object(Builder, "FileChooser"));
    image = GTK_WIDGET(gtk_builder_get_object(Builder, "image"));
    GtkWidget *preview;
    preview = gtk_image_new();
    gtk_file_chooser_set_preview_widget (GTK_FILE_CHOOSER(FileChooser), preview);
    g_signal_connect (FileChooser, "update-preview", G_CALLBACK (update_preview_cb), preview);
    
    g_signal_connect(FileChooser, "file-set", G_CALLBACK(on_FileChoosing_file_set), image);
    g_signal_connect(SaveButton, "clicked", G_CALLBACK(on_SaveButton_clicked), image);

    gtk_widget_show_all(window);

    gtk_main();

    return EXIT_SUCCESS;
}

void update_preview_cb (GtkFileChooser *file_chooser, gpointer data)
{
  GtkWidget *preview;
  char *filename;
  GdkPixbuf *pixbuf;
  gboolean have_preview;

  preview = GTK_WIDGET (data);
  filename = gtk_file_chooser_get_preview_filename (file_chooser);

  pixbuf = gdk_pixbuf_new_from_file_at_size (filename, 128, 128, NULL);
  have_preview = (pixbuf != NULL);
  g_free (filename);

  gtk_image_set_from_pixbuf (GTK_IMAGE (preview), pixbuf);
  if (pixbuf)
    g_object_unref (pixbuf);

  gtk_file_chooser_set_preview_widget_active (file_chooser, have_preview);
}

gboolean on_SaveButton_clicked(GtkButton *f ,gpointer user_data)
{
    GtkWidget *img = GTK_WIDGET(user_data);
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(img));
    GError* error = NULL;

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;

        filename = gtk_file_chooser_get_filename (chooser);
        printf("%s", gtk_button_get_label(f));
        if (gdk_pixbuf_save(pixbuf, filename, "jpeg", &error, NULL) == FALSE)
        {
            fprintf(stderr,"%s" , error->message);
            g_error_free(error);
        }
        g_free (filename);
    }

    if (pixbuf)
        g_object_unref(pixbuf);
    gtk_widget_destroy (dialog);

    return FALSE;
}

gboolean on_FileChoosing_file_set(GtkFileChooserButton *f, gpointer user_data)
{
    GdkPixbuf *pixbuf;
    GtkWidget* image = user_data;
    char *filename;

    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f));
    pixbuf = gdk_pixbuf_new_from_file_at_size(filename, 750,750,NULL);
    g_free(filename);

    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
    if (pixbuf)
        g_object_unref(pixbuf);

    gtk_widget_show(image);

    return FALSE;
}

