<?php
	/***
	This script utilizes 2 different methods to ensure that only intended individuals can see the results.
	1: It MUST be sent using https:// in the URL ( instead of http:// ) .
	2: You MUST have the cookie named 'HotCookie' set to "Jessica Biel" for the domain this script is running on.
	   This was done manually on the developer's machine using Firebug in Firefox.
	 */
?>
<?php require_once($_SERVER['DOCUMENT_ROOT']."/inc/.php/global.php");?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" dir="ltr">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8;charset=utf-8" />
	<title>Preflight</title>
	<?php include_once 'app_common/inc-scripts.php'; ?>
	<script type="text/javascript">
     </script>
</head>

<body>

<h2>
Have index page parse mdi.ini for SELF_CHECK binary var.
IF true then perform self_check for links, etc. ???

</h2>
<?php
	// Skip the security checks if this is running locally on JCDeen's WAMP.
	if (isset($_SERVER['LocalWamp']) && $_SERVER['LocalWamp']=='JCDeen') {
		echo $_SERVER['LocalWamp'];
		//continue;
	}
	else 
	{
		// Die if this cookie is not set in web browser.
		// It can be done manually in Firefox using Firebug.
		if (!isset($_COOKIE['HotCookie']) || $_COOKIE['HotCookie']!='Jessica Biel') {
			die ("reported"); 
		}
		
		// Die if request is not using SSL ( https://).
		if (!isset($_SERVER['HTTPS']) || strtoupper($_SERVER['HTTPS'])!='ON') {
			die ("reported "); 
		}
	}
?>

Parsing ini file ...<br>
<?php include_once 'app_common/inc-header.php';?>
<div id="page" class="clearfloat">
	<div id="page-wrapper">
        <?php include_once 'app_common/inc-nav.php'; ?>
		<div id="rounded-content">
			<div class="content node">
				<br><h3>Parsed mdi.ini Settings</h3>
			<?php
				// at this point we have all the ini_array settings!
				echo "\$_SERVER[REMOTE_ADDR]: " . $_SERVER[REMOTE_ADDR];
				
				$iniSettings = parse_ini_file("mdi.ini", true);
				foreach ($iniSettings as $iniSection => $settingName) 
				{
					echo "<br><b>$iniSection:</b> <table style=\"border: solid black 1px;\">";
					foreach ($settingName as $theVar => $theVal)
					{
						switch ($theVar) {
							case 'entry_index_banner_file':
							case 'entry_index_ticket_front_file':
								$theValDisp = "<a href=\"app_assets/images/$theVal\">$theVal</a>";
								break;
								
							default:
								$theValDisp = $theVal;
						}
						echo "<tr><td> $theVar </td><td> $theValDisp </td></tr>";
					}
					echo "</table>";
				}
				
				$mySettings = SystemSettings::getInstance();
			?>
			</div>
		</div>
	</div>
</div>

<br><hr>
<?php

	$databaseSettings_CompareArray['wamp'] = 
		array('mdi_section_tag' => 'database_wamp', 'db_name' => 'mn_vikings_dev', 'db_user' => 'mnlottery', 'db_pass' => '!password', 'db_host' => 'localhost');

	$databaseSettings_CompareArray['live'] = 
		array('mdi_section_tag' => 'database_live', 'db_name' => 'mnsecondchancegamezone_com', 'db_user' => 'sgi' ,'db_pass' => 'ViatnexDi', 'db_host' => 'localhost');

	$databaseSettings_CompareArray['uat'] = 
		array('mdi_section_tag' => 'database_uat', 'db_name' => 'mnsecondchancegamezone_com_dev', 'db_user' => 'sgi_dev', 'db_pass' => 'lorjAljIbs', 'db_host' => 'localhost');
		
	echo "<hr>$iniSettings[database][db_name]<br>";

	foreach ($databaseSettings_CompareArray as $platformTag => $platformType) 
	{
		echo "$platformTag ";	
		$dbNameVal = $databaseSettings_CompareArray[$platformTag];
		echo "$dbNameVal[db_name] <br>";
		
		if ($iniSettings['database']['db_name'] == $databaseSettings_CompareArray[$platformTag]['db_name']) {
			echo "<h2>Configured for $platformTag</h2><br>";
		}
	}
	
	$handle = fopen("http://www.example.com/", "r");
	$k=1;
/**	
	echo "<hr>mySettings [][][]" ;//. $mySettings['ini_array'] . "<br>";
	$theDbStuff = $mySettings->getSection('database');
	echo print_r($theDbStuff, true);
	***/
?>
<ul>
	<li>Display banner and ticket files
	<li>Display and verify that each Navmenu hyperlink works
	<li>Display which platform the mdi.ini is configured for
	<li>Display contest start/stop time/date
	<li>Display game number; any promo_assoc_games; game type (instant, voucher, etc. )
	<li>Provide way to specifically scan rendered /entry/index.php page ...
	<li>Scan rendered page for all Javascript includes such as jQuery, etc.
	<li>Scan rendered page for mixed mode links <i>HTTP:// vs HTTPS:// </i>
</ul>

<?php include_once 'app_common/inc-footer.php'; ?>
</body>
</html>
