<?php
    /**
    * @package QUINN
    * @abstract FRANK - loader file
    * @author monk-ee https://github.com/monk-ee
    * @version alpha
    * @copyright 2011 Exploding Box Productions
    */

    /**
    * Setup File Paths
    */
    define('INCLUDES', dirname(__FILE__));
    define('INCLUDES_CLASSES', INCLUDES . '/lib');
    /**
    * PHP 5.3+ Date Default
    * In case it has not been set in the php.ini (or set to the wrong thing)
    */
    date_default_timezone_set('Australia/Brisbane');

    /**
    *  Make very sure that clients never see error messages but warnings are logged
    */
    ini_set('display_errors', false);
    ini_set('log_errors', true);

    /**
    * start object caching for FirePHP
    */          
    ob_start();  

    /**
    * Begin session
    */
    session_start();

    /**
    * Server Configuration data
    */
    include("config.php");

    /**
    * AUTOLOADER for lib class files
    */
    spl_autoload_register('__autoload_classes');
    function __autoload_classes($class) {
        if(file_exists($f = INCLUDES_CLASSES . '/' . $class . '.inc'))
            require $f;
    }

    /**
    * DEBUG
    */
    if (DEBUG) {
        error_reporting(E_ALL);
        $firephp = FirePHP::getInstance(true); 
        $firephp->setEnabled(true);
        $firephp->registerErrorHandler();
        $firephp->registerExceptionHandler();             
    } else {
        error_reporting(0); 
        FB::setEnabled(false); 
    } 
    /**
    * Database Global
    */ 
    $GLOBALS['dbcon'] = new mysqliConnect(); 
    $GLOBALS['dbcon']->__construct("set names 'utf8';"); 
    $GLOBALS['dbcon']->__construct("set time_zone = 'Australia/Brisbane';"); 

    /**
    * Session Global
    */
    $GLOBALS['session'] = new sessionManage();

    /**
    * PreFunction Hooks
    */
    //FrankHook::add('before_handler', function() {
    //no one home
    // });

    /**
    * ROUTES - define the routes here
    */
    $site = new FrankApplication(array(
    array('/', 'MainHandler'),
    ));
    /**
    * SERVE Pages
    */
    $site->serve();
?>