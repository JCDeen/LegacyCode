<?php
// This code is for paying only ONE employee.
require_once ("db.php");
include_once ("login-check.php");
include_once ("workadmin-header.php");
include_once ("workadmin-nav.php");

$debugDisplay = isset($_REQUEST['debug']) ? "block" : "none";

$pageAction = ($_SERVER['REQUEST_METHOD'] == 'POST') ? 'process-form' : "process-form";
$formAction = (isset($_REQUEST['form-action'])) ? $_REQUEST['form-action'] : "pay-one-employee-form"; // default.

$resultStr = "UNDEFINED";
$loginMsg = "";
$rowcount = 0;
$jobsArray = array();
$employeesArray = array();

switch ($pageAction)
{
    case "process-form":
        if (mysqli_connect_errno())
        {
            $resultStr = "Failed to connect to MySQL: " . mysqli_connect_error();
        }
        
        $sql = "SELECT id, jobname FROM work_jobs";
        $result = mysqli_query($conn, $sql);
        
        if ($result)
        {
            // Return the number of rows in result set
            $rowcount = mysqli_num_rows($result);
            $resultStr = sprintf("Result set has %d rows.\n", $rowcount);
            
            $i = 0;
            while ($obj = mysqli_fetch_object($result))
            {
                $jobsArray[$i]['id'] = $obj->id;
                $jobsArray[$i]['jobname'] = $obj->jobname;
                $i++;
            }
                
            $sql2 = "SELECT id as employee_id, name as employee_name FROM work_employees";
            $result = mysqli_query($conn, $sql2);
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
  <title>Pay One Employee</title>
  <link rel="stylesheet" href="workadmin.css">
</head>

<body>
	<?php ShowLogo("Pay One Employee"); ?>
    
    <!-- DEBUG DIV will be removed once its working smoothly -->
    <div id="debug-info" style="border: solid red 1px; width: 70%; display: <?php echo $debugDisplay; ?>;">
    	<p>
        <?php echo '$pageAction=[' . $pageAction . "]<br/>"; 
          echo "var_dump(\$_REQUEST):<br />\n";
          var_dump($_REQUEST);
          echo "var_dump(\$_COOKIE):<br />\n";
          var_dump($_COOKIE);
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
    //echo filter_var("asd$12.3 ff", FILTER_SANITIZE_NUMBER_FLOAT, FILTER_FLAG_ALLOW_FRACTION) . "<br />";
    ?>
    
    <?php
        // Always show the inputs form.
        
        echo "<form id='payments-form' method='post' action=''>\n";
        echo "<input type='hidden' name='form-action' value='update-payments'>";
        
        //////////////////////////////////////////////////////////////////////
        // Build the Jobs dropdown. Value=-1 is only to label the dropdown. //
        //////////////////////////////////////////////////////////////////////
        echo "<div class=\"inputDiv\"><span class=\"input-label\">Job: </span> <select name='job_id' id='job_id' required>";
        echo "<option value='' disabled selected hidden>SELECT JOB</option>"; // Hack to show placeholder.
        for($i=0; $i<sizeof($jobsArray); $i++)
        {
            $id = $jobsArray[$i]['id'];
            $jobName = $jobsArray[$i]['jobname'];
            echo "<option value=\"$id\">$jobName</option>";
        }
        echo "</select></div>\n";
        
        
        ///////////////////////////////////////////////////////////////////////////
        // Build the Employees dropdown. Value=-1 is only to label the dropdown. //
        ///////////////////////////////////////////////////////////////////////////
        echo "<div class=\"inputDiv\"><span class=\"input-label\">Employee:</span> <select name='employee_id' id='employee_id' required>";
        echo "<option value='' disabled selected hidden>SELECT EMPLOYEE</option>"; // Hack to show placeholder.
        
        while ($obj = mysqli_fetch_object($result))
        {
            echo "<option data-employee_name=\"$obj->employee_name\" value=\"$obj->employee_id\">$obj->employee_name</option>\n";
        }
        echo "</select></div>\n";

        $today = date('Y-m-d');
        $endCode = " title=\"enter value to enable Submit button\" oninput=\"checkInputs(this);\"";
        echo "<div class=\"inputDiv\"><span class=\"input-label\">Date Paid:</span> <input type='date' id ='date_paid' name='date_paid' value='$today' required></div>";
        echo "<div class=\"inputDiv\"><span class=\"input-label\">Payment : </span> <input type='text' id ='payment' name='payment' size=6 placeholder='payment' $endCode></div>";
        echo "<div class=\"inputDiv\"><span class=\"input-label\">Materials:</span> <input type='text' id ='materials' name='materials' size=6 placeholder='materials' $endCode></div>\n";
        
        echo "<div class=\"inputDiv\"><input type='submit' id='submitBtn' value='submit' disabled  title=\"Enter a value in Payment and/or Materials to enable Submit button.\"></div>";
        echo "</form>\n";

        if ($formAction == "update-payments")
        {
            // Loop over payment. If payment && date_paid then can insert. IF materials then add it also.
            //var_dump($_POST);
            
            $theJobId = $_POST['job_id'];
            $employee_id = $_POST['employee_id'];
            //$employee_name = $_POST['employeename'];
            $date_paid = $_POST['date_paid'];
            $payment = $_POST['payment'];
            $materials = $_POST['materials'];
            
            if ($payment || $materials) // payment or materials field can be blank, but not both.
            {
                if (!$payment)
                    $payment = "0.00";
                
                if (!$materials)
                    $materials = "0.00";
                
                //$payment = filter_var($payment, FILTER_SANITIZE_NUMBER_FLOAT, FILTER_FLAG_ALLOW_FRACTION);
                //$materials = filter_var($materials, FILTER_SANITIZE_NUMBER_FLOAT, FILTER_FLAG_ALLOW_FRACTION);
                
                $sql2 = "insert into work_payments set jobs_id='$theJobId', employees_id='$employee_id'," .
                    "date_paid='$date_paid', payment='$payment', materials='$materials'";

                $result2 = mysqli_query($conn, $sql2);
                $rowcount2 = mysqli_affected_rows($conn);
                $insertedStatus = ($rowcount2 > 0) ? "inserted" : "NOT inserted";
                echo "Payment $payment, Materials $materials on $date_paid for $employee_id was $insertedStatus<br />";
                
            }
            else
            {
                echo "Payment data for $employee_id was NOT inserted<br />";
            }
        }
    ?>
    
    <p>
    <br />
    <?php ShowNavLinks(); ?>
    </p>
    
</body>
<script>
function checkInputs(txtCtrl)
{
	if (txtCtrl.value != "")
	{	
		var subBtn = document.getElementById("submitBtn");
		subBtn.disabled = false;
	}
}

</html>
<?php 
    // Free result set
    if ($pageAction && isset($result) && isset($conn))
    {
        mysqli_free_result($result);
        mysqli_close($conn);
    }
?>
