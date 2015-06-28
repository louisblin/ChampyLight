<?php
/**
 * User: louisblin
 * Date: 16/05/15
 * Time: 18:25
 *
 * Description: handles database connection
 */

$host = 'localhost';
$user = 'root';
$pass = 'root';
$dbName = 'champylight';

// Table names
$patch = 'patch';

$db;
try {
    $db = new pdo('mysql:host=' . $host . ';dbName=' . $dbName . ';charset=utf8',
                  $user,
                  $pass);

    $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

} catch (Exception $e) {
    die('Couldn\'t connect to database... \n' . $e->getMessage());
}

echo 'Connected' . '<br>';
