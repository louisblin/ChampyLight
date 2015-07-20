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

    //public function getIntensity() {
    //    return $this->intensity;
    //}
    
    /**
     *  Contructor
     */
    public function __construct($address, $intensity) {
        $this->address = $address;
        $this->intensity = $intensity;
    }
    
    public function dump() {
        return "<br> ------ Dmx addr[" . $this->address .
               "] - Intensity[" . $this->intensity . "]";
    }
}
