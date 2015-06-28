<?php
/**
 * User: louisblin
 * Date: 16/05/15
 * Time: 17:17
 *
 * Description: handles all the queries made to the database
 */

include_once('dbConnection.php');

/*
 * TABLE PATCH
 */

$event_name = "default";

function getChannelForDMX($dmx) {

    global $event_name;
    global $db, $dbName, $patch;

    $sql = "SELECT channel"
        . " FROM " . $dbName . "." . $patch
        . " WHERE event_name = :event_name AND dmx = :dmx";

    $q = $db->prepare($sql);
    $q->bindParam(':event_name', $event_name, PDO::PARAM_STR);
    $q->bindParam(':dmx', $dmx, PDO::PARAM_INT);
    $q->execute();

    $ans = $q->fetch();
    $q->closeCursor();

    if ($q->rowCount() == 0) {
        return -1;
    }

    return (int) $ans['channel'];
}

function assignChannelToDMX($dmx, $newChannel, $intensity, $type) {
    
    global $event_name;
    global $db, $dbName, $patch;

    $sql = "INSERT INTO " . $dbName . "." . $patch . " ("
        . "   event_name"
        . " , dmx"
        . " , channel"
        . " , intensity"
        . " , type"
        . ") "
        . "VALUES ("
        . "   :event_name"
        . " , :dmx"
        . " , :channel"
        . " , :intensity"
        . " , :type"
        . ")";

    $q = $db->prepare($sql);
    $q->bindParam(':event_name', $event_name, PDO::PARAM_STR);
    $q->bindParam(':dmx', $dmx , PDO::PARAM_INT);
    $q->bindParam(':channel', $newChannel, PDO::PARAM_INT);
    $q->bindParam(':intensity', $intensity, PDO::PARAM_INT);
    $q->bindParam(':type', $type, PDO::PARAM_INT);

    try {
        $q->execute();
    } catch (PDOException $e) {

        // Assignment already in table... Send warning
        // TODO
        
    }
    return true;
}

function setIntensityForChannel($channel, $intensity) {

    global $db, $dbName, $patch;

    $sql = "UPDATE " . $dbName . "." . $patch
        . " SET intensity = :intensity"
        . " WHERE channel = :channel";

    $q = $db->prepare($sql);
    $q->bindParam(':channel', $channel, PDO::PARAM_INT);
    $q->bindParam(':intensity', $intensity, PDO::PARAM_INT);
    $q->execute();
}


/* SCHEMA

CREATE TABLE patch (
    event_name VARCHAR(50),
    dmx INT,
    channel INT NOT NULL,
    intensity INT NOT NULL,
    type VARCHAR(3) NOT NULL,

    PRIMARY KEY(event_name, dmx),
    CONSTRAINT intensity_validity check (0 <= intensity AND intensity <= 100),
    CONSTRAINT dmx_validity check (0 <= dmx AND dmx <= 512),
    CONSTRAINT channel_validity check (0 <= channel AND channel <= 512)
);
