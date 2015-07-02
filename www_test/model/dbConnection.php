<?php
/**
 * User: louisblin
 * Date: 16/05/15
 * Time: 18:25
 *
 * Description: handles database connection
 */

require_once 'errorsHandling.php';

$host = 'localhost';
$user = 'root';
$pass = 'root';
$dbName = 'champylight';

// Table names
$patch = 'patch';
$channel_intensity = 'channel_intensity';

$db;

try {
    $db = new pdo('mysql:host=' . $host . ';dbName=' . $dbName . ';charset=utf8',
                  $user,
                  $pass);

    $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

} catch (PDOException $e) {
    reportError($e);
    die('Couldn\'t connect to database... \n' . $e->getMessage());
}