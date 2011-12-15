<?php
    class AjaxHandler extends FrankHandler {
        public function get() {
            //unset session
            //unset($_SESSION['ajaxdata']);
            //create a value
            $ajaxSingleData = rand(0,150);
            //check for session ajax data:
            if (isset($_SESSION['ajaxdata'])) {
                //set from session
                $ajaxData =   $_SESSION['ajaxdata'];
            } else {
                //create empty
                $ajaxData = array();
            }
            $count = count($ajaxData);
            //ok if count id bigger than lets say 50 start popping valuies of array
            if ($count >= 30) {
                array_shift($ajaxData);
            }
            $count = count($ajaxData); 
            //add to array
            $ajaxData[$count][] = $ajaxSingleData;
            //set into session
            $_SESSION['ajaxdata'] = $ajaxData;
            //now encode and return
            $ajaxData = json_encode($ajaxData);
            echo $ajaxData;
        }
    }
?>