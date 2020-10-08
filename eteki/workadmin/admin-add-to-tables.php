<?php
// This code is for adding to jobs, employees, or users tables.
require_once ("db.php");
include_once ("login-check.php");
include_once ("workadmin-header.php");
include_once ("workadmin-nav.php");

$debugDisplay = isset($_REQUEST['debug']) ? "block" : "none";

$pageAction = ($_SERVER['REQUEST_METHOD'] == 'POST') ? 'process-form' : 'display-form';     // pageAction = display-form if <A HREF> linked.
$formAction = (isset($_REQUEST['form-action'])) ? $_REQUEST['form-action'] : "show-table";  // POST value should be "update-table"
$targetTable = (isset($_REQUEST['target-table'])) ? $_REQUEST['target-table'] : "work_jobs"; // default.
$nameEntered = (isset($_REQUEST['name_entered'])) ? $_REQUEST['name_entered'] : "";
// "add-job":
// "add-employee":
// "add-user":
// Put "edit-users" in separate file such as "edit-users.php" .

$resultStr = "UNDEFINED";
$loginMsg = "";
$rowcount = 0;
//$jobsArray = array();
//$employeesArray = array();
//$usersArray = array();
$resultsArray = array();

// MySQL table names as function of form-action.
$formLabel['work_jobs']      = "Add Jobs";
$formLabel['work_employees'] = "Add Employees";
$formLabel['work_users']     = "Add Users";

// Query to get initial info to display in a table as function of $targetTable.
$setupQueries['work_jobs'] = "SELECT id, jobname as name FROM work_jobs";
$setupQueries['work_employees'] = "SELECT id, name FROM work_employees";
$setupQueries['work_users'] = "SELECT id, username as name FROM work_users";

// Mysql column names for appropriate table.
$sqlColName['work_jobs'] =   'jobname';
$sqlColName['work_employees'] = 'name';
$sqlColName['work_users'] = 'username';

// HTML table column names as function of $targetTable.
$htmlColName['work_jobs'] =   'Job Name';
$htmlColName['work_employees'] = 'Employee Name';
$htmlColName['work_users'] = 'User Name';

// HTML table column names as function of $targetTable.
$spanStyle['work_jobs']       = ' style="width: 90px;"';
$spanStyle['work_employees']  = ' style="width: 120px;"';
$spanStyle['work_users']      = ' style="width: 90px;"';

if (mysqli_connect_errno())
{
    $resultStr = "Failed to connect to MySQL: " . mysqli_connect_error();
}
        
$sql = $setupQueries[$targetTable];
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
        $resultsArray[$i]['name'] = $obj->name;
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
  <title><?php  echo $formLabel[$targetTable]; ?></title>
  <link rel="stylesheet" href="workadmin.css">
</head>

<body>
	<?php ShowLogo($formLabel[$targetTable]); ?>
    <?php echo "<br /><div class=\"dates\" style=\"width:210px;\">Display Time: <span style=\"color: green;\">" . date('F d Y H:i:s') . "</span></div><br />";?>
    
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
        
        echo $targetTable;
        ?>
    </div>
    
    <?php 
    //echo filter_var("asd$12.3 ff", FILTER_SANITIZE_NUMBER_FLOAT, FILTER_FLAG_ALLOW_FRACTION) . "<br />";
    ?>
    
    <?php
    
    // Use $resultsArray[] to build table to display before form is shown.
    //$endCode = " title=\"enter value to enable Submit button\" oninput=\"checkInputs(this);\"";

    echo "Add $htmlColName[$targetTable]<br />";
    echo "<form id='$targetTable-form' method='post' action=''>\n";
    echo "<input type=hidden id='formAction' value='update_table'>";
    echo "<input type=hidden id='target-table' value='$targetTable'>";
    echo "<div class=\"inputDiv\"><span class=\"input-label\" $spanStyle[$targetTable]>$htmlColName[$targetTable]:</span>" .
        "<input type=\text\ id='name_entered' name='name_entered' size=20 placeholder='$htmlColName[$targetTable]' required></div>";
    
    //// TODO:
    ////    NEED to add password field, also for USERS
    
    echo "<div class=\"inputDiv\"><input type='submit' id='submitBtn' value='submit'></div>";
    echo "</form>";

    if ($pageAction == "process-form") 
    {
        $sql2 = "insert into $targetTable set $sqlColName[$targetTable]='$nameEntered'";

        $result2 = mysqli_query($conn, $sql2);
        $rowcount2 = mysqli_affected_rows($conn);
        $insertedStatus = ($rowcount2 > 0) ? "inserted" : "NOT inserted";
            
        echo "<br />Record was $insertedStatus<br />";
    }
    ?>
    
    <br />
    <?php ShowNavLinks(); ?>
    
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
