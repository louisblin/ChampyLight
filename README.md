# ChampyLight 

## Presentation

DMX Light Controller using a web UI and Rasberry Pi connected to the spotlights.

### Web User Interface

User can update the values of the spotlights from a web interface, and create a
"patch" of multiple DMX channels in order to create groups of spotlights.

### Rasberry Pi as a relay

The Rasberry Pi hosts two C programs that have complementary purposes:
- champylight.c : fetches value, transition type and time from the web, and 
                  performs the transition.
- dmxd.c :        a C deamon that takes its inputs from a memory segment shared
                  with champylight.c. It continuously feeds a USB with the 
                  spotlights values.

### From USB to DMX

A simple USB to DMX adaptator does the job by converting a one-off USB signal to 
a continuous DMX data feed. I used the Velleman VM116 Usb Controlled Dmx 
Interface since the deamon was taken from another project.
                

## Author

Louis Blin, student in 1st year at Imperial College London.


## Disclaimer

This project is based on the "Raspberry Pi as a DMX light controller"
(http://www.instructables.com/id/Raspberry-Pi-as-a-DMX-light-controller/).

The code I took from this project was:
- 100% of the deamon dmxd.c
- adapted the dmx.c utiliy file.

## License: Attribution Non-commercial Share Alike (by-nc-sa)

This license lets others remix, tweak, and build upon your work non-commercially, as long as they credit you and license their new creations under the identical terms. Others can download and redistribute your work just like the by-nc-nd license, but they can also translate, make remixes, and produce new stories based on your work. All new work based on yours will carry the same license, so any derivatives will also be non-commercial in nature. 

# Usage

TODO
