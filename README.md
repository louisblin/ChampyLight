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

The code I took from this project and adapted was:
- the deamon dmxd.c
- dmx.c utiliy file.

## License: Attribution Non-commercial Share Alike (by-nc-sa)

This license lets others remix, tweak, and build upon your work non-commercially, 
as long as they credit you and license their new creations under the identical 
terms. Others can download and redistribute your work just like the by-nc-nd 
license, but they can also translate, make remixes, and produce new stories 
based on your work. All new work based on yours will carry the same license, so 
any derivatives will also be non-commercial in nature. 

# Usage (user section)

## Setting up

=== Web interface

Just copy the `www` repository on your host. You don't have anything else to do,
except making sure your host is complying with the minimum requirements Symfony
needs in order to run.

=== ChampyLight values handler

Just copy the `values_handler` repository on your Raspberry Pi. The only 
parameter you'll have to set is the `REMOTE_ADDR` in values_handler/src/constants.c 
which indicate the URL where the web interface is rendering the values for the 
spotlights.

=== dmxd.c deamon

This one is located in the `deamon` repository and should be running on the 
Raspberry Pi too. It should be a plug and play program, if you follow the same
structure for your project, you shouldn't have to modify anything in it.


## Running

First of all, you want to set up your Raspberry Pi by starting the `dmxd.c` 
deamon with root privileges (sudo ./dmxd.bin), and `champylight` program 
(./champylight). Next, as long as your web interface and RPI are configured as
explained in the Setting up, you're ready to go.


# Operating behaviour (developer section)

## Web interface

Tools:
The frontend is using bootstrap, jQuery together with regular HTML, CSS, and 
Javascript files. For the backend, Symfony2 PHP framework is the core tool.
However the project is not taking advantage of all the framework power, such as 
form handling and Doctrine. Further version of this project will consider it.

## ChampyLigth values handler

Tools:
cURL request performed using the libcurl library to fetch the values from the 
web interface. 

Edge cases behaviour:
- No service: leave the spotlights with their original values.
- Invalid range of value [0..255]: ignore value.
- Invalid transition type {linear, cres, decres, cres-decres}: set transition to default (linear).
- Invalid fade time interval [1..255]: set it to 1.

## dmxd deamon

Directly sending packet to the USB ports. For your information, the manufacturer
of the USB converter provided a library which replaces this deamon. However, 
it doesn't seem compatible with the Raspberry Pi (to double-check...), and this
appears to be the reason why the "Raspberry Pi as a DMX light controller" 
project rewrote the library.
