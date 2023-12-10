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
    $registers = mysqli_query($mysqli,"SELECT * FROM DatosAct where UsuarioID = '".$Usuario."'");
    function CreateTable($BeaconID, $Temp, $Encendido, $Velocidad, $Fecha){
        $Horas = strval(500-(intval($Encendido)));
        $Fecha_Act = gmdate("Y-m-d\TH:i:s");
        $time = strtotime($Fecha_Act) - strtotime($Fecha);
        $Estatus ="";
        $Status_Vel="";
        $Status_Temp="";
        if($Velocidad>11){
            $Status_Vel = 'Class ="Rojo"';
        }else{
            $Status_Vel = "";
        }
        if($Temp>85){
            $Status_Temp = 'Class ="Rojo"';
        }else{
            $Status_Temp = "";
        }
        if($time>600){
            $Estatus ='<th Class = "Rojo">Offline</th>';    
        }else{
            $Estatus ='<th Class = "Verde">Online</th>';
        }
        $TablaData = 
        '<tr>
        '.$Estatus.'
        <th>'.$BeaconID.'</th> 
        <th '.$Status_Temp.  '>'.$Temp.'</th> 
        <th '.$Status_Vel. ' >'.$Velocidad.'</th> 
        <th>'.$Encendido.'</th> 
        <th>'.$Horas.'</th>
        <th>'.$Fecha.'</th> 
        </tr>';
        return $TablaData;
    }
    
    $Arreglo = [];
    $i =0;
    while ( $row = mysqli_fetch_assoc($registers) ){
        $Arreglo[$i][0]=$row['BeaconID'];
        $Arreglo[$i][1]=floatval($row['Latitud']);
        $Arreglo[$i][2]=floatval($row['Longitud']);
        $Arreglo[$i][3] = CreateTable($row['BeaconID'],$row['Temp'],$row['Encendido'],$row['Velocidad'],$row['Fecha']);
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

?>