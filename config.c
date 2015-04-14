#include "mp.h" 

gchar config_path[] = "/home/daihao/.my-mp3/config";

int readconfig()
{
	FILE *fp;
	fp=fopen(config_path,"r");
   char ch;
   int i=0;
   ch=fgetc(fp);
   while(ch!=EOF)   
   {	if(ch == '\n')i++;
	   if(i == 2)
	   {	ch=fgetc(fp); 
		   order_or_range=ch-48;
	   }
	   if(i == 4)
	   {    ch=fgetc(fp);
		   play_now_list=ch-48;
	   }
	    ch=fgetc(fp);
   }
	fclose(fp);
	return 0;
}

//配置界面设置

GtkWidget *configbox;

void make_config_ui()
{	void close_config_ui();	
	GtkWidget * button_box;
	GtkWidget * button;
	gtk_widget_hide(play_box);//隐藏播放界面
	configbox = gtk_box_new(TRUE, 0);
	GtkWidget *play_mod_box;
	play_mod_box = gtk_box_new(FALSE, 0); 
	GtkWidget *play_mod_label_box;
	play_mod_label_box = gtk_box_new(TRUE, 0); 
	GSList * play_mod_group=NULL;
GtkWidget *play_mod_1=gtk_radio_button_new_with_mnemonic(NULL, "正序");

GtkWidget *play_mod_2 = gtk_radio_button_new_with_mnemonic(NULL, "随机");

GtkWidget *play_mod_3 = gtk_radio_button_new_with_mnemonic(NULL, "单曲");

GtkWidget * play_mod_label=gtk_label_new("播放模式");
gtk_box_pack_start(GTK_BOX(play_mod_label_box), play_mod_label, TRUE, TRUE, 0);
gtk_box_pack_start(GTK_BOX(configbox),play_mod_label_box,TRUE,TRUE,0);
int i,n=order_or_range;
i=n;
do{
	switch(i)
	{	case 0 :gtk_radio_button_set_group(GTK_RADIO_BUTTON(play_mod_1),play_mod_group);
		play_mod_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(play_mod_1));
		gtk_box_pack_start(GTK_BOX(play_mod_box), play_mod_1, FALSE, FALSE, 0);g_signal_connect (G_OBJECT (play_mod_1),"clicked",G_CALLBACK (order_menu_choose),NULL);break;
		case 1 :gtk_radio_button_set_group(GTK_RADIO_BUTTON(play_mod_2),play_mod_group);
		play_mod_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(play_mod_2));
		gtk_box_pack_start(GTK_BOX(play_mod_box), play_mod_2, FALSE, FALSE, 0);g_signal_connect (G_OBJECT (play_mod_2),"clicked",G_CALLBACK (range_menu_choose),NULL);break;
		case 2 :gtk_radio_button_set_group(GTK_RADIO_BUTTON(play_mod_3),play_mod_group);
		play_mod_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(play_mod_3));
		gtk_box_pack_start(GTK_BOX(play_mod_box), play_mod_3, FALSE, FALSE, 0);g_signal_connect (G_OBJECT (play_mod_3),"clicked",G_CALLBACK (onelast_menu_choose),NULL);break;}
	i=(i+1)%3;}
while(i!=n);
	gtk_box_pack_start(GTK_BOX(configbox),play_mod_box,FALSE,FALSE,0);
	button_box = gtk_box_new(FALSE,0);
	button = gtk_button_new_with_label("返回");
	gtk_box_pack_end(GTK_BOX(button_box),button,FALSE,FALSE,0);
	g_signal_connect_swapped (G_OBJECT (button), "clicked",G_CALLBACK (close_config_ui),NULL);
	gtk_box_pack_end(GTK_BOX(configbox),button_box,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(box),configbox,TRUE,TRUE,0);
        gtk_widget_show_all(configbox);
}

void close_config_ui()
{	
	gtk_widget_destroy(configbox);
        gtk_widget_show_all(play_box);
 }


int writeconfig()
{	
	FILE *fp;
	fp=fopen(config_path,"w");
   char ch;
	char a[]="##这里是mymp3的相关配置";
	char b[]="1.播放模式？（0.顺序；1.随机；2.单曲）";
	char c[]="##上次播放的曲目";
	fputs(a,fp);fputc('\n',fp);
	fputs(b,fp);fputc('\n',fp);
	ch=order_or_range+48;
	fputc(ch,fp);putc('\n',fp);
	fputs(c,fp);fputc('\n',fp);
	ch=play_now_list+48;
	fputc(ch,fp);
	fclose(fp);
	return 0;
}
