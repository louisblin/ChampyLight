<?php
/**
 * User: louisblin
 * Date: 16/05/15
 * Time: 17:17
 *
 * Description: handles all the queries made to the database
 */

namespace AppBundle\Model;

use AppBundle\Model\DbConnection;
use AppBundle\Model\ErrorsHandling;

/*
 * TABLE PATCH
 * 
 * Functions available:
 *
 * Getters for channel, dmx and overall dmx intensity
 * - getTransitionType();
 * - getTransitionFadeTI();
 * - getGMValue();
 * - getIntensityForDMX($dmx);
 * - getIntensityForChannel($channel);
 * - getOverallIntensityForDMX($dmx);
 * - getChannelForDMX($dmx);
 * - getAllChannels();
 * - getTIsinceLastRenderQuery();
 *
 * Setters for channel and dmx intensity
 * - setTransitionType($trans_id);
 * - setTransitionFadeTI($fade_ti);
 * - setGMValue($gm);
 * - setIntensityForDMX($dmx, $newIntensity);
 * - setIntensityForChannel($channel, $newIntensity);
 * - setLastRenderQueryToNow();
 *
 * Creating / Deleting dmx assignments
 * - assignChannelToDMX($dmx, $newChannel, $intensity, $type);
 * - deletePatch($channel);
 *
 * Create 512 channels set to max intensity of 255
 * - resetChannelIntensities();
 *
 */

$event_name = "default";

class DataQueries {
    
    // GETTERS

    public static function getTransitionType() {

        require_once 'DbConnection.php';
        
        global $db, $dbName, $meta;
        
        $sql = "SELECT transition_type_id"
             . " FROM " . $dbName . "." . $meta;

        $q = $db->prepare($sql);
        
        try {
            $q->execute();
        } catch (\PDOException $e) {

            ErrorsHandling::reportPDOError($e);
            return -1;
        }

        $ans = $q->fetch();
        $q->closeCursor();

        ErrorsHandling::reportPDOError(NULL);
        if ($q->rowCount() == 0) {
            return -1;
        }

        return (int) $ans['transition_type_id'];
    }
    
    public static function getTransitionFadeTI() {
    
        require_once 'DbConnection.php';
        
        global $db, $dbName, $meta;
        
        $sql = "SELECT transition_fade_ti"
             . " FROM " . $dbName . "." . $meta;

        $q = $db->prepare($sql);
        
        try {
            $q->execute();
        } catch (\PDOException $e) {

            ErrorsHandling::reportPDOError($e);
            return -1;
        }

        $ans = $q->fetch();
        $q->closeCursor();

        ErrorsHandling::reportPDOError(NULL);
        if ($q->rowCount() == 0) {
            return -1;
        }

        return (int) $ans['transition_fade_ti'];
    }

    public static function getGMValue() {
    
        require_once 'DbConnection.php';
        
        global $db, $dbName, $meta;
        
        $sql = "SELECT gm"
             . " FROM " . $dbName . "." . $meta;

        $q = $db->prepare($sql);
        
        try {
            $q->execute();
        } catch (\PDOException $e) {

            ErrorsHandling::reportPDOError($e);
            return -1;
        }

        $ans = $q->fetch();
        $q->closeCursor();

        ErrorsHandling::reportPDOError(NULL);
        if ($q->rowCount() == 0) {
            return -1;
        }

        return (int) $ans['gm'];
    }
    
    public static function getIntensityForDMX($dmx) {

        require_once 'DbConnection.php';
        
        global $db, $dbName, $patch;
        
        $sql = "SELECT intensity"
             . " FROM " . $dbName . "." . $patch
             . " WHERE dmx = :dmx";

        $q = $db->prepare($sql);
        $q->bindParam(':dmx', $dmx, \PDO::PARAM_INT);
        
        try {
            $q->execute();
        } catch (\PDOException $e) {

            ErrorsHandling::reportPDOError($e);
            return -1;
        }

        $ans = $q->fetch();
        $q->closeCursor();

        ErrorsHandling::reportPDOError(NULL);
        if ($q->rowCount() == 0) {
            return -1;
        }

        return (int) $ans['intensity'];
    }

    public static function getIntensityForChannel($channel) {

        require_once 'DbConnection.php';
        
        global $db, $dbName, $channel_intensity;
        
        $sql = "SELECT intensity"
             . " FROM " . $dbName . "." . $channel_intensity
             . " WHERE channel = :channel";

        $q = $db->prepare($sql);
        $q->bindParam(':channel', $channel, \PDO::PARAM_INT);
        
        try {
            $q->execute();
        } catch (\PDOException $e) {

            ErrorsHandling::reportPDOError($e);
            return -1;
        }

        $ans = $q->fetch();
        $q->closeCursor();

        ErrorsHandling::reportPDOError(NULL);
        if ($q->rowCount() == 0) {
            return -1;
        }

        return (int) $ans['intensity'];
    }

    public static function getOverallIntensityForDMX($dmx) {
        
        require_once 'DbConnection.php';
        
        global $db, $dbName, $patch, $channel_intensity, $meta;

        $sql = "SELECT FLOOR(meta.gm / 255 * ci.intensity / 255 * pa.intensity) as intensity"
             . " FROM " . $dbName . "." . $meta . ", "
                        . $dbName . "." . $channel_intensity . " ci"
             . " JOIN " . $dbName . "." . $patch . " pa"
             . " USING (channel)"
             . " WHERE pa.dmx = :dmx";

        $q = $db->prepare($sql);
        $q->bindParam(':dmx', $dmx, \PDO::PARAM_INT);
        
        try {
            $q->execute();
        } catch (\PDOException $e) {

            ErrorsHandling::reportPDOError($e);
            return -1;
        }

        $ans = $q->fetch();
        $q->closeCursor();

        ErrorsHandling::reportPDOError(NULL);
        
        // Returning default value
        if ($q->rowCount() == 0) {
            return 255;
        }

        return (int) $ans['intensity'];
    }

    public static function getChannelForDMX($dmx) {

        require_once 'DbConnection.php';
        
        global $event_name;
        global $db, $dbName, $patch;

        $sql = "SELECT channel"
            . " FROM " . $dbName . "." . $patch
            . " WHERE event_name = :event_name AND dmx = :dmx";

        $q = $db->prepare($sql);
        $q->bindParam(':event_name', $event_name, \PDO::PARAM_STR);
        $q->bindParam(':dmx', $dmx, \PDO::PARAM_INT);
        
        try {
            $q->execute();
        } catch (\PDOException $e) {

            ErrorsHandling::reportPDOError($e);
            return -1;
        }

        $ans = $q->fetch();
        $q->closeCursor();

        ErrorsHandling::reportPDOError(NULL);
        if ($q->rowCount() == 0) {
            return -1;
        }

        return (int) $ans['channel'];
    }

    public static function getAllChannels() {

        require_once 'DbConnection.php';
        
        global $event_name;
        global $db, $dbName, $patch, $channel_intensity;

        $sql = "SELECT ci.intensity AS patch_intensity, p.dmx, p.channel AS channel, p.intensity AS dmx_intensity, p.type"
            . " FROM " . $dbName . "." . $channel_intensity . " ci"
            . " JOIN " . $dbName . "." . $patch . " p"
            . " USING (channel)"
            . " ORDER BY p.channel ASC, p.dmx ASC";

        $q = $db->prepare($sql);

        try {
            $q->execute();
        } catch (\PDOException $e) {
            
            ErrorsHandling::reportPDOError($e);        
            return NULL;
        }
        
        ErrorsHandling::reportPDOError(NULL);
        return $q->fetchAll();
    }

    public static function getTIsinceLastRenderQuery() {

        require_once 'DbConnection.php';
        
        global $db, $dbName, $meta; 
        
        $sql = "SELECT TIMESTAMPDIFF(SECOND, meta.last_render_query, NOW()) as ti"
             . " FROM " . $dbName . "." . $meta
             . " WHERE 1";
    
        $q = $db->prepare($sql);
        
        try {
            $q->execute();
        } catch (\PDOException $e) {

            ErrorsHandling::reportPDOError($e);
            return -1;
        }

        $ans = $q->fetch();
        $q->closeCursor();

        ErrorsHandling::reportPDOError(NULL);
        if ($q->rowCount() == 0) {
            return -1;
        }

        return (int) $ans['ti'];
    }

    // SETTERS


    public static function setTransitionType($trans_id) {

        require_once 'DbConnection.php';
        
        global $db, $dbName, $meta; 

        $sql = "UPDATE " . $dbName . "." . $meta
            . " SET transition_type_id = :trans_id"
            . " WHERE 1";
        
        $q = $db->prepare($sql);
        $q->bindParam(':trans_id', $trans_id, \PDO::PARAM_INT);

        try {
            $q->execute();
        } catch (\PDOException $e) {
            
            ErrorsHandling::reportPDOError($e);        
            return false;
        }
        
        ErrorsHandling::reportPDOError(NULL);
        return true;
    }

    public static function setTransitionFadeTI($fade_ti) {
        
        require_once 'DbConnection.php';
        
        global $db, $dbName, $meta; 

        $sql = "UPDATE " . $dbName . "." . $meta
            . " SET transition_fade_ti = :fade_ti"
            . " WHERE 1";
        
        $q = $db->prepare($sql);
        $q->bindParam(':fade_ti', $fade_ti, \PDO::PARAM_INT);

        try {
            $q->execute();
        } catch (\PDOException $e) {
            
            ErrorsHandling::reportPDOError($e);        
            return false;
        }
        
        ErrorsHandling::reportPDOError(NULL);
        return true;
    }
    
    public static function setGMValue($gm) {
    
        require_once 'DbConnection.php';
        
        global $db, $dbName, $meta; 

        $sql = "UPDATE " . $dbName . "." . $meta
            . " SET gm = :gm"
            . " WHERE 1";
        
        $q = $db->prepare($sql);
        $q->bindParam(':gm', $gm, \PDO::PARAM_INT);

        try {
            $q->execute();
        } catch (\PDOException $e) {
            
            ErrorsHandling::reportPDOError($e);        
            return false;
        }
        
        ErrorsHandling::reportPDOError(NULL);
        return true;
    }
    
    public static function setIntensityForDMX($dmx, $newIntensity) {

        require_once 'DbConnection.php';
        
        global $db, $dbName, $patch;

        $sql = "UPDATE " . $dbName . "." . $patch
             . " SET `intensity` = :intensity"
             . " WHERE dmx = :dmx";

        $q = $db->prepare($sql);
        $q->bindParam(':intensity', $newIntensity, \PDO::PARAM_INT);
        $q->bindParam(':dmx', $dmx , \PDO::PARAM_INT);

        try {
            $q->execute();
        } catch (\PDOException $e) {
            
            ErrorsHandling::reportPDOError($e);        
            return false;
        }
        
        ErrorsHandling::reportPDOError(NULL);
        return true;
    }

    public static function setIntensityForChannel($channel, $newIntensity) {

        require_once 'DbConnection.php';
        
        global $db, $dbName, $channel_intensity;
       
        $sql = "UPDATE " . $dbName . "." . $channel_intensity
             . " SET intensity = :intensity"
             . " WHERE channel = :channel";

        $q = $db->prepare($sql);
        $q->bindParam(':intensity', $newIntensity, \PDO::PARAM_INT);
        $q->bindParam(':channel', $channel , \PDO::PARAM_INT);

        try {
            $q->execute();
        } catch (\PDOException $e) {
            
            ErrorsHandling::reportPDOError($e);        
            return false;
        }
        
        ErrorsHandling::reportPDOError(NULL);
        return true;
    }

    public static function setLastRenderQueryToNow() {
    
        require_once 'DbConnection.php';
        
        global $db, $dbName, $meta;
        
        $sql = "UPDATE " . $dbName . "." . $meta
            . " SET last_render_query=NOW()"
            . " WHERE 1"; 
    
        $q = $db->prepare($sql);
        
        try {
            $q->execute();
        } catch (\PDOException $e) {
            
            ErrorsHandling::reportPDOError($e);        
            return false;
        }
        
        ErrorsHandling::reportPDOError(NULL);
        return true;
    }
    


    // CREATE / DELETE patch

    public static function assignChannelToDMX($dmx, $newChannel, $intensity, $type) {
        
        require_once 'DbConnection.php';
        
        global $event_name;

        $event_name = "default";
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
        $q->bindParam(':event_name', $event_name, \PDO::PARAM_STR);
        $q->bindParam(':dmx', $dmx , \PDO::PARAM_INT);
        $q->bindParam(':channel', $newChannel, \PDO::PARAM_INT);
        $q->bindParam(':intensity', $intensity, \PDO::PARAM_INT);
        $q->bindParam(':type', $type, \PDO::PARAM_INT);

        try {
            $q->execute();
        } catch (\PDOException $e) {

            ErrorsHandling::reportPDOError($e);        
            return false;
        }
        
        ErrorsHandling::reportPDOError(NULL);
        return true;
    }

    public static function deletePatch($channel) {

        require_once 'DbConnection.php';
        
        global $db, $dbName, $patch;
        
        $sql = "DELETE FROM " . $dbName . "." . $patch
             . " WHERE channel = :channel";

        $q = $db->prepare($sql);
        $q->bindParam(':channel', $channel , \PDO::PARAM_INT);

        try {
            $q->execute();
        } catch (\PDOException $e) {

            ErrorsHandling::reportPDOError($e);
            return false;
        }
        
        ErrorsHandling::reportPDOError(NULL);
        return true;
    }

    // RESETING

    public static function resetChannelIntensities() {

        require_once 'DbConnection.php';
        
        global $db, $dbName, $channel_intensity;

        $sql =  "START TRANSACTION;"
              . " SET FOREIGN_KEY_CHECKS=0;"
              . " DELETE FROM " . $dbName . "." . $channel_intensity . ";"
              . " INSERT INTO " . $dbName . "." . $channel_intensity
              . " VALUES"
              . " (\"1\", \"255\"),"
              . " (\"2\", \"255\"),"
              . " (\"3\", \"255\"),"
              . " (\"4\", \"255\"),"
              . " (\"5\", \"255\"),"
              . " (\"6\", \"255\"),"
              . " (\"7\", \"255\"),"
              . " (\"8\", \"255\"),"
              . " (\"9\", \"255\"),"
              . " (\"10\", \"255\"),"
              . " (\"11\", \"255\"),"
              . " (\"12\", \"255\"),"
              . " (\"13\", \"255\"),"
              . " (\"14\", \"255\"),"
              . " (\"15\", \"255\"),"
              . " (\"16\", \"255\"),"
              . " (\"17\", \"255\"),"
              . " (\"18\", \"255\"),"
              . " (\"19\", \"255\"),"
              . " (\"20\", \"255\"),"
              . " (\"21\", \"255\"),"
              . " (\"22\", \"255\"),"
              . " (\"23\", \"255\"),"
              . " (\"24\", \"255\"),"
              . " (\"25\", \"255\"),"
              . " (\"26\", \"255\"),"
              . " (\"27\", \"255\"),"
              . " (\"28\", \"255\"),"
              . " (\"29\", \"255\"),"
              . " (\"30\", \"255\"),"
              . " (\"31\", \"255\"),"
              . " (\"32\", \"255\"),"
              . " (\"33\", \"255\"),"
              . " (\"34\", \"255\"),"
              . " (\"35\", \"255\"),"
              . " (\"36\", \"255\"),"
              . " (\"37\", \"255\"),"
              . " (\"38\", \"255\"),"
              . " (\"39\", \"255\"),"
              . " (\"40\", \"255\"),"
              . " (\"41\", \"255\"),"
              . " (\"42\", \"255\"),"
              . " (\"43\", \"255\"),"
              . " (\"44\", \"255\"),"
              . " (\"45\", \"255\"),"
              . " (\"46\", \"255\"),"
              . " (\"47\", \"255\"),"
              . " (\"48\", \"255\"),"
              . " (\"49\", \"255\"),"
              . " (\"50\", \"255\"),"
              . " (\"51\", \"255\"),"
              . " (\"52\", \"255\"),"
              . " (\"53\", \"255\"),"
              . " (\"54\", \"255\"),"
              . " (\"55\", \"255\"),"
              . " (\"56\", \"255\"),"
              . " (\"57\", \"255\"),"
              . " (\"58\", \"255\"),"
              . " (\"59\", \"255\"),"
              . " (\"60\", \"255\"),"
              . " (\"61\", \"255\"),"
              . " (\"62\", \"255\"),"
              . " (\"63\", \"255\"),"
              . " (\"64\", \"255\"),"
              . " (\"65\", \"255\"),"
              . " (\"66\", \"255\"),"
              . " (\"67\", \"255\"),"
              . " (\"68\", \"255\"),"
              . " (\"69\", \"255\"),"
              . " (\"70\", \"255\"),"
              . " (\"71\", \"255\"),"
              . " (\"72\", \"255\"),"
              . " (\"73\", \"255\"),"
              . " (\"74\", \"255\"),"
              . " (\"75\", \"255\"),"
              . " (\"76\", \"255\"),"
              . " (\"77\", \"255\"),"
              . " (\"78\", \"255\"),"
              . " (\"79\", \"255\"),"
              . " (\"80\", \"255\"),"
              . " (\"81\", \"255\"),"
              . " (\"82\", \"255\"),"
              . " (\"83\", \"255\"),"
              . " (\"84\", \"255\"),"
              . " (\"85\", \"255\"),"
              . " (\"86\", \"255\"),"
              . " (\"87\", \"255\"),"
              . " (\"88\", \"255\"),"
              . " (\"89\", \"255\"),"
              . " (\"90\", \"255\"),"
              . " (\"91\", \"255\"),"
              . " (\"92\", \"255\"),"
              . " (\"93\", \"255\"),"
              . " (\"94\", \"255\"),"
              . " (\"95\", \"255\"),"
              . " (\"96\", \"255\"),"
              . " (\"97\", \"255\"),"
              . " (\"98\", \"255\"),"
              . " (\"99\", \"255\"),"
              . " (\"100\", \"255\"),"
              . " (\"101\", \"255\"),"
              . " (\"102\", \"255\"),"
              . " (\"103\", \"255\"),"
              . " (\"104\", \"255\"),"
              . " (\"105\", \"255\"),"
              . " (\"106\", \"255\"),"
              . " (\"107\", \"255\"),"
              . " (\"108\", \"255\"),"
              . " (\"109\", \"255\"),"
              . " (\"110\", \"255\"),"
              . " (\"111\", \"255\"),"
              . " (\"112\", \"255\"),"
              . " (\"113\", \"255\"),"
              . " (\"114\", \"255\"),"
              . " (\"115\", \"255\"),"
              . " (\"116\", \"255\"),"
              . " (\"117\", \"255\"),"
              . " (\"118\", \"255\"),"
              . " (\"119\", \"255\"),"
              . " (\"120\", \"255\"),"
              . " (\"121\", \"255\"),"
              . " (\"122\", \"255\"),"
              . " (\"123\", \"255\"),"
              . " (\"124\", \"255\"),"
              . " (\"125\", \"255\"),"
              . " (\"126\", \"255\"),"
              . " (\"127\", \"255\"),"
              . " (\"128\", \"255\"),"
              . " (\"129\", \"255\"),"
              . " (\"130\", \"255\"),"
              . " (\"131\", \"255\"),"
              . " (\"132\", \"255\"),"
              . " (\"133\", \"255\"),"
              . " (\"134\", \"255\"),"
              . " (\"135\", \"255\"),"
              . " (\"136\", \"255\"),"
              . " (\"137\", \"255\"),"
              . " (\"138\", \"255\"),"
              . " (\"139\", \"255\"),"
              . " (\"140\", \"255\"),"
              . " (\"141\", \"255\"),"
              . " (\"142\", \"255\"),"
              . " (\"143\", \"255\"),"
              . " (\"144\", \"255\"),"
              . " (\"145\", \"255\"),"
              . " (\"146\", \"255\"),"
              . " (\"147\", \"255\"),"
              . " (\"148\", \"255\"),"
              . " (\"149\", \"255\"),"
              . " (\"150\", \"255\"),"
              . " (\"151\", \"255\"),"
              . " (\"152\", \"255\"),"
              . " (\"153\", \"255\"),"
              . " (\"154\", \"255\"),"
              . " (\"155\", \"255\"),"
              . " (\"156\", \"255\"),"
              . " (\"157\", \"255\"),"
              . " (\"158\", \"255\"),"
              . " (\"159\", \"255\"),"
              . " (\"160\", \"255\"),"
              . " (\"161\", \"255\"),"
              . " (\"162\", \"255\"),"
              . " (\"163\", \"255\"),"
              . " (\"164\", \"255\"),"
              . " (\"165\", \"255\"),"
              . " (\"166\", \"255\"),"
              . " (\"167\", \"255\"),"
              . " (\"168\", \"255\"),"
              . " (\"169\", \"255\"),"
              . " (\"170\", \"255\"),"
              . " (\"171\", \"255\"),"
              . " (\"172\", \"255\"),"
              . " (\"173\", \"255\"),"
              . " (\"174\", \"255\"),"
              . " (\"175\", \"255\"),"
              . " (\"176\", \"255\"),"
              . " (\"177\", \"255\"),"
              . " (\"178\", \"255\"),"
              . " (\"179\", \"255\"),"
              . " (\"180\", \"255\"),"
              . " (\"181\", \"255\"),"
              . " (\"182\", \"255\"),"
              . " (\"183\", \"255\"),"
              . " (\"184\", \"255\"),"
              . " (\"185\", \"255\"),"
              . " (\"186\", \"255\"),"
              . " (\"187\", \"255\"),"
              . " (\"188\", \"255\"),"
              . " (\"189\", \"255\"),"
              . " (\"190\", \"255\"),"
              . " (\"191\", \"255\"),"
              . " (\"192\", \"255\"),"
              . " (\"193\", \"255\"),"
              . " (\"194\", \"255\"),"
              . " (\"195\", \"255\"),"
              . " (\"196\", \"255\"),"
              . " (\"197\", \"255\"),"
              . " (\"198\", \"255\"),"
              . " (\"199\", \"255\"),"
              . " (\"200\", \"255\"),"
              . " (\"201\", \"255\"),"
              . " (\"202\", \"255\"),"
              . " (\"203\", \"255\"),"
              . " (\"204\", \"255\"),"
              . " (\"205\", \"255\"),"
              . " (\"206\", \"255\"),"
              . " (\"207\", \"255\"),"
              . " (\"208\", \"255\"),"
              . " (\"209\", \"255\"),"
              . " (\"210\", \"255\"),"
              . " (\"211\", \"255\"),"
              . " (\"212\", \"255\"),"
              . " (\"213\", \"255\"),"
              . " (\"214\", \"255\"),"
              . " (\"215\", \"255\"),"
              . " (\"216\", \"255\"),"
              . " (\"217\", \"255\"),"
              . " (\"218\", \"255\"),"
              . " (\"219\", \"255\"),"
              . " (\"220\", \"255\"),"
              . " (\"221\", \"255\"),"
              . " (\"222\", \"255\"),"
              . " (\"223\", \"255\"),"
              . " (\"224\", \"255\"),"
              . " (\"225\", \"255\"),"
              . " (\"226\", \"255\"),"
              . " (\"227\", \"255\"),"
              . " (\"228\", \"255\"),"
              . " (\"229\", \"255\"),"
              . " (\"230\", \"255\"),"
              . " (\"231\", \"255\"),"
              . " (\"232\", \"255\"),"
              . " (\"233\", \"255\"),"
              . " (\"234\", \"255\"),"
              . " (\"235\", \"255\"),"
              . " (\"236\", \"255\"),"
              . " (\"237\", \"255\"),"
              . " (\"238\", \"255\"),"
              . " (\"239\", \"255\"),"
              . " (\"240\", \"255\"),"
              . " (\"241\", \"255\"),"
              . " (\"242\", \"255\"),"
              . " (\"243\", \"255\"),"
              . " (\"244\", \"255\"),"
              . " (\"245\", \"255\"),"
              . " (\"246\", \"255\"),"
              . " (\"247\", \"255\"),"
              . " (\"248\", \"255\"),"
              . " (\"249\", \"255\"),"
              . " (\"250\", \"255\"),"
              . " (\"251\", \"255\"),"
              . " (\"252\", \"255\"),"
              . " (\"253\", \"255\"),"
              . " (\"254\", \"255\"),"
              . " (\"255\", \"255\"),"
              . " (\"256\", \"255\"),"
              . " (\"257\", \"255\"),"
              . " (\"258\", \"255\"),"
              . " (\"259\", \"255\"),"
              . " (\"260\", \"255\"),"
              . " (\"261\", \"255\"),"
              . " (\"262\", \"255\"),"
              . " (\"263\", \"255\"),"
              . " (\"264\", \"255\"),"
              . " (\"265\", \"255\"),"
              . " (\"266\", \"255\"),"
              . " (\"267\", \"255\"),"
              . " (\"268\", \"255\"),"
              . " (\"269\", \"255\"),"
              . " (\"270\", \"255\"),"
              . " (\"271\", \"255\"),"
              . " (\"272\", \"255\"),"
              . " (\"273\", \"255\"),"
              . " (\"274\", \"255\"),"
              . " (\"275\", \"255\"),"
              . " (\"276\", \"255\"),"
              . " (\"277\", \"255\"),"
              . " (\"278\", \"255\"),"
              . " (\"279\", \"255\"),"
              . " (\"280\", \"255\"),"
              . " (\"281\", \"255\"),"
              . " (\"282\", \"255\"),"
              . " (\"283\", \"255\"),"
              . " (\"284\", \"255\"),"
              . " (\"285\", \"255\"),"
              . " (\"286\", \"255\"),"
              . " (\"287\", \"255\"),"
              . " (\"288\", \"255\"),"
              . " (\"289\", \"255\"),"
              . " (\"290\", \"255\"),"
              . " (\"291\", \"255\"),"
              . " (\"292\", \"255\"),"
              . " (\"293\", \"255\"),"
              . " (\"294\", \"255\"),"
              . " (\"295\", \"255\"),"
              . " (\"296\", \"255\"),"
              . " (\"297\", \"255\"),"
              . " (\"298\", \"255\"),"
              . " (\"299\", \"255\"),"
              . " (\"300\", \"255\"),"
              . " (\"301\", \"255\"),"
              . " (\"302\", \"255\"),"
              . " (\"303\", \"255\"),"
              . " (\"304\", \"255\"),"
              . " (\"305\", \"255\"),"
              . " (\"306\", \"255\"),"
              . " (\"307\", \"255\"),"
              . " (\"308\", \"255\"),"
              . " (\"309\", \"255\"),"
              . " (\"310\", \"255\"),"
              . " (\"311\", \"255\"),"
              . " (\"312\", \"255\"),"
              . " (\"313\", \"255\"),"
              . " (\"314\", \"255\"),"
              . " (\"315\", \"255\"),"
              . " (\"316\", \"255\"),"
              . " (\"317\", \"255\"),"
              . " (\"318\", \"255\"),"
              . " (\"319\", \"255\"),"
              . " (\"320\", \"255\"),"
              . " (\"321\", \"255\"),"
              . " (\"322\", \"255\"),"
              . " (\"323\", \"255\"),"
              . " (\"324\", \"255\"),"
              . " (\"325\", \"255\"),"
              . " (\"326\", \"255\"),"
              . " (\"327\", \"255\"),"
              . " (\"328\", \"255\"),"
              . " (\"329\", \"255\"),"
              . " (\"330\", \"255\"),"
              . " (\"331\", \"255\"),"
              . " (\"332\", \"255\"),"
              . " (\"333\", \"255\"),"
              . " (\"334\", \"255\"),"
              . " (\"335\", \"255\"),"
              . " (\"336\", \"255\"),"
              . " (\"337\", \"255\"),"
              . " (\"338\", \"255\"),"
              . " (\"339\", \"255\"),"
              . " (\"340\", \"255\"),"
              . " (\"341\", \"255\"),"
              . " (\"342\", \"255\"),"
              . " (\"343\", \"255\"),"
              . " (\"344\", \"255\"),"
              . " (\"345\", \"255\"),"
              . " (\"346\", \"255\"),"
              . " (\"347\", \"255\"),"
              . " (\"348\", \"255\"),"
              . " (\"349\", \"255\"),"
              . " (\"350\", \"255\"),"
              . " (\"351\", \"255\"),"
              . " (\"352\", \"255\"),"
              . " (\"353\", \"255\"),"
              . " (\"354\", \"255\"),"
              . " (\"355\", \"255\"),"
              . " (\"356\", \"255\"),"
              . " (\"357\", \"255\"),"
              . " (\"358\", \"255\"),"
              . " (\"359\", \"255\"),"
              . " (\"360\", \"255\"),"
              . " (\"361\", \"255\"),"
              . " (\"362\", \"255\"),"
              . " (\"363\", \"255\"),"
              . " (\"364\", \"255\"),"
              . " (\"365\", \"255\"),"
              . " (\"366\", \"255\"),"
              . " (\"367\", \"255\"),"
              . " (\"368\", \"255\"),"
              . " (\"369\", \"255\"),"
              . " (\"370\", \"255\"),"
              . " (\"371\", \"255\"),"
              . " (\"372\", \"255\"),"
              . " (\"373\", \"255\"),"
              . " (\"374\", \"255\"),"
              . " (\"375\", \"255\"),"
              . " (\"376\", \"255\"),"
              . " (\"377\", \"255\"),"
              . " (\"378\", \"255\"),"
              . " (\"379\", \"255\"),"
              . " (\"380\", \"255\"),"
              . " (\"381\", \"255\"),"
              . " (\"382\", \"255\"),"
              . " (\"383\", \"255\"),"
              . " (\"384\", \"255\"),"
              . " (\"385\", \"255\"),"
              . " (\"386\", \"255\"),"
              . " (\"387\", \"255\"),"
              . " (\"388\", \"255\"),"
              . " (\"389\", \"255\"),"
              . " (\"390\", \"255\"),"
              . " (\"391\", \"255\"),"
              . " (\"392\", \"255\"),"
              . " (\"393\", \"255\"),"
              . " (\"394\", \"255\"),"
              . " (\"395\", \"255\"),"
              . " (\"396\", \"255\"),"
              . " (\"397\", \"255\"),"
              . " (\"398\", \"255\"),"
              . " (\"399\", \"255\"),"
              . " (\"400\", \"255\"),"
              . " (\"401\", \"255\"),"
              . " (\"402\", \"255\"),"
              . " (\"403\", \"255\"),"
              . " (\"404\", \"255\"),"
              . " (\"405\", \"255\"),"
              . " (\"406\", \"255\"),"
              . " (\"407\", \"255\"),"
              . " (\"408\", \"255\"),"
              . " (\"409\", \"255\"),"
              . " (\"410\", \"255\"),"
              . " (\"411\", \"255\"),"
              . " (\"412\", \"255\"),"
              . " (\"413\", \"255\"),"
              . " (\"414\", \"255\"),"
              . " (\"415\", \"255\"),"
              . " (\"416\", \"255\"),"
              . " (\"417\", \"255\"),"
              . " (\"418\", \"255\"),"
              . " (\"419\", \"255\"),"
              . " (\"420\", \"255\"),"
              . " (\"421\", \"255\"),"
              . " (\"422\", \"255\"),"
              . " (\"423\", \"255\"),"
              . " (\"424\", \"255\"),"
              . " (\"425\", \"255\"),"
              . " (\"426\", \"255\"),"
              . " (\"427\", \"255\"),"
              . " (\"428\", \"255\"),"
              . " (\"429\", \"255\"),"
              . " (\"430\", \"255\"),"
              . " (\"431\", \"255\"),"
              . " (\"432\", \"255\"),"
              . " (\"433\", \"255\"),"
              . " (\"434\", \"255\"),"
              . " (\"435\", \"255\"),"
              . " (\"436\", \"255\"),"
              . " (\"437\", \"255\"),"
              . " (\"438\", \"255\"),"
              . " (\"439\", \"255\"),"
              . " (\"440\", \"255\"),"
              . " (\"441\", \"255\"),"
              . " (\"442\", \"255\"),"
              . " (\"443\", \"255\"),"
              . " (\"444\", \"255\"),"
              . " (\"445\", \"255\"),"
              . " (\"446\", \"255\"),"
              . " (\"447\", \"255\"),"
              . " (\"448\", \"255\"),"
              . " (\"449\", \"255\"),"
              . " (\"450\", \"255\"),"
              . " (\"451\", \"255\"),"
              . " (\"452\", \"255\"),"
              . " (\"453\", \"255\"),"
              . " (\"454\", \"255\"),"
              . " (\"455\", \"255\"),"
              . " (\"456\", \"255\"),"
              . " (\"457\", \"255\"),"
              . " (\"458\", \"255\"),"
              . " (\"459\", \"255\"),"
              . " (\"460\", \"255\"),"
              . " (\"461\", \"255\"),"
              . " (\"462\", \"255\"),"
              . " (\"463\", \"255\"),"
              . " (\"464\", \"255\"),"
              . " (\"465\", \"255\"),"
              . " (\"466\", \"255\"),"
              . " (\"467\", \"255\"),"
              . " (\"468\", \"255\"),"
              . " (\"469\", \"255\"),"
              . " (\"470\", \"255\"),"
              . " (\"471\", \"255\"),"
              . " (\"472\", \"255\"),"
              . " (\"473\", \"255\"),"
              . " (\"474\", \"255\"),"
              . " (\"475\", \"255\"),"
              . " (\"476\", \"255\"),"
              . " (\"477\", \"255\"),"
              . " (\"478\", \"255\"),"
              . " (\"479\", \"255\"),"
              . " (\"480\", \"255\"),"
              . " (\"481\", \"255\"),"
              . " (\"482\", \"255\"),"
              . " (\"483\", \"255\"),"
              . " (\"484\", \"255\"),"
              . " (\"485\", \"255\"),"
              . " (\"486\", \"255\"),"
              . " (\"487\", \"255\"),"
              . " (\"488\", \"255\"),"
              . " (\"489\", \"255\"),"
              . " (\"490\", \"255\"),"
              . " (\"491\", \"255\"),"
              . " (\"492\", \"255\"),"
              . " (\"493\", \"255\"),"
              . " (\"494\", \"255\"),"
              . " (\"495\", \"255\"),"
              . " (\"496\", \"255\"),"
              . " (\"497\", \"255\"),"
              . " (\"498\", \"255\"),"
              . " (\"499\", \"255\"),"
              . " (\"500\", \"255\"),"
              . " (\"501\", \"255\"),"
              . " (\"502\", \"255\"),"
              . " (\"503\", \"255\"),"
              . " (\"504\", \"255\"),"
              . " (\"505\", \"255\"),"
              . " (\"506\", \"255\"),"
              . " (\"507\", \"255\"),"
              . " (\"508\", \"255\"),"
              . " (\"509\", \"255\"),"
              . " (\"510\", \"255\"),"
              . " (\"511\", \"255\"),"
              . " (\"512\", \"255\");"
              . " SET FOREIGN_KEY_CHECKS=1;"
              . " COMMIT;";

        $q = $db->prepare($sql);

        try {
            $q->execute();
        } catch (\PDOException $e) {

            ErrorsHandling::reportPDOError($e);
            return false;
        }
        
        ErrorsHandling::reportPDOError(NULL);
        return true;
    }
}

/* SCHEMA

CREATE TABLE channel_intensity (
    channel INT, 
    intensity INT,

    PRIMARY KEY (channel),
    CONSTRAINT intensity_validity check (0 <= intensity AND intensity <= 255),
    CONSTRAINT channel_validity check (0 <= channel AND channel <= 512)
);

CREATE TABLE patch (
    event_name VARCHAR(50),
    dmx INT,
    channel INT NOT NULL,
    intensity INT NOT NULL,
    type VARCHAR(3) NOT NULL,

    PRIMARY KEY (event_name, dmx),
    FOREIGN KEY (channel) references channel_intensity (channel),
    CONSTRAINT intensity_validity check (0 <= intensity AND intensity <= 255),
    CONSTRAINT dmx_validity check (0 <= dmx AND dmx <= 512),
    CONSTRAINT channel_validity check (0 <= channel AND channel <= 512)
);

 */
