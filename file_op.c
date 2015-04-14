#include "mp.h"
gchar playlist_path[]="/home/daihao/.my-mp3/playlist";
void file_open();
void write_list_back();
void floder_open(GtkAction *action);
gint file_list_change=0;
gint handler_id;
GtkTreeSelection *selection;
void delete_list();
void clear_list();

//读取播放列表文件名
void readlist()
{   FILE *fp;
    fp=fopen(playlist_path,"r");
   char ch;
   int i=0,j=0;
   ch=fgetc(fp);
   while(ch!=EOF)
   {  	if(ch != '\n')
         { file_list[i].lr3[j++]=ch;}
        else
          {  file_list[i].lr3[j]='\0';
             j=0;
             i++;
          }
        ch=fgetc(fp);
   }
 list_length = i;
   fclose(fp);
}


//播放列表的ui界面

gint file_add_menu(GtkWidget *widget,GdkEvent *event);
enum{LIST_ITEM = 0,N_COLUMNS};
//初始化列表栏
static void init_list(GtkWidget *list){
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("List Items",renderer, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list),GTK_TREE_MODEL(store));
	g_object_unref(store);
}
//添加列表栏
static void add_to_list(GtkWidget *list, const gchar *str){
	GtkListStore *store;
	GtkTreeIter iter;
	store =GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
}
//播放选中列表
void play_choose_list(GtkWidget *widget) {
	g_signal_handler_disconnect(selection,handler_id);//解除绑定change和play
	GtkTreeIter iter;
	GtkTreeModel *model;
	char *value;
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget),&model, &iter)) 
{
	gtk_tree_model_get(model, &iter, LIST_ITEM, &value,-1);
	int i=0,n;	
	while(value[i]!='.')i++;
	value[i]='\0';
	sscanf(value,"%d",&n);
	play_now_list=n-1;
	stop_pressed();
	play_choose_list_on=1;
	playlist();
	g_free(value);
}
}

//生成右击菜单栏
void show_list_op_menu()
{	GtkWidget *menu_delete;
	GtkWidget *menu_delete_all;
	GtkWidget *menu_move;
	GtkWidget * list_op_menu = gtk_menu_new();
	menu_delete = gtk_menu_item_new_with_label("删除");
	gtk_menu_shell_append(GTK_MENU_SHELL(list_op_menu),menu_delete);
	gtk_widget_show (menu_delete);
	g_signal_connect(G_OBJECT(menu_delete),"activate",G_CALLBACK(delete_list),NULL);
	menu_move = gtk_menu_item_new_with_label("移动");
	gtk_menu_shell_append(GTK_MENU_SHELL(list_op_menu),menu_move);
	gtk_widget_show (menu_move);
	menu_delete_all = gtk_menu_item_new_with_label("清空");
	gtk_menu_shell_append(GTK_MENU_SHELL(list_op_menu),menu_delete_all);
	gtk_widget_show (menu_delete_all);
	g_signal_connect(G_OBJECT(menu_delete_all),"activate",G_CALLBACK(clear_list),NULL);
	gtk_widget_show_all(list_op_menu);
	gtk_menu_popup (GTK_MENU (list_op_menu),NULL,NULL,0,NULL,0,0);	
}

//列表左右击事件选择
gboolean popmenu_or_play(GtkWidget* widget,GdkEventButton* event)
{	
	if (event->type == GDK_BUTTON_PRESS)
	{	switch(event->button)
		{		
		case 1 : handler_id = 
g_signal_connect(selection, "changed",G_CALLBACK(play_choose_list),NULL);break;//绑定change和play
		case 3 : show_list_op_menu();break;
		}
	return FALSE;
	}
}

	GtkWidget *listbox;
	GtkWidget *sw;//滑动条
	GtkTreeSelection *selection;
	GtkWidget * button_box;
	GtkWidget * button;
	GtkWidget *list;
void make_list_ui(){
	
	void close_list_ui(GtkWidget *list);
	gchar mp3name[500];
	gchar show_label[500];
        gtk_widget_hide(play_box);//隐藏播放界面
	list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);
	listbox = gtk_box_new(TRUE, 0);
	sw = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw),
	GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start(GTK_BOX(listbox), sw, TRUE, TRUE,5);
	gtk_container_add(GTK_CONTAINER (sw), list);
	int i,m,n=0,j=0;
	init_list(list);
	for(i=1;i<list_length+1;i++)
	{   strcpy(mp3name,file_list[i-1].lr3);
	if(i>99)
	{   m=i;
	  while(m>9)
	  {  	n=2;j=n;
		show_label[n--]=m % 10 +48;
		m=m/10;
	  }
	show_label[0]=m+48;
	}
	else  if(i>9)
	{   m=i;
	  while(m>9)
	  {  	n=1;j=n;
		show_label[n--]=m % 10 +48;
		m=m/10;
	  }
	show_label[0]=m+48;
	}
	else show_label[j]=i+48;
	show_label[j+1]='.';show_label[j+2]='\0';
		strcat(show_label,1 + strrchr(mp3name, '/'));
		add_to_list(list,show_label);
	}
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
	
	g_signal_connect(G_OBJECT(list),"button-press-event",G_CALLBACK(popmenu_or_play),selection);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit), NULL);
	button_box = gtk_box_new(FALSE,0);
	button = gtk_button_new_with_label("添加");
	gtk_box_pack_start(GTK_BOX(button_box),button,FALSE,TRUE,0);
 
	GtkWidget * menu = gtk_menu_new();
        GtkWidget *menu_Open_file = gtk_menu_item_new_with_label("选择文件");
	g_signal_connect(G_OBJECT(menu_Open_file),"activate",G_CALLBACK(file_open),NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menu_Open_file);
gtk_widget_show (menu_Open_file);
        GtkWidget *menu_Open_folder = gtk_menu_item_new_with_label("添加文件夹");
	g_signal_connect(G_OBJECT(menu_Open_folder),"activate",G_CALLBACK(floder_open),NULL);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu),menu_Open_folder);
gtk_widget_show (menu_Open_folder);
	g_signal_connect_swapped (G_OBJECT (button), "event",G_CALLBACK (file_add_menu),menu);

	button = gtk_button_new_with_label("返回");
	gtk_box_pack_end(GTK_BOX(button_box),button,FALSE,FALSE,0);
	g_signal_connect_swapped (G_OBJECT (button), "clicked",G_CALLBACK (close_list_ui),list);
	gtk_box_pack_end(GTK_BOX(listbox),button_box,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(box),listbox,TRUE,TRUE,0);
        gtk_widget_show_all(listbox);
}

void destroy() {
    gtk_main_quit();
}



void close_list_ui(GtkWidget *list)
{	if(file_list_change) write_list_back();
	gtk_widget_destroy(listbox);
        gtk_widget_show_all(play_box);
 }

//播放列表ui代码到此为止


//文件打开添加列表


void file_open()
{  GSList *op_list;
	gchar mp3name[500];
	gchar show_label[500];
GtkWidget *file_chooser = gtk_file_chooser_dialog_new("Open File",
GTK_WINDOW(window),GTK_FILE_CHOOSER_ACTION_OPEN,
GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,GTK_STOCK_OPEN,
GTK_RESPONSE_ACCEPT,NULL);
if (gtk_dialog_run(GTK_DIALOG(file_chooser)) == GTK_RESPONSE_ACCEPT)
{

op_list = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(file_chooser));
//list是一个结构体，成员data即文件名
int m,n=0,j=0;
strcpy(file_list[list_length].lr3,op_list->data);
strcpy(mp3name,file_list[list_length].lr3);
	if(list_length+1>99)
	{   m=list_length+1;
	  while(m>9)
	  {  	n=2;j=n;
		show_label[n--]=m % 10 +48;
		m=m/10;
	  }
	show_label[0]=m+48;
	}
	else  if(list_length+1>9)
	{   m=list_length+1;
	  while(m>9)
	  {  	n=1;j=n;
		show_label[n--]=m % 10 +48;
		m=m/10;
	  }
	show_label[0]=m+48;
	}
	else show_label[j]=list_length+1+48;
	show_label[j+1]='.';show_label[j+2]='\0';
		strcat(show_label,1 + strrchr(mp3name, '/'));
		add_to_list(list,show_label);
list_length++;
}
file_list_change=1;
gtk_widget_destroy(file_chooser);
}

//文件夹打开添加列表
void floder_open(GtkAction *action)
{	gchar mp3name[500];
	gchar *path;
	gchar path3[1000];
	gchar show_label[500];
	int m,n,j;
GtkWidget *file_chooser = gtk_file_chooser_dialog_new("Open Floder",
GTK_WINDOW(window),GTK_FILE_CHOOSER_ACTION_OPEN,
GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,GTK_STOCK_OPEN,
GTK_RESPONSE_ACCEPT,NULL);
if (gtk_dialog_run((GtkDialog *) file_chooser) == GTK_RESPONSE_ACCEPT)
{
path = gtk_file_chooser_get_current_folder((GtkFileChooser *) file_chooser);
	DIR * dir;
                struct dirent * ptr;
                dir=opendir(path);
                ptr=readdir(dir);
		while (ptr !=NULL)
		{   if ( strstr(ptr->d_name,".MP3")||strstr(ptr->d_name,".WMA")||strstr(ptr->d_name,".OGG")||strstr(ptr->d_name,".WAV")||strstr(ptr->d_name,".APE")||strstr(ptr->d_name,".FLAC")||strstr(ptr->d_name,".ACC")||strstr(ptr->d_name,".OGV")||strstr(ptr->d_name,".WMV")||strstr(ptr->d_name,".mp3")||strstr(ptr->d_name,".wma")||strstr(ptr->d_name,".ogg")||strstr(ptr->d_name,".wav")||strstr(ptr->d_name,".ape")||strstr(ptr->d_name,".flac")||strstr(ptr->d_name,".acc")||strstr(ptr->d_name,".ogv")||strstr(ptr->d_name,".wmv")){
                        strcpy(path3,path);
                        strcat(path3,"/");
                        strcat(path3,ptr->d_name);
			n=0;j=0;
strcpy(file_list[list_length].lr3,path3);
strcpy(mp3name,file_list[list_length].lr3);
	if(list_length+1>99)
	{   m=list_length+1;
	  while(m>9)
	  {  	n=2;j=n;
		show_label[n--]=m % 10 +48;
		m=m/10;
	  }
	show_label[0]=m+48;
	}
	else  if(list_length+1>9)
	{   m=list_length+1;
	  while(m>9)
	  {  	n=1;j=n;
		show_label[n--]=m % 10 +48;
		m=m/10;
	  }
	show_label[0]=m+48;
	}
	else show_label[j]=list_length+1+48;
	show_label[j+1]='.';show_label[j+2]='\0';
		strcat(show_label,1 + strrchr(mp3name, '/'));
		add_to_list(list,show_label);
list_length++;
			}
		 ptr=readdir(dir);
		}
	 closedir(dir);
}
file_list_change=1;
gtk_widget_destroy(file_chooser);
}

//删除
void delete_list()
{	GtkTreeIter iter;
	GtkTreeModel *model;
	char *value;
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection),&model, &iter)) 
{
	gtk_tree_model_get(model, &iter, LIST_ITEM, &value,-1);
	int i=0,n;	
	while(value[i]!='.')i++;
	value[i]='\0';
	sscanf(value,"%d",&n);
	for(i=n-1;i<list_length-1;i++)
	{strcpy(file_list[i].lr3,file_list[i+1].lr3);}
	list_length--;
	gtk_widget_destroy(listbox);
	make_list_ui();
	g_free(value);
	file_list_change=1;
}
}

//清空
void clear_list()
{	list_length=0;
	gtk_widget_destroy(listbox);
	make_list_ui();
	file_list_change=1;
}

//写列表
void write_list_back()
{	int i;	
	FILE *fp;
	fp=fopen(playlist_path,"w");
	for(i=0;i<list_length;i++)
	{  fputs(file_list[i].lr3,fp);
		fputc('\n',fp);
	}
	fclose(fp);
}
