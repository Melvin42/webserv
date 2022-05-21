<html>
<body>


<form  method="post">
Name: <input type="text" name="name"><br>
E-mail: <input type="text" name="email"><br>
<input type="submit">
</form>


<?php if(isset($_POST["name"])):?>

    <p>

    Welcome <?php echo $_POST["name"]; ?><br>
    Your email address is: <?php echo $_POST["email"]; ?>

    </p>

<?php endif ?>

</body>
</html> 