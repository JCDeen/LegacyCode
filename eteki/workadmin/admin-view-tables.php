<?php
// This code is for adding to jobs, employees, or users tables.
require_once ("db.php");
include_once ("login-check.php");
include_once ("workadmin-header.php");
include_once ("workadmin-nav.php");

$debugDisplay = isset($_REQUEST['debug']) ? "block" : "none";

$pageAction = ($_SERVER['REQUEST_METHOD'] == 'POST') ? 'process-form' : 'display-form';
$formAction = (isset($_REQUEST['form-action'])) ? $_REQUEST['form-action'] : "show-table"; // POST value should be "update-table"
$targetTable = (isset($_REQUEST['target-table'])) ? $_REQUEST['target-table'] : "work_jobs"; // default.

$resultStr = "UNDEFINED";
$rowcount = 0;
//$jobsArray = array();
//$employeesArray = array();
//$usersArray = array();
$resultsArray = array();

// MySQL table names as function of form-action.
$formLabel['work_jobs']      = "View Jobs";
$formLabel['work_employees'] = "View Employees";
$formLabel['work_users']     = "View Users";

// Query to get initial info to display in a table as function of $targetTable.
$setupQueries['work_jobs'] = "SELECT id, jobname as name FROM work_jobs";
$setupQueries['work_employees'] = "SELECT id, name FROM work_employees";
$setupQueries['work_users'] = "SELECT id, username as name, is_active, is_admin FROM work_users";

// Mysql column names for appropriate table.
//$sqlColName['work_jobs'] =   'jobname';
//$sqlColName['work_employees'] = 'name';
//$sqlColName['work_users'] = 'username';

// HTML table column names as function of $targetTable.
$htmlColName['work_jobs'] =   'Job Name';
$htmlColName['work_employees'] = 'Employee Name';
$htmlColName['work_users'] = 'User Name';

// HTML table column names as function of $targetTable.
//$spanStyle['work_jobs']       = ' style="width: 90px;"';
//$spanStyle['work_employees']  = ' style="width: 120px;"';
//$spanStyle['work_users']      = ' style="width: 90px;"';

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
        
        if ($targetTable=="work_users")
        {
            $isActiveStr = ( $obj->is_active ) ? '&#10004;' : '&nbsp;'; // Bold checkmark if user is active
            $isAdminStr = ( $obj->is_admin ) ? '&#10004;' : '&nbsp;';   // Bold checkmark if user is admin.
            $resultsArray[$i]['userdata'] = "<td>$isActiveStr</td><td>$isAdminStr</td>";
        }
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
  <title>View Tables</title>
  <link rel="stylesheet" href="workadmin.css">
</head>

<body>
	<?php ShowLogo($formLabel[$targetTable]); ?>

	<!-- for some reason the table is one insertion behind -->
    <table id="previewTable" class="tg">
        <thead>
        <tr>
        <th scope="col" class="tg-0pky"><?php echo $htmlColName[$targetTable]; ?></th>
        <?php
        if ($targetTable == "work_users")
        {
            echo "<th scope=\"col\" class=\"tg-0pky\">Active</th>" .
                "<th scope=\"col\" class=\"tg-0pky\">Admin</th>";
        }
        ?>
        </tr>
        </thead>
        
    <?php 
        foreach ($resultsArray as $value)
        {   
            $userdata = ( $targetTable=="work_users" ) ? "$value[userdata]" : ""; // IFF users table then show active & admin status.
            echo "<tr><td>$value[name]</td>$userdata</tr>\n";
        }
    ?>
    </table>
    
    <?php echo "<br /><div class=\"dates\" style=\"width:210px;\">Display Time: <span style=\"color: green;\">" . date('F d Y H:i:s') . "</span></div><br />"; ?>
    
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
