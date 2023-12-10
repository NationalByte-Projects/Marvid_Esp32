<?php
/* Do some operation here, like talk to the database, the file-session
 * The world beyond, limbo, the city of shimmers, and Canada.
 *
 * AJAX generally uses strings, but you can output JSON, HTML and XML as well.
 * It all depends on the Content-type header that you send with your AJAX
 * request. */
/*$Arreglo = array(
    "a1"    => "a",
    "a2"  => "b",
    "a3"  => "c",
    "a4" => "d",
);*/

/*
    ['Bondi Beach', -33.890542, 151.274856, 4],
    ['Coogee Beach', -33.923036, 151.259052, 5],
    ['Cronulla Beach', -34.028249, 151.157507, 3],
    ['Manly Beach', -33.80010128657071, 151.28747820854187, 2],
    ['Maroubra Beach', -33.950198, 151.259302, 1]
*/
$Arreglo = array(
    array("Beacon1", 23.082142734755976, -102.47748277671411),
    array("Beacon2", 21.738670501341247, -102.07352040885236),  
    array("Beacon3", 20.892461851136286, -100.02263454124657)
);

echo json_encode($Arreglo); // In the end, you need to echo the result.
                      // All data should be json_encode()d.

                      // You can json_encode() any value in PHP, arrays, strings,
                      //even objects.
?>