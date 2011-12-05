<?php
  define("DEBUG",true);
  define("DEV",false); 
  define("CACHE",false);
  //db stuff
  if (DEV) {
      define("mysql_connect","");
      define("mysql_user",""); 
      define("mysql_pass",""); 
      define("mysql_db",""); 
  } else {
      define("mysql_connect","");
      define("mysql_user",""); 
      define("mysql_pass",""); 
      define("mysql_db",""); 
  }
  //cookie name
  define("cookie_name",""); 
  define("salt",""); 

  //memcache
  define("memcache_host","localhost"); 
  define("memcache_port","11211"); 
?>
