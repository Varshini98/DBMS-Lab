<?php
    include_once("header.php");

    if(!isloggedin())
    {
        header("Location: login.php");
    }

    if($_SERVER['REQUEST_METHOD'] == 'POST')
    {
        $itype = $_POST['itype'];
        $yop = $_POST['year'];
        $price = $_POST['price'];

        $query = $conn->prepare("INSERT INTO adverts (init_date, owner_id, year_purchase, price) VALUES (NOW(), ?, ?, ?)") or die($conn->error);
        $query->bind_param("ddd", $_SESSION["user_id"], $yop, $price) or die($conn->error);
        $query->execute() or die($conn->error);

        $advert_id = $conn->insert_id;

        if($itype == 'book')
        {
            $name = $_POST['book_name'];
            $cond = $_POST['book_condition'];
            $author = "";    

            $query = $conn->prepare("INSERT INTO books (name, cond, advert_id) VALUES (?, ?, ?)") or die($conn->error);
            $query->bind_param("ssd", $name, $cond, $advert_id) or die($conn->error);
            $query->execute() or die($conn->error);

            $book_id = $conn->insert_id;

            $query = $conn->prepare("INSERT INTO authors (book_id, name) VALUES (?, ?)") or die($conn->error);
            $query->bind_param("ds", $book_id, $author) or die($conn->error);

            $n = 1;
            while(isset($_POST["book_auth" . $n]))
            {
                //$authors[] = $_POST["book_auth" . $n];
                $author = $_POST["book_auth" . $n];
                $query->execute() or die($conn->error);
                $n = $n + 1;
            }

            header("Location: index.php");
        }

        elseif($itype == 'mobile')
        {
            $man = $_POST['manufacturer'];
            $model = $_POST['model'];

            $query = $conn->prepare("INSERT INTO mobiles (manufacturer, model, advert_id) VALUES (?, ?, ?)") or die($conn->error);
            $query->bind_param("ssd", $man, $model, $advert_id) or die($conn->error);
            $query->execute() or die($conn->error);

            header("Location: index.php");
        }

        elseif($itype == 'laptop')
        {
            $man = $_POST['manufacturer'];
            $model = $_POST['model'];
            $battery = ($_POST['laptop_battery_status'] == 'Yes')? 1 : 0;

            $query = $conn->prepare("INSERT INTO laptops (manufacturer, model, advert_id, battery_status) VALUES (?, ?, ?, ?)") or die($conn->error);
            $query->bind_param("ssdi", $man, $model, $advert_id, $battery) or die($conn->error);
            $query->execute() or die($conn->error);

            header("Location: index.php");
        }
    }

?>

    <body onload="radio()">
        <?php
            include_once("navbar.php");
        ?>

        <div id="body">
            <h2>Submit an Ad</h2>
        
            <form method="post" action="submit.php" id="add">
                <label for="itype">Product type</label> <br />
                <input id="b" type="radio" name="itype" value="book" onchange="radio()" checked />Book 
                <input id="m" type="radio" name="itype" value="mobile" onchange="radio()" />Mobile 
                <input id="l" type="radio" name="itype" value="laptop" onchange="radio()" />Laptop <br /><br />

                <label for="book_name" class="book">Name of the book</label> <br class="book" />
                <input type="text" name="book_name" class="book" /> <br class="book" />
                <label for="book_auth1" class="book">Authors</label> <br class="book" />
                <div id="author">
                    <input type="text" name="book_auth1" class="book" placeholder="Author 1" /> <br class="book" />                
                </div> 
                <input type="button" onclick="addAuth()" value="Add author" class="book" /> <br class="book" />
                <label for="book_codition" class="book">Condition of the book</label> <br class="book" />
                <select name="book_condition" class="book">
                    <option value="Good">Good</option>
                    <option value="Average">Average</option>
                    <option value="Bad">Bad</option>
                </select> <br class="book" />

                <label for="manufacturer" class="mobile laptop">Manufacturer</label> <br class="mobile laptop" />
                <input type="text" name="manufacturer" class="mobile laptop" /> <br class="mobile laptop" />
                <label for="model" class="mobile laptop">Model</label> <br class="mobile laptop" />
                <input type="text" name="model" class="mobile laptop" /> <br class="mobile laptop" />

                <label for="laptop_battery_status" class="laptop">Is the laptop battery backed up</label> <br class="laptop" />
                <p class="laptop"><input type="radio" name="laptop_battery_status" value="Yes" class="laptop" />Yes</p>
                <p class="laptop"><input type="radio" name="laptop_battery_status" value="No" class="laptop" />No</p>

                <label for="year">Year of purchase</label> <br />
                <input type="number" name="year" /> <br />
                <label for="price">Expected Price</label> <br />
                <input type="number" name="price" /> <br />

                <input type="submit" value="Submit Ad" />
                <input type="reset" value="Clear all" />
            </form>
        </div>




        <script type="text/javascript">
            function addAuth() {
                var container = document.getElementById("author");
                var len = container.getElementsByTagName("input").length;

                var input = document.createElement("input");
                var br = document.createElement("br");
                br.className = "book";
                input.type = "text";
                input.className = "book";
                input.name = "book_auth" + (len + 1);
                input.placeholder = "Author " + (len + 1);

                container.appendChild(input);
                container.appendChild(br);
            }


            function radio() {
                var b = document.getElementById("b");
                var m = document.getElementById("m");
                var l = document.getElementById("l");

                if (b.checked) { book(); }
                else if (m.checked) { mobile(); }
                else if (l.checked) { laptop(); }
            }

            function book() {
                for (let el of document.querySelectorAll('.book')) el.style.display = 'block';
                for (let el of document.querySelectorAll('.laptop')) el.style.display = 'none';
                for (let el of document.querySelectorAll('.mobile')) el.style.display = 'none';
            }
            function laptop() {
                for (let el of document.querySelectorAll('.book')) el.style.display = 'none';                
                for (let el of document.querySelectorAll('.mobile')) el.style.display = 'none';
                for (let el of document.querySelectorAll('.laptop')) el.style.display = 'block';
            }
            function mobile() {
                for (let el of document.querySelectorAll('.book')) el.style.display = 'none';
                for (let el of document.querySelectorAll('.laptop')) el.style.display = 'none';
                for (let el of document.querySelectorAll('.mobile')) el.style.display = 'block';
            }
        </script>
    </body>
</html>
