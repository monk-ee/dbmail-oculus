<?php
  define("DEBUG",true);
  define("DEV",true); 
  define("CACHE",false);
  //db stuff
  if (DEV) {
      define("mysql_connect","localhost");
      define("mysql_user","oculus"); 
      define("mysql_pass","Z42Ye4aGrK86Q4KQ"); 
      define("mysql_db","lfc_core"); 
  } else {
      define("mysql_connect","localhost");
      define("mysql_user",""); 
      define("mysql_pass",""); 
      define("mysql_db",""); 
  }
  //cookie name
  define("cookie_name","oculus"); 
  //crypt salt
  define("hashsalt","\$2a\$10\$fXBYyUdSfpO7VEFEG8LJXu"); 

  //memcache
  define("memcache_host","localhost"); 
  define("memcache_port","11211"); 
?>
