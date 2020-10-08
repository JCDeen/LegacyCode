<?php
require_once($_SERVER['DOCUMENT_ROOT']."/inc/.php/global.php");

/***********************************************/
/* This is the NEW August 2012 loginproc.php ! */
/***********************************************/

$log_redirect_newplayers = true; // INSERT redirect data into DB table redirection_log.
$log_redirect_updates = true; // INSERT redirect data into DB table redirection_log.

if(!isset($_SESSION['redirect'])) {
	$_SESSION['redirect'] = '/';
}
$_SESSION['destination'] = 'entry';

$p = new Player();
$statusPlayerIsInDB = $p->getPlayerById($_REQUEST['playerID']);

// Map the form's $_POST vars to the player table vars.
$mapFormToDB = array('playerID'=>'id',	'firstName'=>'first_name', 
	'lastName'=>'last_name', 'email'=>'email', 'address1'=>'address1',
	'address2'=>'address2',	'city'=>'city', 'stateAbbr'=>'state',
	'zip'=>'zip',	'dob'=>'dob', 'phone'=>'phone1');

// Loop thru the form vars. If anything is different from DB then update the DB.
$lmn_player = array();
$bUpdateThePlayer = false;
foreach ($_POST as $postKey => $postVal) 
{
	if (array_key_exists($postKey, $mapFormToDB)) 	{
		$lmn_player[$postKey] = $postVal;
		if ($postVal != $p->$mapFormToDB[$postKey]) {
			$bUpdateThePlayer = true;
		}
	}
}

// Verify the checksum value here.
$checksum_str = $lmn_player['playerID']. $lmn_player['firstName']. $lmn_player['lastName']. $lmn_player['email']. $lmn_player['address1'].
	$lmn_player['address2']. $lmn_player['city']. $lmn_player['stateAbbr']. $lmn_player['zip']. $lmn_player['dob']. $lmn_player['phone'].
	UserAuthPassthru::getChecksumKey();
$chksumMd5 = md5($checksum_str);

if ($_POST['chksum'] !=  $chksumMd5) {
	error_log("$_POST[chksum] != $chksumMd5 for player_id=$lmn_player[playerID]");
	UserAuth::redirToLogin();
}

//if player exists in our DB...
if($statusPlayerIsInDB != false)
{
	//if the player has updated his/her data on LuckyMN, update our DB to reflect the new data.
	if($bUpdateThePlayer)
	{
		$p->updateFromLMN($lmn_player);
		
		if ($log_redirect_updates) {
			$query = "INSERT INTO redirection_log SET source='$_SERVER[PHP_SELF]', ";
			$query2 = " referer=REPLACE('$_SERVER[HTTP_REFERER]','://www.luckymn.com/contests/second_chance_contests','...luckymn...'),"; 
			$query3 = " lmn_player_id=$p->id, ";
			$query4 = " data='BEEFIER! UPDATE diff(\$lmn_player)=" . print_r(array_diff_assoc($lmn_player, $p->getPlayerAsArray()),true) . ", "; 
			$query5 = " diff(\$p)=" . print_r(array_diff_assoc($p->getPlayerAsArray(), $lmn_player),true) . ", "; 
			$query7 = "\$_REQUEST:" . print_r($_REQUEST, true) . " END_STR'";
			$result = DK_DB::getConnection()->query($query.$query2.$query3.$query4.$query5.$query6.$query7);
		}
	}
	
	$_SESSION['player'] = $p->getPlayerAsArray();
	$user_auth = UserAuth::getInstance();
	$user_auth->is_logged_in = true;
	$_SESSION['is_logged_in'] = true;
	$p->updateLastLogin();
	$p->resetLoginLockout();
	$user_auth->ensureLogin();
	
	//make this go to the home page.
	header('Location: ' . PFP_SITE );
	exit(0);
	
//if the player doesn't exist in our DB, insert him/her
} else if ($statusPlayerIsInDB == false)
{
	$p->saveFromLMN($lmn_player);	
	
	$_SESSION['player'] = $p->getPlayerAsArray();
	$user_auth = UserAuth::getInstance();
	$user_auth->getInstance()->is_logged_in = true;
	$_SESSION['is_logged_in'] = $user_auth->getInstance()->is_logged_in;
	$p->updateLastLogin();
	$p->resetLoginLockout();
	$user_auth->ensureLogin();
	
	if ($log_redirect_newplayers) {
		$query = "INSERT INTO redirection_log SET source='$_SERVER[PHP_SELF]', ";
		$query2 = " referer=REPLACE('$_SERVER[HTTP_REFERER]','://www.luckymn.com/contests/second_chance_contests','...luckymn...'),"; 
		$query3 = " lmn_player_id=$p->id, ";
		$query4 = " data='BEEFIER! NEW diff(\$lmn_player)=" . print_r(array_diff_assoc($lmn_player, $p->getPlayerAsArray()),true) . ", "; 
		$query5 = " diff(\$p)=" . print_r(array_diff_assoc($p->getPlayerAsArray(), $lmn_player),true) . ", "; 
		$query7 = "\$_REQUEST:" . print_r($_REQUEST, true) . " END_STR'";
		$result = DK_DB::getConnection()->query($query.$query2.$query3.$query4.$query5.$query6.$query7);
	}
	
	header('Location: ' . PFP_SITE );
	exit(0);
}
?>
