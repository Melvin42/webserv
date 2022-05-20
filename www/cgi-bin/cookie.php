<html>
<body>


<form  method="post">
Set cookie: <input type="text" name="cookie"><br>
<input type="submit">
</form>

<?php 

if(isset($_POST["cookie"]))
{
    setcookie("cookie", $_POST["cookie"]);
    $_COOKIE["cookie"] = $_POST["cookie"];
}

?>

<?php if(isset($_COOKIE["cookie"])):?>

    <p>

    This is the set cookie: <?php echo $_COOKIE["cookie"]; ?><br>

    </p>

<?php endif ?>

</body>
</html> 