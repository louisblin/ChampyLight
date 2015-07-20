<?php

namespace AppBundle\Helper;

use AppBundle\Helper\LightType;
use AppBundle\Helper\Dmx;

class Spotlight {

    /**
     *  Starting dmx address associated with the spotlight
     */
    public $address;

    /**
     *  Type of spotlight of the input
     */
    public $type;

    /**
     *  List of channels used to control the spotlight
     */
    public $dmxs;

    public function appendDmx($dmx) {
        $this->dmxs[] = $dmx;
    }

    /**
     *  Contructor
     */
    public function __construct($address, $type) {
        $this->address = $address;
        $this->type = $type;
        $this->dmxs = array();
    }
    
    public function dump() {
        $out = "<br> --- Start Add[" . $this->address .
               "] - Type[" . $this->type . 
               "] - Dmxs[";

        foreach ($this->dmxs as $dmx) {
            $out .= $dmx->dump();
        } 
               
        return $out . "<br> --- ]";
    }
}
