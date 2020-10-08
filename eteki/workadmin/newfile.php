<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Login</title>
  
</head>

<body>

.form-div {
    margin: 50px auto 50px;
    padding: 25px 15px 10px 15px;
    border: 4px solid #80ced7;
    border-radius: 30px;
    font-size: 1.1em;
    font-family: 'Merriweather&display=swap', serif;
}

.form-control:focus {
    box-shadow: none;
}

form p {
    font-size: .89;
}

.form-div.login {
    margin-top: 100px;
}

.logout {
    color: red;
}

.form-field {
    display: flex;
}

.form-group {
    width: 100%;
}

.form-group label {
    width: 120px;
    display: inline-block;
}

<div class="form-field">
<div class="form-group">
<label for="fname">First Name</label>
<input type="text" name="fname" value="<?php $fname; ?>" class="form-control form-control-lg">
</div>

<div class="form-group">
<label for="lname">Last Name</label>
<input type="text" name="lname" value="<?php $lname; ?>" class="form-control form-control-lg">
</div>
</div>
<div class="form-field">
<div class="form-group">
<label for="username">User Name</label>
<input type="text" name="username" value="<?php $username; ?>" class="form-control form-control-lg">
</div>
<div class="form-group">
<label for="email">Email</label>
<input type="email" name="email" value="<?php $email; ?>" class="form-control form-control-lg">
</div>
</div>
<div class="form-field">
<div class="form-group">
<label for="phone">Phone</label>
<input type="tele" name="phone" value="<?php $phone; ?>" class="form-control form-control-lg">
</div>
<div class="form-group">
<label for="password">Password</label>
<input type="password" name="password" class="form-control form-control-lg">
</div>
</div>
<div class="form-group">
<label for="passwordConf">Confirm Password</label>
<input type="password" name="passwordConf" class="form-control form-control-lg">
</div>

