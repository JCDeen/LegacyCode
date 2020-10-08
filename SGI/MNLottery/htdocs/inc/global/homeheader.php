<?php 

	die("stopped here");
	
$user_auth = UserAuth::getInstance();
$pointarray = $user_auth->getPlayer()->points;
$pointsum = 0;
for($i=0; $i<count($pointarray); $i++)
{
	$pointsum = $pointsum + $pointarray[$i];
}
//$cart = Cart::getCart();
$display_header = !isset($display_header) || $display_header;
$loggedin = $user_auth->isLoggedIn();
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">	
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
	<title><?php if(isset($page_title)) echo $page_title ?></title>
	<meta name="description" content="" />
	<meta name="keywords" content="" />
	<link rel="stylesheet" href="/app_assets/css/newstyles.css" type="text/css" />	
	<!--[if IE 6]>
		<link rel="stylesheet" href="/app_assets/css/ie6.css" type="text/css" />
	<![endif]-->
	<!--[if lt IE 7]>
	<script defer type="text/javascript" src="/inc/js/global/jquery.pngFix.js"></script>
	<![endif]-->
	<!--[if IE 7]>
	<link rel="stylesheet" type="text/css" href="/app_assets/css/ie7.css" />
	<![endif]-->
	<!--[if IE 8]>
	<link rel="stylesheet" type="text/css" href="/app_assets/css/newstyles.css" />
	<![endif]-->
	<!--  deens comment here -->
	<script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.3.1/jquery.min.js"></script>
    <script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.min.js"></script>	
	<script type="text/javascript" src="/inc/js/global/jquery.pngFix.js"></script>
	<script type="text/javascript" src="/inc/js/global/functions.js"></script>
	<script type='text/javascript' src='/inc/js/global/zapatec/utils/zapatec.js'></script>
	<script type="text/javascript" src="/inc/js/global/zapatec/zpcal/src/calendar.js"></script>
	<script type="text/javascript" src="/inc/js/global/zapatec/zpcal/lang/calendar-en.js"></script>
	<script type="text/javascript" src="/inc/js/global/jquery.scrollable-1.0.2.js"></script>
	<script type="text/javascript" src="/inc/js/global/slider.js"></script>
	<link href="/inc/js/global/zapatec/css/layout-2d.css" rel="stylesheet" type="text/css">
	<?php
		$k = 1; 
		if(isset($headers_local)) 
			echo $headers_local; 
	?>
    <script type="text/javascript">

		$(document).ready(function()
		{
			$(document).pngFix();
			if($.browser.msie)
			{
				var w = $(document).width();
				var push = ((w-971)/2);
				$("#wrapper").css("margin-left", push);
			}
		});
	</script>
	
	<!-- $theme_header_tags	-->	
</head>
<body id="games" class="internal">
	<div id="wrapper">
		<a href="#"><img id="overlay_logo" src="/app_assets/images/global/nav_logo_only.png" alt="Lucky MN The Minnesota State Lottery Players Club" height="103" width="210" border="0"/></a>
		<div id="logo">
			
		</div><!-- logo -->
		<div id="nav">
				<ul>
					<li><a id="lucky_home" href="<?php echo LUCKYMN; ?>/">Home</a></li>
                    <li><a id="lucky_contests" href="<?php echo LUCKYMN; ?>/contests/">Contests</a></li>
                    <li><a id="lucky_exclusives" href="<?php echo LUCKYMN; ?>/lucky_exclusives/">Lucky Exclusives</a></li>
                    <li><a id="lucky_fun_stuff" href="<?php echo LUCKYMN; ?>/fun_stuff/">Fun Stuff</a></li>
                    <li><a id="lucky_winners" href="<?php echo LUCKYMN; ?>/winners/">Winners</a></li>
                    <li><a id="lucky_events" href="<?php echo LUCKYMN; ?>/events/">Events</a></li>
				</ul>
			</div><!-- nav -->
		<div id="content">
			<div id="signin_internal">
                <div class="TopButtons">
                    <?php if($loggedin): ?>
                        <div class="loggedin-btns">

                                <div class="left">
                                    <div class="LoggedIn">

                                        <span class="Points">
											&nbsp;
                                        </span>
                                    </div><!-- LoggedIn -->
                                </div><!-- left -->
                                <div class="right">
                                	<div class="LoggedInRight">
									<!-- 
                                        <a href="/users/points.php">My WPT&reg; Account</a>
									-->
                                        <a href="/users/logout.php">Log Out</a>
									
                                    </div><!-- LoggedInRight -->
                                </div><!-- right -->

                        </div><!-- loggedin-btns -->
                    <?php endif; ?><!-- $loggedin -->
                </div><!-- TopButtons -->
            </div><!-- signin_internal -->
               <div id="internal_no_left_nav">	
				<div id="internal_banner"> <!-- internal banner if used --> </div>				
				<div class="Content">
                	<div class="LeftNav">
                    	<div class="LeftNav_Inner">
                        	<ul id="nav_accordion" class="left_nav_content">
                            	<li>
                               		<a href="http://www.luckymn.com/contests/second_chance_contests/" class="LeftNav_Header" alt="Second Chance Drawings" title="Second Chance Drawings">Second Chance Drawings</a>
                                </li>
                                <li>
                                	<a href="/" class="LeftNav_Header" alt="Scratch-O-Rama&reg;" title="Scratch-O-Rama&reg;">Scratch-O-Rama&reg;</a>
                                    <ul class="LeftNav_Sub">
                                    	<li><a href="/" alt="Home" title="Home">Home</a></li>
                                        <li><a href="/entry/index.php" alt="Enter Tickets" title="Enter Tickets">Enter Tickets</a></li>
										<li><a href="/prizes/index.php" alt="Redeem Points" title="Redeem Points">Redeem Points</a></li>
                                        <li><a href="/winners.php" alt="Drawing Info" title="Drawing Info">Drawing Info</a></li>
                                        <li><a href="/help.php" alt="Help/FAQ" title="Help/FAQ">Help/FAQ</a></li>
                                        <li><a href="/feedback.php" alt="Feedback" title="Feedback">Feedback</a></li>
										<li><a href="/rules.php" alt="Rules/Terms/Legal" title="Rules/Terms/Legal">Rules/Terms/Legal</a></li>
                                    </ul>
                                </li>
                                <li>
                                	<a href="http://www.luckymn.com/contests/other_contests/" class="LeftNav_Header" alt="Other Contests" title="Other Contests">Other Contests</a>
                                </li>
                            </ul>
                        </div><!-- LeftNav_Inner -->
                        <div class="LeftNav_Bottom">
                        </div><!-- LeftNav_Bottom -->
                    </div><!-- LeftNav -->
               		<div class="HomeHeader">
                       	<img src="/app_assets/images/home/home-masthead-new.jpg" alt="Minnesota Points For Prizes - Earn Points to redeem for Great Merchandise" width="771" height="279" usemap="#Map" />
                   	</div>	<!-- HomeHeader -->
                    <div id="clearFloats">
                    </div>
               	</div><!-- Content -->
            
