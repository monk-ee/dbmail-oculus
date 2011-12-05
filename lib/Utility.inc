<?php
  class Utility {
      public static function decodePayload($asArray=true) {
          if (!isset($_POST['payload'])) {
              fb::log("No Payload");
              return false;
          }
          return json_decode($_POST['payload'],$asArray);
      }
      public static function encodePayload($payload) {
        echo json_encode($payload);     
      }
      public static function toggleStatus($value) {
          if ($value) {
              return;
          }
          $value = false;
      }
  }
?>