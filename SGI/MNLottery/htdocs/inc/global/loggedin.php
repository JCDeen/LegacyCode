<?php

if (!isset($ignore_password_update)) {
	$ignore_password_update = false;
}

?>
<?php if($user_auth->isLoggedIn($ignore_password_update) && empty($hide_cart)): ?>
	<div class="LoggedIn">
		Hello, <?php echo UserAuth::getInstance()->getPlayer()->first_name ?>. 
		<span class="Points">You have 
			<?php echo array_sum(UserAuth::getInstance()->getPlayer()->points) ?> points.</span> | 
		<a href="/users/points.php">My WPT Points</a> | 
		<a href="/users/logout.php">Log Out</a> | 
		<?php if($cart->isEmpty()): ?>
			Your cart is empty.
		<?php else: ?>
			You have <?php echo array_sum($cart->getNumItemsByGame()) ?> 
			item<?php if(array_sum($cart->getNumItemsByGame()) > 1): ?>s<?php endif; ?>
			in your cart.
		<?php endif; ?>
		<a href="/cart/"><img src="/app_assets/images/global/cart.gif" width="17" height="14" alt="Cart"/></a>
	</div>
<?php endif; ?>