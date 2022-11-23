<?php
    include_once("header.php");
    if(!isloggedin())
    {
        header("Location: login.php");
    }

    if(isset($_POST["item_dlt"]))
    {
        $dlt = $conn->prepare("DELETE FROM adverts WHERE id = ? AND owner_id = ?") or die($conn->error);
        $dlt->bind_param("dd", $_POST["id"], $_SESSION["user_id"]);
        $dlt->execute() or die($conn->error);
        $dlt->close();

        header("Location: manage.php");
    }

    $qbooks = 'SELECT A.owner_id AS own_id, A.id, U.fname AS owner, B.cond, DATE_FORMAT(A.init_date, "%d-%m %l:%i %p") AS init_date, A.year_purchase, A.price, B.name, GROUP_CONCAT(C.name) AS auth FROM books B INNER JOIN adverts A on B.advert_id = A.id INNER JOIN authors C ON C.book_id = B.id INNER JOIN users U ON U.id = A.owner_id WHERE A.owner_id = ? GROUP BY B.name ORDER BY A.init_date DESC';
    $qmob = 'SELECT A.owner_id AS own_id, A.id, U.fname AS owner, DATE_FORMAT(A.init_date, "%d-%m %l:%i %p") AS init_date, A.year_purchase, A.price, B.manufacturer, B.model FROM mobiles B INNER JOIN adverts A ON A.id = B.advert_id INNER JOIN users U ON U.id = A.owner_id WHERE A.owner_id = ? ORDER BY A.init_date DESC';
    $qlap = 'SELECT A.owner_id AS own_id, A.id, U.fname AS owner, DATE_FORMAT(A.init_date, "%d-%m %l:%i %p") AS init_date, A.year_purchase, A.price, B.manufacturer, B.model, B.battery_status FROM laptops B INNER JOIN adverts A ON A.id = B.advert_id INNER JOIN users U ON U.id = A.owner_id WHERE A.owner_id = ? ORDER BY A.init_date DESC';

    $books = $conn->prepare($qbooks);
    $books->bind_param("d", $_SESSION["user_id"]);
    $books->execute() or die($conn->error);
    $books = $books->get_result();

    $mobs = $conn->prepare($qmob);
    $mobs->bind_param("d", $_SESSION["user_id"]);
    $mobs->execute() or die($conn->error);
    $mobs = $mobs->get_result();

    $laps = $conn->prepare($qlap);
    $laps->bind_param("d", $_SESSION["user_id"]);
    $laps->execute() or die($conn->error);
    $laps = $laps->get_result();

?>

    <body>
        <?php
            include_once("navbar.php");
        ?>
        <div id="body">
            <div id="laptops" class="items">
                    <ul class="list">
                    <?php
                        while($l = $laps->fetch_assoc())
                        {
                            echo '<li>';
                            echo '<div class="item_lap"><h3>' . $l["model"] . '</h3><h4>Manufacturer: ' . $l["manufacturer"] . '</h4>';
                            echo '<p>Battery ';
                            if($l["battery_status"] != 1) {echo 'not ';}
                            echo 'backed up. Purchased on' . $l["year_purchase"] . '</p>';
                            echo '<p>Posted on ' . $l["init_date"] . ' by ' . $l["owner"];
                            echo '<form method="post" action="manage.php">';
                            echo '<input type="hidden" name="id" value="' . $l["id"] . '" />';
                            echo '<input class="del" type="submit" name="item_dlt" value="Delete" />';
                            echo '</form>';
                            echo '</li>';
                        }
                    ?>
                    </ul>
                </div>            

                <div id="mobiles" class="items">  
                    <ul class="list">
                    <?php
                        while($l = $mobs->fetch_assoc())
                        {
                            echo '<li>';
                            echo '<div class="item_mob"><h3>' . $l["model"] . '</h3><h4>Manufacturer: ' . $l["manufacturer"] . '</h4>';  
                            echo '<p>Purchased on ' . $l["year_purchase"] . '</p>';                      
                            echo '<p>Posted on ' . $l["init_date"] . ' by ' . $l["owner"];
                            echo '<form method="post" action="manage.php">';
                            echo '<input type="hidden" name="id" value="' . $l["id"] . '" />';
                            echo '<input class="del" type="submit" name="item_dlt" value="Delete" />';
                            echo '</form>';
                            echo '</li>';
                        }
                    ?>
                    </ul>
                </div>


                <div id="books" class="items">
                    <ul class="list">
                    <?php
                        while($b = $books->fetch_assoc())
                        {
                            echo '<li>';
                            echo '<div class="item_book"><h3>' . $b["name"] . '</h3><h4 title="' . $b["auth"] . '">Authors: ' . $b["auth"] . '</h4>';
                            echo '<p>' . $b["cond"] . ' condition. Purchased on ' . $b["year_purchase"] . '</p>';
                            echo '<p>Posted on ' . $b["init_date"] . ' by ' . $b["owner"];
                            echo '<form method="post" action="manage.php">';
                            echo '<input type="hidden" name="id" value="' . $b["id"] . '" />';
                            echo '<input class="del" type="submit" name="item_dlt" value="Delete" />';
                            echo '</form>';
                            echo '</li>';
                        }
                    ?>
                    </ul>
                </div>
            </div>
    </body>
</html>
