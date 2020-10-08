<?php
// This code is for adding to jobs, employees, or users tables.
require_once ("db.php");
include_once ("login-check.php");
include_once ("workadmin-header.php");
include_once ("workadmin-nav.php");

$debugDisplay = isset($_REQUEST['debug']) ? "block" : "none";

$pageAction = ($_SERVER['REQUEST_METHOD'] == 'POST') ? 'process-form' : 'display-form';     // pageAction = display-form if <A HREF> linked.
$formAction = (isset($_REQUEST['form-action'])) ? $_REQUEST['form-action'] : "list_users";  // POST value should be "update_users"
$userID = (isset($_REQUEST['userID'])) ? $_REQUEST['userID'] : "";
$passwordEntered = (isset($_REQUEST['password_entered'])) ? $_REQUEST['password_entered'] : "";

$resultStr = "UNDEFINED";
$rowcount = 0;
$resultsArray = array();

if (mysqli_connect_errno())
{
    $resultStr = "Failed to connect to MySQL: " . mysqli_connect_error();
}
        
$sql = "SELECT * FROM work_users";
$result = mysqli_query($conn, $sql);

if ($result)
{
    // Return the number of rows in result set
    $rowcount = mysqli_num_rows($result);
    $resultStr = sprintf("Result set has %d rows.\n", $rowcount);
    
    // Fill the array to use for building display table.
    $i = 0;
    while ($obj = mysqli_fetch_object($result))
    {
        $resultsArray[$i]['id'] = $obj->id;
        $resultsArray[$i]['username'] = $obj->username;
        $resultsArray[$i]['password'] = $obj->password;
        $i++;
    }
}
else
    $resultStr = "";
?>
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Edit User Password</title>
  <link rel="stylesheet" href="workadmin.css">
</head>

<body>
	<?php ShowLogo("Edit User Password"); ?>
    <?php echo "Display Time:" . date('F d Y H:i:s A') . "<br />"; ?>
    
    <!-- DEBUG DIV will be removed once its working smoothly -->
    <div id="debug-info" style="border: solid red 1px; width: 70%; display: <?php echo $debugDisplay; ?>;">
    	<p>
        <?php 
        echo "\$pageAction=[$pageAction]<br/>"; 
        echo "\$formAction=[$formAction]<br/>";
        
        echo "var_dump(\$_REQUEST):<br />\n";
        var_dump($_REQUEST);
        echo "var_dump(\$_COOKIE):<br />\n";
        var_dump($_COOKIE);
        
        echo "Display Time:<p>" . date('F d Y H:i:s A') . "</p>";
        if (isset($sql))
            echo "\$sql=[$sql]<br />";
    
        echo "\$resultStr=[$resultStr]<br/>";
        echo "\$_SERVER['REQUEST_METHOD']=[$_SERVER[REQUEST_METHOD]]<br/>";
        
        ?>
    </div>
    
    <?php 
    //echo filter_var("asd$12.3 ff", FILTER_SANITIZE_NUMBER_FLOAT, FILTER_FLAG_ALLOW_FRACTION) . "<br />";
    ?>
    
    <?php
    
    // Use $resultsArray[] to build table to display before form is shown.
    //$endCode = " title=\"enter value to enable Submit button\" oninput=\"checkInputs(this);\"";
    
    $spanStyle = ' style="width: 80px;"';
    
    echo "Edit User Password<br />";
    echo "<form id='manage-users-form' method='post' action=''>\n";
    echo "<input type='hidden' name='form-action' value='update-users'>";
    
    ///////////////////////////////////////////////////////////////////////
    // Build the Users dropdown. Value=-1 is only to label the dropdown. //
    ///////////////////////////////////////////////////////////////////////
    echo "<div class=\"inputDiv\"><span class=\"input-label\" $spanStyle>User: </span> <select name='user_id' id='user_id' required>";
    echo "<option value='' disabled selected hidden>SELECT USER</option>"; // Hack to show placeholder.
    for($i=0; $i<sizeof($resultsArray); $i++)
    {
        $id = $resultsArray[$i]['id'];
        $username = $resultsArray[$i]['username'];
        $password = $resultsArray[$i]['password'];
        echo "<option value=\"$id\" data-pw=\"$password\">$username</option>\n";
    }
    echo "</select></div>\n";

    echo "<input type=hidden id='formAction' value='update_users'>";
    echo "<div class=\"inputDiv\"><span class=\"input-label\" $spanStyle>Password :</span>" .
    "<input type=\text\ id='password_entered' name='password_entered' size=20 placeholder='new password' required></div>";
    
    //// TODO:
    ////    NEED to add password field, also for USERS
    
    echo "<div class=\"inputDiv\"><input type='submit' id='submitBtn' value='submit'></div>";
    echo "</form>";

    if ($pageAction == "process-form") 
    {
        $sql2 = "update work_users set password='$passwordEntered' where id=$id";
        $result2 = mysqli_query($conn, $sql2);
        $rowcount2 = mysqli_affected_rows($conn);
        $queryStatus = ($rowcount2 > 0) ? "Updated" : "NOT updated";
        
        echo "Query Status: $queryStatus<br />";
    }
    ?>
    
    <p>
    <br />
    <?php ShowNavLinks(); ?>
    </p>
    
</body>
</html>
<?php 
    // Free result set
    if ($pageAction && isset($result) && isset($conn))
    {
        mysqli_free_result($result);
        mysqli_close($conn);
    }
?>
