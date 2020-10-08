<?php
    switch ($_SERVER['HTTP_HOST']) 
    {
        case 'dcsatl.com':
        case 'www.dcsatl.com':
            $conn = mysqli_connect("mysqlcluster2","dcsatluser","dcsatlPw30","dcsatldb");
            break;
            
        case 'chrisdeen.com':
        case 'www.chrisdeen.com':
            $conn = mysqli_connect("localhost","uzbpsqaydat89","dcsatlPw30","dbwdjc296t29bv");
            break;
            
        default:        
	       $conn = mysqli_connect("localhost","root","","test");
    }
?>