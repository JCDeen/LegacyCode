<?php

// Leave args blank to show default nav links.
// Returns true if AdminLinks are shown.
function ShowNavLinks()
{

    // No need to show this link if on the "pay-one-employee.php" page.
    echo "<hr>";
    
    if ( stripos($_SERVER['PHP_SELF'], "pay-one-employee.php") === FALSE )
    {
?>
    <a href="pay-one-employee.php">	<input type="button" id="pay-one-employee" value="Add Pay & Materials for One Employee"></a>
    <br /><br />
<?php 
    }
?>

    Reports:<br />
    <a href="report-jobs.php">		<input type="button" id="report-jobs"      value="Report:jobs">		</a><br />
    <a href="report-payments.php">	<input type="button" id="report-payments"  value="Report:payments">	</a><br />
    <a href="report-employees.php">	<input type="button" id="report-employees" value="Report:employees">	</a><br /><br />

<?php
    $adminLinksShown = ShowAdminLinks();
    return ($adminLinksShown);
}

function ShowAdminLinks($showItAnyway = false)
{
    $adminLinksShown = false;
    
    $userRole = ( isset ($_COOKIE['DCSATL_user_role'])) ? $_COOKIE['DCSATL_user_role'] : "non-admin";
    if (($userRole == "admin") || $showItAnyway)
    {
        ?>
        <span title="Only visible to ADMIN users">Admin Only Functions:</span><br />
        <a href="admin-add-to-tables.php?target-table=work_jobs">	 <input type="button" id="add-jobs"  	 value="Add:jobs">	   	</a><br />
        <a href="admin-add-to-tables.php?target-table=work_employees"><input type="button" id="add-employees" value="Add:employees">	</a><br />
        <a href="admin-add-to-tables.php?target-table=work_users">	 <input type="button" id="add-users" 	 value="Add:users">		</a><br />
        <a href="admin-edit-pw.php">	 						     <input type="button" id="edit-password" value="Edit:password">		</a><br />
        
        <br />

        <a href="admin-view-tables.php?target-table=work_jobs">	 	<input type="button" id="view-jobs"  	 value="View:jobs">	   	</a><br />
        <a href="admin-view-tables.php?target-table=work_employees"><input type="button" id="view-employees" value="View:employees">	</a><br />
        <a href="admin-view-tables.php?target-table=work_users">	<input type="button" id="view-users" 	 value="View:users">		</a>
    <?php
    
        $adminLinksShown = true;
    }
    return ($adminLinksShown);
}
?>
