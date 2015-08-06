<?php

namespace AppBundle\Controller;

use Symfony\Bundle\FrameworkBundle\Controller\Controller;
use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
use Symfony\Component\HttpFoundation\Response;

use AppBundle\Helper\Patch;
use AppBundle\Helper\LightType;
use AppBundle\Helper\Spotlight;
use AppBundle\Helper\Dmx;
use AppBundle\Model\DbConnection;
use AppBundle\Model\DataQueries;

class DefaultController extends Controller
{ 
    /**
     * @Route("/", name="home")
     */
    public function indexAction()
    {
        return $this->redirectToRoute('control');
    }
    
    /**
     * @Route("/control", name="control")
     */
    public function controlAction()
    {
        $channels = DefaultController::createPatchTable();
        $gm = DataQueries::getGMValue();

        $timeInt = DataQueries::getTIsinceLastRenderQuery();
        if ($timeInt < 60.0) {
            $isOnline = true;
        }
        else {
            $isOnline = false;
        }
        
        
        return $this->render(
            'default/control.html.twig', 
            array('channels' => $channels,
                  'gm' => $gm,
                  'isOnline' => $isOnline)
        );
    }
    
    /**
     * @Route("/patch", name="patch")
     */
    public function patchAction()
    {
        $patchs = DefaultController::createPatchTable();

        return $this->render(
            'default/patch.html.twig', 
            array('patchs' => $patchs)
        );
    }

    private static function createPatchTable() {

        $raw_channels = DataQueries::getAllChannels();
        $channels = array();

        //echo "<br>Query gave:<br>";
        //print_r($raw_channels);

        // For all rows
        $i = 0;
        while ($i < count($raw_channels)) {

            $raw_channel = $raw_channels[$i];
            $current_channel = $raw_channel['channel'];
            $is_rgb_patch = true;

            $new_patch = new Patch(
                            $current_channel,
                            $raw_channel['patch_intensity']
                         );
            

            // For all rows of the same channel, append a spotlight
            while ($current_channel == $raw_channel['channel']) {

                if (strcmp($raw_channel['type'], "LED") == 0) {                  
                    $new_type = LightType::LED;
                } 
                else if (strcmp($raw_channel['type'], "DEL") == 0) {                  
                    $new_type = LightType::DEL;
                }
                else {
                    die('Error: unkown type ' . $raw_channel['type']);
                }
                
                $new_spotlight = new Spotlight(
                                    $raw_channel['dmx'],
                                    $new_type
                                 );


                // If RGB
                if (strcmp($raw_channel['type'], "LED") == 0) {
                    
                    // Process the 4 next raws
                    for ($ii = 0; $ii < 4; $ii++) {

                        $new_spotlight->appendDmx(new Dmx(
                                                    $raw_channels[$i]['dmx'],
                                                    $raw_channels[$i]['dmx_intensity']
                                                    )
                                                 );
                        $i++;
                    }
                
                } else {
                    $is_rgb_patch = false;
                    
                    // Updating values only for BW since RGB is updated up there
                    $i++;
                }
                
                $new_patch->appendSpotlight($new_spotlight);
                
                //echo "<br><br>Added spotlight:";
                //print_r($new_spotlight->dmxs);

                // Exit if end of raw_channels
                if ($i < count($raw_channels)) {
                    $raw_channel = $raw_channels[$i];
                } 
                else {
                    break;
                }
            }
            $new_patch->setType($is_rgb_patch ? LightType::LED : LightType::DEL);
            $channels[] = $new_patch;
        }

        //foreach ($channels as $patch) {
        //    echo $patch->dump();
        //}
    
        return $channels;
    }
}
