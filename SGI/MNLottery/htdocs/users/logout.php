<?php require_once '../app_common/z-errors.php'; ?>
<?php require_once($_SERVER['DOCUMENT_ROOT']."/inc/.php/global.php");?>
<?php
$pg_title_text = 'Log Out';
$pg_title_image = '';
$app_dir_common = $_SERVER['DOCUMENT_ROOT'] . "/app_common" ;

$user_auth = UserAuth::getInstance()->logout();
//require(SCBZ_COMMON.'/code/users/logout.php');
$_SESSION = array();
//header("Location: " . LOGOUT_REDIRECT); exit();
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
	<?php require_once $app_dir_common . '/scripts.php'; ?>
</head>
	
	
<body id="games" class="internal">
<div id="wrapper">
	<?php require_once $app_dir_common . '/header.php'; ?>
	
	<div id="content">
		<?php require_once $app_dir_common . '/welcome.php'; ?>
		
		<div class="Content">
			<?php //  require_once $app_dir_common . '/left-nav.php'; ?>
			
			<div class="SubPageTitle">
				<h1><?php echo $pg_title_text; ?></h1>	
			</div>
			
			<div style="width:770px; margin-left:190px;">
				<h3>You have successfully logged out.</h3>
			</div>
			
		</div><!-- internal_no_left_nav -->
		
		<?php require_once $app_dir_common . '/footer.php'; ?>
			
	</div><!-- content -->
</div><!-- wrapper -->

</body>
</html>				
