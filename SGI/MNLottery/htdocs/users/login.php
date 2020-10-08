<?php require_once($_SERVER['DOCUMENT_ROOT']."/inc/.php/global.php");?>
<?php 

$user_auth = UserAuth::getInstance();
$user_auth->ensureLogin();

redirect($_SESSION['redirect']);

?>