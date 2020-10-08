<?php
// Signsend

/****
function geturlonly() 
{
    $urlpath = explode('/', $_SERVER['PHP_SELF']);
    array_pop($urlpath);
    $scriptname = implode("/", $urlpath);
    $http_protocol = 'http';
    if((!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] !== 'off') || ($_SERVER['SERVER_PORT'] == 443)){
      $http_protocol = 'https';
    }
    return $http_protocol . "://" . $_SERVER["HTTP_HOST"] . $scriptname . "/";
}
**/

function base64_to_jpeg($base64_string, $output_file) 
{
    $ifp = @fopen($output_file, "wb");

    $data = explode(',', $base64_string);

    @fwrite($ifp, base64_decode($data[1]));
    @fclose($ifp);
    return $output_file;

}

// This gets called even if not actually emailed.
function add_watermark($inputfile, $outputfile) 
{

//    var_dump(gd_info());
    $im = @imagecreatefrompng($inputfile);

    $bg = @imagecolorallocate($im, 255, 255, 255);
    $textcolor = @imagecolorallocate($im, 255, 255, 255);

    list($x, $y, $type) = getimagesize($inputfile);

    $txtpos_x = $x - 200;
    $txtpos_y = $y - 20;

    @imagestring($im, 5, $txtpos_x, $txtpos_y, 'Powered by DCSATL.com', $textcolor);

    $txtpos_x = $x - 145;
    $txtpos_y = 20;

    @imagestring($im, 3, $txtpos_x, $txtpos_y, date("Y-m-d H:i:s"), $textcolor);

    @imagepng($im, $outputfile);

    // Output the image
    //imagejpeg($im);

    @imagedestroy($im);

}

date_default_timezone_set("America/New_York");
$output_file = "uploads/UserName" . date("Y-m-d-H-i-s-").time(). ".png";
base64_to_jpeg($_POST["image"], $output_file);
add_watermark($output_file, $output_file);

echo "File Uploaded: $output_file";
var_dump($_REQUEST);
?>
