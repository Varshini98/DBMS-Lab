<?php
    session_start();

    //for debugging
    ini_set("log_errors", 1);
    ini_set("error_log", "log.txt");

    //mysql connection settings
    $user = "root";
    $pass = "ssl";
    $ser = "localhost";
    $db = "olx";

    $conn = new mysqli($ser, $user, $pass, $db);

    if($conn->connect_error)
    {
        die("Database Connection failed" . $conn->connect_error);
    }

    include_once("db.php");

    function isloggedin()
    {
        return isset($_SESSION["username"]);
    }
?>

<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="utf-8" />
        <link href="style.css" rel="stylesheet" type="text/css" />
        <title>OLX</title>
    </head>
