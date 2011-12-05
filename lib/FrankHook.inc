<?php
 class FrankHook {
    private static $instance;
  
    private $hooks = array();
  
    private function __construct() { }
    private function __clone() { }
  
    public static function add($hook_name, $fn) {
        $instance = self::get_instance();
        $instance->hooks[$hook_name][] = $fn;
    }
  
    public static function fire($hook_name, $params = NULL) {
        $instance = self::get_instance();
        if (array_key_exists($hook_name, $instance->hooks)) {
            foreach ($instance->hooks[$hook_name] as $fn) {
                call_user_func_array($fn, array(&$params));
            }
        }
    }
  
    public static function get_instance() {
        if (!isset(self::$instance)) {
            self::$instance = new FrankHook();
        }
        return self::$instance;
    }
} 
?>