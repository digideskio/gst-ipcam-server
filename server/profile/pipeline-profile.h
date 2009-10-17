/**
 * Gst-ipcam-server
 * pipeline profile
 * Copyright (C) 2009 Duong Chien Thang <duongchien.thang@nomovok.com>
 * Copyright (C) 2009 Nguyen Thanh Trung <nguyenthanh.trung@nomovok.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
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
 *
 * Created on September 11, 2009, 5:30 PM
 */
#ifndef _PIPELINE_PROFILE_H
#define	_PIPELINE_PROFILE_H

#include <glib.h>

#ifdef	__cplusplus
extern "C" {
#endif

	/**
	 * Define supported pipeline type in our server configuration
	 * We have only 2 types right now: video and audio
	 */
	typedef enum {
		/** invalid pipeline type */
		GST_RTSP_PIPELINE_TYPE_INVALID = -1,
		/** video pipeline */
		GST_RTSP_PIPELINE_TYPE_VIDEO = 0,
		/** audio pipeline */
		GST_RTSP_PIPELINE_TYPE_AUDIO = 1,
	} GstRTSPPipelineType;

	/**
	 * check if the profile contain a video pipeline
	 */
#define gst_rtsp_pipeline_profile_is_video(profile) ( (profile != NULL) && (profile->pipeline_type == GST_RTSP_PIPELINE_TYPE_VIDEO) )

	/**
	 * check if the profile contain an audio pipeline
	 */
#define gst_rtsp_pipeline_profile_is_audio(profile) ( (profile != NULL) && (profile->pipeline_type == GST_RTSP_PIPELINE_TYPE_AUDIO) )

	/**
	 * Structure storing information about profile for gst-ipcam-server servers.
	 * This structure will contains:
	 * - a string to describe the pipeline with some variables. For examples:
	 *      v4l2src ! fpsbin framerate=${fps} ! rtpsink
	 * The ${fps} represent a variable name {fps}
	 *
	 * - a list of variables name
	 * - a hash table to store variables and their values. Each variable will have a default value
	 *
	 * To load a profile from file use gst_rtsp_pipeline_profile_load()
	 * We can use gst_rtsp_pipeline_profile_set_var() to set the value for available variable
	 * After that use gst_rtsp_pipeline_profile_build_pipeline() to build the last pipeline. Variables will be replaced by values
	 */
	typedef struct GstRTSPPipelineProfile {
		gchar * pipeline_name; /// name of the pipeline
		GstRTSPPipelineType pipeline_type; /// type of the pipeline, see GstRTSPPipelineType for more information
		gchar * pipeline_codec; /// both video and audio support some codec
		gchar * pipeline_desc; /// the string represent the pipeline
		GList * vars_name; /// list of variables name
		GHashTable * vars; /// table of variables and their values
	} GstRTSPPipelineProfile;

	/**
	 * Load a profile from a configuration file
	 * If the configuration file contain more than one pipeline, this function will load the 1st one.
	 *
	 * For the format of configuration file and pipeline definition, see GstRTSPServerConfiguration declaration.
	 *
	 * @param file_name gchar* name of the configuration file
	 *
	 * @return GstRTSPPipelineProfile* a profile or NULL if something wrong
	 */
	GstRTSPPipelineProfile * gst_rtsp_pipeline_profile_load(const gchar * name);

	/**
	 * Load profile from given configuration contents.
	 * The contents is an array of text line.
	 *
	 * @param lines gchar** configuration contents to load pipeline
	 * @param start_line gint* start line to parse
	 *
	 * @return GstRTSPPipelineProfile * a profile or NULL if something wrong
	 */
	GstRTSPPipelineProfile * gst_rtsp_pipeline_profile_load_from_text(gchar ** lines, gint * start_line);

	/**
	 * Free the memory used for given GstRTSPPipelineProfile.
	 *
	 * @param profile GstRTSPPipelineProfile* the profile that we want to free
	 *
	 * @return None
	 */
	void gst_rtsp_pipeline_profile_free(GstRTSPPipelineProfile * profile);

	/**
	 * Set name for the given pipeline profile
	 *
	 * @param profile GstRTSPPipelineProfile* the pipeline profile to set name
	 * @param name gchar* the name to set to
	 *
	 * @return None
	 */
	void gst_rtsp_pipeline_profile_set_name(GstRTSPPipelineProfile * profile, const gchar * name);

	/**
	 * Get the name of given pipepline profile
	 *
	 * @param profile GstRTSPPipelineProfile* the pipeline profile to get name
	 *
	 * @return gchar* the name of given profile
	 */
	const gchar * gst_rtsp_pipeline_profile_get_name(const GstRTSPPipelineProfile * profile);

	/**
	 * Get the type of given pipeline profile.
	 * See GstRTSPPipelineType for the list of supported type.
	 * This function will return GST_RTSP_PIPELINE_TYPE_INVALID if something wrong (for exam: the profile is NULL)
	 *
	 * @param profile GstRTSPPipelineProfile* the pipeline profile to get type
	 *
	 * @return GstRTSPPipelineType the type of the given pipeline
	 */
	GstRTSPPipelineType gst_rtsp_pipeline_profile_get_type(const GstRTSPPipelineProfile * profile);

	/**
	 * Set the value for a variable in the profile.
	 * If any of parameters is NULL or the var_name is not available in the profile then return FALSE.
	 *
	 * @param profile GstRTSPPipelineProfile* the profile we want to use
	 * @param var_name gchar* name of the variable to set
	 * @param value gchar* the value we want to set to
	 *
	 * @return gboolean TRUE if everything is ok or FALSE otherwise
	 */
	gboolean gst_rtsp_pipeline_profile_set_var(GstRTSPPipelineProfile * profile, const gchar * var_name, const gchar * value);

	/**
	 * Get the double linked list of variables list of the profile.
	 * The return value should not be free or changed
	 *
	 * @param profile GstRTSPPipelineProfile* the profile we want to use
	 *
	 * @return GList* the double linked list storing variables name
	 */
	const GList * gst_rtsp_pipeline_profile_get_vars_list(GstRTSPPipelineProfile * profile);

	/**
	 * Build the pipeline of the profile from the pipeline description.
	 * Variables will be replaced by their values.
	 *
	 * If the value for a variable is not set, default value will be used.
	 * Use gst_rtsp_pipeline_profile_set_var() to set value for a variable.
	 *
	 * @param profile GstRTSPPipelineProfile* the profile we want to use
	 * @return gchar* the pipeline for this profile
	 */
	gchar * gst_rtsp_pipeline_profile_build_pipeline(GstRTSPPipelineProfile * profile);

	gboolean gst_rtsp_server_configuration_should_skip_line(const gchar * line);
#ifdef	__cplusplus
}
#endif

#endif	/* _PIPELINE_PROFILE_H */

