<?php
include_once("options.php");

$name04 = $_GET['name'];

// ����� ����� �� ������ 
// ��������� ���� ����������� � ��
$query = 'SELECT * FROM sav_controllers c WHERE enable=1 AND name =\''.$name.'\'';
$result = mysql_query($query) or die('<b>'.$query.'</b> - ������ � ���������� ������� �� ������: ' . mysql_error());
if( !$row = mysql_fetch_assoc($result) )die( '���������� '.$name.' �� ���������������� ��� �� ��������');
mysql_free_result($result);
 
$id04   = $_GET['id'];
$t04    = $_GET['t'];
$a04    = $_GET['soil'];
$cnt04  = $_GET['cnt'];
$vcc04  = $_GET['vcc'];


$query = 'INSERT INTO sav_plant_c04(name,id,dt,count,soil,vcc,t) VALUES(\''.$name04.
   '\',\''.$id04.'\',now(),\''.$cnt04.'\',\''.$a04.'\',\''.$vcc04.'\',\''.$t04.'\')';
echo $query.'<br>';   
$result = mysql_query($query) or die('���������� � �� �� �������: ' . mysql_error());

echo 'ok';
// ��������� ����������
mysql_close($link);




?>

