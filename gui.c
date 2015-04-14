#include "mp.h"

	
        gchar mp3name[100];
        GtkWidget *button;
	GtkWidget *button_box;
	GtkWidget *title_label_box;
        GtkWidget *title_label;
	GtkWidget *order_or_range_menu_button;
	GtkWidget *order_or_range_menu_label;
        GtkStatusIcon *trayIcon;
	GtkWidget *next_eventbox;
	GtkWidget *list_eventbox;
	char ui_path[]="/home/daihao/DEBIAN/";
	int windowstate=0;
gint show_tray_menu( GtkWidget *widget
,GdkEvent *event );
gboolean change_on_picture (GtkWidget *widget,GdkEventCrossing *event,GtkWidget *image);
gboolean change_off_picture (GtkWidget *widget,GdkEventCrossing *event,GtkWidget *image);
void show_or_hide() {
        if (windowstate) {gtk_widget_show_all(window);windowstate=0;}
        else {gtk_window_iconify (GTK_WINDOW(window) );gtk_widget_hide(window);windowstate=1;}
    }



GtkWidget * gui()
{       
        GtkWidget *bar_box;
        GtkActionGroup *actiongroup;
        GtkUIManager *ui_manager;
        
      // 窗口初始化
	gtk_window_set_title(GTK_WINDOW(window), "MyMP3");
	g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK	(delete_event),NULL);
	gtk_window_set_default_size(GTK_WINDOW(window),300,200);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	//gtk_window_set_decorated(GTK_WINDOW(window),FALSE); /*没有边框*/


//
	 gtk_window_set_icon(GTK_WINDOW(window),(GdkPixbuf *)gdk_pixbuf_new_from_file("~/DEBIAN/mymp3.png",NULL));//左侧栏图标
trayIcon=gtk_status_icon_new_from_file("~/DEBIAN/mymp3.png");//任务栏图标
 gtk_status_icon_set_visible(GTK_STATUS_ICON(trayIcon),TRUE);
 gtk_widget_set_app_paintable(window, TRUE);

g_signal_connect((gpointer) trayIcon ,"activate" ,G_CALLBACK(show_or_hide), NULL);	
g_signal_connect_swapped((gpointer)trayIcon ,"popup-menu" ,G_CALLBACK(show_tray_menu),NULL);


	box = gtk_box_new(TRUE,0);
	play_box = gtk_box_new(TRUE,0);
        title_label_box = gtk_box_new(TRUE,0);
	button_box = gtk_box_new(FALSE,0);
        
	
	gtk_container_add(GTK_CONTAINER(window),box);
	gtk_box_pack_start(GTK_BOX(box),play_box,FALSE,FALSE,0);
       
        
       //mp3标题设置
       title_label=gtk_label_new("");
	gtk_label_set_line_wrap(GTK_LABEL(title_label),TRUE);
        gtk_box_pack_start(GTK_BOX(title_label_box),title_label,FALSE,FALSE,20);
        gtk_box_pack_start(GTK_BOX(play_box),title_label_box,FALSE,FALSE,0);

//按钮

char dir[100];
	
	//last按钮设置
	strcpy(dir,ui_path);
	GtkWidget *prev_eventbox;
	GtkWidget *prev_image;
	prev_eventbox = gtk_event_box_new ();
	strcat(dir,"prev.png");
	prev_image=gtk_image_new_from_file(dir);
	gtk_container_add (GTK_CONTAINER (prev_eventbox),prev_image);
	g_signal_connect(G_OBJECT(prev_eventbox),"button_press_event",G_CALLBACK(play_last),NULL);
	gtk_box_pack_start(GTK_BOX(button_box),prev_eventbox,FALSE,FALSE,0);

	//stop按钮设置
	strcpy(dir,ui_path);
	GtkWidget *stop_eventbox;
	GtkWidget *stop_image;
	stop_eventbox = gtk_event_box_new ();
	strcat(dir,"stop.png");
	stop_image=gtk_image_new_from_file(dir);
	gtk_container_add (GTK_CONTAINER (stop_eventbox),stop_image);
	g_signal_connect(G_OBJECT(stop_eventbox),"button_press_event",G_CALLBACK(stop_pressed),stop_image);
	gtk_box_pack_start(GTK_BOX(button_box),stop_eventbox,FALSE,FALSE,0);

	//next按钮设置
	strcpy(dir,ui_path);
	
	GtkWidget *next_image;
	next_eventbox = gtk_event_box_new ();
	strcat(dir,"next.png");
	next_image=gtk_image_new_from_file(dir);
	gtk_container_add (GTK_CONTAINER (next_eventbox),next_image);
	/*g_signal_connect ((gpointer) next_eventbox,    "enter_notify_event",G_CALLBACK (change_on_picture),(gpointer)next_image);
  	g_signal_connect ((gpointer) next_eventbox,    "leave_notify_event",G_CALLBACK (change_off_picture),(gpointer)next_image);*/
	g_signal_connect(G_OBJECT(next_eventbox),"button_press_event",G_CALLBACK(play_next),NULL);
	
	//g_signal_connect(G_OBJECT(next_eventbox),"event",G_CALLBACK (change_picture),(gpointer)next_image);
	gtk_box_pack_start(GTK_BOX(button_box),next_eventbox,FALSE,FALSE,0);
	
	//play按钮设置
	strcpy(dir,ui_path);
	GtkWidget *play_eventbox;
	GtkWidget *play_image;
	play_eventbox = gtk_event_box_new ();
	strcat(dir,"play.png");
	play_image=gtk_image_new_from_file(dir);
	gtk_container_add (GTK_CONTAINER (play_eventbox),play_image);
	g_signal_connect(G_OBJECT(play_eventbox),"button_press_event",G_CALLBACK(pause_pressed),play_image);
	gtk_box_pack_end(GTK_BOX(button_box),play_eventbox,FALSE,FALSE,15);
	

        gtk_box_pack_start(GTK_BOX(play_box),button_box,FALSE,FALSE,0);

 	//当前播放状态标签 
        bar_box = gtk_box_new(TRUE,0);  
        status_label = gtk_label_new("Stopped");
        gtk_box_pack_start(GTK_BOX(bar_box),status_label,TRUE, TRUE, 0);
        time_label = gtk_label_new("—:—:—");    //播放时间标签
        gtk_box_pack_start(GTK_BOX(bar_box), time_label, TRUE, TRUE, 0);
  

	//播放进度条设置
         
        bar = gtk_scale_new_with_range(FALSE,0, 100, 1);
        gtk_scale_set_draw_value(GTK_SCALE(bar),FALSE);
        gtk_widget_set_sensitive(bar, FALSE);
        gtk_box_pack_start(GTK_BOX(bar_box),bar,FALSE,FALSE,0);
       gtk_box_pack_start(GTK_BOX(play_box),bar_box,FALSE,FALSE,10);

       
	//顺序or随机播放标签
	GtkWidget *order_or_range_menu_box;
	order_or_range_menu_box = gtk_box_new(FALSE,0);

	strcpy(dir,ui_path);
	GtkWidget *config_eventbox;
	GtkWidget *config_image;
	config_eventbox = gtk_event_box_new ();
	strcat(dir,"button.config.png");
	config_image=gtk_image_new_from_file(dir);
	gtk_container_add (GTK_CONTAINER (config_eventbox),config_image);
	g_signal_connect_swapped(G_OBJECT(config_eventbox),"button_press_event",G_CALLBACK(make_config_ui),NULL);
	 switch(order_or_range)
	 {
		 case 0 :order_or_range_menu_label=gtk_label_new("正序");break;
		 case 1 :order_or_range_menu_label=gtk_label_new("随机");break;
		case 2 :order_or_range_menu_label=gtk_label_new("单曲");break;
	 }
       gtk_box_pack_start (GTK_BOX (order_or_range_menu_box), config_eventbox, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(order_or_range_menu_box), order_or_range_menu_label, TRUE, TRUE, 0);
       gtk_box_pack_start(GTK_BOX(play_box),order_or_range_menu_box,FALSE,FALSE,0);
	
    
      //播放列表按钮
	strcpy(dir,ui_path);
	
	GtkWidget *list_image;
	list_eventbox = gtk_event_box_new ();
	strcat(dir,"button.list.png");
	list_image=gtk_image_new_from_file(dir);
	gtk_container_add (GTK_CONTAINER (list_eventbox),list_image);
	/*g_signal_connect ((gpointer)list_eventbox,    "enter_notify_event",G_CALLBACK (change_on_picture),(gpointer)list_image);
  	g_signal_connect ((gpointer) list_eventbox,    "leave_notify_event",G_CALLBACK (change_off_picture),(gpointer)list_image);*/
	g_signal_connect_swapped(G_OBJECT(list_eventbox),"button_press_event",G_CALLBACK(make_list_ui),NULL);
	gtk_box_pack_end(GTK_BOX(order_or_range_menu_box),list_eventbox,FALSE,TRUE,0);
	
        
}

	//播放信息更新
void info_update()
{    strcpy(mp3name,filename);
    gtk_label_set_text(GTK_LABEL(title_label), 1 + strrchr(mp3name, '/'));

}

void order_menu_choose()
{	gtk_label_set_text(GTK_LABEL(order_or_range_menu_label),"正序");
	order_or_range = 0;
}


void range_menu_choose()
{	gtk_label_set_text(GTK_LABEL(order_or_range_menu_label),"随机");
	order_or_range = 1;
}

void onelast_menu_choose()
{	gtk_label_set_text(GTK_LABEL(order_or_range_menu_label),"单曲");
	order_or_range = 2;
}

gint file_add_menu( GtkWidget *widget
,GdkEvent *event )
{
if (event->type == GDK_BUTTON_PRESS) {GdkEventButton *bevent = (GdkEventButton *) event;
gtk_menu_popup (GTK_MENU (widget), NULL, NULL, NULL, NULL,
bevent->button, bevent->time);
//告诉调用代码我们已经处理了这个事件;事件传播(buck)在这里停止。
return TRUE;
}
//告诉调用代码我们没有处理这个事件;继续传播它
return FALSE;
}


gint show_tray_menu( GtkWidget *widget
,GdkEvent *event )
{
	GtkWidget *menu_pause;
	GtkWidget *menu_quit;
	GtkWidget * tray_menu = gtk_menu_new();
	menu_pause = gtk_menu_item_new_with_label("暂停");
	gtk_menu_shell_append(GTK_MENU_SHELL(tray_menu),menu_pause);
	g_signal_connect(G_OBJECT(menu_pause),"activate",G_CALLBACK(pause_pressed),NULL);
	gtk_widget_show (menu_pause);

	GtkWidget *menu_next = gtk_menu_item_new_with_label("下一首");
	gtk_menu_shell_append(GTK_MENU_SHELL(tray_menu),menu_next);
	g_signal_connect(G_OBJECT(menu_next),"activate",G_CALLBACK(play_next),NULL);
	gtk_widget_show (menu_next);

	GtkWidget *menu_last = gtk_menu_item_new_with_label("上一首");
	gtk_menu_shell_append(GTK_MENU_SHELL(tray_menu),menu_last);
	g_signal_connect(G_OBJECT(menu_last),"activate",G_CALLBACK(play_last),NULL);
	gtk_widget_show (menu_last);
        menu_quit = gtk_menu_item_new_with_label("退出");
	gtk_menu_shell_append(GTK_MENU_SHELL(tray_menu),menu_quit);
	g_signal_connect(G_OBJECT(menu_quit),"activate",G_CALLBACK(delete_event),NULL);
	gtk_widget_show (menu_quit);
	
	gtk_widget_show (tray_menu);
//if (event->type == GDK_BUTTON_PRESS){
gtk_widget_show_all(tray_menu);
gtk_menu_popup (GTK_MENU (tray_menu), NULL, NULL, 0, NULL,0,0);
return TRUE;
//}
//else return FALSE;
}


//改变图标
gboolean change_on_picture (GtkWidget *widget,GdkEventCrossing *event,GtkWidget *image)
{	char dir[100];
	strcpy(dir,ui_path);
	strcat(dir,"button.list.on.png");
	GtkWidget *new_image;
	gtk_container_remove(GTK_CONTAINER(list_eventbox),GTK_WIDGET(image));
  	new_image = gtk_image_new_from_file(dir);
  	gtk_widget_show(new_image);
  	gtk_container_add(GTK_CONTAINER(list_eventbox),new_image);
	return FALSE;
}

gboolean change_off_picture (GtkWidget *widget,GdkEventCrossing *event,GtkWidget *image)
{	char dir[100];
	strcpy(dir,ui_path);
	strcat(dir,"button.list.png");
	GtkWidget *new_image;
	gtk_container_remove(GTK_CONTAINER(list_eventbox),GTK_WIDGET(image));
  	new_image = gtk_image_new_from_file(dir);
  	gtk_widget_show(new_image);
  	gtk_container_add(GTK_CONTAINER(list_eventbox),new_image);
	return FALSE;
}
