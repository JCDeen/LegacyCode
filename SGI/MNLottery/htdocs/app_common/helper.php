<?php

$LUCKYMN = 'http://www.luckymn.com';


/*****************
 Useful Functions
 *****************/
function isDevSession(){
	if (strpos($_SERVER['HTTP_HOST'], 'uat.secondchancebonuszone'))
		return true;
	else
		return false;
}

function isLoggedIn(){
	if(UserAuth::getInstance()->isLoggedIn())
		return true;
	else
		return false;
}