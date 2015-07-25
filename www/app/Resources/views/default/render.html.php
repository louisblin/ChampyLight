<?php
    //echo '000';
    //echo '005';

    //echo '255';
    //echo '000';
    //echo '000';
    //echo '000';
    //
    //echo '000';
    //echo '255';
    //echo '000';
    //echo '000';

    //echo '000';
    //echo '000';
    //echo '255';
    //echo '000';
    //
    //echo '255';
    //echo '255';
    //echo '000';
    //echo '000';
    //
    //echo '255';
    //echo '000';
    //echo '255';
    //echo '000';
    //
    //echo '000';
    //echo '255';
    //echo '255';
    //echo '000';

    if (!isset($intensities)) {
        echo "C'est la mer noire, intensities is not set !!";
        exit;
    }

    foreach ($intensities as $nb) {

        if ($nb < 10) {
            echo "00" . $nb;
        }
        else if ($nb < 100) {
            echo "0" . $nb;
        }
        else {
            echo $nb;
        } 
    }
?>
