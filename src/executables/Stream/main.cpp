#include "ShaderTools/DefaultRenderLoop.h"
#include "gst/gst.h"
#include "glib.h"

GstElement* streamer;
GstElement* gltestsrc;
GstElement* gltestsink;
GMainLoop* mainloop;

int main(int argc, char *argv[]) {

    gst_init(NULL, NULL);
    streamer = gst_pipeline_new(NULL);
    gltestsrc = gst_element_factory_make("gltestsrc", NULL);
    gltestsink = gst_element_factory_make("glimagesink", NULL);
    
    gst_bin_add_many((GstBin*)streamer, gltestsrc, gltestsink, NULL);

    gst_element_link(gltestsrc, gltestsink);
    gst_element_set_state(streamer, GST_STATE_PLAYING);

    mainloop = g_main_loop_new (NULL, FALSE);
    g_object_set(gltestsrc, "pattern", 6, NULL);
    g_main_loop_run (mainloop);

    // renderLoop([]{
    //     glBegin(GL_TRIANGLES);
    //     glColor3f(1,0,0);
    //     glVertex2f(-0.5,-0.5);
    //     glColor3f(0,1,0);
    //     glVertex2f(0,0.5);
    //     glColor3f(0,0,1);
    //     glVertex2f(0.5,-0.5);
    //     glEnd();
    // });
}
