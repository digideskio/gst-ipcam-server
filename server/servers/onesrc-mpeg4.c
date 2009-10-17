/* GStreamer
 * Copyright (C) 2008 Wim Taymans <wim.taymans at gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

#include "server-profile.h"
#include "profile/pipeline-profile.h"

// default profile file for this server
#define DEFAULT_PROFILE_FILE_VIDEO "onesrc-mpeg4.ini"
#define DEFAULT_PROFILE_FILE_AUDIO_AAC "audio-aac.ini"
#define DEFAULT_PROFILE_FILE_AUDIO_G711 "audio-g711.ini"
#define DEFAULT_PROFILE_FILE_AUDIO_G726 "audio-g726.ini"

static gboolean
timeout (GstRTSPServer *server, gboolean ignored)
{
  GstRTSPSessionPool *pool;

  pool = gst_rtsp_server_get_session_pool (server);
  gst_rtsp_session_pool_cleanup (pool);
  g_object_unref (pool);

  return TRUE;
}

int
main (int argc, char *argv[])
{
  GMainLoop *loop;
  GstRTSPServer *server;
  GstRTSPMediaMapping *mapping;
  GstRTSPMediaFactory *factory;
  GstRTSPPipelineProfile *profile_video, *profile_audio = NULL;
  gchar *pipeline_str = NULL, *pipeline_video_str = NULL, *pipeline_audio_str = NULL;

  gst_init (&argc, &argv);

  loop = g_main_loop_new (NULL, FALSE);

  /* create a server instance */
  server = gst_rtsp_server_new ();

  /* get the mapping for this server, every server has a default mapper object
   * that be used to map uri mount points to media factories */
  mapping = gst_rtsp_server_get_media_mapping (server);

  /* make a media factory for a mpeg 4 video stream and audio (aac, g711 or g726) stream. The default media factory can use
   * gst-launch syntax to create pipelines. 
   * any launch line works as long as it contains elements named pay%d. Each
   * element with pay%d names will be a stream */
  factory = gst_rtsp_media_factory_new ();

  /* start building the pipeline */
  profile_video = gst_rtsp_pipeline_profile_load(DEFAULT_PROFILE_FILE_VIDEO);
  if (profile_video == NULL) {
	  pipeline_str = g_strdup("");
  } else {
	  /* we can set some common server parameter by using functions in server-profile.h
	   * but default values will be used here
	   */
	  pipeline_video_str = gst_rtsp_pipeline_profile_build_pipeline(profile_video);
         pipeline_str = pipeline_video_str;

     if (argc > 1) {
         if (g_strrstr(argv[1], "aac")) {
			    profile_audio = gst_rtsp_pipeline_profile_load(DEFAULT_PROFILE_FILE_AUDIO_AAC);
  	      } else if (g_strrstr(argv[1], "g726")) {
		  		 profile_audio = gst_rtsp_pipeline_profile_load(DEFAULT_PROFILE_FILE_AUDIO_G726);
	      } else if (g_strrstr(argv[1], "g711")) {
		   	 profile_audio = gst_rtsp_pipeline_profile_load(DEFAULT_PROFILE_FILE_AUDIO_G711);
         }
	      if (profile_audio != NULL) {
		   	 pipeline_audio_str = gst_rtsp_pipeline_profile_build_pipeline(profile_audio);
             pipeline_str = g_strdup_printf("%s%s", g_strndup(pipeline_video_str, strlen(pipeline_video_str) -1), pipeline_audio_str);
             /* free pipeline audio video string */
             g_free(pipeline_video_str);
             g_free(pipeline_audio_str);
             /* free profile audio string */
             g_free(profile_audio);
         }
	  }
     /* free profile string */
     g_free(profile_video);
	  g_print("Our pipeline is '%s'\n", pipeline_str);
  }

  gst_rtsp_media_factory_set_launch (factory, pipeline_str);

  /* free unneeded string */
  g_free(pipeline_str);

  /* share the pipeline with multiple clients */
  gst_rtsp_media_factory_set_shared(factory, TRUE);

  /* attach the test factory to the /mp4 url */
  gst_rtsp_media_mapping_add_factory (mapping, "/mp4", factory);

  /* don't need the ref to the mapper anymore */
  g_object_unref (mapping);

  /* attach the server to the default maincontext */
  gst_rtsp_server_attach (server, NULL);

  g_timeout_add_seconds (2, (GSourceFunc) timeout, server); 

  /* start serving */
  g_main_loop_run (loop);

  return 0;
}
