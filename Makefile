MODULE_big = pg_injection
OBJS = pg_injection.o
PG_CONFIG = pg_config
PGXS = $(shell $(PG_CONFIG) --pgxs)
REGRESS = $(patsubst sql/%.sql,%,$(TESTS))
REGRESS_OPTS = --use-existing
SHLIB_LINK = -linjection
TESTS = $(wildcard sql/*.sql)
include $(PGXS)
