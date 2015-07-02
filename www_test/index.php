<?php

ini_set('display_errors',1);
ini_set('display_startup_errors',1);
error_reporting(-1);

// Load and initialize any global libraries
//require_once 'model/data_queries.php';
require_once 'controller.php';

// Route the request internally
$uri = parse_url($_SERVER['REQUEST_URI'], PHP_URL_PATH);

echo 'uri is ' . $uri;

if ('/www/index.php' == $uri) {
    control_action();
} 
elseif ('/www/patch.php' == $uri) {
    echo 'Displaying patch';
    patch_action();
}
else {
    header('Status: 404 Not Found');
    echo '<html><body><h1>Page Not Found</h1></body></html>';
}
