componere-test-coverage:
	CCACHE_DISABLE=1 EXTRA_CFLAGS="-fprofile-arcs -ftest-coverage" $(MAKE) clean all

componere-test-coverage-lcov: componere-test-coverage
	lcov -c --directory $(top_srcdir)/src/.libs --output-file $(top_srcdir)/coverage.info

componere-test-coverage-html: componere-test-coverage-lcov
	genhtml $(top_srcdir)/coverage.info --output-directory=$(top_srcdir)/html
