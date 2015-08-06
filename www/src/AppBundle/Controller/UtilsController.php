<?php

namespace AppBundle\Controller;

use Symfony\Bundle\FrameworkBundle\Controller\Controller;
use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
use Symfony\Component\HttpFoundation\Response;

use AppBundle\Model\DataQueries;
use AppBundle\Model\ErrorsHandling;

class UtilsController extends Controller
{ 
    /**
     * @Route("/add_patch", name="add_patch")
     */
    public function addAction()
    {        
        $success = true;
        $error_msg = "";

        // PARSING HMTL FORM

        // channel_selected
        $channel = $_POST['channel_selected'];
        if (isset($channel)) {

            if (!is_numeric($channel)) {
                $success = false;
                $error_msg .= 'channel format: ' . htmlspecialchars($channel) 
                    . ' is not a number...';
            }
            else {
                // Nothing to do... all channels already in database.
            } 
        }
        else {
            $success = false;
            $error_msg .= 'channel_selected not defined in POST...';
        }

        // inputs
        if (isset($_POST['inputs'])) {

            // Loop through all inputs 
            foreach ($_POST['inputs'] as $key => $input) {

                $dmx = $input['dmx'];
                
                if (isset($input['type_led'])) {
                    $isLed = $input['type_led'];
                }
                else {
                    $isLed = 'off';                    
                }
                
                if (isset($input['type_del'])) {
                    $isDel = $input['type_del'];
                }
                else {
                    $isDel = (strcmp($isLed, 'on') == 0 ? 'off' : 'on');
                }

                // Consistency checks...
                if (!isset($dmx)) {
                    $success = false;
                    $error_msg .= 'inputs[' . $key . '][dmx] not defined in POST...';
                }
                if (!is_numeric($dmx)) {
                    $success = false;
                    $error_msg .= 'inputs[' . $key . '][dmx] = ' . htmlspecialchars($dmx) . ' is not a number...';
                }

                if (!isset($isLed)) {
                    $success = false;
                    $error_msg .= 'inputs[' . $key . '][isLed] not defined in POST...';
                }
                if (strcmp($isLed, 'on') != 0 && strcmp($isLed, 'off') != 0) {
                    $success = false;
                    $error_msg .= 'inputs[' . $key . '][isLed] = ' . htmlspecialchars($isLed) . ' is illegal value...';
                }
                
                if (!isset($isDel)) {
                    $success = false;
                    $error_msg .= 'inputs[' . $key . '][isDel] not defined in POST...';
                }
                if (strcmp($isDel, 'on') != 0 && strcmp($isDel, 'off') != 0) {
                    $success = false;
                    $error_msg .= 'inputs[' . $key . '][isDel] = ' . htmlspecialchars($isDel) . ' is illegal value...';
                }

                // Saving in database
                if (strcmp($isDel, 'on') == 0) {

                    if (!DataQueries::assignChannelToDMX($dmx, $channel, 255, 'DEL')) {
                        $success = false;
                        $error_msg .= 'DMX ' . $dmx . ' is already in patch'; 
                    }
                }
                else {
                    
                    if (!DataQueries::assignChannelToDMX($dmx, $channel, 0, 'LED') ||
                        !DataQueries::assignChannelToDMX($dmx + 1, $channel, 0, 'LED') ||
                        !DataQueries::assignChannelToDMX($dmx + 2, $channel, 0, 'LED') ||
                        !DataQueries::assignChannelToDMX($dmx + 3, $channel, 0, 'LED')) {

                        $success = false;    
                        $error_msg .= 'DMX ' . $dmx . ' is already in patch'; 
                    }
                }
            }
        }
        else {
            $success = false;
            $error_msg .= 'inputs not defined in POST...';
        }

        // GENERATING ouput
        if ($success) {

            $this->addFlash(
                'success',
                'Patch successfully created'
            );
        }
        else {
            // i.e. if error occured with database query
            if (empty($error_msg)) {
                
                $this->addFlash(
                    'failure',
                    'Error while creating patch: ' . ErrorsHandling::getErrorMsg()
                );
            }
            else {
                $this->addFlash(
                    'failure',
                    'Error while creating patch: ' . $error_msg
                );
            }
        }
        return $this->redirectToRoute('patch');
    }
    
    /**
     * @Route("/delete_patch/{count}", name="delete_patch")
     */
    public function deleteAction($count)
    {
        $success = DataQueries::deletePatch($count) ;
            
        // GENERATING ouput
        if ($success) {

            echo "All OK";

            $this->addFlash(
                'success',
                'Patch successfully deleted'
            );
        }
        else {

            $this->addFlash(
                'failure',
                'Error while deleting patch: ' . ErrorsHandling::getErrorMsg()
            );
        }
        return $this->redirectToRoute('patch');
    }
    
    /**
     * @Route("/submit_values", name="submit_values")
     */
    public function submitAction() 
    {
        $success = true;
        $error_msg = "";

        // PARSING HMTL FORM

        // Transition type
        if (isset($_POST['transition_type'])) {

            $transition_type = $_POST['transition_type'];

            if(strcmp($transition_type, "linear") == 0) {
                DataQueries::setTransitionType(0);
            }
            else if(strcmp($transition_type, "crescendo") == 0) {
                DataQueries::setTransitionType(1);
            }
            else if(strcmp($transition_type, "decrescendo") == 0) {
                DataQueries::setTransitionType(2);
            }
            else if(strcmp($transition_type, "crescendodecrescendo") == 0) {
                DataQueries::setTransitionType(3);
            }
            else if(strcmp($transition_type, "strobe") == 0) {
                DataQueries::setTransitionType(4);
            }
            else {
                $success = false;
                $error_msg .= "Unknown transition_type " . htmlspecialchars($transition_type) . "";
            }
        }
        else {
            $success = false;
            $error_msg .= 'transition_type not defined in POST...';
        }

        // Transition timer
        if (isset($_POST['slider_timer'])) {
            DataQueries::setTransitionFadeTI(htmlspecialchars($_POST['slider_timer']));
        }
        else {
            $success = false;
            $error_msg .= 'slider_timer not defined in POST...';
        }

        // Grand Master
        if (isset($_POST['gm'])) {
            DataQueries::setGMValue(htmlspecialchars($_POST['gm']));
        }
        else {
            $success = false;
            $error_msg .= 'gm not defined in POST...';
        }

        // Patch intensities
        if (isset($_POST['channel_intensity'])) {

            foreach ($_POST['channel_intensity'] as $channel => $intensity) {

                DataQueries::setIntensityForChannel(
                    htmlspecialchars($channel),
                    htmlspecialchars($intensity)
                );
            }
        }
        else {
            $success = false;
            $error_msg .= 'channel_intensity not defined in POST...';
        }

        // Dmx
        if (isset($_POST['dmxs'])) {

            foreach ($_POST['dmxs'] as $dmxAddr => $intensity) {
                
                DataQueries::setIntensityForDMX(
                    htmlspecialchars($dmxAddr),
                    htmlspecialchars($intensity)
                );

                // Update siblings, if any, to the same intensity as 
                // leading spotlight...
                if (isset($_POST['cascade'])) {
                    $cascade = $_POST['cascade'];
                
                    if (isset($cascade[$dmxAddr])) {

                        foreach ($cascade[$dmxAddr] as $siblingDmxAddr) {
                        
                            DataQueries::setIntensityForDMX(
                                htmlspecialchars($siblingDmxAddr),
                                htmlspecialchars($intensity)
                            );
                        }
                    }
                }
            }
        }
        else {
            $success = false;
            $error_msg .= 'dmxs not defined in POST...';
        }
        
        // GENERATING ouput
        if ($success) {

            $this->addFlash(
                'success',
                'Transition set successfully'
            );
        }
        else {
            
            if (empty($error_msg)) {
                
                $this->addFlash(
                    'failure',
                    'Error while setting transition: ' . ErrorsHandling::getErrorMsg()
                );
            }
            else {
                $this->addFlash(
                    'failure',
                    'Error while setting transition: ' . $error_msg
                );
            }
        }
        return $this->redirectToRoute('control');
    }
    
    /**
     * @Route("/render", name="render")
     */
    public function renderAction()
    {
        // Register action in database
        DataQueries::setLastRenderQueryToNow();

        $intensities = array();

        // Meta
        $intensities[] = DataQueries::getTransitionType();
        $intensities[] = DataQueries::getTransitionFadeTI();

        // Dmx values
        for ($i = 1; $i <= 512; $i++) {
            $intensities[] = DataQueries::getOverallIntensityForDMX($i); 
        }

        // Creating answer
        foreach ($intensities as $intensity) {
        
            if ($intensity < 10) {
                echo "00" . $intensity;
            }
            else if ($intensity < 100) {
                echo "0" . $intensity;
            }
            else {
                echo $intensity;
            } 
        }
        
        return new Response('');
    }

    /**
     * @Route("/logout", name="logout")
     */
    public function logoutAction()
    {
        return new Response('Logged out! Redirecting...');
    }
}
