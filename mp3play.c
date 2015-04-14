#include "mp.h"
static GstElement *pipeline;


int stop_update=0;



//定义消息处理函数,
static gboolean
bus_call(GstBus *bus,
        GstMessage *msg,
        gpointer data)
{
    GMainLoop *loop = (GMainLoop *) data;//这个是主循环的指针，在接受EOS消息时退出循环
    switch (GST_MESSAGE_TYPE(msg)){
        case GST_MESSAGE_EOS:
            g_print("End of stream\n");
            g_main_loop_quit(loop);
            break;
        case GST_MESSAGE_ERROR:{
                           gchar *debug;
                           GError *error;

                           gst_message_parse_error(msg,&error,&debug);
                           g_free(debug);
                           g_printerr("ERROR:%s\n",error->message);
                           g_error_free(error);
                           g_main_loop_quit(loop);
                           break;
                       }
        default:     break;
    }
    return TRUE;
}

int start()
{  
    GMainLoop *loop;
    GstElement *source,*decoder,*sink;//定义组件
    GstBus *bus;
    loop = g_main_loop_new(NULL,FALSE);//创建主循环，在执行 g_main_loop_run后正式开始循环

    //创建管道和组件
    pipeline = gst_pipeline_new("audio-player");
    source = gst_element_factory_make("filesrc","file-source");
    decoder = gst_element_factory_make("mad","mad-decoder");
    sink = gst_element_factory_make("autoaudiosink","audio-output");

    if(!pipeline||!source||!decoder||!sink){
        g_printerr("One element could not be created.Exiting.\n");
         if(!pipeline)
             g_printerr("pipeline could not be created.Exiting.\n");
         if(!source)
             g_printerr("source could not be created.Exiting.\n");
         if(!decoder)
             g_printerr("decoder could not be created.Exiting.\n");
        return -1;
    }
    //设置 source的location 参数。即 文件地址.
    g_object_set(G_OBJECT(source),"location",filename,NULL);
    //得到 管道的消息总线
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
   //添加消息监视器
    gst_bus_add_watch(bus,bus_call,loop);
    gst_object_unref(bus);
    //把组件添加到管道中.管道是一个特殊的组件，可以更好的让数据流动
    gst_bin_add_many(GST_BIN(pipeline),source,decoder,sink,NULL);
   //依次连接组件
   gst_element_link_many(source,decoder,sink,NULL);
   //开始播放
    gst_element_set_state(pipeline,GST_STATE_PLAYING);
    g_print("Running...\n");
    g_timeout_add(1000,(GSourceFunc)update_time_callback,NULL);
    //开始循环
    g_main_loop_run(loop);   
    g_print("Returned,stopping playback\n");
    gst_element_set_state(pipeline,GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
    return 0;
}

//获取时间长度和当前状态
gboolean update_time_callback()
{             
                if(stop_update) return FALSE;
             else{
		GstFormat fmt = GST_FORMAT_TIME;
		gint64 pos, len;
                gchar time_buffer[25];
		if(gst_element_query_position (pipeline, &fmt, &pos) && 
		gst_element_query_duration (pipeline, &fmt, &len)) {
		gtk_range_set_value(GTK_RANGE(bar),((gdouble)pos / (gdouble)len) * 100.0);
	        g_snprintf(time_buffer, 24,"%u:%02u:%02u", GST_TIME_ARGS(pos));
		gtk_label_set_text(GTK_LABEL(time_label), time_buffer);

		}
         return TRUE;
           }
}
//暂停
void pause_pressed()
{
    GstState state;
    gst_element_get_state(pipeline, &state, NULL, -1);
    if(state == GST_STATE_PLAYING)
    {
    gst_element_set_state(pipeline, GST_STATE_PAUSED);
    gtk_label_set_text(GTK_LABEL(status_label),"Paused");
    }
    else if(state == GST_STATE_PAUSED)
    {
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    gtk_label_set_text(GTK_LABEL(status_label),"Playing");
	
    }
    else if(state == GST_STATE_PLAYING || state == GST_STATE_PAUSED)
	return;
    else
	{play_choose_list_on=1;
     playlist();
  //   gtk_label_set_text(GTK_LABEL(status_label),"Playing");
        }
}

//停止
void stop_pressed()
{    if(pipeline != NULL) {
	gst_element_set_state(pipeline,GST_STATE_NULL);
        gst_object_unref(GST_OBJECT(pipeline));
        gtk_label_set_text(GTK_LABEL(status_label),"Stopped");
        stop_update=1;
        gtk_label_set_text(GTK_LABEL(time_label),"—:—.—");
        gtk_range_set_value(GTK_RANGE(bar),0);
	}                        
}



//结束程序
gint delete_event()
{   writeconfig();
    gst_element_set_state(pipeline,GST_STATE_NULL);
    gst_object_unref (GST_OBJECT(pipeline));
    gtk_main_quit();  
    gtk_widget_destroy(window);
    exit(0);
	return TRUE;
}

//顺序/随机播放
int playlist()
{ 
	
	switch(order_or_range){
	case 0 :
	while(TRUE)
	{  
		strcpy(filename,file_list[play_now_list].lr3);
                info_update();
                stop_update=0;
                gtk_label_set_text(GTK_LABEL(status_label),"Playing");
		start();
                play_now_list=(play_now_list+1)%list_length;
	};break;

	case 1 :
	while(TRUE)
	{    if(play_choose_list_on == 0)  
		play_now_list=rand()%list_length;
		play_choose_list_on=0;
		strcpy(filename,file_list[play_now_list].lr3);
                info_update();
                stop_update=0;
                gtk_label_set_text(GTK_LABEL(status_label),"Playing");
		start();
	};break;

	case 2 :
	while(TRUE)
	{	strcpy(filename,file_list[play_now_list].lr3);
                info_update();
                stop_update=0;
                gtk_label_set_text(GTK_LABEL(status_label),"Playing");
		start();
		};break;
       }
    return 0;
}

//下一曲
gboolean play_next(GtkWidget *widget,GdkEventButton *event)
{   
	play_now_list=(play_now_list+1)%list_length;
	stop_pressed();
	playlist();
	return TRUE;
}

//上一曲
void play_last()
{    play_now_list=(play_now_list-1+list_length)%list_length;
	stop_pressed();
	playlist();
}
