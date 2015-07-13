<?php

namespace AppBundle\Helper;

class Dmx {

    /**
     *  Dmx address of $this input
     */
    public $address;

    /**
     *  Intensity (value) of the input
     */
    public $intensity;
    
    public function dump() {
        return "Dmx addr[" . $this->address .
               "] - Intensity[" . $this->intensity . "]";
    }
}
