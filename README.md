componere
=========
*composing classes at runtime since 2018*

[![Build Status](https://travis-ci.org/krakjoe/componere.svg?branch=master)](https://travis-ci.org/krakjoe/componere)

This extension allows the composition and re-composition of classes at runtime, using dark scary magic, but of the kind you might take to production.

API
===

```php
namespace Componere {
	class Definition {
		public function __construct(string $name);
		public function __construct(string $name, string $parent);
		public function __construct(string $name, array $interfaces);
		public function __construct(string $name, string $parent, array $interfaces);

		public function addMethod(string $name, Method $method);
		public function addTrait(string $name);
		public function addProperty(string $name, Value $property);
		public function addConstant(string $name, Value $constant);

		public function getClosure(string $name, $this = null) : Closure;
		public function getClosures($this = null) : array;
 
		public function register();
	}
	class Method {
		public function __construct(string $name, Closure $method);
		public function setProtected();
		public function setPrivate();
	}
	class Value {
		public function __construct(string $name, $value);
		public function setProtected();
		public function setPrivate();
	}
}
```

Explanation
===========

These are for the weak ...
