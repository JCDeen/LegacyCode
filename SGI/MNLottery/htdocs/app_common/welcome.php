		<div id="signin_internal">
			<div class="TopButtons">
				<?php if(isLoggedIn()): ?>
				<div class="loggedin-btns">
				
					<div class="left">
						<div class="LoggedIn">
							Hello, <?php echo $user_auth->getPlayer()->first_name; ?>
							<!--<span class="Points">
								<a href="/users/points.php">You have <?php //echo $user_auth->getPlayer()->points; ?> points.</a>
							</span>-->
						</div>
					</div>
			
				</div><!-- loggedin-btns -->
				<?php endif; ?>
			
			</div><!-- TopButtons -->
		
		</div><!-- signin-internal -->
