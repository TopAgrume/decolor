#include <gtk/gtk.h>
#include <stdio.h>
#include "gtk.h"
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

GdkRGBA* color;
GtkColorChooser* ColorButton;
GtkWidget *window;
GtkRadioButton* brush;
GtkRadioButton* bucket;
GtkRadioButton* eraser;
GtkRadioButton* bigeraser;
GtkRadioButton* segment;
GtkRadioButton* square;
GtkRadioButton* triangle;
GtkRadioButton* circle;
GtkWidget* toolsgrid;
GtkWidget* previous;
GtkWidget* next;
GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
GtkButton *SaveButton;
GtkWidget *dialog;
GtkWidget *image;
GtkFileChooser *FileChooser;
GtkFileChooser *chooser;
GtkScale* scale;
unsigned char scale_nb = 0;
GtkComboBoxText* filtres;
GtkButton* apply;
GtkWidget* eventbox;

int tool_value = -1;


//functions
gboolean on_Color_set(GtkColorChooser *self, gpointer user_data);
void set_tools_group(GtkGrid* toolsgrid, GtkRadioButton* brush);
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
gboolean update_scale_val(GtkScale *self, gpointer user_data);
static gboolean mouse_moved(GtkWidget *widget,GdkEvent *event, gpointer user_data);

int main(int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    GtkBuilder* Builder = gtk_builder_new_from_file("GUI.glade");

    //tools = g_slist_alloc();
    // Getting objects
    window = GTK_WIDGET(gtk_builder_get_object(Builder, "MyWindow"));
    ColorButton = GTK_COLOR_CHOOSER(gtk_builder_get_object(Builder, "Color"));
    image = GTK_WIDGET(gtk_builder_get_object(Builder, "image"));
    toolsgrid = GTK_WIDGET(gtk_builder_get_object(Builder, "toolsgrid"));
    dialog = gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(window), action,
    "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL);
    SaveButton = GTK_BUTTON(gtk_builder_get_object(Builder, "Save"));
    FileChooser = GTK_FILE_CHOOSER(gtk_builder_get_object(Builder, "FileChooser"));
    eventbox = gtk_event_box_new ();
    gtk_container_add(GTK_CONTAINER (eventbox), image);
    // Tools buttons
    brush = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "brush"));
    //gtk_radio_button_set_group (brush ,tools);
    bucket = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "bucket")); 
    eraser = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "eraser"));
    bigeraser = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "bigeraser"));
    segment = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "segment"));
    square = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "square"));
    triangle = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "triangle"));
    circle = GTK_RADIO_BUTTON(gtk_builder_get_object(Builder, "circle"));
    //set_tools_group(toolsgrid, brush);
    scale = GTK_SCALE(gtk_builder_get_object(Builder, "Scale")); 
    filtres = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(Builder, "Filtres"));
    apply = GTK_BUTTON(gtk_builder_get_object(Builder, "Appliquer"));
    // Previous and Next
    previous = GTK_WIDGET(gtk_builder_get_object(Builder, "previous"));
    next = GTK_WIDGET(gtk_builder_get_object(Builder, "next"));

    chooser = GTK_FILE_CHOOSER(dialog);
    gtk_file_chooser_set_current_name (chooser, "Saving Image");

    // Handling Signals
    g_signal_connect(ColorButton, "color-set", G_CALLBACK(on_Color_set), NULL);
    g_signal_connect(brush, "toggled", G_CALLBACK(on_brush), NULL);
    g_signal_connect(bucket, "toggled", G_CALLBACK(on_bucket), NULL);
    g_signal_connect(eraser, "toggled", G_CALLBACK(on_eraser), NULL);
    g_signal_connect(bigeraser, "toggled", G_CALLBACK(on_bigeraser), NULL);
    g_signal_connect(segment, "toggled", G_CALLBACK(on_segment), NULL);
    g_signal_connect(square,"toggled", G_CALLBACK(on_square), NULL);
    g_signal_connect(triangle, "toggled", G_CALLBACK(on_triangle), NULL);
    g_signal_connect(circle, "toggled", G_CALLBACK(on_circle), NULL);
    g_signal_connect(scale, "value_changed", G_CALLBACK(update_scale_val), NULL);
    
    g_signal_connect(previous, "clicked", G_CALLBACK(on_previous), NULL);
    g_signal_connect(next, "clicked", G_CALLBACK(on_next), NULL);
    //replace NULL by the stack containing the modifications.

    g_signal_connect(SaveButton, "clicked", G_CALLBACK(on_SaveButton_clicked), image);
    g_signal_connect(FileChooser, "file-set", G_CALLBACK(on_FileChoosing_file_set), image);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect (G_OBJECT (eventbox), "motion-notify-event",G_CALLBACK (mouse_moved), NULL);    
    gtk_widget_set_events(eventbox, GDK_POINTER_MOTION_MASK);

    gtk_widget_show_all(window);

    gtk_main();

    return EXIT_SUCCESS;
}

void update(SDL_Surface* src)
{
    SDL_SaveBMP(src, "tmpfile.bmp");
    GdkPixbuf *pixbuf;

    pixbuf = gdk_pixbuf_new_from_file("tmpfile.bmp", NULL);
    
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);

    if (pixbuf)
        g_object_unref(pixbuf);

    gtk_widget_show(image);
}

static gboolean mouse_moved(GtkWidget *widget,GdkEvent *event, gpointer user_data) 
{
    if (event->type==GDK_MOTION_NOTIFY && widget != NULL && user_data == NULL) 
    {
        GdkEventMotion* e =(GdkEventMotion*)event;
        printf("Coordinates: (%u,%u)\n", (guint)e->x,(guint)e->y);
    }
    return FALSE;
}

gboolean on_previous(GtkButton* self, gpointer user_data)
{
    //use this fonction to revert last modification
    if (self && user_data)
        return FALSE;
    return FALSE;
}

gboolean on_next(GtkButton* self, gpointer user_data)
{
    //use this fonction to re-do last modification that was reverted
    if (self && user_data)
        return FALSE;  
    return FALSE;
}

gboolean on_brush(GtkRadioButton *self, gpointer user_data)
{
    if (user_data == NULL &&
        gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
    {
        tool_value = 1;
    }

    return FALSE;
}

gboolean on_bucket(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL &&
         gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
     {
         tool_value = 2;
     }
     return FALSE;
}

gboolean on_eraser(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL &&
         gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
     {
         tool_value = 3;
     }
     return FALSE;
}

gboolean on_bigeraser(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL &&
         gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
     {
         tool_value = 4;
     }
     return FALSE;
}

gboolean on_segment(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL &&
         gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
     {
         tool_value = 5;
     }
     return FALSE;
}

gboolean on_square(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL &&
         gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
     {
         tool_value = 6;
     }
     return FALSE;
}

gboolean on_triangle(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL &&
         gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
     {
         tool_value = 7;
     }
     return FALSE;
}

gboolean on_circle(GtkRadioButton *self, gpointer user_data)
{
     if (user_data == NULL &&
         gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)) == TRUE)
     {
         tool_value = 8;
     }
     return FALSE;
}

void set_tools_group(GtkGrid* toolsgrid, GtkRadioButton* brush)
{
    for (int row = 0; row < 2; row++)
    {
        for (int col = 1-row; col < 4; col++)
        {
            gtk_radio_button_join_group(GTK_RADIO_BUTTON(gtk_grid_get_child_at(toolsgrid,col,row)),
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
    pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    g_free(filename);

    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
    if (pixbuf)
        g_object_unref(pixbuf);

    gtk_widget_show(image);

    return FALSE;
}

gboolean on_Color_set(GtkColorChooser *self, gpointer user_data)
{

    if (user_data != NULL)
        return FALSE;

    gtk_color_chooser_get_rgba(self,color);

    return FALSE;
}


gboolean update_scale_val(GtkScale *self, gpointer user_data)
{
    scale_nb = gtk_range_get_value(&(self->range));
    printf("Scale number : %i\n", scale_nb);
    
    return FALSE;
}


gboolean on_apply_clicked(GtkButton *self, gpointer user_data)
{
    // Just for avoiding warnings you can change if you want
    if (self == NULL && user_data != NULL)
        return FALSE;


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
