<?php
    // Verify is logged in. Else show not logged in error.
    // NOTE: Extremely simple - not robust at all !
$loggedIn = ( isset($_COOKIE['DCSATL_username']) ) ? true : false;
if (!$loggedIn)
    die ("You need to login!<br /><a href=\"../workadmin/\">Click Here to Login</a>");
    // TODO: redirect to appropriate /wordkadmin so works on test also.
?>