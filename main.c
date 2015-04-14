#include "mp.h"
int main(int argc,char *argv[])
{       gst_init(&argc,&argv);
	gtk_init(&argc, &argv);
        GtkWidget *ui;
	readconfig();
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gui();
        gtk_widget_show_all(window);
        readlist();
 	gtk_main();
	return 0;
}

