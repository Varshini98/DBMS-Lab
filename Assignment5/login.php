<?php
    include_once("header.php");

    if(isloggedin())
    {
        header("Location: index.php");
    }

    $e = FALSE;
    $err = "";

    if(isset($_POST["register"]))
    {
        $username = trim($_POST["username"]);
        $password = $_POST["password"];
        $fname = trim($_POST["fname"]);
        $lname = trim($_POST["lname"]);
        $mob = trim($_POST["mobile"]);
        $utype = trim($_POST["utype"]);

        $e = empty($username) || empty($password) || empty($fname) || empty($lname) || empty($mob) || empty($utype);
        if($e)
            $err .= "Fill the required fields. ";
        $e = $e || !filter_var($username, FILTER_VALIDATE_EMAIL);
        if(!filter_var($username, FILTER_VALIDATE_EMAIL))
            $err .= "Enter your NITC email address correctly. ";
        $e = $e || !preg_match("/^\d{10}$/",$mob);
        if(!preg_match("/^\d{10}$/",$mob))
            $err .= "Enter a proper mobile number. ";       
        
        if(!$e)
        {
            $q = "INSERT INTO users (email, password, fname, lname, mob, type) VALUES (?, SHA(?), ?, ?, ?, ?)";
            $query = $conn->prepare($q) or die("error - " . $conn->error);
            $query->bind_param("ssssss", $username, $password, $fname, $lname, $mob, $utype) or die("error - " . $conn->error);

            $query->execute() or die("error - " . $conn->error);

            if($conn->affected_rows == 1)
            {
                $_SESSION["username"] = $username;
                $_SESSION["fname"] = $fname;
                $_SESSION["lname"] = $lname;
                $_SESSION["user_id"] = $conn->insert_id;
                header("Location: index.php");
            }
            $query->close();
        }
    }

    if(isset($_POST["login"]))
    {
        $username = trim($_POST["username"]);
        $password = $_POST["password"];

        $e = empty($username) || empty($password);
        if($e)
        {
            $err = "Username / Password cannot be empty. ";
        }
        else
        {
            $id = 0;
            $fname = "";
            $lname = "";
            $q = "SELECT id, fname, lname FROM users WHERE email = ? AND password = SHA(?)";
            $query = $conn->prepare($q) or die("error1 - " . $conn->error);
            $query->bind_param("ss", $username, $password) or die("error2 - " . $conn->error);

            $query->execute() or die("error - " . $conn->error);

            $query->store_result();

            $query->bind_result($id, $fname, $lname) or die("error - " . $conn->error);
            $query->fetch() or die("error - " . $conn->error);

            if($query->num_rows == 1)
            { 
                $_SESSION["username"] = $username;
                $_SESSION["fname"] = $fname;
                $_SESSION["lname"] = $lname;
                $_SESSION["user_id"] = $id;
                header("Location: index.php");
            }

            else
            {
                $e = TRUE;
                $err = "Username & Password does not match. ";
            }

            $query->close();
        }
    }
?>


    
        <?php
            if($e)
            {
                echo '<div id="error"><p><b>'. $err . '<b></p></div>';
            }
        ?>
        <div id="login">
            <h1>LOGIN</h1>
            <a href="#register"><h3>Register</h3></a>

            <form method="post" action="login.php">
                <label for="username">NITC email address</label>    <br />
                <input type="email" name="username" placeholder="NITC email address" required /><br />
                <label for="password">Password</label>    <br />
                <input type="password" name="password" placeholder="Password" required />       <br />
                <input type="submit" name="login" value="Login" />
            </form>
        </div>



        <div id="register">
            <h1>REGISTER</h1>
            <a href="#login"><h3>Login</h3></a>

            <form action="login.php" method="post">
                <label for="username">NITC email address</label>    <br />
                <input type="email" name="username" placeholder="NITC email address" required /><br />
                <label for="password">Choose a Password</label>    <br />
                <input type="password" name="password" placeholder="Password" required />       <br />
                <label for="fname">First name</label>    <br />
                <input type="text" name="fname" placeholder="First name" required />            <br />
                <label for="lname">Last name</label>    <br />
                <input type="text" name="lname" placeholder="Last name" />             <br />
                <label for="mobile">Mobile number</label>    <br />
                <input type="tel" name="mobile" placeholder="Mobile number" required />         <br />
                <label for="utype">You are a</label>    <br />
                <input type="radio" name="utype" value="student" checked />Student
                <input type="radio" name="utype" value="staff" />Staff
                <input type="radio" name="utype" value="faculty" />Faculty                      <br />
                <input type="submit" name="register" value="Register" />
                <input type="reset" value="Reset" />
            </form>
        </div>

    </body>
</html>
