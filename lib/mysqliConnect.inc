<?php
    class mysqliConnect {
        private $debugMode = true;
        private $query;
        private $_mysqli;
        private $data;

        public function __construct($query="",$action="") {
            $this->query = $query;
            $this->connectDB();
            if ($query != "") {
                $this->queryDB($query);
            }
            if ($this->debugMode) fb::log($action,"mysqli action");
            if ($this->debugMode) fb::log($this->data,"mysqli data");
            if ($this->data === false) {
                return;	
            }
            switch($action) {
                case "da": //dataset assoc
                    $dta =$this->datasetAssoc();
                    return $dta;
                    break;
                case "do": //dataset object
                    $dta = $this->datasetObject();   
                    return $dta;
                    break;    
                default:
                    //shouldnt really get here
                    if ($this->debugMode) FB::log("Database Action has not been selected");
                    return $this->data;
            }
            //$this->closeData();
            //$this->closeDB();
        }	
        public function datasetAssoc() {
            if ($this->debugMode) fb::log($this->data,"dataset assoc");
            $result = array();
            while ($row = $this->data->fetch_assoc()) {
                //fb::log($row,"row");
                $result[] = $row;
            }
            $this->closeData();
            return $result;
        }
        public function datasetObject() {
            if ($this->debugMode) fb::log($this->data,"dataset object");
            $result = array();
            while ($row = $this->data->fetch_object()) {
                //fb::log($row,"row");
                $result[] = $row;
            }
            $this->closeData();
            return $result;
        }
        private function connectDB() {
            $this->_mysqli = new mysqli(mysql_connect, mysql_user, mysql_pass, mysql_db);
            if ($this->_mysqli->connect_error) {
                if  ($this->debugMode) FB::error($this->_mysqli->connect_errno, "mysqli connect errno");
                if  ($this->debugMode) FB::error($this->_mysqli->connect_error, "mysqli connect error");
            }
            if (DEBUG) {
                FB::log($this->_mysqli->host_info,"mysqli Connect host info");
                fb::log($this->_mysqli, "mysqli resource");
            }   
        }
        public function queryDB($query="",$action="") {
            if ($query != "") {
                $this->query = $query;    
            }
            $this->data = $this->_mysqli->query($this->query);
            if (DEBUG) {
                FB::log($this->query,"mysqli query");
                FB::log($this->data,"mysqli data");
            }
            if  ($this->debugMode) fb::log($action,"mysqli action");
            if ($this->data === false) {
                return;	
            }
            switch($action) {
                case "da": //dataset assoc
                    $dta = $this->datasetAssoc();
                    return $dta;
                    break;
                case "do": //dataset object
                    $dta = $this->datasetObject();   
                    return $dta;
                    break;
                default:
                    //shouldnt really get here
                    if  ($this->debugMode)  FB::log("Database Action default");
                    return $this->data;
            }
        }
        /**
        * quote smart works out the escaping of your sql column value by type
        *  really only quotes integers vs string
        * 
        * @param string $value the value to be single quoted
        * @return string
        */
        public function quote_smart($value) {
            if (!is_numeric($value) || substr($value, 0, 1) == '0') {
                $value = trim($value);
                $value = $this->escapeMyString($value);
                $value = "'" . $value . "'";
            }
            return $value;
        }
        static function quickInsert($table, $values, $returnAutoID) {
            $db = new self;
            $insert = array();
            foreach($values as $field => $value) {
                if(is_int($value))
                    $insert[] = "$field = " . intval($value);
                else
                    $insert[] = "$field = '{$db->escapeMyString($value)}'";
            }
            $query = "INSERT INTO `$table` SET " . implode(', ', $insert);
            if($returnAutoID) {
                $db->__construct($query, 'id');
                return $db->actionDataset();
            }
            else
                $db->__construct($query);
        }

        function quickReplace($table, $values) {
            $db = new self;
            $replace = array();
            foreach($values as $field => $value) {
                if(is_int($value))
                    $replace[] = "$field = " . intval($value);
                else
                    $replace[] = "$field = '{$db->escapeMyString($value)}'";
            }
            $query = "REPLACE INTO `$table` SET " . implode(', ', $replace);
            $db->__construct($query);
        }

        // you might find it easier to use these static functions

        static function fetchOneCell($query, $mustExist = true) {
            $db = new dbConnect($query);
            if($row = mysql_fetch_row($db->actionDataset()))
                return $row[0];

            if($mustExist)
                warn(new Exception("Query result was empty: $query"));
            return false;
        }

        static function fetchOneRow($query, $mustExist = true) {
            $db = new dbConnect($query);
            if($row = mysql_fetch_assoc($db->actionDataset()))
                return $row;

            if($mustExist)
                warn(new Exception("Query result was empty: $query"));
            return false;
        }

        static function fetchOneColumn($query) {
            $db = new dbConnect($query);
            $return = array();
            $result = $db->actionDataset();
            while($row = mysql_fetch_row($result))
                $return[] = $row[0];
            mysql_free_result($result);
            return $return;
        }

        static function fetchAssocRows($query) {
            $db = new dbConnect($query);
            $return = array();
            $result = $db->actionDataset();
            while($row = mysql_fetch_assoc($result))
                $return[] = $row;
            mysql_free_result($result);
            return $return;
        }

        static function fetchIndexedRows($query, $index = 0) {
            $db = new dbConnect($query);
            $return = array();
            $result = $db->actionDataset();
            // if $index is a number, we need to find the name of that column
            if(is_int($index))
                $index = mysql_field_name($result, $index);
            // grab the rows, but put them under the Index field
            while($row = mysql_fetch_assoc($result))
                $return[$row[$index]] = $row;
            mysql_free_result($result);
            return $return;
        }

        static function fetchList($query) {
            $db = new dbConnect($query);
            $return = array();
            $result = $db->actionDataset();
            while($row = mysql_fetch_row($result))
                $return[$row[0]] = $row[1];
            mysql_free_result($result);
            return $return;
        }

        static function execute($query) {
            $db = new dbConnect($query);
            if(!$db->actionDataset()) {
                warn(new Exception("Query failed: $query"));
                return false;
            }

            return true;
        }

        private $__inTransaction = false; 
        function begin() {
            if($this->__inTransaction) {
                warn(new Exception("Trying to begin() when already in a transaction!"));
                return;
            }
            $this->__inTransaction = true;
            $this->__construct("BEGIN");
        }

        function commit() {
            if(!$this->__inTransaction) {
                warn(new Exception("Trying to commit() when not in a transaction!"));
                return;
            }
            $this->__inTransaction = false;
            $this->__construct("COMMIT");
        }
        
        public function closeData() {
            $this->data->close();
        }
        public function closeDB() {
            $this->_mysqli->close();
        }
        public function ems($escape_string) {
            return $this->_mysqli->real_escape_string($escape_string);
        }

    }
?>
