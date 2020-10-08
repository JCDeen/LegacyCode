<?php

// TODO:
//  Employees PAY only per week.
//  Totals if can do it.
//  Have way to select weeks.
//  Default to current week.

require_once ("db.php");
include_once ("login-check.php");
include_once ("workadmin-header.php");
include_once ("workadmin-nav.php");

$debugDisplay = isset($_REQUEST['debug']) ? "block" : "none";

// TODO: Verify is logged in. Else show not logged in error.

$pageAction = ($_SERVER['REQUEST_METHOD'] == 'POST') ? 'process-form' : "process-form";
$formAction = (isset($_REQUEST['form-action'])) ? $_REQUEST['form-action'] : "list-all-employees"; // default.
$minDateEntered = (isset($_REQUEST['min_Date_Entered'])) ? $_REQUEST['min_Date_Entered'] : "";
$maxDateEntered = (isset($_REQUEST['max_Date_Entered'])) ? $_REQUEST['max_Date_Entered'] : "";

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

        // TODO: Make this a common workadmin routine.
        // Get the min & max dates to set up the date picker range controls LIMITs.
        $sqlMinMaxDates = "select min(date_paid) as mindate, max(date_paid) as maxdate from work_payments";
        $resultMinMaxDates = mysqli_query($conn, $sqlMinMaxDates);
        $rowcountDates = mysqli_num_rows($resultMinMaxDates);
        if ($resultMinMaxDates && $rowcountDates)
        {
            $objDates = mysqli_fetch_object($resultMinMaxDates);            
            
            $minDateLimit = $objDates->mindate;
            $maxDateLimit = $objDates->maxdate;
        }
        else
        {
            // Empirical values from examining database.
            $minDateLimit = "2020-07-16"; // 1st date in work_payments table as of 2020-07-21.
            $maxDateLimit = "2022-12-31"; // 2 years into the future.
        }
        
        
        ////////////////////////////////////
        // SUM all payments for a given date
        ////////////////////////////////////
        
        $dateRangeStr = ($maxDateEntered && $minDateEntered) ? " AND (date_paid <= '$maxDateEntered') AND ('$minDateEntered' <= date_paid) " : "";
        $sql = "SELECT name, date_paid, SUM(payment) as paymentSum " .
            ", min(date_paid) as minDatePaid, max(date_paid) as maxDatePaid " .
            " from work_payments, work_employees " .
            " WHERE work_payments.employees_id=work_employees.id " . $dateRangeStr .
            " GROUP BY name ORDER BY name ASC";
        
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
  <title>WorkAdmin: Report-Employees</title>
  <link rel="stylesheet" href="workadmin.css">
  <!-- <link rel="stylesheet" href="login-page.css"> -->
</head>

<body>
	<?php ShowLogo("Report - EMPLOYEES"); ?>
    
    <!-- DEBUG DIV will be removed once its working smoothly -->
    <div id="debug-info" style="border: solid red 1px; width: 70%; display: <?php echo $debugDisplay; ?>;">
    	<p>
        <?php echo '$pageAction=[' . $pageAction . "]<br/>"; 
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
    
    <form id='report-employees-form' method='post' action=''>
        <input type=hidden id='formAction' value='update_report_employees'>
        <div class="inputDiv"><span class="input-label">Start Date:</span>
    		<input type='date' id ='min_Date_Entered' name='min_Date_Entered' value='<?php echo $minDateEntered; ?>' min='<?php echo $minDateLimit; ?>' required>
    	</div>
        <div class="inputDiv"><span class="input-label">End Date:</span>
    		<input type='date' id ='max_Date_Entered' name='max_Date_Entered' value='<?php echo $maxDateEntered; ?>' required>
    	</div>
        <div class="inputDiv"><input type=submit id='submit' value='SUBMIT' title="Refresh table for date range entered"></div>
    </form>
    <br />
    
    <table id="paymentsTable" class="tg">
      <thead>
        <tr>
          <th scope="col" class="tg-0pky">Employee Name</th>
          <!-- <th scope="col" class="tg-0pky">Dates Paid</th> -->
          <th scope="col" class="tg-0pky">Payment</th>
        </tr>
      </thead>
    
    <?php
        $minDatePaid = "2099-12-31"; // Start with high value;
        $maxDatePaid = ""; // "1999-01-01"; // Start with low value;
            
        if ($result)
        {
            $grandTotalPayments = 0;
            $subTotalPayments = 0;
            $rowStyle = "";
            
            while ($obj = mysqli_fetch_object($result))
            {
                echo "<tr $rowStyle><td>$obj->name</td><td>$obj->paymentSum</td></tr>"; 
                $rowStyle = ( $rowStyle ) ? "" : "class=\"tgs\"";
                $grandTotalPayments = $grandTotalPayments + $obj->paymentSum;
                
                $minDatePaid = ($minDatePaid > $obj->minDatePaid) ? $obj->minDatePaid : $minDatePaid;
                $maxDatePaid = ($maxDatePaid < $obj->maxDatePaid) ? $obj->maxDatePaid : $maxDatePaid;
            }
        }

        // TODO: $formatted = sprintf("%01.2f", $money);
        echo "<tr $rowStyle><td ><b>Grand Totals</b><td><b>$grandTotalPayments</b></td></tr>";
    ?>
	</table>
	
	<?php echo "<br /><div class=\"dates\">Table shown is for date range: <span style=\"color: green;\">$minDatePaid </span> to <span style=\"color: green;\">$maxDatePaid</span></div><br />"; ?>

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