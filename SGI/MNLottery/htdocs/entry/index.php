<?php require_once '../app_common/z-errors.php'; ?>
<?php require_once($_SERVER['DOCUMENT_ROOT']."/inc/.php/global.php");?>
<?php

$reqLoc = PFP_COMMON . '/code/entry/index.php';
require(PFP_COMMON . '/code/entry/index.php');
$pg_title_text = 'Ticket Entry';
$pg_title_image = '';
$app_dir_common = $_SERVER['DOCUMENT_ROOT'] . "/app_common" ;
$app_dir_shared = SHARED_ROOT . "/pfp/" . ENVIRONMENT_DIR . "/htdocs" ;

// If the user pasted a copied URL into browser address box
// then we make sure he is logged in. If not, we redirect
// to LuckyMN.com . ( Note that the legacy function EnsureLogin(
// didn't really do much of anything.

$user_auth = UserAuth::getInstance();
if ($user_auth->getIsLoggedIn()) {
	$player_id = $user_auth->getPlayer()->id;
}
else {
	redirect("http://www.mnlottery.com");
	//echo "Would redirect";
}
$k=1;
?>
<!DOCTYPE html>
<!--[if lt IE 7 ]> <html class="ie6 lteie7 lteie8 lteie9 no-js" lang="en"> <![endif]-->
<!--[if IE 7 ]>    <html class="ie7 lteie7 lteie8 lteie9 no-js" lang="en"> <![endif]-->
<!--[if IE 8 ]>    <html class="ie8 lteie8 lteie9 no-js" lang="en"> <![endif]-->
<!--[if IE 9 ]>    <html class="ie9 lteie9 no-js" lang="en"> <![endif]-->
<!--[if (gt IE 9)|!(IE)]><!-->
<html style="" class="js no-touch csstransforms3d csstransitions placeholder respotray-gpu" lang="en"><!--<![endif]-->
<head>
	<meta http-equiv="content-type" content="text/html; charset=UTF-8">
	<meta charset="utf-8">
	<title>MN Lottery - Ticket Entry</title>

	<!--[if !HTML5]>
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<![endif]-->
	<meta charset="utf-8">
	<meta name="author" content="Clockwork.net">

	<!-- Mobile viewport optimization h5bp.com/ad -->
	<!-- Modified in plugins.js on mobile for "Full Site" link -->
	<meta name="viewport" content="width=device-width">
	<meta name="HandheldFriendly" content="True">
	<meta name="MobileOptimized" content="320">

	<meta property="og:title" content="Monopoly">
		<meta property="og:type" content="website">
	    <meta property="og:image" content="https://mnlottery.stage.clockwork.net/amm/themes/mnlottery_2014/images/facebook.png"><!-- min: 50x50, png, jpg, gif -->
	
	<link rel="shortcut icon" href="https://mnlottery.stage.clockwork.net/amm/themes/mnlottery_2014/images/favicon.ico">
	<link rel="apple-touch-icon" sizes="57x57" href="https://mnlottery.stage.clockwork.net/amm/themes/mnlottery_2014/images/apple-touch-icon-57x57.png">
	<link rel="apple-touch-icon" sizes="114x114" href="https://mnlottery.stage.clockwork.net/amm/themes/mnlottery_2014/images/apple-touch-icon-114x114.png">
	<link rel="apple-touch-icon" sizes="72x72" href="https://mnlottery.stage.clockwork.net/amm/themes/mnlottery_2014/images/apple-touch-icon-72x72.png">
	<link rel="apple-touch-icon" sizes="144x144" href="https://mnlottery.stage.clockwork.net/amm/themes/mnlottery_2014/images/apple-touch-icon-144x144.png">
	<link rel="apple-touch-icon" sizes="60x60" href="https://mnlottery.stage.clockwork.net/amm/themes/mnlottery_2014/images/apple-touch-icon-60x60.png">
	<link rel="apple-touch-icon" sizes="120x120" href="https://mnlottery.stage.clockwork.net/amm/themes/mnlottery_2014/images/apple-touch-icon-120x120.png">
	<link rel="apple-touch-icon" sizes="76x76" href="https://mnlottery.stage.clockwork.net/amm/themes/mnlottery_2014/images/apple-touch-icon-76x76.png">
	<link rel="apple-touch-icon" sizes="152x152" href="https://mnlottery.stage.clockwork.net/amm/themes/mnlottery_2014/images/apple-touch-icon-152x152.png">
	<link rel="icon" type="image/png" href="https://mnlottery.stage.clockwork.net/amm/themes/mnlottery_2014/images/favicon-196x196.png" sizes="196x196">
	<link rel="icon" type="image/png" href="https://mnlottery.stage.clockwork.net/amm/themes/mnlottery_2014/images/favicon-160x160.png" sizes="160x160">
	<link rel="icon" type="image/png" href="https://mnlottery.stage.clockwork.net/amm/themes/mnlottery_2014/images/favicon-96x96.png" sizes="96x96">
	<link rel="icon" type="image/png" href="https://mnlottery.stage.clockwork.net/amm/themes/mnlottery_2014/images/favicon-32x32.png" sizes="32x32">
	<link rel="icon" type="image/png" href="https://mnlottery.stage.clockwork.net/amm/themes/mnlottery_2014/images/favicon-16x16.png" sizes="16x16">
	<meta name="msapplication-TileColor" content="#00aba9">
	<meta name="msapplication-TileImage" content="/amm/themes/mnlottery_2014/images/mstile-144x144.png">
	<meta name="msapplication-square70x70logo" content="/amm/themes/mnlottery_2014/images/mstile-70x70.png">
	<meta name="msapplication-square144x144logo" content="/amm/themes/mnlottery_2014/images/mstile-144x144.png">
	<meta name="msapplication-square150x150logo" content="/amm/themes/mnlottery_2014/images/mstile-150x150.png">
	<meta name="msapplication-square310x310logo" content="/amm/themes/mnlottery_2014/images/mstile-310x310.png">
	<meta name="msapplication-wide310x150logo" content="/amm/themes/mnlottery_2014/images/mstile-310x150.png">
	
	<meta name="Description" content="Second Chance contest to win cash">
	
	<script type="text/javascript" src="index_files/jquery_002.js"></script>

    <link rel="stylesheet" href="index_files/styles.css">
	<script type="text/javascript" src="index_files/modernizr.js"></script> <!--  modernizr.js is needed for the theme!!!!! -->
 	
	<style>html, body { background-color: #C1D42F; }</style>
	<style type="text/css"></style><!-- base href="https://mnlottery.stage.clockwork.net/contests__events/monopoly/" -->
	
		<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.6.2/jquery.js"></script>
	<style type="text/css">
		sup, sub {
		   vertical-align: baseline;
		   position: relative;
		   top: -0.4em;
		}
		sub { top: 0.4em; }
	</style>
</head>

	<body id="theme_id_26" class="theme_id_26 theme_mnlottery_2014_internal contests__events">
	<div id="wrapper" class="wrapper">

		<?php 
			include_once($app_dir_shared . '/themes/ScratchGames_April17_2014/top_header_nav_inc.php');
		?>

	<?php 
	// END of top Nav bar.
	// END of top Nav bar.
	?>
				<!-- Page Content -->

				<div id="main" role="main" class="main">
					<div class="colgroup col-5">
						<section class="col-right span-4">
							<div class="message message_type-" style="display:none"></div>
							<!-- START Home Carousel/Sidebar -->
<!-- END Home Carousel/Sidebar -->
<!-- start cols w/callout ============================= -->
<div class="colgroup col-1">

	<!-- START Main/Center Column -->
	<div id="hoist_for_mobile" class="col">

	<?php require_once $app_dir_common . '/scripts.php'; ?>

	<script type="text/javascript">

	var debounceFlag = true;
	$(document).ready(function() {
		// "de-bounce" the submit button. This approach allows
		// the form to be submitted exactly once. 
		$('.button2 #submit').click(function() {
			var returnFlag = false;
			if (debounceFlag) {
				debounceFlag = false;
				returnFlag = true;
			}
			return returnFlag;
		});

		// If the user clicks the verify age box, force the
		// submit botton to a state of readiness. This covers
		// the case where the user clicked submit without 
		// checking the verify box ... and so must check 
		// and submit again. 
		$('.checkBox #verifyBox').click(function() {
			debounceFlag = true;
			return true;
		});
	});
	</script>
						
			<div style="width:775px; margin-left:-20px;">
			
			<div class="SubPageTitle">
                		<span class="header_internal">
                        		<img src="/app_assets/images/<?php echo ENTRY_INDEX_BANNER_FILE; ?>" alt="<?php echo GAME_TITLE_STRING_SHORT; ?> Banner" />
               		</span>
			</div>
				<div style="float:left; width:49%;">
				
					<span style="font-size:larger; font-weight: bolder;"><?php echo INSTRUCTIONS_HEADER_TITLE . "<br>"; ?></span>
				
					<!-- <h3>Instructions</h3> -->
					<p><b>To submit an  entry:</b></p>
					<ol>
						<li>Enter the 10-digit VALIDATION NUMBER. A ticket's VALIDATION NUMBER can be found by 
							scratching off the entire play area on the front of the ticket 
							<span id="ShowTicket" name="Showticket" style="font-style:italic; cursor:pointer; color: black; text-decoration:underline;">(click here for example)</span>
						<li>Check the box to confirm  that you are 18 years or older</li>
						<li>Click 'Submit'</li>
					</ol>
					<p>When you click "submit," a message will appear which confirms your successful entry into the 
					drawing and your entry will appear in Entries For This Session.  The Entries For This Session listing  
					shows the most recent 100 entries during your current logged-in session.  To view all your 
					entries, click View Complete Entry History.</p>
					
					<p>Enter as many times as you like but any single non-winning Minnesota State Lottery scratch ticket may only be used once in this contest, and cannot also be used for any 
					other Lottery second chance contest.  Odds of winning depend on the number of entries received.</p>
				</div>
				
<!---->
<div class="currentEntries">

	<div class="notifications">
		<div id="notChecked" class="entryError">
			<span class="errorMsg">
				Please verify that you are 18 years of age or older.
			</span>
		</div><!-- notChecked -->
		<?php 
			if($form->checkErrorMsg()):
			$k=1;
		?>
			<div class="entryError">
				<span class="errorMsg">
					<?php 
						$errMsg = $form->getErrorMsg();
						echo $form->getErrorMsg();
						$k=1;
					 ?>
				</span>
			</div><!-- entryError -->
		<?php elseif(!empty($success)): ?>
			<div class="entrySuccess">
				<span class="successMsg">
					<em>Congratulations!</em><br />Your ticket was validated<br />
				</span>
			</div><!-- entrySuccess -->
		<?php endif; ?>
	</div>
	
	<div class="ticketEntry adjustOrangeBackgnd">
                <div class="warning">
                        <span class="warningMsg">Incorrectly submitting an entry five (5) consecutive times will lock you out for twenty-four (24) hours.</span>
                </div>
		<?php echo $form->beginForm() ?>		
			<?php
				 
				//$isGameOpen = 
				if(count($game_options)): ?>
				<dl>
					<dt>Validation Number: </dt>
					<dd class="entryField">
						<?php echo $form->create_textbox('ticket_number', '', $ticket_number_length, $ticket_number_length) ?>
					</dd>
					<dd class="checkBox">
						<div>
							<input type="checkbox" id="verifyBox" />I affirm that I am 18 years of age or older.
						</div>
					</dd>
					<br />
					<br />
					<dd>
						<span class="button2"><?php echo $form->create_submit('submit', 'Submit') ?></span>
						<div class="clearFloats"></div>
					</dd>
				</dl>
			<?php else: ?>
				There are no games open for ticket entry right now.  Please check back again later!
			<?php endif; ?>
			<?php
				$gameRecord = new Game();
				$gameRecord->loadGameByGameNumber(THIS_GAME_NUMBER);
				$gameID = $gameRecord->getId();
				echo $form->create_hidden('game_id', $gameID); 
			?>
			<?php echo $form->create_hidden('form_submitted', 1) ?>
		<?php echo $form->endForm() ?>		
	</div><!-- ticketEntry -->

	<?php 
		include_once($app_dir_shared . '/themes/ScratchGames_April17_2014/session_entries_list_inc.php');
	?>
	
	</div><!-- currentEntries -->
<div class="clearFloats"></div>
<!---->
				<br class="clear" />
			</div>
<!--  ENTRY PAGE STOP ..................................................................... -->
	
	</div>
	<!-- END Main/Center Column -->

</div>
<!-- End cols w/callout ============================= -->
</section>

	<?php 
		include_once($app_dir_shared . '/themes/ScratchGames_April17_2014/left_nav_inc.php');
	?>
		</div>
	</div>
</div><!-- End #wrapper -->
	<?php 
		include_once($app_dir_shared . '/themes/ScratchGames_April17_2014/footer_inc.php');
	?>
	</div>
	
	<div id="SmokeScreen" name="SmokeScreen"
	style="background-color: grey;
	position: absolute;
	left:0; right:0;
	top:0; bottom:-300px;
	padding:0;
	margin: 0;
	z-Index:1000;
	z-index:1000;
	opacity: 0.85;
	filter: alpha(opacity=85);
	display: none;">
</div>

<div id="UnavailableSmokeScreen" name="UnavailableSmokeScreen"
	style="background-color: black;
	position: fixed;
	left:-150px; right:0px;
	top:0; bottom:-100px;
	width: 130%;
	padding:0;
	margin: 0;
	z-Index:2000;
	z-index:2000;
	opacity: 0.75;
	filter: alpha(opacity=75);
	display: none;">
		<div style="margin-top:30%; margin-left:45%; position: fixed; width: 75%; height: 25%; background-color: black; z-Index:4020; z-index:4020;">
			<h1 style="color: white; "><p>THIS CONTEST IS CLOSED.</p></h1>
		</div>
</div>

<div id="FullTicketDiv" name="FullTicketDiv"
	style="position: absolute;
	padding:0;	margin:0;
	top:15px; z-Index:1500;
	z-index:1500; display: none;"><br>&nbsp;<br>
	<div style="float: right; position:relative; left:-20px; top:-20px; color:white;">
		<img src="../../app_assets/images/drewsX.gif" style="margin-top:5px; padding-top:5px;">
		<h2 style="margin-left:25px;">Click to close ticket image</h2>
	
		<img src="../../app_assets/images/<?php echo $entry_tickets[0]; ?>" alt="<?php echo GAME_TITLE_STRING_SHORT; ?> Ticket" />
		<img src="../../app_assets/images/<?php echo $entry_tickets[1]; ?>" alt="<?php echo GAME_TITLE_STRING_SHORT; ?> Ticket" />
	</div>
</div>
</body>

<script type="text/javascript">
$('div#FullTicketDiv').hide();
$('div#notChecked').hide();

//get the height of the parent   
var parent_height = $('#FullTicketDiv').parent().height();   
var container_height = $('#FullTicketDiv').height();   
var topOffset = (parent_height - container_height)/2;   

var parent_width = $('#FullTicketDiv').parent().width();   
var container_width = $('#FullTicketDiv').width();   
var leftOffset = (parent_width - container_width)/2;   

//and change the margin-top css attribute of the image   
//$('#FullTicketDiv').css('top' , topOffset + 'px');   
$('#FullTicketDiv').css('left' , leftOffset);   

$('span#ShowTicket').click(function() {
	$('div#SmokeScreen').show();
	$('div#FullTicketDiv').show();
});

$('div#FullTicketDiv').click(function() {
	$('div#SmokeScreen').hide();
	$('div#FullTicketDiv').hide();
});

</script>
</html>
