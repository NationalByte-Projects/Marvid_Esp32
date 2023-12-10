<?php

$Data = $_POST['Data'];
$Alerts="";
$DataArray=[];

$DataArray = explode(",",$Data);
$Date = gmdate("Y-m-d\ H:i:s");

/*$db_servername = "127.0.0.1";
$db_username = "root";
$db_password = "";
$db_name = "sql3438985";
*/
$db_servername = "sql3.freesqldatabase.com:3306";
$db_username = "sql3668653";
$db_password = "khRM31ENcb";
$db_name = "sql3668653";
    
 
    $mysqli = new mysqli($db_servername,$db_username,$db_password,$db_name);
    // UsuarioID,BeaconID,Temp,Horas,Velocidad,Latitud,Longitud
    $SQLSTRING ="UPDATE `DatosAct` SET `Temp`= '".$DataArray[2]."',`Encendido`= '".$DataArray[3]."',`Velocidad`= '".$DataArray[4]."' ,`Latitud`= '".$DataArray[5]. "' ,`Longitud`= '".$DataArray[6]."' ,`Fecha`= '".$Date."' WHERE `UsuarioID`= '".$DataArray[0]."' and `BeaconID`= '".$DataArray[1]."';";
    //echo $SQLSTRING;
    $registers = mysqli_query($mysqli,$SQLSTRING);
    if(intval($DataArray[2])>85 || intval($DataArray[4]>11)){
        if(floatval($DataArray[2])>85 && floatval($DataArray[4]>11)){
            $Alerts ="Temperatura: ".$DataArray[2]." Velocidad: ".$DataArray[3];   
        }else{
            if(intval($DataArray[2])>85){
                $Alerts = "Temperatura: ".$DataArray[2];
            }else{
                $Alerts = "Velocidad: ".$DataArray[4];
            }
        }
        
        $SQLSTRING = "INSERT INTO `Historial` (`UsuarioID`, `BeaconID`, `Alerta`, `Fecha`) VALUES ('".$DataArray[0]."','".$DataArray[1]."','".$Alerts."','".$Date."');";
        
        $registers = mysqli_query($mysqli,$SQLSTRING);
    }
    
    //echo "SELECT * FROM Usuarios where UsuarioID = '".$User."' And Passw = '".$Passw."'";
    
    mysqli_close($mysqli);



?>