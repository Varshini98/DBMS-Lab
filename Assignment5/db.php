<?php
    //create the tables
    //users
    $q = "CREATE TABLE IF NOT EXISTS users (";
    $q .= "id INT PRIMARY KEY AUTO_INCREMENT,";
    $q .= "email VARCHAR(50) NOT NULL,";
    $q .= "password VARCHAR(40) NOT NULL,";
    $q .= "fname VARCHAR(25) NOT NULL,";
    $q .= "lname VARCHAR(25),";
    $q .= "mob VARCHAR(10),";
    $q .= "type VARCHAR(8))";

    $conn->query($q) or die("User Table Create error -> " . $conn->error);

    //adverts
    $q = "CREATE TABLE IF NOT EXISTS adverts (";
    $q .= "id INT PRIMARY KEY AUTO_INCREMENT,";
    $q .= "init_date DATETIME,";
    $q .= "owner_id INT NOT NULL,";
    $q .= "year_purchase YEAR,";
    $q .= "price INT NOT NULL,";
    $q .= "FOREIGN KEY (owner_id) REFERENCES users (id))";

    $conn->query($q) or die("Advert Table Create error -> " . $conn->error);

    //mobiles
    $q = "CREATE TABLE IF NOT EXISTS mobiles (";
    $q .= "id INT PRIMARY KEY AUTO_INCREMENT,";
    $q .= "manufacturer VARCHAR(25),";
    $q .= "model VARCHAR(30),";
    $q .= "advert_id INT NOT NULL,";
    $q .= "FOREIGN KEY (advert_id) REFERENCES adverts (id) ON DELETE CASCADE)";

    $conn->query($q) or die("Mobiles Table Create error -> " . $conn->error);

    //laptops
    $q = "CREATE TABLE IF NOT EXISTS laptops (";
    $q .= "id INT PRIMARY KEY AUTO_INCREMENT,";
    $q .= "manufacturer VARCHAR(25),";
    $q .= "model VARCHAR(30),";
    $q .= "advert_id INT NOT NULL,";
    $q .= "battery_status TINYINT(1),";
    $q .= "FOREIGN KEY (advert_id) REFERENCES adverts (id) ON DELETE CASCADE)";

    $conn->query($q) or die("Laptops Table Create error -> " . $conn->error);

    //books
    $q = "CREATE TABLE IF NOT EXISTS books (";
    $q .= "id INT PRIMARY KEY AUTO_INCREMENT,";
    $q .= "name VARCHAR(80),";
    $q .= "cond VARCHAR(8),";
    $q .= "advert_id INT NOT NULL,";
    $q .= "FOREIGN KEY (advert_id) REFERENCES adverts (id) ON DELETE CASCADE)";

    $conn->query($q) or die("Books Table Create error -> " . $conn->error);

    //authors
    $q = "CREATE TABLE IF NOT EXISTS authors (";
    $q .= "id INT PRIMARY KEY AUTO_INCREMENT,";
    $q .= "book_id INT NOT NULL,";
    $q .= "name VARCHAR(25),";
    $q .= "FOREIGN KEY (book_id) REFERENCES books (id) ON DELETE CASCADE)";

    $conn->query($q) or die("Authors Table Create error -> " . $conn->error);

    //messages
    $q = "CREATE TABLE IF NOT EXISTS messages (";
    $q .= "id INT PRIMARY KEY AUTO_INCREMENT,";
    $q .= "sender_id INT NOT NULL,";
    $q .= "receiver_id INT NOT NULL,";
    $q .= "message VARCHAR(250),";
    $q .= "time DATETIME,";
    $q .= "FOREIGN KEY (sender_id) REFERENCES users (id),";
    $q .= "FOREIGN KEY (receiver_id) REFERENCES users (id))";

    $conn->query($q) or die("Messages Table Create error -> " . $conn->error);
?>
