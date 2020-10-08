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
	
	$form_name = isset($_REQUEST['form_name']) ? $_REQUEST['form_name'] : ''; // Get form name if it was passed.
	$mn_player_id = $_REQUEST['mn_player_id'];
	
	require_once($_SERVER['DOCUMENT_ROOT']."/inc/.php/global.php");
	$db = DK_DB::getConnection();
	
	if ($form_name == 'unlock_form') {
		$phrase2 =  ($mn_player_id == 0) ? " player_id in (100,200,300,400,500,600,700)" : " player_id='$mn_player_id'";
		$query = "DELETE FROM strikes where " . $phrase2;
		$result = $db->query($query);
		$fake=1;
	}
	
	$strikesCnt = array(100=>0, 200=>0, 300=>0, 400=>0, 500=>0, 600=>0, 700=>0);

	$query = "Select * FROM strikes where player_id in (100,200,300,400,500,600,700)";
	$result = $db->query($query);
	while ($obj = $db->resultObject($result)) {
		$strikesCnt[$obj->player_id] = $obj->strikes;
	}


	//creating 100, 400, 500, 600 & 700 as requested.
	
	function get_login_form($mn_player_id, $fname, $lname, $email, $dest="entry", $address1='123 Monkey Street', $city = 'CLEVELAND', $state = 'OH', $zip = '44114')
	{
		// These values never really change so we just hard code them here:
		$address2 = 'address 2';
		$dob = '1955-05-05';
		$phone = '555-555-1239'; 
		
		$checksum_str = $mn_player_id.$fname.$lname.$email.$address1.$address2.$city.$state.$zip.$dob.$phone.UserAuthPassthru::getChecksumKey();
		$chksum = md5($checksum_str);
		
		$subBtnAppend = $dest=='entry' ? 'entry' : 'redeem';
		
		$formStr = "<form name=\"testform\" action=\"/users/loginproc.php\" method=\"post\"  style=\"display: inline;\">" .
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
			"<input type=\"hidden\" name=\"destination\" value=\"$dest\" >" .
			"<input type=\"submit\" name=\"submit\" value=\"Login Player $mn_player_id\" title=\"Login Test Account $mn_player_id\">" .
			"</form>\n";
		
		return ($formStr);
	}
	
	function get_unlock_form($userID)
	{
		global $strikesCnt;
		$btnLabel = ($userID == 0) ? "Unlock ALL" : "Unlock $userID";
		$titleStr = ($userID == 0) ? "Unlock ALL Test Accounts" : "Unlock Test Account $userID";
		$btnMode = (($strikesCnt[$userID] == 0) && ($userID!= 0)) ? ' disabled ' : '' ;
		
		$formStr = "<form name=\"unlockform\" action=\"$_SERVER[PHP_SELF]\" method=\"post\" style=\"display: inline;\">" .
		"<input type=\"hidden\" name=\"form_name\" value=\"unlock_form\"/>" .
		"<input type=\"hidden\" name=\"mn_player_id\" value=\"$userID\"/>" .
		"<input type=\"submit\" name=\"submit\" value=\"$btnLabel\" title=\"$titleStr \" $btnMode >" .
		"</form>\n";
		
		return ($formStr);
	}
	
	function get_login_row ($userID) {
		global $strikesCnt;
		$tn = $userID/100;
		$cntColor = ($strikesCnt[$userID] == 0) ? "style= \"color:LightGrey;\"" : '' ;

		$localServerStr =  (isset($_SERVER['LocalWamp'])) ? "http://admin.mn.localhost" : 'http://uat-admin.mnsecondchancegamezone.com';
		
		$rowStr = "<tr><td>$userID</td><td>";
		$rowStr .= get_login_form($userID, "Test" .$tn, "Tester" . $userID, "Tester" . $userID . "@digiknow.com"); 
		$rowStr .=  "</td><td $cntColor>" . $strikesCnt[$userID] . "</td><td>". get_unlock_form($userID) . "</td>";
		$rowStr .=  "<td><input type=\"button\" value=\"UAT Entries: Player $userID\" onClick=\"window.open('$localServerStr/feeds/test.ticket_history.php?player_id=$userID&form_submitted=true','_blank');\"></td></tr>";
		return ($rowStr);
	}
	
?>

	<p>
	<?php echo date('M d, Y H:m:s'); ?>
	</p>
	<table style="text-align: center; border: solid black 1px;">
	<tr><thead style="background-color: #F5F5F5;"><tr><th style=" padding-left:3px; padding-right:3px;"> Test ID </th><th> Login </th><th style="padding-left:3px; padding-right:3px;"> Strikes </th><th> Reset </th><th> Entries History<br><i>( new window/tab )</i></th></thead></tr>
		<?php 
			echo get_login_row(100); 
			echo get_login_row(200);
			echo get_login_row(300);
			echo get_login_row(400);
			echo get_login_row(500);
			echo get_login_row(600);
			echo get_login_row(700);
		?>
		<tr><td colspan=3>&nbsp;</td><td><?php echo get_unlock_form(0); ?></td></tr>
	</table>
