<?php
try{
session_start();
session_destroy();
echo json_encode("Cerrado");
}catch (Exception $e){
    echo json_encode("Nope"); 
}

?>