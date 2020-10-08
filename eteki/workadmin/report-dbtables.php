<?php
require_once ("db.php");
echo "report-dbtabless is coming sometime soon.";

/****

$debugDisplay = isset($_REQUEST['debug']) ? "block" : "none";

// Verify is logged in. Else show not logged in error.

$pageAction = ($_SERVER['REQUEST_METHOD'] == 'POST') ? 'process-form' : "process-form";
$formAction = (isset($_REQUEST['form-action'])) ? $_REQUEST['form-action'] : "list-jobs-employees"; // default.
//           = 1) "list-jobs-employees": list all jobs with each employee & button to select specific employee
//             2) "update-payments"    : update jobs_payments with selected employee payment & materials cost

$resultStr = "UNDEFINED";
$loginMsg = "";
$rowcount = 0;
$bLoggedIn = false;
$jobsEmployeesArray = array();

switch ($pageAction)
{
    case "process-form":
        if (mysqli_connect_errno())
        {
            $resultStr = "Failed to connect to MySQL: " . mysqli_connect_error();
        }
        
        //$sql = "SELECT * FROM jobs_employees";
        $sql = "SELECT jobs.id as job_id, jobs_employees.id as employee_id, jobname, name as employeename FROM jobs, " .
            "jobs_employees WHERE jobs.id = jobs_employees.jobs_id ORDER BY job_id ASC";
        $result = mysqli_query($conn, $sql);
        
        if ($result)
        {
            // Return the number of rows in result set
            $rowcount = mysqli_num_rows($result);
            $resultStr = sprintf("Result set has %d rows.\n", $rowcount);
        }
        else
            $resultStr = "";
            
            break;
            
    default:
        //$resultStr = "NO RESULTS";
}
?>
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Login</title>
  <!-- <link rel="stylesheet" href="login-page.css"> -->
  <!-- <script defer src="login-page.js"></script> -->
</head>

<body>
    <h1 id="login-header">DCSATL Work Admin</h1>
    
    <div id="debug-info" style="border: solid red 1px; width: 70%; display: <?php echo $debugDisplay; ?>;">
    	<p>
        <?php echo '$pageAction=[' . $pageAction . "]<br/>"; 
          var_dump($_REQUEST);
        ?>
        Display Time: <?php echo "<p>" . date('F d Y H:i:s A') . "</p>";
        if (isset($sql))
            echo "\$sql=[$sql]<br />";?>
    
        <?php
            echo '$resultStr=[' . $resultStr . "]<br/>";
            echo '$_SERVER[\'REQUEST_METHOD\']=[' . $_SERVER['REQUEST_METHOD'] . "]<br/>";
        ?>
    </div>
    
    <?php
        if ($formAction == "list-jobs-employees" && $result)
        {
            $prevJobName = "";
            
            echo "<form id='login-form' method='post' action=''>\n";
            echo "<input type='hidden' name='form-action' value='update-payments'>";
            
            while ($obj = mysqli_fetch_object($result))
            {
                if ($prevJobName != $obj->jobname)
                {
                    // Use CSS to do this better
                    if ($prevJobName)
                        echo "<br \> \n";
                    
                    $prevJobName = $obj->jobname;
                    echo "$prevJobName: <br />\n";
                }
                
                // Use CSS to style text fields.
                // Use jQuery to style numeric text fields for monetary input.
                // TIM: Do we need decimal points ?
                echo "$obj->job_id $obj->employee_id $obj->employeename " .     
                    "<input type='date' name='date_paid[]' value='2020-07-04'> " . 
                    "<input type='text' name='payment[]' size=5 placeholder='payment'> " .
                    "<input type='text' name='materials[]' size=5 placeholder='materials' title='cost of materials'> <br />\n";
            }
            echo "<input type='submit' value='submit'>";
            echo "</form>\n";
        }
        else if ($formAction == "update-payments")
        {
            echo "upload the data and echo values to be uploaded here ...";
             
        }
    
    /***
    echo $loginMsg; 
	if (!$bLoggedIn) // If logged in then don't need to redisplay login form.
	{
	    ?>        
	}
	***/
    ?>
    
  </main>
</body>
</html>
<?php 
/***
    // Free result set
    if ($pageAction && $result)
    {
        mysqli_free_result($result);
        mysqli_close($conn);
    }
    ***/
?>