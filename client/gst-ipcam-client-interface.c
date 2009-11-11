/**
 * \file:   gst-ipcam-client-interface.c
 * \author: Dam Quang Tuan <damquang.tuan@nomovok.com>
 *
 * \date 8-26-2009
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "gst-ipcam-client-callbacks.h"
#include "gst-ipcam-client-interface.h"
#include "gst-ipcam-client-support.h"
#include "gst-ipcam-client-windowid.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

/**
 * create the main window
 *
 * @param void
 *
 * @return nothing
 */
GtkWidget*
gst_ipcam_client_create_main_window(void)
{
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gchar * main_window_title = gst_ipcam_client_window_create_title(_("Gst Ipcam Client"));
	gtk_window_set_title(GTK_WINDOW(main_window), main_window_title);
	g_free(main_window_title);
	gtk_window_set_default_size(GTK_WINDOW(main_window), 730, 50);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox);
	gtk_container_add(GTK_CONTAINER(main_window), vbox);

	toolbar = gtk_toolbar_new();
	gtk_widget_show(toolbar);
	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);
	tmp_toolbar_icon_size = gtk_toolbar_get_icon_size(GTK_TOOLBAR(toolbar));

	toolitem_connect = (GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem_connect);
	gtk_container_add(GTK_CONTAINER(toolbar), toolitem_connect);

	btn_connect = gtk_button_new_from_stock("gtk-connect");
	gtk_widget_show(btn_connect);
	gtk_container_add(GTK_CONTAINER(toolitem_connect), btn_connect);

	btn_disconnect = gtk_button_new_from_stock("gtk-disconnect");
	gtk_widget_set_sensitive(btn_disconnect, FALSE);

	toolitem_pause = (GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem_pause);
	gtk_container_add(GTK_CONTAINER(toolbar), toolitem_pause);

	btn_pause = gtk_button_new_from_stock("gtk-media-pause");
	gtk_widget_show(btn_pause);
	gtk_widget_set_sensitive(btn_pause, FALSE);
	gtk_container_add(GTK_CONTAINER(toolitem_pause), btn_pause);

	btn_resume = gtk_button_new();
	gtk_widget_set_sensitive(btn_resume, FALSE);

	algn_resume = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_widget_show(algn_resume);
	gtk_container_add(GTK_CONTAINER(btn_resume), algn_resume);

	hbox_resume_button = gtk_hbox_new(FALSE, 2);
	gtk_widget_show(hbox_resume_button);
	gtk_container_add(GTK_CONTAINER(algn_resume), hbox_resume_button);

	img_resume = gtk_image_new_from_stock("gtk-media-play", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show(img_resume);
	gtk_box_pack_start(GTK_BOX(hbox_resume_button), img_resume, FALSE, FALSE, 0);

	lbl_resume_button = gtk_label_new_with_mnemonic("_Resume");
	gtk_widget_show(lbl_resume_button);
	gtk_box_pack_start(GTK_BOX(hbox_resume_button), lbl_resume_button, FALSE, FALSE, 0);

	toolitem_about = (GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem_about);
	gtk_container_add(GTK_CONTAINER(toolbar), toolitem_about);

	btn_about = gtk_button_new_from_stock("gtk-about");
	gtk_widget_show(btn_about);
	gtk_container_add(GTK_CONTAINER(toolitem_about), btn_about);
	
	toolitem_quit = (GtkWidget*) gtk_tool_item_new();
	gtk_tool_item_set_expand (GTK_TOOL_ITEM(toolitem_quit), TRUE);
	gtk_widget_show(toolitem_quit);

	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(toolitem_quit), -1);

	toolitem_quit = (GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem_quit);
	btn_quit = gtk_button_new_from_stock("gtk-quit");
	gtk_widget_show(btn_quit);
	gtk_container_add(GTK_CONTAINER(toolitem_quit), btn_quit);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(toolitem_quit), -1);

	toolbar1 = gtk_toolbar_new();
	gtk_widget_show(toolbar1);
	gtk_widget_set_sensitive(toolbar1, FALSE);
	gtk_box_pack_start(GTK_BOX(vbox), toolbar1, FALSE, TRUE, 0);
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar1), GTK_TOOLBAR_BOTH);
	tmp_toolbar_icon_size = gtk_toolbar_get_icon_size(GTK_TOOLBAR(toolbar1));

	toolitem6 = (GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem6);
	gtk_container_add(GTK_CONTAINER(toolbar1), toolitem6);

	lbl_fps = gtk_label_new(_("Frame rate:"));
	gtk_widget_show(lbl_fps);
	gtk_container_add(GTK_CONTAINER(toolitem6), lbl_fps);

	toolitem7 = (GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem7);
	gtk_container_add(GTK_CONTAINER(toolbar1), toolitem7);

	cbx_entry_fps = gtk_combo_box_entry_new_text();
	gtk_widget_show(cbx_entry_fps);
	gtk_container_add(GTK_CONTAINER(toolitem7), cbx_entry_fps);
	gtk_widget_set_size_request(cbx_entry_fps, 70, -1);

	gtk_combo_box_append_text(GTK_COMBO_BOX(cbx_entry_fps), _("1/1"));
	gtk_combo_box_append_text(GTK_COMBO_BOX(cbx_entry_fps), _("5/1"));
	gtk_combo_box_append_text(GTK_COMBO_BOX(cbx_entry_fps), _("10/1"));
	gtk_combo_box_append_text(GTK_COMBO_BOX(cbx_entry_fps), _("15/1"));
	gtk_combo_box_append_text(GTK_COMBO_BOX(cbx_entry_fps), _("20/1"));
	gtk_combo_box_append_text(GTK_COMBO_BOX(cbx_entry_fps), _("25/1"));
	gtk_combo_box_append_text(GTK_COMBO_BOX(cbx_entry_fps), _("30/1"));

	toolitem3 = (GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem3);
	gtk_container_add(GTK_CONTAINER(toolbar1), toolitem3);

	alignment1 = gtk_alignment_new(0, 0, 1, 1);
	gtk_widget_show(alignment1);
	gtk_container_add(GTK_CONTAINER(toolitem3), alignment1);
	gtk_alignment_set_padding(GTK_ALIGNMENT(alignment1), 0, 0, 0, 20);

	lbl_fps_unit = gtk_label_new(_("(fps)"));
	gtk_widget_show(lbl_fps_unit);
	gtk_container_add(GTK_CONTAINER(alignment1), lbl_fps_unit);
	
	toolitem8 = (GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem8);
	gtk_container_add(GTK_CONTAINER(toolbar1), toolitem8);
	
	lbl_fsize = gtk_label_new(_("Frame size:"));
	gtk_widget_show(lbl_fsize);
	gtk_container_add(GTK_CONTAINER(toolitem8), lbl_fsize);

	toolitem11 = (GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem11);
	gtk_container_add(GTK_CONTAINER(toolbar1), toolitem11);
	
	cbx_entry_fsize = gtk_combo_box_entry_new_text();
	gtk_widget_show(cbx_entry_fsize);
	gtk_container_add(GTK_CONTAINER(toolitem11), cbx_entry_fsize);
	gtk_widget_set_size_request(cbx_entry_fsize, 90, -1);

	gtk_combo_box_append_text(GTK_COMBO_BOX(cbx_entry_fsize), _("320x240")); /*QVGA*/
	gtk_combo_box_append_text(GTK_COMBO_BOX(cbx_entry_fsize), _("640x480")); /*VGA*/
	gtk_combo_box_append_text(GTK_COMBO_BOX(cbx_entry_fsize), _("1280x720")); /*720P*/
	gtk_combo_box_append_text(GTK_COMBO_BOX(cbx_entry_fsize), _("1280x960")); /*Quad-VGA*/
	gtk_combo_box_append_text(GTK_COMBO_BOX(cbx_entry_fsize), _("1280x1024")); /*SXGA*/

	toolitem4 = (GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem4);
	gtk_container_add(GTK_CONTAINER(toolbar1), toolitem4);

	alignment2 = gtk_alignment_new(0, 0, 1, 1);
	gtk_widget_show(alignment2);
	gtk_container_add(GTK_CONTAINER(toolitem4), alignment2);
	gtk_alignment_set_padding(GTK_ALIGNMENT(alignment2), 0, 0, 0, 20);

	lbl_fsize_unit = gtk_label_new(_("(pixels)"));
	gtk_widget_show(lbl_fsize_unit);
	gtk_container_add(GTK_CONTAINER(alignment2), lbl_fsize_unit);
	
	toolitem9 = (GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem9);
	gtk_container_add(GTK_CONTAINER(toolbar1), toolitem9);

	lbl_bitrate = gtk_label_new(_("Bit rate:"));
	gtk_widget_show(lbl_bitrate);
	gtk_container_add(GTK_CONTAINER(toolitem9), lbl_bitrate);

	toolitem12 = (GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem12);
	gtk_container_add(GTK_CONTAINER(toolbar1), toolitem12);
	
	entry_bitrate = gtk_entry_new();
	gtk_widget_show(entry_bitrate);
	gtk_container_add(GTK_CONTAINER(toolitem12), entry_bitrate);
	gtk_widget_set_size_request(entry_bitrate, 120, -1);
	gtk_entry_set_invisible_char(GTK_ENTRY(entry_bitrate), 9679);

	toolitem5 = (GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem5);
	gtk_container_add(GTK_CONTAINER(toolbar1), toolitem5);

	alignment3 = gtk_alignment_new(0, 0, 1, 1);
	gtk_widget_show(alignment3);
	gtk_container_add(GTK_CONTAINER(toolitem5), alignment3);
	gtk_alignment_set_padding(GTK_ALIGNMENT(alignment3), 0, 0, 0, 20);

	lbl_bitrate_unit = gtk_label_new(_("(bps)"));
	gtk_widget_show(lbl_bitrate_unit);
	gtk_container_add(GTK_CONTAINER(alignment3), lbl_bitrate_unit);
	
	toolitem10 = (GtkWidget*) gtk_tool_item_new();
	gtk_tool_item_set_expand (GTK_TOOL_ITEM(toolitem10), TRUE);
	gtk_widget_show(toolitem10);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar1), GTK_TOOL_ITEM(toolitem10), -1);

	toolitem10 = (GtkWidget*) gtk_tool_item_new();
	gtk_widget_show(toolitem10);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar1), GTK_TOOL_ITEM(toolitem10), -1);

	btn_change = gtk_button_new_with_mnemonic(_("Change"));
	gtk_widget_show(btn_change);
	gtk_container_add(GTK_CONTAINER(toolitem10), btn_change);
	
	vbox_prw_video = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox_prw_video);
	gtk_box_pack_start(GTK_BOX(vbox), vbox_prw_video, TRUE, FALSE, 0);

	prw_video = gtk_drawing_area_new();
	gtk_widget_show(prw_video);
	gtk_box_pack_start(GTK_BOX(vbox_prw_video), prw_video, FALSE, TRUE, 0);
	
	status_bar = gst_ipcam_client_init_status_bar();
	gtk_widget_show(status_bar);
	gtk_box_pack_end(GTK_BOX(vbox), status_bar, FALSE, TRUE, 0);
	gtk_statusbar_set_has_resize_grip(GTK_STATUSBAR(status_bar), FALSE);

	g_signal_connect_swapped((gpointer) btn_connect, "clicked",
													 G_CALLBACK(gst_ipcam_client_on_btn_connect_clicked),
													 GTK_OBJECT(NULL));

	g_signal_connect_swapped((gpointer) btn_disconnect, "clicked",
													 G_CALLBACK(gst_ipcam_client_on_btn_disconnect_clicked),
													 GTK_OBJECT(NULL));

	g_signal_connect_swapped((gpointer) btn_pause, "clicked",
													 G_CALLBACK(gst_ipcam_client_on_btn_pause_clicked),
													 GTK_OBJECT(NULL));

	g_signal_connect_swapped((gpointer) btn_resume, "clicked",
													 G_CALLBACK(gst_ipcam_client_on_btn_resume_clicked),
													 GTK_OBJECT(NULL));

	g_signal_connect_swapped((gpointer) btn_about, "clicked",
													 G_CALLBACK(gst_ipcam_client_on_btn_about_clicked),
													 GTK_OBJECT(NULL));
	g_signal_connect_swapped((gpointer) btn_quit, "clicked",
													 G_CALLBACK(gst_ipcam_client_on_btn_quit_clicked),
													 GTK_OBJECT(NULL));
	g_signal_connect_swapped((gpointer) main_window, "destroy",
													 G_CALLBACK(gst_ipcam_client_on_main_window_destroy),
													 GTK_OBJECT(NULL));
	g_signal_connect_swapped((gpointer) btn_change, "clicked",
													 G_CALLBACK(gst_ipcam_client_on_btn_change_clicked),
													 GTK_OBJECT(NULL));

	/* Store pointers to all widgets, for use by lookup_widget(). */
	GLADE_HOOKUP_OBJECT_NO_REF(main_window, main_window, "main_window");
	GLADE_HOOKUP_OBJECT(main_window, vbox, "vbox");
	GLADE_HOOKUP_OBJECT(main_window, toolbar, "toolbar");
	GLADE_HOOKUP_OBJECT(main_window, toolitem_connect, "toolitem_connect");
	GLADE_HOOKUP_OBJECT(main_window, btn_connect, "btn_connect");
	GLADE_HOOKUP_OBJECT(main_window, btn_disconnect, "btn_disconnect");
	GLADE_HOOKUP_OBJECT(main_window, hbox_resume_button, "hbox_resume_button");
	GLADE_HOOKUP_OBJECT(main_window, lbl_resume_button, "lbl_resume_button");
	GLADE_HOOKUP_OBJECT(main_window, toolitem_pause, "toolitem_pause");
	GLADE_HOOKUP_OBJECT(main_window, btn_pause, "btn_pause");
	GLADE_HOOKUP_OBJECT(main_window, btn_resume, "btn_resume");
	GLADE_HOOKUP_OBJECT(main_window, toolitem_about, "toolitem_about");
	GLADE_HOOKUP_OBJECT(main_window, btn_about, "btn_about");
	GLADE_HOOKUP_OBJECT(main_window, toolitem_quit, "toolitem_quit");
	GLADE_HOOKUP_OBJECT(main_window, btn_quit, "btn_quit");
	GLADE_HOOKUP_OBJECT(main_window, vbox_prw_video, "vbox_prw_video");
	GLADE_HOOKUP_OBJECT(main_window, toolbar1, "toolbar1");
	GLADE_HOOKUP_OBJECT(main_window, toolitem3, "toolitem3");
	GLADE_HOOKUP_OBJECT(main_window, toolitem4, "toolitem4");
	GLADE_HOOKUP_OBJECT(main_window, toolitem5, "toolitem5");
	GLADE_HOOKUP_OBJECT(main_window, toolitem6, "toolitem6");
	GLADE_HOOKUP_OBJECT(main_window, toolitem7, "toolitem7");
	GLADE_HOOKUP_OBJECT(main_window, toolitem8, "toolitem8");
	GLADE_HOOKUP_OBJECT(main_window, toolitem9, "toolitem9");
	GLADE_HOOKUP_OBJECT(main_window, toolitem10, "toolitem10");
	GLADE_HOOKUP_OBJECT(main_window, toolitem11, "toolitem11");
	GLADE_HOOKUP_OBJECT(main_window, toolitem12, "toolitem12");
	GLADE_HOOKUP_OBJECT(main_window, alignment1, "alignment1");
	GLADE_HOOKUP_OBJECT(main_window, alignment2, "alignment2");
	GLADE_HOOKUP_OBJECT(main_window, alignment3, "alignment3");
	GLADE_HOOKUP_OBJECT(main_window, lbl_bitrate, "lbl_bitrate");
	GLADE_HOOKUP_OBJECT(main_window, btn_change, "btn_change");
	GLADE_HOOKUP_OBJECT(main_window, lbl_fps, "lbl_fps");
	GLADE_HOOKUP_OBJECT(main_window, lbl_fsize, "lbl_fsize");
	GLADE_HOOKUP_OBJECT(main_window, prw_video, "prw_video");
	GLADE_HOOKUP_OBJECT(main_window, status_bar, "status_bar");
	GLADE_HOOKUP_OBJECT(main_window, entry_bitrate, "entry_bitrate");
	
	return main_window;
}

/**
 * create the connection dialog
 *
 * @param void
 *
 * @return nothing
 */
GtkWidget*
gst_ipcam_client_create_connection_dialog(GtkWidget * parent_window)
{
	connection_dialog = gtk_dialog_new();
	gchar * connection_dialog_title = gst_ipcam_client_window_create_title(_("Connection"));
	gtk_window_set_title(GTK_WINDOW(connection_dialog), connection_dialog_title);
	g_free(connection_dialog_title);
	gtk_window_set_resizable(GTK_WINDOW(connection_dialog), FALSE);
	gtk_window_set_type_hint(GTK_WINDOW(connection_dialog), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_widget_set_size_request(connection_dialog, 290, -1);

	dialog_vbox = GTK_DIALOG(connection_dialog)->vbox;
	gtk_widget_show(dialog_vbox);

	tbl_con_info = gtk_table_new(1, 2, FALSE);
	gtk_widget_show(tbl_con_info);
	gtk_box_pack_start(GTK_BOX(dialog_vbox), tbl_con_info, TRUE, TRUE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(tbl_con_info), 3);
	gtk_table_set_row_spacings(GTK_TABLE(tbl_con_info), 4);
	gtk_table_set_col_spacings(GTK_TABLE(tbl_con_info), 11);

	lbl_url = gtk_label_new(_("Url"));
	gtk_widget_show(lbl_url);
	gtk_table_attach(GTK_TABLE(tbl_con_info), lbl_url, 0, 1, 0, 1,
									 (GtkAttachOptions) (GTK_FILL),
									 (GtkAttachOptions) (0), 0, 0);
	gtk_misc_set_alignment(GTK_MISC(lbl_url), 0, 0.5);

	entry_url = gtk_entry_new();
	gtk_widget_show(entry_url);
	gtk_table_attach(GTK_TABLE(tbl_con_info), entry_url, 1, 2, 0, 1,
									 (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
									 (GtkAttachOptions) (0), 0, 0);
	gtk_entry_set_invisible_char(GTK_ENTRY(entry_url), 9679);
	dialog_action_area = GTK_DIALOG(connection_dialog)->action_area;
	gtk_widget_show(dialog_action_area);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area), GTK_BUTTONBOX_END);

	btn_connect_dialog = gtk_button_new_from_stock("gtk-connect");
	gtk_widget_show(btn_connect_dialog);
	gtk_dialog_add_action_widget(GTK_DIALOG(connection_dialog), btn_connect_dialog, 0);
	GTK_WIDGET_SET_FLAGS(btn_connect_dialog, GTK_CAN_DEFAULT);

	gint id_response = gtk_dialog_get_response_for_widget(GTK_DIALOG(connection_dialog), GTK_WIDGET(btn_connect_dialog));
	gtk_dialog_set_default_response(GTK_DIALOG(connection_dialog), id_response);
	gtk_entry_set_activates_default(GTK_ENTRY(entry_url), TRUE);

	g_signal_connect((gpointer) connection_dialog, "key_press_event",
									 G_CALLBACK(gst_ipcam_client_on_connection_dialog_key_press_event),
									 NULL);

	g_signal_connect_swapped((gpointer) connection_dialog, "destroy",
													 G_CALLBACK(gst_ipcam_client_on_connection_dialog_destroy),
													 GTK_OBJECT(NULL));

	g_signal_connect_swapped((gpointer) btn_connect_dialog, "clicked",
													 G_CALLBACK(gst_ipcam_client_on_btn_connect_dialog_clicked),
													 GTK_OBJECT(NULL));

	/** Store pointers to all widgets, for use by lookup_widget(). */
	GLADE_HOOKUP_OBJECT_NO_REF(connection_dialog, connection_dialog, "connection_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF(connection_dialog, dialog_vbox, "dialog_vbox");
	GLADE_HOOKUP_OBJECT(connection_dialog, tbl_con_info, "tbl_con_info");
	GLADE_HOOKUP_OBJECT(connection_dialog, lbl_url, "lbl_url");
	GLADE_HOOKUP_OBJECT(connection_dialog, entry_url, "entry_url");
	GLADE_HOOKUP_OBJECT_NO_REF(connection_dialog, dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT(connection_dialog, btn_connect_dialog, "btn_connect_dialog");

	if (parent_window != NULL) {
		gtk_window_set_transient_for(GTK_WINDOW(connection_dialog), GTK_WINDOW(parent_window));
	}

	return connection_dialog;
}

/**
 * init the statusbar widget
 * 
 * @param void
 *
 * @return the statusbar widget
 */
GtkWidget*
gst_ipcam_client_init_status_bar()
{
	GtkWidget * separator;

	/* init the outside containter*/
	status_bar_vbox = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(status_bar_vbox);

	separator = gtk_hseparator_new();

	gtk_widget_show(separator);
	gtk_box_pack_start(GTK_BOX(status_bar_vbox), separator, FALSE, FALSE, 0);

	/* init the inside containter*/
	status_bar_hbox = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(status_bar_hbox);

	gtk_widget_set_size_request(status_bar_hbox, 500, 25);
	gtk_box_pack_start(GTK_BOX(status_bar_vbox), status_bar_hbox, FALSE, FALSE, 0);

	/* init the status label*/
	statusbar_label_status = gtk_label_new("");
	gtk_widget_show(statusbar_label_status);

	gtk_widget_set_size_request(statusbar_label_status, 60, -1);
	gtk_box_pack_start(GTK_BOX(status_bar_hbox), statusbar_label_status, FALSE, TRUE, 5);

	/* separator*/
	separator = gtk_vseparator_new();
	gtk_widget_show(separator);

	gtk_box_pack_start(GTK_BOX(status_bar_hbox), separator, FALSE, FALSE, 0);

	{
		/* Video type*/
		statusbar_label_video_type = gtk_label_new("");
		gtk_widget_show(statusbar_label_video_type);

		gtk_widget_set_size_request(statusbar_label_video_type, 130, -1);
		gtk_box_pack_start(GTK_BOX(status_bar_hbox), statusbar_label_video_type, FALSE, FALSE, 0);
		/*gtk_box_pack_start_defaults(GTK_BOX(status_bar_hbox), statusbar_label_video_type);*/

		/* Audio type*/
		statusbar_label_audio_type = gtk_label_new("");
		gtk_widget_show(statusbar_label_audio_type);

		gtk_widget_set_size_request(statusbar_label_audio_type, 120, -1);
		gtk_box_pack_start(GTK_BOX(status_bar_hbox), statusbar_label_audio_type, FALSE, FALSE, 0);
	}

	/* separator*/
	separator = gtk_vseparator_new();
	gtk_widget_show(separator);

	gtk_box_pack_start(GTK_BOX(status_bar_hbox), separator, FALSE, FALSE, 0);

	/* Properties label*/
	statusbar_label_properties = gtk_label_new("");
	gtk_widget_show(statusbar_label_properties);

	/*gtk_widget_set_size_request(__statusBar_labelSong, 500, -1);*/
	gtk_box_pack_start(GTK_BOX(status_bar_hbox), statusbar_label_properties, FALSE, TRUE, 5);

	return status_bar_vbox;
}

/**
 *  set the status text on the status bar
 * 
 * @param void
 *
 * @return nothing
 */
void
gst_ipcam_client_set_status_text(const gchar* text)
{
	gtk_label_set_text(GTK_LABEL(statusbar_label_status), text);
}

/**
 * show the properties
 * 
 * @param name const gchar* the status name
 *
 * @return nothing
 */
void
gst_ipcam_client_set_status_properties(const gchar* name)
{
	gtk_label_set_text(GTK_LABEL(statusbar_label_properties), name);
}

/**
 * show the video type
 * 
 * @param type_name const gchar* the video type
 *
 * @return nothing
 */
void
gst_ipcam_client_set_status_video_type(const gchar* type_name)
{
	gtk_label_set_text(GTK_LABEL(statusbar_label_video_type), type_name);
}

/**
 * show the audio type
 *
 * @param type_name const gchar* the audio type
 *
 * @return nothing
 */
void
gst_ipcam_client_set_status_audio_type(const gchar* type_name)
{
	gtk_label_set_text(GTK_LABEL(statusbar_label_audio_type), type_name);
}

GtkWidget *
gst_ipcam_client_create_error_dialog(const gchar* message, GtkWindow * parent)
{
	GtkWidget *dialog;
	gchar * window_title;
	window_title = gst_ipcam_client_window_create_title("Error");
	dialog = gtk_dialog_new_with_buttons(window_title, parent,
			GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
			GTK_STOCK_OK,
			GTK_RESPONSE_ACCEPT,
			NULL);
	
	g_return_val_if_fail(dialog != NULL, NULL);

	{
		GtkWidget * hbox;
		GtkWidget * label;
		GtkWidget * icon;
		GtkWidget * vbox;

		hbox = gtk_hbox_new(FALSE, 10);
		label = gtk_label_new(message);
		icon = gtk_image_new_from_stock(GTK_STOCK_DIALOG_ERROR, GTK_ICON_SIZE_DIALOG);
		gtk_box_pack_start(GTK_BOX(hbox), icon, FALSE, FALSE, 13);
		gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 5);

		vbox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
		gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 5);
		gtk_widget_show_all(vbox);
	}

	gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(parent));
	
	gtk_window_set_title(GTK_WINDOW(dialog), window_title);
	g_free(window_title);

	return dialog;
}

/**
 * set all video properties to all blank boxes
 *
 * @param type_name const gchar* the audio type
 *
 * @return nothing
 */
void
gst_ipcam_client_set_video_props(gchar* frame_rate, gchar* frame_size, gchar* bitrate)
{
	gtk_entry_set_text(GTK_ENTRY(entry_bitrate), bitrate);

	gtk_combo_box_insert_text(GTK_COMBO_BOX(cbx_entry_fps), 2, frame_rate);
	gtk_combo_box_set_active  (GTK_COMBO_BOX(cbx_entry_fps), 2);
	gtk_combo_box_remove_text(GTK_COMBO_BOX(cbx_entry_fps), 2);

	gtk_combo_box_insert_text(GTK_COMBO_BOX(cbx_entry_fsize), 2, frame_size);
	gtk_combo_box_set_active  (GTK_COMBO_BOX(cbx_entry_fsize), 2);
	gtk_combo_box_remove_text(GTK_COMBO_BOX(cbx_entry_fsize), 2);
}