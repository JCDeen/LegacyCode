<?php require_once($_SERVER['DOCUMENT_ROOT']."/inc/.php/global.php");?>
<?php
//$user_auth = MN_UserAuthPassthru::getInstance(); //used by SCBZ_COMMON.'/code/entry/index.php'

die ("is this even used? .....................");
?>
<?php require(SCBZ_COMMON.'/code/entry/index.php') ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">    
        <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <meta http-equiv="X-UA-Compatible" content="IE=7" />
                <title>Lucky MN - Mega Monopoly</title> <!-- Internal 2 Column -->
                <!-- Standard CSS -->
                <link rel="stylesheet" href="../css/styles.css" type="text/css" />
                <!--<link href="css/slider.css" rel="stylesheet" type="text/css" media="all" />-->
               
                <script src="../js/jquery.js" type="text/javascript"></script>         
                <script src="../js/slider.js" type="text/javascript"></script>
 
               
                <script type="text/javascript" src="../js/jquery.pngFix.js"></script>
                <script type="text/javascript" src="../js/sticky_nav.js"></script>                     
                <script type="text/javascript" src="../js/winning_numbers.js"></script>
               
                <script type="text/javascript">
                <!--
                function init_template( ) {
                        j('body').pngFix( );
                        Sticky('nav');
                }
                //-->
                </script>
               
                        <!--[if IE 7]>
                        <link rel="stylesheet" href="../css/ie7.css" type="text/css" />
                <![endif]-->
               
                <!--[if IE 6]>
                        <link rel="stylesheet" href="../css/ie6.css" type="text/css" />
                <![endif]-->
               
        <link rel="stylesheet" href="/amm/css/components.css" type="text/css" media="screen" />
        <script type="text/javascript" language="JavaScript">
                <!--
                function init() {
                        var site_id = '2';
 
                        /* call init_template if it exists in the theme  */
                        if ( window.init_template ) {
                                init_template();
                        }
 
                        if( window.rewrite_preview_hrefs ) {
                                rewrite_preview_hrefs( site_id );
                        }
                       
                        if( window.add_preview_control_panel ) {
                                add_preview_control_panel( );
                        }
                       
                       
                }
            // -->
        </script>      
       <style type="text/css">

table.whiteBorders{
border-collapse:collapse;
text-align:center;
width:75%;
}

table.whiteBorders tr td {
border:1px solid #FFFFFF;
color:#FFFFFF;
font-size:12px;
padding:2px 5px;
}
</style>
        </head>
       
        <body id="internal" class="internal" onLoad="init()">
                <div id="wrapper">
                        <?php include ("inc/header_entry.php"); ?>
                       
                        <div id="content">
 <div style="clear:both;"></div>
                                <?php include ("inc/leftnav.php"); ?>
                               
                                <div id="internal_content">    
                    <div class="monopolyThemedBackground">
                    <div class="padder">
                 <!--INTERNAL CONTENT GOES HERE-->
 
				<div class="section-header">
					<div class="section-loginsubmit"><span>LOGIN/SUBMIT</span></div>
				</div>
				<p><b>Entry Page for <?php echo $user_auth->getPlayer()->getFullName() ?></b></p>
				
				<div class="error"><?php echo $form->getErrorMsg() ?></div>
				
<p>
Enter the ticket information where indicated, then click <strong>PLAY THE GAME</strong> button to play the Virtual MONOPOLY Bonus Board Game.  You will be entered into the current prize drawing.  Each second chance promotion entry requires ONE (1) non-winning Minnesota State Lottery Mega MONOPOLY scratch ticket (Game 632).  A record of your entries is at the bottom of this screen.
</p>

<p>
Players can enter their non-winning MONOPOLY tickets into a series of second chance drawings to win merchandise and other prizes or the Grand Prize of $20,000!
</p>

<p>
Prizes forfeited by drawing winners will be awarded to alternates. 
</p>

<p>
In each of the 1st, 2nd, 3rd, 4th and 5th Mega MONOPOLY Second Chance Promotion Drawings, one hundred and four (104) entries will be selected.  The following prizes will be awarded in each of 1st through 5th Mega MONOPOLY Second Chance Drawing Promotions:
</p>
<center>
<table>
<tr><td align="left">
<li><font color="ffffff", size="3">
One (1) $1,000 Travel Voucher and Duffle Bag
</font></li>

<li><font color="ffffff", size="3">
One (1) $500 Cash and Travel Kit
</font></li>

<li><font color="ffffff", size="3">
Two (2) $200 Cash
</font></li>

<li><font color="ffffff", size="3">
Ten (10) Premier MONOPOLY Sets
</font></li>
</td></tr></table>

<table bgcolor="#000000">
<tr><td align="center">
<font color="ffffff", size="3"><strong>NOTE: Submitting an Entry three (3) consecutive times will lock you out for twenty-four (24) hours.</strong>
</td></tr></table>
				


				<?php echo $form->beginForm() ?>
					<table>
						<tr>
							<td align="right"><b>VALIDATION NUMBER: </b></td>
							<td><?php echo $game->tf(1,1,1) ?></td>
							<td></td>
						</tr>
						<tr>
							<td align="CENTER" colspan="4">
								<input type="image" src="../images/btn_playthegame.gif" alt="Play the Game"/>
							</td>
						</tr>
					</table>
					<input type="hidden" name="form_submitted" value="1" />
				<?php echo $form->endForm() ?>
				
				<p>Type the 10-digit validation number from a non-winning Minnesota State Lottery Mega MONOPOLY ticket in the box above then click the <strong>PLAY THE GAME</strong> button.</p>
				
				<table>
					<tr>
						<td>&nbsp;&nbsp;&nbsp;</td>
						<td>
							<img src="../images/ticketart.gif" width="125" height="315" align="left" class="ticket" alt="Ticket" />
						</td>
						<td valign="top">
							<p>	
								See the illustration on this page for the location of the 10-digit validation number on the front of the ticket.
							</p>
							<p>
								When your entry is validated, it will appear at the bottom of this screen for your records.
							</p>
							<p>
								Enter as many times as you wish.  Any single non-winning ticket may be used only once.
							</p>
							<p>
								GOOD LUCK!
							</p>
							<p>
								<table class="whiteBorders">
									<tr>
										<td align="center"><b>Entry Number</b></td>
										<!--<td align="center"><b>Point Value</b></td>-->
										<td align="center"><b>Drawing Entries Won</b></td>
										<td align="center"><b>Entry Date</b></td>
									</tr>
									
									<?php if (count($entries)): ?>
										<?php foreach ($entries as $entry): ?>
											<tr>
												<td><?php echo join("<br/>",$entry->tickets) ?></td>
												<!--<td><?php echo $entry->points ?></td>-->
												<td><?php echo $entry->points / 1000 ?></td>
												<td><?php echo $lottery->date(DATE_DISPLAY_FORMAT,strtotime($entry->entry_date)) ?></td>
											</tr>
										<?php endforeach; ?>
									<?php else: ?>
										<tr>
											<td colspan="4" align="center">No entries submitted.</td>
										</tr>
									<?php endif; ?>
								</table>
							</p>
						</td>
					</tr>
				</table>
			<!--
				<p>	
					<img src="../images/ticketart.gif" width="125" height="315" align="left" class="ticket" alt="Ticket" />
					See the illustration on this page for the location of the 10-digit validation number on the front of the ticket.
				</p>
				
				<p>When your entry is validated, it will appear at the bottom of this screen for your records.</p>
			
				<p>Enter as many times as you wish.  Any single non-winning ticket may be used only once.</p>
				
				<p>GOOD LUCK!</p>
			-->
			
				
			
			 <!--INTERNAL CONTENT ENDS HERE-->
					 </div>
				</div>
				<div style="clear:both;"></div>
				</div>
						   
			<?php include ("inc/footnav.php"); ?>
				   
		</div>
		<?php include ("inc/winningnumbers.php"); ?>
	</div>
</body>
</html>