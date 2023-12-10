<?php
/*$Data ='2021-10-27T04:37:21';
$Fecha = gmdate("Y-m-d\TH:i:s");
echo $Fecha;
echo "<br>";
echo strtotime($Data);
echo "<br>";
echo strtotime($Fecha); 
$time = strtotime($Fecha) - strtotime($Data);
echo "<br>";
echo $time;
echo "<br>";
$datetime = date("Y-m-d H:i:s");
echo $datetime;
*/

$db_servername = "remotemysql.com:3306";
$db_username = "hmkGpg6nMP";
$db_password = "gqgncVFAKg";
$db_name = "hmkGpg6nMP";
$mysqli = new mysqli($db_servername,$db_username,$db_password,$db_name);
$registers = mysqli_query($mysqli,"SELECT * FROM Usuarios where UsuarioID = 'Marvid' And Passw = 'e3afed0047b08059d0fada10f400c1e5'");
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