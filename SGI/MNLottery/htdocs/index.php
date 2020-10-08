<?php 

require 'app_common/helper.php';

if(isDevSession()) {
	header('location: testlogin.php');
	exit();
}

header('location: entry/');
exit();
