--TEST--
Componere\cast
--FILE--
<?php
use function Componere\cast;

class MyDateTime extends DateTime {
	public $thing = 10;
}

class MyOtherDateTime extends MyDateTime {
	public $other = 20;
}

$date = new DateTime;

var_dump($date);

$cast = cast(MyDateTime::class, $date);

$cast->thing = new stdClass;

var_dump($cast);

$cast = cast(MyOtherDateTime::class, $cast);

$cast->other = new stdClass;

var_dump($cast);

$cast = cast(MyDateTime::class, $cast);

var_dump($cast);

$cast = cast(DateTime::class, $cast);

var_dump($cast);
?>
--EXPECTF--
object(DateTime)#1 (3) {
  ["date"]=>
  string(%d) "%s"
  ["timezone_type"]=>
  int(%d)
  ["timezone"]=>
  string(%d) "%s"
}
object(MyDateTime)#2 (4) {
  ["thing"]=>
  object(stdClass)#3 (0) {
  }
  ["date"]=>
  string(%d) "%s"
  ["timezone_type"]=>
  int(%d)
  ["timezone"]=>
  string(%d) "%s"
}
object(MyOtherDateTime)#4 (5) {
  ["other"]=>
  object(stdClass)#2 (0) {
  }
  ["thing"]=>
  object(stdClass)#3 (0) {
  }
  ["date"]=>
  string(%d) "%s"
  ["timezone_type"]=>
  int(%d)
  ["timezone"]=>
  string(%d) "%s"
}
object(MyDateTime)#5 (4) {
  ["thing"]=>
  object(stdClass)#3 (0) {
  }
  ["date"]=>
  string(%d) "%s"
  ["timezone_type"]=>
  int(%d)
  ["timezone"]=>
  string(%d) "%s"
}
object(DateTime)#4 (3) {
  ["date"]=>
  string(%d) "%s"
  ["timezone_type"]=>
  int(%d)
  ["timezone"]=>
  string(%d) "%s"
}

