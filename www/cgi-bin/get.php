<html>
<body>

<form action="#" method="get">
Name: <input type="text" name="name"><br>
E-mail: <input type="text" name="email"><br>
<input type="submit">
</form>


<?php if(isset($_GET["name"])):?>

    <p>

    Welcome <?php echo $_GET["name"]; ?><br>
    Your email address is: <?php echo $_GET["email"]; ?>

    </p>

<?php endif ?>

</body>
</html> 