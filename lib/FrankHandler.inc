<?php
  class FrankHandler {
    public function __construct() { }

    public function __call($name, $arguments) {
        header('HTTP/1.0 404 Not Found');
        echo '404 Not Found';
        exit;
    }
}
?>