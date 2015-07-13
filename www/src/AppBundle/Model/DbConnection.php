<?php
/**
 * User: louisblin
 * Date: 16/05/15
 * Time: 18:25
 *
 * Description: handles database connection
 */

namespace AppBundle\Model;

use AppBundle\Model\ErrorsHandling;

$dbName = 'champylight';

// Table names
$GLOBALS['patch'] = 'patch';
$GLOBALS['channel_intensity'] = 'channel_intensity';

$GLOBALS['db'] = NULL;

class DbConnection {

    public static function connect() {

        global $db;

        $host = '127.0.0.1';
        $port = '8889';
        $user = 'root';
        $pass = 'root';
        $dbName = 'champylight';

        try {
            $db = new \PDO('mysql:host=' . $host . ';port=' . $port . ';dbname=' . $dbName . ';charset=utf8',
                          $user,
                          $pass);

            $db->setAttribute(\PDO::ATTR_ERRMODE, \PDO::ERRMODE_EXCEPTION);

        } catch (PDOException $e) {
            reportError($e);
            die('Couldn\'t connect to database... \n' . $e->getMessage());
        }
    }
}

DbConnection::connect();
