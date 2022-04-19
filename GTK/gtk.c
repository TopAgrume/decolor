#include <gtk/gtk.h>
#include <stdio.h>
#include "gtk.h"
#include <stdlib.h>

GdkRGBA* color;
GtkColorButton* ColorButton;
GtkWidget *window;
GtkRadioButton* brush;
GtkRadioButton* bucket;
GtkRadioButton* eraser;
GtkRadioButton* bigeraser;
GtkRadioButton* segment;
GtkRadioButton* square;
GtkRadioButton* triangle;
GtkRadioButton* circle;
GtkGrid* toolsgrid;
GtkButton* previous;
GtkButton* next;
GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
GtkWidget *SaveButton;
GtkWidget *dialog;
GtkWidget *image;
GtkWidget *FileChooser;
GtkFileChooser *chooser;
GtkScale* scale;
unsigned char scale_nb = 0;
GtkComboBoxText* filtres;
GtkButton* apply;

GSList* tools = g_list_alloc();
int tool_value = -1;


//functions
gboolean on_Color_set(GtkColorButton *self, gpointer user_data);
//void set_tools_group(GtkGriid* toolsgrid, GtkRadioButton* brush);
gboolean on_brush(GtkRadioButton *self, gpointer user_data);
gboolean on_bucket(GtkRadioButton *self, gpointer user_data);
gboolean on_eraser(GtkRadioButton *self, gpointer user_data);
gboolean on_bigeraser(GtkRadioButton *self, gpointer user_data);
gboolean on_segment(GtkRadioButton *self, gpointer user_data);
gboolean on_square(GtkRadioButton *self, gpointer user_data);
gboolean on_triangle(GtkRadioButton *self, gpointer user_data);
gboolean on_circle(GtkRadioButton *self, gpointer user_data);
gboolean on_previous(GtkButton *self, gpointer user_data);
gboolean on_next(GtkButton *self, gpointer user_data);
gboolean on_FileChoosing_file_set(GtkFileChooserButton *f, gpointer user_data);
gboolean on_SaveButton_clicked(GtkButton *f, gpointer user_data);
gboolean on_apply_clicked(GtkButton *self, gpointer user_data);


int main(int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    GtkBuilder* Builder = gtk_builder_new_from_file("GUI.glade");

    // Getting objects
    window = GTK_WIDGET(gtk_builder_get_object(Builder, "MyWindow"));
    ColorButton = gtk_builder_get_object(Builder, "Color");
    image = GTK_WIDGET(gtk_image_new());
    toolsgrid = gtk_builder_get_object(Builder, "toolsgrid");
    dialog = gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(window), action,
    "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL);
    SaveButton = GTK_WIDGET(gtk_builder_get_object(Builder, "Save"));
    FileChooser = GTK_WIDGET(gtk_builder_get_object(Builder, "FileChooser"));
    // Tools buttons
    brush = gtk_builder_get_object(Builder, "brush");
    gtk_radio_button_set_group (brush ,tools);
    bucket = gtk_builder_get_object(Builder, "bucket"); 
    eraser = gtk_builder_get_object(Builder, "eraser");
    bigeraser = gtk_builder_get_object(Builder, "bigeraser");
    segment = gtk_builder_get_object(Builder, "segment");
    square = gtk_builder_get_object(Builder, "square");
    triangle = gtk_builder_get_object(Builder, "triangle");
    circle = gtk_builder_get_object(Builder, "circle");
    set_tools_group(toolsgrid, brush);
    scale = GTK_SCALE(gtk_builder_get_object(Builder, "scale"));
    scale_nb = gtk_range_get_value(&(scale->range));
    printf("Scale number : %c", scale_nb);
    filtres = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(Builder, "Filtres"));
    apply = GTK_BUTTON(gtk_builder_get_object(Builder, "Appliquer"));
    // Previous and Next
    previous = gtk_builder_get_object(Builder, "previous");
    next = gtk_builder_get_object(Builder, "next");

    chooser = GTK_FILE_CHOOSER(dialog);
    gtk_file_chooser_set_current_name (chooser, "Saving Image");

    // Handling Signals
    g_signal_connect(ColorButton, "color-set", G_CALLBACK(on_Color_set), NULL);
    g_signal_connect(brush, "toggled", G_CALLBACK(on_brush), NULL);
    g_signal_connect(bucket, "toggled", G_CALLBACK(on_bucket), NULL);
    g_signal_connect(eraser, "toggled", G_CALLBACK(on_eraser), NULL);
    g_signal_connect(bigeraser, "toggled", G_CALLBACK(on_bigeraser), NULL);
    g_signal_connect(segment, "toggled", G_CALLBACK(on_segment), NULL);
    g_signal_connect(square, "toggled", G_CALLBACK(on_square), NULL);
    g_signal_connect(triangle, "toggled", G_CALLBACK(on_triangle), NULL);
    g_signal_connect(circle, "toggled", G_CALLBACK(on_circle), NULL);
    
    g_signal_connect(previous, "clicked", G_CALLBACK(on_previous), NULL);
    g_signal_connect(next, "clicked", G_CALLBACK(on_next), NULL);
    //replace NULL by the stack containing the modifications.

    g_signal_connect(SaveButton, "clicked", G_CALLBACK(on_SaveButton_clicked), image);
    g_signal_connect(FileChooser, "file-set", G_CALLBACK(on_FileChoosing_file_set), image);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    

    gtk_widget_show_all(window);

    gtk_main();

    return EXIT_SUCCESS;
}

gboolean on_previous(GtkButton* self, gpointer user_data)
{
    //use this fonction to revert last modification
    return FALSE;
}

gboolean on_next(GtkButton* self, gpointer user_data)
{
    //use this fonction to re-do last modification that was reverted
    return FALSE;
}

gboolean on_brush(GtkRadioButton *self, gpointer user_data)
{
    if (user_data == NULL && gtk_toggle_button_get_active(self) == TRUE)
        tool_value = 1;

    return FALSE;
}

gboolean on_bucket(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL && gtk_toggle_button_get_active(self) == TRUE)
         tool_value = 2;
     return FALSE;
}

gboolean on_eraser(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL && gtk_toggle_button_get_active(self) == TRUE)
         tool_value = 3;
     return FALSE;
}

gboolean on_bigeraser(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL && gtk_toggle_button_get_active(self) == TRUE)
         tool_value = 4;
     return FALSE;
}

gboolean on_segment(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL && gtk_toggle_button_get_active(self) == TRUE)
         tool_value = 5;
     return FALSE;
}

gboolean on_square(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL && gtk_toggle_button_get_active(self) == TRUE)
         tool_value = 6;
     return FALSE;
}

gboolean on_triangle(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL && gtk_toggle_button_get_active(self) == TRUE)
         tool_value = 7;
     return FALSE;
}

gboolean on_circle(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL && gtk_toggle_button_get_active(self) == TRUE)
         tool_value = 8;
     return FALSE;
}

void set_tools_group(GtkGrid* toolsgrid, GtkRadioButton* brush)
{
    for (int row = 0; row < 2; row++)
    {
        for (int col = 1-row; col < 4; col++)
        {
            gtk_radio_button_join_group(gtk_grid_get_child_at (toolsgrid,col,row),
            brush);
        }
    }
}

gboolean on_SaveButton_clicked(GtkButton *f ,gpointer user_data)
{
    GtkWidget *img = GTK_WIDGET(user_data);
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(img));
    GError* error = NULL;

    int res = gtk_dialog_run (GTK_DIALOG (dialog));
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

gboolean on_Color_set(GtkColorButton *self, gpointer user_data)
{
    gtk_color_button_get_rgba(self,color);

    return FALSE;
}


gboolean on_apply_clicked(GtkButton *self, gpointer user_data)
{
    char* fil = gtk_combo_box_text_get_active_text(filtres);

    switch(fil[0])
    {
        case 'N':
            return FALSE; //Filtre noir et blanc
        
        case 'I':
            return FALSE; //Filtre d'inversion de couleur

        case 'C':
            return FALSE; //Filtre de Contraste
        
        case 'L':
            return FALSE; //Filtre de Luminosité

        case 'S':
        {
            if(fil[2] == 'p')
                return FALSE; //Filtre Sépia
            
            else
                return FALSE; //Filtre Saturation
        } 

        default :
            return FALSE; //Aucun filtre
    }

    free(fil);

    return FALSE;
}
