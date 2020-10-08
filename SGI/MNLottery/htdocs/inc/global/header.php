<?php 

$user_auth = UserAuth::getInstance();
$display_header = !isset($display_header) || $display_header;

if (!isset($ignore_password_update)) {
	$ignore_password_update = false;
}
//$cart = Cart::getCart();
$loggedin = $user_auth->isLoggedIn($ignore_password_update);
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
	<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
		<title><?php if(isset($page_title)) echo $page_title ?></title>
		<meta name="description" content="" />
		<meta name="keywords" content="" />	
        <link rel="stylesheet" href="/app_assets/css/newstyles.css" type="text/css" />		
		<!--[if lt IE 7]>
		<script defer type="text/javascript" src="/inc/js/global/jquery.pngFix.js"></script>
		<![endif]-->
		<!--[if IE 7]>
		<link rel="stylesheet" type="text/css" href="/app_assets/css/ie7.css" />
		<![endif]-->
		<!--[if IE 6]>
		<link rel="stylesheet" href="/app_assets/css/ie6.css" type="text/css" />
		<![endif]-->
		<!--[if IE 8]>
		<link rel="stylesheet" type="text/css" href="/app_assets/css/newstyles.css" />
		<![endif]-->
		<script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.3.1/jquery.min.js"></script>
        <script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.min.js"></script>		
		<script type="text/javascript" src="/inc/js/global/jquery.pngFix.js"></script>
		<script type="text/javascript" src="/inc/js/global/functions.js"></script>
		<script type='text/javascript' src='/js/global/zapatec/utils/zapatec.js'></script>
		<script type="text/javascript" src="/js/global/zapatec/zpcal/src/calendar.js"></script>
		<script type="text/javascript" src="/js/global/zapatec/zpcal/lang/calendar-en.js"></script>
		<link href="/inc/js/global/zapatec/css/layout-2d.css" rel="stylesheet" type="text/css">
		<?php if(isset($headers_local)) echo $headers_local; ?>
        <script type="text/javascript">

		<!-- HEADER.PHP -->	
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
	</head>
	<body id="games" class="internal">
		<div id="wrapper">
        	<a href="#" class="logoAnchor"><img id="overlay_logo" src="/app_assets/images/global/nav_logo_only.png" alt="Lucky MN The Minnesota State Lottery Players Club" height="103" width="210" border="0"/></a>
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
                    <?php if($display_header): ?>
                    	<div class="TopButtons">
                            
                            <?php if($loggedin): ?>
                                <div class="loggedin-btns">
                                    
                                    <?php if($user_auth->isLoggedIn($ignore_password_update)):  ?>
                                        <div class="left">
                                            <div class="LoggedIn">
                                                Hello, <?php echo UserAuth::getInstance()->getPlayer()->first_name ?>. 
                                                <span class="Points">
													<!-- 
													<a href="/users/points.php">You have <?php echo array_sum(UserAuth::getInstance()->getPlayer()->points) ?> points.</a>
													-->
													<a href="/users/points.php">You have <?php echo $user_auth->getPlayer()->points ?> points.</a>
												</span>
                                            </div>
                                        </div>
                                        <div class="right">
											<!-- 
                                            <a href="/users/points.php">My Vikings Account</a> | 
											-->
                                            <a href="/users/logout.php">Log Out</a> 

											<?php if($cart->isEmpty()): ?>
                                                Your cart is empty.
                                            <?php else: ?>
                                                Your cart contains: <a href="/cart/"><?php echo array_sum($cart->getNumItemsByGame()) ?> 
                                                item<?php if(array_sum($cart->getNumItemsByGame()) > 1): ?>s<?php endif; ?></a>
												<a href="/cart/"><img src="/app_assets/images/global/cart.png" width="19" height="16" alt="Cart"/></a>
                                            <?php endif; ?>

                                        </div><!-- right -->
                                    <?php endif; ?>
                                    
                                </div><!-- loggedin-btns -->

                            <?php endif; ?>
                            
                        </div><!-- TopButtons -->
                    </div><!-- signin-internal -->
                    <?php endif; ?>
                    <?php if($display_header): ?>
                    	<div id="internal_banner">
                        </div>
                    <?php endif; ?>
                
                    <div class="Content">
                    	<div class="LeftNav">
                            <div class="LeftNav_Inner">
                                <ul id="nav_accordion" class="left_nav_content">
								<li>
                               		<a href="http://www.luckymn.com/contests/my_second_chance_entries/" class="LeftNav_Header" alt="My Second Chance Entries" title="My Second Chance Entries">My Second Chance Entries</a>
                                </li>
                            	<li>
                               		<a href="http://www.luckymn.com/contests/second_chance_contests/" class="LeftNav_Header" alt="Second Chance Contests" title="Second Chance Contests">Second Chance Contests</a>
                                </li>
                                <li>
                                	<a href="/" class="LeftNav_Header" alt="Vikings" title="Vikings">Vikings</a>
                                    <ul class="LeftNav_Sub LeftNav_Sub_SHARED">
                                    	<!--   <li><a href="/" alt="Home" title="Home">Home</a></li> -->
                                        <li><a href="/entry/" alt="Enter Tickets" title="Enter Tickets">Enter Tickets</a></li>
										<li><a href="/prizes/index.php" alt="Redeem Points" title="Redeem Points">Redeem Points</a></li>
                                        <li><a href="http://www.luckymn.com/helpfaq/helpfaq/" alt="Help/FAQ" title="Help/FAQ">Help/FAQ ttttt</a></li>
                                        <li><a href="http://www.luckymn.com/contests/second_chance_contests/vikings/feedback/" alt="Feedback" title="Feedback">Feedback</a></li>
										<li><a href="http://www.luckymn.com/contests/second_chance_contests/vikings/rules/" alt="Rules" title="Rules">Rules</a></li>
                                        <li><a href="http://www.luckymn.com/winners/lucky_contest_winners/vikings/" alt="Winners" title="Winners">Winners</a></li>
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
                        
            
