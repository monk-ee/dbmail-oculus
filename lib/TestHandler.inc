<?php
    class TestHandler extends FrankHandler {
        private $page;

        public function get() {
            $db = $GLOBALS['dbcon']; 
            $query = "SELECT *
            FROM tmp_experience te, tmp_category tc
            WHERE te.category_id = tc.id";
            $data = $db->queryDB($query,"da");  
            foreach ($data as $value) {
                $query = "INSERT INTO `dbquinna_core`.`experience` (
                `_id` ,
                `start_date` ,
                `end_date` ,
                `category_name`
                )
                VALUES (
                '".$value['_id']."', '0000-00-00 00:00:00', '0000-00-00 00:00:00', '".$value['category_name']."'
                );";
                $db->queryDB($query);
        }
        }
        public function post() {
            echo "post test";
        }
        private function regionMap(){
            $db = $GLOBALS['dbcon']; 
            $query = "SELECT *
            FROM tmp_region tr, tmp_region_map trp
            WHERE tr.region_id = trp.region_id";
            $data = $db->queryDB($query,"da");  
            foreach ($data as $value) {
                $query = "INSERT INTO `dbquinna_core`.`regions` (
                `_id` ,
                `start_date` ,
                `end_date` ,
                `region_name`
                )
                VALUES (
                '".$value['_id']."', '0000-00-00 00:00:00', '0000-00-00 00:00:00', '".$value['region_name']."'
                );";
                $db->queryDB($query);
            }
        }
    }
?>