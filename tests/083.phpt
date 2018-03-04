--TEST--
Definition inherit from final class
--FILE--
<?php
use Componere\Definition;

final class B {}

$d = new Definition(A::class, B::class);

echo "ok";
?>
--EXPECT--
ok

