<?php
require_once ("db.php");
include_once ("login-check.php");
include_once ("workadmin-header.php");
include_once ("workadmin-nav.php");

$debugDisplay = isset($_REQUEST['debug']) ? "block" : "none";

$pageAction = ($_SERVER['REQUEST_METHOD'] == 'POST') ? 'process-form' : "process-form";
$formAction = (isset($_REQUEST['form-action'])) ? $_REQUEST['form-action'] : "list-all-payments"; // default.

$resultStr = "UNDEFINED";
$rowcount = 0;
$bLoggedIn = false;

switch ($pageAction)
{
    case "process-form":
        if (mysqli_connect_errno())
        {
            $resultStr = "Failed to connect to MySQL: " . mysqli_connect_error();
        }
        
        $sql = "SELECT work_jobs.id as jobid, jobname, name, date_paid, payment , materials " .
            " from work_payments, work_employees, work_jobs " .
            " WHERE work_payments.jobs_id=work_jobs.id AND work_payments.employees_id=work_employees.id" .
            " ORDER BY jobid, jobname, name, date_paid ASC";
            
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
  <title>WorkAdmin: Report - Employees</title>
  <link rel="stylesheet" href="workadmin.css">
  <!-- <script defer src="login-page.js"></script> -->
</head>

<body>
	<?php ShowLogo("Report - Payments"); ?>
    
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
    
 	  <table id="paymentsTable" class="tg">
		  <thead>
		    <tr>
		      <th scope="col" class="tg-0pky">Job Name</th>
		      <th scope="col" class="tg-0pky">Employee</th>
		      <th scope="col" class="tg-0pky">Date Paid</th>
		      <th scope="col" class="tg-0pky">Payment</th>
		      <th scope="col" class="tg-0pky">Materials</th>
		    </tr>
		  </thead>
    
    <?php
        if ($formAction == "list-all-payments" && $result)
        {
            $grandTotalPayments = $grandTotalMaterials = 0;
            $prevJobname = "";
            $subTotalPayments = $subTotalMaterials = 0;
            $boolPrintSubtotals = false;
            $rowStyle = "";
                       
            while ($obj = mysqli_fetch_object($result))
            {
                if ($obj->jobname != $prevJobname)
                {
                    $prevTotalPayments = $prevTotalMaterials = 0;
                    $prevJobname = $obj->jobname; 
                    
                    if ($subTotalPayments)
                    {
                        // Need to cast/convert subtotals to decimal 10.2
                        echo "<tr $rowStyle><td colspan=2>&nbsp;</td><td><b>subtotal</b></td><td><b>$subTotalPayments</b></td><td><b>$subTotalMaterials</b></td></tr>";
                        $subTotalPayments = $subTotalMaterials = 0;
                    }
                    
                    $jobNameLabel = $obj->jobname;
                    $rowStyle = ( $rowStyle ) ? "" : "class=\"tgs\"";
                }
                else
                {
                    $jobNameLabel = "&nbsp;";
                }
                echo "<tr $rowStyle><td>$jobNameLabel</td><td>$obj->name</td><td>$obj->date_paid</td><td>$obj->payment</td><td>$obj->materials</td></tr>"; 
                $subTotalPayments =         $subTotalPayments +      $obj->payment  ;
                $subTotalMaterials =        $subTotalMaterials +     $obj->materials;
                $grandTotalPayments =       $grandTotalPayments +    $obj->payment  ;
                $grandTotalMaterials =      $grandTotalMaterials +   $obj->materials;
/**
                $subTotalPayments =         number_format( $subTotalPayments +      $obj->payment, 2);
                $subTotalMaterials =        number_format( $subTotalMaterials +     $obj->materials, 2);
                $grandTotalPayments =       number_format( $grandTotalPayments +    $obj->payment  , 2);
                $grandTotalMaterials =      number_format( $grandTotalMaterials +   $obj->materials, 2);
**/                
            }
        }
    ?>
    <?php
    // USE $formatted = sprintf("%01.2f", $money);
        echo "<tr $rowStyle><td colspan=2>&nbsp;</td><td ><b>subtotal</b><td><b>$subTotalPayments</b></td><td><b>$subTotalMaterials</b></td></tr>";
        echo "<tr $rowStyle><td colspan=2>&nbsp;</td><td ><b>Grand Totals</b><td><b>$grandTotalPayments</b></td><td><b>$grandTotalMaterials</b></td></tr>";
    ?>
	</table>

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
