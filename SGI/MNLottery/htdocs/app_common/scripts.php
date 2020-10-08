	<title><?php echo $pg_title_text; ?></title>
	<!-- <link rel="stylesheet" href="/app_assets/css/newstyles.css" type="text/css" />		 -->
	<!--[if lt IE 7]>
	<script defer type="text/javascript" src="/app_assets/js/jquery.pngFix.js"></script>
	<![endif]-->
	<!--[if IE 7]>
	<link rel="stylesheet" type="text/css" href="/app_assets/css/ie7.css" />
	<![endif]-->
	<!--[if IE 6]>
	<link rel="stylesheet" type="text/css href="/app_assets/css/ie6.css"" />
	<![endif]-->
	<!--[if IE 8]>
	<!-- <link rel="stylesheet" type="text/css" href="/app_assets/css/newstyles.css" /> -->
	<![endif]-->
	<link rel="stylesheet" href="/app_assets/css/style-scbz-addon.css" type="text/css" />		
	
	<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.min.js"></script>		
	<script type="text/javascript" src="/app_assets/js/functions.js"></script>
	<script type="text/javascript">
    	function validateData()
        {
                $(".entryError").hide();
                $(".entrySuccess").hide();
                if(document.getElementById("verifyBox").checked != true)
                {
                        if($("div#notChecked").length != 0)
                        {
                                $("div#notChecked").show();
                        }
                        if($("div#notCheckedLong").length != 0)
                        {
                                $("div#notCheckedLong").show();
                        }
                        return false;
                } else if(document.getElementById("verifyBox").checked == true)
                {
                        if($("div#notChecked").length != 0)
                        {
                                $("div#notChecked").hide();
                        }
                        if($("div#checked").length != 0)
                        {
                                $("div#checked").show();
                        }
                        return true;
                }
        }
	</script>	
	
	<?php require_once $_SERVER['DOCUMENT_ROOT'] . '/app_common/helper.php' ?>
