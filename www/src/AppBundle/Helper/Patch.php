<?php

namespace AppBundle\Helper;

use AppBundle\Helper\LightType;
use AppBundle\Helper\Spotlight;

class Patch {

    /** 
     *  Channel number
     */
    public $channel;

    /** 
     *  Overall intensity of the patch
     */
    public $intensity;

    /**
     *  Type of patch:
     *    - convenience LED patch
     *    - regular patch
     */
    public $type;

    /**
     *  List of patched spotlights
     */
    public $spotlights;


    public function setType($type) {
        $this->type = $type;
        return $this;
    }

    public function appendSpotlight(Spotlight $spot) {
        $this->spotlights[] = $spot;
    }

    /**
     *  Contructor
     */
    public function __construct($channel, $intensity) {

        $this->channel = $channel;
        $this->intensity = $intensity;
        $this->spotlights = array();
    }

    public function dump() {
        $out = "<br><br>Chan[" . $this->channel .
               "] - Intensity[" . $this->intensity .
               "] - Type[" . $this->type . 
               "] - Spotlights[";

        foreach ($this->spotlights as $spot) {
            $out .= $spot->dump();
        }

        return $out . "<br>]";
    }
}
