before the call<br>
<?php

	print_r(hash_algos());

	echo phpinfo();

	//echo print_r(mcrypt_list_algorithms("/usr/local/lib/libmcrypt"), true);
	echo "<br>";
	//echo print_r(mcrypt_list_modes("/usr/local/lib/libmcrypt"), true);

?>
<br>
after the call ...