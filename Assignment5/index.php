<?php
    include_once("header.php");


    if(!isloggedin())
    {
        header("Location: login.php");
    }
    $conn->query("DELETE FROM adverts WHERE DATE(init_date) < DATE_SUB(CURDATE(), INTERVAL 10 DAY);");

    $qbooks = 'SELECT A.owner_id AS own_id, A.id, U.fname AS owner, B.cond, DATE_FORMAT(A.init_date, "%d-%m %l:%i %p") AS init_date, A.year_purchase, A.price, B.name, GROUP_CONCAT(C.name) AS auth FROM books B INNER JOIN adverts A on B.advert_id = A.id INNER JOIN authors C ON C.book_id = B.id INNER JOIN users U ON U.id = A.owner_id GROUP BY B.name ORDER BY A.init_date DESC';
    $qmob = 'SELECT A.owner_id AS own_id, A.id, U.fname AS owner, DATE_FORMAT(A.init_date, "%d-%m %l:%i %p") AS init_date, A.year_purchase, A.price, B.manufacturer, B.model FROM mobiles B INNER JOIN adverts A ON A.id = B.advert_id INNER JOIN users U ON U.id = A.owner_id ORDER BY A.init_date DESC';
    $qlap = 'SELECT A.owner_id AS own_id, A.id, U.fname AS owner, DATE_FORMAT(A.init_date, "%d-%m %l:%i %p") AS init_date, A.year_purchase, A.price, B.manufacturer, B.model, B.battery_status FROM laptops B INNER JOIN adverts A ON A.id = B.advert_id INNER JOIN users U ON U.id = A.owner_id ORDER BY A.init_date DESC';

    $books = $conn->query($qbooks);
    $mobs = $conn->query($qmob);    
    $laps = $conn->query($qlap);
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
                        echo '<div class="item_lap"><h3 title="' . $l["model"] .'">' . $l["model"] . '</h3><h4 title="' . $l["manufacturer"] . '">Manufacturer: ' . $l["manufacturer"] . '</h4>';
                        echo '<p>Battery ';
                        if($l["battery_status"] != 1) {echo '<b>not</b> ';}
                        echo 'backed up. Purchased on ' . $l["year_purchase"] . '</p>';
                        echo '<h4>INR ' . $l["price"] . '</h4>';
                        echo '<p>Posted on ' . $l["init_date"] . ' by ' . $l["owner"];
                        if($l["own_id"] != $_SESSION["user_id"])
                        {
                            echo '<form method="post" action="message.php?uid=' . $l["own_id"] . '">';
                            echo '<input type="hidden" name="id" value="' . $l["id"] . '" />';
                            echo '<input type="hidden" name="itype" value="laptop" />';
                            echo '<input type="submit" name="item_msg" value="Enquire" />';
                            echo '</form>';
                        }
                        else
                        {
                            echo '<form method="post" action="manage.php">';
                            echo '<input type="hidden" name="id" value="' . $l["id"] . '" />';
                            echo '<input class="del" type="submit" name="item_dlt" value="Delete" />';
                            echo '</form>';
                        }                        
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
                        echo '<div class="item_lap"><h3 title="' . $l["model"] .'">' . $l["model"] . '</h3><h4 title="' . $l["manufacturer"] . '">Manufacturer: ' . $l["manufacturer"] . '</h4>';  
                        echo '<p>Purchased on ' . $l["year_purchase"] . '</p>';   
                        echo '<h4>INR ' . $l["price"] . '</h4>';                   
                        echo '<p>Posted on ' . $l["init_date"] . ' by ' . $l["owner"];
                        if($l["own_id"] != $_SESSION["user_id"])
                        {
                            echo '<form method="post" action="message.php?uid=' . $l["own_id"] . '">';
                            echo '<input type="hidden" name="id" value="' . $l["id"] . '" />';
                            echo '<input type="hidden" name="itype" value="mobile" />';
                            echo '<input type="submit" name="item_msg" value="Enquire" />';
                            echo '</form>';
                        }
                        else
                        {
                            echo '<form method="post" action="manage.php">';
                            echo '<input type="hidden" name="id" value="' . $l["id"] . '" />';
                            echo '<input class="del" type="submit" name="item_dlt" value="Delete" />';
                            echo '</form>';
                        } 
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
                        echo '<div class="item_book"><h3 title="' . $b["name"] . '">' . $b["name"] . '</h3><h4 title="' . $b["auth"] . '">Authors: ' . $b["auth"] . '</h4>';
                        echo '<p>' . $b["cond"] . ' condition. Purchased on ' . $b["year_purchase"] . '</p>';
                        echo '<h4>INR ' . $b["price"] . '</h4>';
                        echo '<p>Posted on ' . $b["init_date"] . ' by ' . $b["owner"];
                        if($b["own_id"] != $_SESSION["user_id"])
                        {
                            echo '<form method="post" action="message.php?uid=' . $b["own_id"] . '">';
                            echo '<input type="hidden" name="id" value="' . $b["id"] . '" />';
                            echo '<input type="hidden" name="itype" value="book" />';
                            echo '<input type="submit" name="item_msg" value="Enquire" />';
                            echo '</form>';
                        }
                        else
                        {
                            echo '<form method="post" action="manage.php">';
                            echo '<input type="hidden" name="id" value="' . $b["id"] . '" />';
                            echo '<input class="del" type="submit" name="item_dlt" value="Delete" />';
                            echo '</form>';
                        } 
                        echo '</li>';
                    }
                ?>
                </ul>
            </div>
        </div>
    </body>
</html>
