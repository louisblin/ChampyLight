// ==========================================================================
// Velleman K8062 DMX controller library for VM116/K8062
// ==========================================================================

typedef unsigned char ubyte;

int  dmxOpen           ();
void dmxClose          ();
void dmxSetMaxChannels ( int maxChannels );
void dmxSetValue       ( ubyte channel , ubyte value );


