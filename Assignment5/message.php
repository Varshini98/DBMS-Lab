<?php
    include_once("header.php");
    if(!isloggedin())
    {
        header("Location: login.php");
    }

    $to = $_GET['uid'];

    if(isset($_POST["item_msg"]))
    {
        if($_POST["itype"] == "mobile")
        {
            $iquery = "SELECT model, manufacturer FROM mobiles WHERE advert_id = ?";
        }
        elseif($_POST["itype"] == "laptop")
        {
            $iquery = "SELECT model, manufacturer FROM laptops WHERE advert_id = ?";
        }
        elseif($_POST["itype"] == "book")
        {
            $iquery = "SELECT B.name, GROUP_CONCAT(C.name) AS auth FROM books B INNER JOIN authors C ON C.book_id = B.id WHERE B.advert_id = ?";
        }

        $iquery = $conn->prepare($iquery) or die($conn->error);
        $iquery->bind_param("d", $_POST["id"]);
        $iquery->execute() or die($conn->error);
        $iquery->bind_result($pr, $mn);
        $iquery->fetch();
        $iquery->close();

        $imsg = "I am interested in your " . $_POST["itype"] . ', ' . $pr . ' by ' . $mn;

        $iquery = "INSERT INTO messages (sender_id, receiver_id, message, time) VALUES (?, ?, ?, NOW())";
        $iquery = $conn->prepare($iquery) or die($conn->error);
        $iquery->bind_param("dds", $_SESSION["user_id"], $to, $imsg);
        $iquery->execute() or die($conn->error);

        header("Location: message.php?uid=" . $to);

    }

    if(isset($_POST["msg_send"]))
    {
        $imsg = $_POST["msg"];

        $iquery = "INSERT INTO messages (sender_id, receiver_id, message, time) VALUES (?, ?, ?, NOW())";
        $iquery = $conn->prepare($iquery) or die($conn->error);
        $iquery->bind_param("dds", $_SESSION["user_id"], $to, $imsg);
        $iquery->execute() or die($conn->error);

        header("Location: message.php?uid=" . $to);
    }

    $qall = $conn->prepare("SELECT DISTINCT U.id, U.fname FROM messages M INNER JOIN users U ON (M.sender_id = U.id OR M.receiver_id = U.id) WHERE (M.sender_id = ? OR M.receiver_id = ?) AND U.id != ?");
    $qall->bind_param("ddd", $_SESSION["user_id"], $_SESSION["user_id"], $_SESSION["user_id"]);
    $qall->execute() or die($conn->error);
    $qall = $qall->get_result();

    $query = "SELECT M.sender_id, M.receiver_id, U1.fname AS sender, U2.fname AS receiver, M.message, M.time FROM messages M INNER JOIN users U1 ON U1.id = M.sender_id";
    $query .= " INNER JOIN users U2 ON U2.id = M.receiver_id WHERE (M.sender_id = ? AND M.receiver_id = ?) OR (M.sender_id = ? AND M.receiver_id = ?) ORDER BY M.time";

    $q = $conn->prepare($query) or die($conn->error);
    $q->bind_param("dddd", $_SESSION["user_id"], $to, $to, $_SESSION["user_id"]) or die($conn->error);
    
    $q->execute() or die($conn->error);
    $q->bind_result($sid, $rid, $s, $r, $m, $t);

?>

    <body>
        <?php
            include_once("navbar.php");
        ?>  

        <div id="body">
            <div id="chats">
                <ul>
                <?php                
                    while($r = $qall->fetch_assoc())
                    {
                        echo '<a href="message.php?uid=' . $r["id"] . '"><li>' . $r["fname"] . '</li></a>';
                    }
                ?>
                </ul>
            </div>

            <div id="msgs">
                <ul>
                <?php
                    while($q->fetch())
                    {
                        if($sid == $_SESSION["user_id"])
                        {
                            echo '<li class="self"><div><h4>'.$m.'</h4><p>'.$t.'</p></div></li>';
                        }
                        else
                        {
                            echo '<li class="other"><div><h4>'.$m.'</h4><p>'.$t.'</p></div></li>';
                        }
                    }
                ?>
                </ul>
            </div>
        
            <div id="send">
            <?php
                if(isset($_GET["uid"]) && $_GET["uid"] != NULL)
                {
                    echo '<form method="post" action="message.php?uid='. $to .'">';
                    echo '<input type="text" name="msg" />';
                    echo '<input type="submit" name="msg_send" value="Send" />';
                    echo '</form>';
                }
            
            ?>
            </div>
        </div>
    </body>
</html>
