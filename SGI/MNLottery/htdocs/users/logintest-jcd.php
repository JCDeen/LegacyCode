<?php require("$_SERVER[DOCUMENT_ROOT]/inc/.php/global.php"); ?>
<?php
if (strpos($_SERVER['DOCUMENT_ROOT'], 'live') === false)
{
	echo "DEV/UAT: $_SERVER[DOCUMENT_ROOT]<br>";
}
else
{
	//if (!isset($_GET['justTesting'])) {
	//	die('no access');
	//}
}

$form_name = isset($form_name) ? $form_name : '';

	//creating 100, 400, 500 and 600 as requested.
	
	function echo_login_form($mn_player_id, $fname, $lname, $email, $dest="entry", $address1='2205 Spring Hill Rd.', $city = 'Smyrna', $state = 'GA', $zip = '30080', $submitText='Login TestAccount ')
	{
		// These values never really change so we just hard code them here:
		//$address1 = '1234 Monkey Street';
		$address2 = '.....';
		$dob = '1959-08-13';
		$phone = '555-555-1239'; 
		
		$checksum_str = $mn_player_id.$fname.$lname.$email.$address1.$address2.$city.$state.$zip.$dob.$phone.UserAuthPassthru::getChecksumKey();
		$chksum = md5($checksum_str);
		//$chksum = md5('12332131');
		
		$subBtnAppend = $dest=='entry' ? 'entry' : 'redeem';
		
		echo "<form name=\"testform\" action=\"/users/loginproc.php\" method=\"post\">" .
			"<input type=\"hidden\" name=\"playerID\" value=\"$mn_player_id\">" .
			"<input type=\"hidden\" name=\"firstName\" value=\"$fname\">" .
			"<input type=\"hidden\" name=\"lastName\" value=\"$lname\">" .
			"<input type=\"hidden\" name=\"email\" value=\"$email\">" .
			"<input type=\"hidden\" name=\"address1\" value=\"$address1\">" .
			"<input type=\"hidden\" name=\"address2\" value=\"$address2\">" .
			"<input type=\"hidden\" name=\"city\" value=\"$city\">" .
			"<input type=\"hidden\" name=\"stateAbbr\" value=\"$state\">" .
			"<input type=\"hidden\" name=\"zip\" value=\"$zip\">" .
			"<input type=\"hidden\" name=\"dob\" value=\"$dob\">" .
			"<input type=\"hidden\" name=\"phone\" value=\"$phone\">" .
			"<input type=\"hidden\" name=\"chksum\" value=\"$chksum\">" .
			"<input type=\"hidden\" name=\"destination\" value=\"$dest\"/>" .
			"<input type=\"submit\" name=\"submit\" value=\"$submitText $mn_player_id ($subBtnAppend)\">" .
			"</form>\n";
	}

	function echo_unlock_form($mn_player_id, $boolShowPlayerID=false)
	{
		$displayID = $boolShowPlayerID ? '' : $mn_player_id;
		echo "<form name=\"unlockform\" action=\"$_SERVER[PHP_SELF]\" method=\"post\">" .
			"<input type=\"hidden\" name=\"form_name\" value=\"unlock_form\"/>" .
			"<input type=\"hidden\" name=\"mn_player_id\" value=\"$mn_player_id\"/>" .
			"<input type=\"submit\" name=\"submit\" value=\"Unlock $displayID\">" .
			"</form>\n";
	}
	
	if ($form_name == 'unlockform')
	
	echo_login_form(105063, 'James', 'deen', 'james.deen@cont.scientificgames.com');
	echo_unlock_form(105063);
	
	echo_login_form(124204, 'jimbo', 'deen', 'jimbo.deen@yahoo.com');
	echo_login_form(151482, 'James-Tester', 'deen', 'nospam@amazingrobotics.com');
	
	echo print_r($_REQUEST, true);
?>