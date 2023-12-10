<?php

    
   /*$db_servername = "127.0.0.1";
$db_username = "root";
$db_password = "";
$db_name = "sql3438985";
*/
$db_servername = "sql3.freesqldatabase.com:3306";
$db_username = "sql3668653";
$db_password = "khRM31ENcb";
$db_name = "sql3668653";
    
    session_start();
    $Usuario = $_SESSION['UsuarioID'] ;
    //echo gmdate("Y-m-d\TH:i:s\Z");
    $mysqli = new mysqli($db_servername,$db_username,$db_password,$db_name);
    $registers = mysqli_query($mysqli,"SELECT * FROM Historial where UsuarioID = '".$Usuario.'");
    function CreateTable($BeaconID, $Alerta,$Fecha){

        $TablaData = 
        '<tr><th>'.$BeaconID.'</th> 
        <th>'.$Alerta.'</th> 
        <th>'.$Fecha.'</th> 
        </tr>';
        return $TablaData;
    }
    
    $Arreglo = [];
    $i =0;
    while ( $row = mysqli_fetch_assoc($registers) ){
        $Arreglo[$i][0] = CreateTable($row['BeaconID'],$row['Alerta'],$row['Fecha']);
        $i++;
        
     }
    /*echo $row['UsuarioID'];
    echo $row['Temp'];
    echo $row['Encendido'];
    echo $row['Velocidad'];
    echo $row['Latitud'];
    echo $row['Longitud'];
    echo $row['Fecha'];*/
    //$row = mysqli_fetch_assoc($registers);
    //echo $row['UsuarioID'];
    echo json_encode($Arreglo);
    mysqli_close($mysqli);
