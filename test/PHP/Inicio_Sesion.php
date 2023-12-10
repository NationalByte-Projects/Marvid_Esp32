<?php

    //USuario prueba Marvid Admin

/*$db_servername = "127.0.0.1";
$db_username = "root";
$db_password = "";
$db_name = "sql3438985";
*/

// $db_servername = "remotemysql.com:3306";
// $db_username = "hmkGpg6nMP";
// $db_password = "gqgncVFAKg";
// $db_name = "hmkGpg6nMP";

$db_servername = "sql3.freesqldatabase.com:3306";
$db_username = "sql3668653";
$db_password = "khRM31ENcb";
$db_name = "sql3668653";
    $echo("wazaaa");
    $User = $_POST['Userx'];
    $Passw = md5($_POST['Passw']);   
    $mysqli = new mysqli($db_servername,$db_username,$db_password,$db_name);
    $registers = mysqli_query($mysqli,"SELECT * FROM Usuarios where UsuarioID = '".$User."' And Passw = '".$Passw."'");
    //echo "SELECT * FROM Usuarios where UsuarioID = '".$User."' And Passw = '".$Passw."'";
    $num_rows = mysqli_num_rows($registers);
    if($num_rows>0){
        $row = mysqli_fetch_assoc($registers);
        session_start();
        $_SESSION['UsuarioID'] = $row['UsuarioID'];
        $_SESSION['Sesion'] = "Activa";
        echo json_encode("Bienvenido ".$_SESSION['UsuarioID']);
    }else{
        echo json_encode("Error");
    }
    
    ?>