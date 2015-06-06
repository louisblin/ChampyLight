// ===========================================================================
// DMXWheel - a simple program that sets the color on a DMX device using
//            the color wheel selector on the screen
// ===========================================================================


// ===========================================================================

#include <gtk/gtk.h>                          // GTK+3.0 graphics library
#include <dmx.h>                              // DMX interface library


// constants and definitions

#define RedChannel  2                         // DMX channel for red control
#define GrnChannel  3                         // DMX channel for green control
#define BluChannel  4                         // DMX channel for blue control
#define NumChannels 3                         // # of DMX channels used


// global variables

static GtkWidget * window;                    // window widget
static GtkWidget * hsv;                       // HSV color widget

static GdkColor bgCol = { 0    ,              // allocated color ( unused )
                          0x7fff ,              // red value
                          0x7fff ,              // green value
                          0x7fff };             // blue value


// forward declarations

static gboolean onWindowClose ( GtkWidget *, GdkEvent  *, gpointer );
static gboolean onHSVModify   ( GtkWidget *, GdkEvent  *, gpointer );
static int      initGUI       ( int argc, char *argv[] );
static void     launchGUI     ();

static void     exitGUI       ();
static int      initDMX       ();
static void     setDMXColor   ( gdouble, gdouble, gdouble );
static void     exitDMX       ();


// ===========================================================================
//  main program
// ===========================================================================

int main( int argc, char *argv[] )
{
  int error;


  // initialize

  error = initGUI( argc , argv );
  if ( error < 0 ) return ( error );

  error = initDMX();
  if ( error < 0 ) return ( error );


  // launch the GUI

  onHSVModify ( hsv , NULL , NULL );
  launchGUI();


  // terminate

  exitDMX();
  exitGUI();


  return ( 0 );

}

// ===========================================================================
// initialize the graphical user interface
// ===========================================================================

int initGUI( int argc, char *argv[] )
{

  // initalize GTK library

  gtk_init ( &argc , &argv );



  // create the window widget

  window = gtk_window_new ( GTK_WINDOW_TOPLEVEL );
  if ( !window ) return ( -1 );

  gtk_window_set_title ( GTK_WINDOW ( window  ),  "DMXWheel" );

  g_signal_connect ( window , "delete-event" ,
                    G_CALLBACK ( onWindowClose ) , NULL );

  gtk_widget_modify_bg( window , GTK_STATE_NORMAL , &bgCol );


  // create the HSV color widget

  hsv = gtk_hsv_new();
  if ( !hsv ) return ( -2 );

  gtk_hsv_set_metrics ( GTK_HSV ( hsv ) , 500 , 50 );

  gtk_hsv_set_color   ( GTK_HSV ( hsv ) , 0.0 , 0.8 , 0.8 );

  gtk_container_add ( GTK_CONTAINER ( window ) , hsv );

  g_signal_connect ( hsv , "changed" , G_CALLBACK ( onHSVModify ) , NULL );
  g_signal_connect ( hsv , "move"    , G_CALLBACK ( onHSVModify ) , NULL );





  // show the widgets

  gtk_widget_show ( hsv );
  gtk_widget_show ( window );


  // return valid status

  return ( 0 );

}

// ===========================================================================
// launchGUI -- launch the GUI thread ( blocks until GUI closes )
// ===========================================================================

void launchGUI()
{

  // launch the GTK main progran
 
  gtk_main();

}


// ===========================================================================
// exitGUI -- close down the GUI
// ===========================================================================

void exitGUI()
{

}

// ===========================================================================
// initDMX -- initialize DMX interface
// ===========================================================================

int initDMX()
{

  // open DMX interface

  int success = dmxOpen();
  if ( success < 0 ) return ( success );


  // configure

  dmxSetMaxChannels ( NumChannels );


  // return valid status

  return ( 0 );


}

// ===========================================================================
// setDMXColor -- set the color values for the DMX device
// ===========================================================================

void setDMXColor ( gdouble red, gdouble grn, gdouble blu )
{

  // convert values to unsigned bytes

  ubyte redVal = (ubyte) ( 255.0f * red );
  ubyte grnVal = (ubyte) ( 255.0f * grn );
  ubyte bluVal = (ubyte) ( 255.0f * blu );


  // set the channel colors

  dmxSetValue ( RedChannel , redVal );
  dmxSetValue ( GrnChannel , grnVal );
  dmxSetValue ( BluChannel , bluVal );
}


// ===========================================================================
// exitDMX -- terminate the DMX interface
// ===========================================================================

void exitDMX()
{

 // blackout

  dmxSetValue ( RedChannel , 0 );
  dmxSetValue ( GrnChannel , 0 );
  dmxSetValue ( BluChannel , 0 );


  // close the DMX connection

  dmxClose();

}

// ===========================================================================
// onWindowClose -- handler when user closes window
// ===========================================================================

static gboolean onWindowClose ( GtkWidget *widget,
                                GdkEvent  *event ,
                                gpointer   data  )
{
  gtk_main_quit();
  return ( FALSE );
}

// ===========================================================================
// onHSVModyf -- handler when HSV value changes
// ===========================================================================

static gboolean onHSVModify ( GtkWidget *widget,
                              GdkEvent  *event ,
                              gpointer   data  )
{

  // read the current HSV values and convert to RGB

  gdouble h, s, v, r, g, b;

  gtk_hsv_get_color ( GTK_HSV ( widget ) , &h , &s, &v );
  gtk_hsv_to_rgb    ( h, s, v, &r, &g, &b  );


  // set the DMX color


  setDMXColor ( r, g, b );

}
