<?php

$user_auth = UserAuth::getInstance();
if(empty($player_id)) {
	$player_id = $user_auth->getPlayer()->id;
}

$mail_selector = new PFPMailSelector();
$mail_selector->setFilterClause(array(" player_id='$player_id' ", " was_read='' "));
$unread_mail_count = $mail_selector->getCount();
$plural = ($unread_mail_count > 1) ? 's' : '';

?>

<?php if($user_auth->isLoggedIn(true)): ?>

					<ul class="accountNav">
						<!-- <li class="first"><a href="<?php echo ACCOUNT_LINK ?>">Account Profile</a></li>-->
						<li><a href="/users/orders.php">Order History</a></li>
						<li><a href="/users/points.php">Points History</a></li>
						<li class="last">
							<a href="/users/webmail.php">Message Center</a>
							<?php if($unread_mail_count > 0): ?>
								(<?php echo $unread_mail_count ?> new message<?php echo $plural ?>)
							<?php endif; ?>
						</li>
						<!--<li class="last"><a href="<?php echo CANCEL_LINK ?>">Cancel Membership</a></li>-->
					</ul>
<?php endif; ?>