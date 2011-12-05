<?php
    /** Filename: session management Class
    * Description: Main functions include
    * @author monkee
    * @version v2.1
    *
    */

    /**
    * session management class
    *
    */
    class sessionManage {
        const VISITOR = 1;
        const MANAGER = 2;
        const ADMINISTRATOR = 4;
        
        public function __construct() {
            fb::warn($this->checkSessionExists(),"SESSION");
            fb::log($_SESSION,"SESSION Data"); 
        }
        public static function hashPassword($password) {
              return crypt($password, hashsalt.'\$');            
        }
        public static function generateCryptToken() {
            $random_id_length = 32; 
            $rnd_id = crypt(uniqid(rand(),1)); 
            $rnd_id = strip_tags(stripslashes($rnd_id)); 
            $rnd_id = str_replace(".","",$rnd_id); 
            $rnd_id = strrev(str_replace("/","",$rnd_id)); 
            $rnd_id = substr($rnd_id,0,$random_id_length);    
            return $rnd_id;
        }
        
        public static function setSession($token,$_id,$administrator=false) {
            $_SESSION['token'] = $token;
            $_SESSION['_id'] = $_id;                      
            $_SESSION['administrator'] = $administrator;
            session_write_close();
        }
        
        public static function setSessionAndGo($token,$_id,$administrator=false) {
            $_SESSION['token'] = $token;
            $_SESSION['_id'] = $_id;                
            $_SESSION['administrator'] = $administrator;      
            session_write_close();
            header("Location: /");
        }

        public static function setSessionAndLocate($token,$_id,$administrator=false,$locate = "/") {
            $_SESSION['token'] = $token;
            $_SESSION['_id'] = $_id;                
            $_SESSION['administrator'] = $administrator;      
            session_write_close();
            header("Location: ".$locate);
        }

        public static function checkSessionExists() {
            if (isset($_SESSION['token'])) {
                return true;
            } else {
                return false;
            }
        }
        
        public static function checkAdministrator() {
            if (!isset($_SESSION['administrator'])) {
                return false;
            }
            return  (bool) $_SESSION['administrator'];
        }     
           
        public static function authenticate($auth) {
            fb::warn($auth,"AUTH MASK");
            if (!isset($_SESSION['administrator'])) {
                $admin = false;
            }
            $admin = (bool) $_SESSION['administrator'];
            
            switch($auth) {
                case 7: //public access - everyone
                        break;
                case 6: // registered users - everyone who is logged in
                        if (!isset($_SESSION['token'])) throw new Exception("You need to be logged in.");  
                        break;
                case 4: //only admins
                        if (!$admin) Throw new Exception("Administrators Only");      
                        break;
                case 2: //only managers even restricts admin users
                        if (!isset($_SESSION['token'])) throw new Exception("Users Only");
                        if ($admin) throw new Exception("Administrators cannot act as Users");
                        break;
                case 1: //only not logged in people - eg login page!
                        if (isset($_SESSION['token'])) throw new Exception("Not for the logged in."); 
                        break;
                default: //wig out here
                    throw new Exception("Class MUST have an authentication state."); 
            }
             
        }
        
        public static function compareTokens($token) {
            if (!isset($_SESSION['token'])) {
                return false;  
            } 
            if ($_SESSION['token'] == $token ) {
                return true;
            }
            return false;
        }

        public static function returnToken() {
            if (isset($_SESSION['token'])) {
                return $_SESSION['token'];
            } else {
                return 0;
            }
        }

        public static function returnAccountID() {
            if (isset($_SESSION['_id'])) {
                return $_SESSION['_id'];
            } else {
                return 0;
            }
        }
        public static function logout() {
            session_destroy();
            session_write_close();
            header("Location: /");
        }

    }
?>
