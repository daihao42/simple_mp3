#include <gst/gst.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <math.h>
#include <dirent.h>
#include <gdk/gdkkeysyms.h>
int play_now_list;
int play_choose_list_on;
void stop_pressed();
void pause_pressed();
gint readmp3();
gint delete_event();
GtkWidget *window;
GtkWidget *box;
GtkWidget *play_box;
gboolean play_next(GtkWidget *widget,GdkEventButton *event);
void play_last();
void info_update();
gboolean update_time_callback();
void make_list_ui();
void make_config_ui();
gint file_add_menu( GtkWidget *widget,GdkEvent *event );
void order_menu_choose();
void range_menu_choose();
void onelast_menu_choose();
GtkWidget * bar;
GtkWidget *time_label;
GtkWidget *status_label;
static FILE *fname;
int list_length;
gint order_or_range;
GtkWidget *order_or_range_menu_button;
struct lr3list
{    int num;
     char lr3[1000];
}file_list[200];

gchar filename[1000];

