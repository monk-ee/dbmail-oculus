<?php
      class LogoutHandler extends FrankHandler {
        public function get() {
            echo sessionManage::returnToken();    
        }
        public function post() {
            echo sessionManage::returnToken();
        }
    }
?>