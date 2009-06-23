<?php
if (!function_exists('toLog')) {
 /**
  * Get var dump as a string
  *
  * @param $Var
  * @return boolean
  */
 function print_r_log($Var) {
  ob_start();
  print_r($Var);
  $ret_str = ob_get_contents();
  ob_end_clean();
  return $ret_str;
 }

 /**
  * Retrieve all different elements
  *
  * @param $Message
  * @return boolean
  */
 function toLog( $Message=false, $Method="a", $FileName="error.log" ) {
  if(is_array($Message) or is_object($Message)) $Message = print_r_log($Message);
  $fp=fopen("{$FileName}", $Method);
  if($FileName=="error.log")
  fwrite($fp, "LOG ".date("Y-m-d H:i:s")."\n".$Message."\n" );
  else
  fwrite($fp, $Message );
  fclose($fp);
  return true;
 }
}

?>