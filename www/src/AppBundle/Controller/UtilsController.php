<?php

namespace AppBundle\Controller;

use Symfony\Bundle\FrameworkBundle\Controller\Controller;
use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
use Symfony\Component\HttpFoundation\Response;

use AppBundle\Model\DataQueries;

class UtilsController extends Controller
{ 
    /**
     * @Route("/add_patch", name="add_patch")
     */
    public function addAction()
    {
        print_r($_POST);
        return new Response(
            '<html><br><br>Add action</html>'
        );
    }
    
    /**
     * @Route("/delete_patch/{count}", name="delete_patch")
     */
    public function deleteAction($count)
    {
        if (!DataQueries::deletePatch($count)) {
            echo "Something went wrong";
        }
        
        return $this->redirectToRoute('patch');
    }
    
    /**
     * @Route("/submit_values", name="submit_values")
     */
    public function submitAction() 
    {
        $success = true;
        $error_msg = "<br>";

        // PARSING HMTL FORM

        // Transition type
        if (isset($_POST['transition_type'])) {

            $transition_type = $_POST['transition_type'];

            if(strcmp($transition_type, "linear") == 0) {
                DataQueries::setTransitionType(0);
            }
            else if(strcmp($transition_type, "cres") == 0) {
                DataQueries::setTransitionType(1);
            }
            else if(strcmp($transition_type, "crescendo") == 0) {
                DataQueries::setTransitionType(2);
            }
            else if(strcmp($transition_type, "decrescendo") == 0) {
                DataQueries::setTransitionType(3);
            }
            else if(strcmp($transition_type, "strobe") == 0) {
                DataQueries::setTransitionType(4);
            }
            else {
                $success = false;
                $error_msg .= "Unknown transition_type " . htmlspecialchars($transition_type) . "<br>";
            }
        }
        else {
            $success = false;
            $error_msg .= 'transition_type not defined in POST...<br>';
        }

        // Transition timer
        if (isset($_POST['slider_timer'])) {
            DataQueries::setTransitionFadeTI(htmlspecialchars($_POST['slider_timer']));
        }
        else {
            $success = false;
            $error_msg .= 'slider_timer not defined in POST...<br>';
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
            $error_msg .= 'channel_intensity not defined in POST...<br>';
        }

        // Dmx
        if (isset($_POST['dmxs'])) {

            foreach ($_POST['dmxs'] as $dmxAddr => $intensity) {
                
                DataQueries::setIntensityForDMX(
                    htmlspecialchars($dmxAddr),
                    htmlspecialchars($intensity)
                );
            }
        }
        else {
            $success = false;
            $error_msg .= 'dmxs not defined in POST...<br>';
        }
        
        // GENERATING ouput
        if ($success) {
            return new Response(
                '<html>Submitted values SUCCESSFULLY
                <script>setTimeout(function(){window.location.href = "control";}, 2000)</script>
                </html>'
            );
        }
        else {
            print_r($_POST);
            return new Response(
                '<html><br><br>ERROR while submitting values:' . $error_msg . '</html>'
            );
        }
    }
    
    /**
     * @Route("/render", name="render")
     */
    public function renderAction()
    {
        $intensities = array();

        // Meta
        $intensities[] = DataQueries::getTransitionType();
        $intensities[] = DataQueries::getTransitionFadeTI();

        // Dmx values
        for ($i = 1; $i <= 512; $i++) {
            $intensities[] = DataQueries::getOverallIntensityForDMX($i); 
        }

        return $this->render(
            'default/render.html.twig',
            array("intensities" => $intensities)
        );
    }
}
