<?php
// Параметры подключения к БД
$server   = '??????????';
$user     = '??????????';
$password = '??????????'; 
$base     = '??????????';

$K_soil   = 8;
$K_soil1  = 8;
//$secret     = $_GET['secret'];
//if( $secret != $secret1 )die("");

// Таймаут в секундах между архивными записями
$min_sec  = 60;
// Таймаут в секундах между архивными записями если значение не менялось
$min_sec1  = 600;

// Порог изменения освещенности в %  между архивными записями
$min_proc = 0;

// Соединяемся, выбираем базу данных
$link = mysql_connect($server, $user, $password)
    or die('Не удалось соединиться: ' . mysql_error());
//echo 'Соединение успешно установлено<br>';
mysql_select_db($base) or die('Не удалось выбрать базу данных');


$tz_group  = 3;
$tz_moscow = 3;

$id        = $_GET['id'];
$name      = $_GET['name'];
$tz        = $_GET['tz'];
if( $tz == '' )$tz = 5;
$hour      = $_GET['hour'];
if( $hour == '' || $hour < 1 || $hour > 240 )$hour = 96;
$period    = $_GET['period'];
if( $period == '' )$period = 96;
if( $period >= 240 )$period = 240;
$period_sec = $period*3600-1;
$sensor_name = '??';
$controller_name = 'Неизвестный контроллер';

if( $id != '' && $name != '' ){
// Считываем название сенсора
   $query = 'SELECT text,k FROM sav_plant_sprav04 WHERE id='.$id.' AND name =\''.$name.'\'';
   $result = mysql_query($query) or die('<b>'.$query.'</b> - запрос к справочной таблице не удался: ' . mysql_error());
   if( $row = mysql_fetch_assoc($result) ){
      $sensor_name = $row['text'];
	  $K_soil1     = $row['k'];
   }
   mysql_free_result($result);
// Считываем название контроллера   
   $query = 'SELECT text FROM sav_controllers WHERE name =\''.$name.'\'';
   $result = mysql_query($query) or die('<b>'.$query.'</b> - запрос к справочной таблице не удался: ' . mysql_error());
   if( $row = mysql_fetch_assoc($result) )$controller_name = $row['text'];
   mysql_free_result($result);
   
}


$time1 = $_GET['time1'];
$time2 = $_GET['time2'];


$x = $_GET['x'];
if( $x == '' || $x < 500 || $x > 2000 )$x = 1280;
$y = $_GET['y'];
if( $y == '' || $y < 200 || $y > 2000 )$y = 600;

$tm_max    = 300;

/**
* Функция получения последнего значения по всем датчикам
*/
function get_last_value(){
   global $K_soil;
   $a = array();
   $query = 'SELECT p4.id,p4.name,p4.dt + INTERVAL 2 HOUR dt1,p4.count,p4.soil,p4.vcc,p4.t,s4.k,s4.text text1,cs.text text2 FROM sav_plant_c04 p4,sav_plant_sprav04 s4,sav_controllers cs, ( SELECT MAX(p4.dt) dt1,p4.name name,p4.id id FROM sav_plant_c04 p4,sav_plant_sprav04 s4,sav_controllers cs WHERE p4.id = s4.id AND s4.enable = 1 AND p4.name = cs.name AND cs.enable = 1 GROUP BY p4.name,p4.id,s4.text,cs.text ) m4 WHERE p4.id = s4.id AND s4.enable = 1 AND p4.name = cs.name AND cs.enable = 1 AND m4.dt1 = p4.dt AND m4.id = p4.id AND m4.name = p4.name';
   
   $result = mysql_query($query) or die('<b>'.$query.'</b> - запрос к таблице текущих значений не удался: ' . mysql_error());
   
   $count = 0;
   for ($i=0; $row = mysql_fetch_assoc($result); $i++) {
	  $id = $row['id'];
	  $a[$i]['id']   = $row['id'];
	  $a[$i]['name'] = $row['name'];
	  $a[$i]['dt']   = $row["dt1"];
//	  $a[$i]['dt']   = date('d.m.Y H:i:s',$row["dt"]+$delta_sec);
	  $a[$i]['count'] = $row['count'];
	  $a[$i]['k'] = $row['k'];
	  if( $a[$i]['k'] == '' || $a[$i]['k'] == 0 )$a[$i]['k'] = $K_soil;
	  $a[$i]['soil'] = round($row['soil']/$a[$i]['k']);
	  $a[$i]['vcc'] = round($row['vcc']/1000,2);
	  $a[$i]['t'] = $row['t']/10;
	  if( $a[$i]['t'] == 0 )$a[$i]['t'] = '-';
	  $a[$i]['text1'] = $row['text1'];
	  $a[$i]['text2'] = $row['text2'];
	  
   }
   mysql_free_result($result);
   return $a;
}

/**
* Функция получения числового ряда по одному датчику за период
*/
function get_values_one(){
   global $a1,$id,$name,$hour,$K_soil,$K_soil1;
   $a = array();
   
   $query = 'SELECT k FROM sav_plant_sprav04 WHERE id='.$id.' AND name =\''.$name.'\'';
   $result = mysql_query($query) or die('<b>'.$query.'</b> - запрос к справочной таблице не удался: ' . mysql_error());
   if( $row = mysql_fetch_assoc($result) )$K_soil1 = $row['k'];
   mysql_free_result($result);
   if( $K_soil1 == '' || $K_soil1 == 0 )$K_soil1 = $K_soil;
   
   $query = 'SELECT *,UNIX_TIMESTAMP(dt + INTERVAL 2 HOUR) dt1 FROM sav_plant_c04 WHERE id='.$id.' AND name = \''.$name.'\' AND dt>= NOW() - INTERVAL '.$hour.' HOUR ORDER BY dt';
   
   $result = mysql_query($query) or die('<b>'.$query.'</b> - запрос к таблице текущих значений не удался: ' . mysql_error());
   
   
   $count = 0;
   $x = 6;//round($hour/9);
//   echo $x;
   if( $x <= 0 )$x = 2;
//   $x = 10;
   for ($i=0; $row = mysql_fetch_assoc($result); $i++) {
      if( ($i % $x) == 0 )$a[$i]['dt']   = date('D H:i',$row["dt1"]);
	  else $a[$i]['dt'] = '';
	  $a[$i]['count'] = $row['count'];
	  $a[$i]['soil'] = round($row['soil']/$K_soil1);
	  $a[$i]['vcc'] = round($row['vcc']/1000,2);
	  $a[$i]['t'] = $row['t']/10;
	  
   }
   mysql_free_result($result);
   return $a;
}

function array_column( $a, $key ){
   $b = array();
   $i = 0;
   foreach( $a as $item )$b[$i++] = $item[$key];
   return $b;
}


?>