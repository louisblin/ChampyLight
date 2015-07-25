<?php
/**
 * User: louisblin
 * Date: 19/06/15
 * Time: 14:09
 *
 * Description: handles errors thrown by any function
 */

namespace AppBundle\Model;

$last_error_msg;

class ErrorsHandling {

    public static function reportPDOError($error) {

        global $last_error_msg;

        if (is_null($error)) {
         
            $last_error_msg = "ALL OK";
        }
        else {

            $last_error_msg = "[Error no " . $error->getCode()
                            . " in " . $error->getFile() 
                            . ":" . $error->getLine() . "]: " 
                            . $error->getMessage();
        }
    }

    public static function getErrorMsg() {
    
        return $last_error_msg;
    }
}
