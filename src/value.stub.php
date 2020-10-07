<?php

/**
 * @generate-function-entries
 * @generate-legacy-arginfo
 */

namespace Componere;

class Value {
	public function __construct(mixed $value = NULL) {}

	public function setProtected():Value {}

	public function setPrivate():Value {}

	public function setStatic():Value {}

	public function isPrivate():bool {}

	public function isProtected():bool {}

	public function isStatic():bool {}

	public function hasDefault():bool {}
}
